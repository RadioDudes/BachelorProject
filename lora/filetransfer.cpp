#include "filetransfer.h"
// For file transfer protocol

// Max amount of packets
#define MAX_PACKET_AMOUNT 65536
// Max length of the file name
#define FILENAME_SIZE 100

// Timeout defined in milliseconds
unsigned long timeoutTime = 500;

// Amount of packets timed out/"lost"
unsigned int packetLoss = 0;

unsigned long bytesTransferred = 0;

// Size of each packet
int packetSize = 30;

// Assumption is that only one file is being transferred at a time
File file;
char filename[FILENAME_SIZE];
uint16_t packetAmount;

uint16_t lastReceivedPacket = 0;

uint16_t receivedPacketCount = UINT16_MAX;
volatile bool metadataReceived = false;
volatile bool packetReceived = false;

// --------------------------------------------- //
//         FILE TRANSFER - SHARED                //
// --------------------------------------------- //

void setTimeout(unsigned long time)
{
    timeoutTime = time;
    printSetTimeout(time);
}

void setPacketSize(int size)
{
    packetSize = size;
    printSetPacketSize(size);
}

bool payloadType(uint8_t *message, size_t size)
{
    int type = message[0] >> 6;
    printContentType(type);
    if (type == 0b00)
    {
        receiveContent(message + 1, size - 1);
    }
    else if (type == 0b01)
    {
        receiveMetadata(message + 1, size - 1);
    }
    else if (type == 0b10)
    {
        if (size >= 3)
        {
            receivedPacketCount = (message[1] << 8) + message[2];
            packetReceived = true;
        }
        else
        {
            printError("Content packet too short!");
            return false;
        }
    }
    else if (type == 0b11)
    {
        metadataReceived = true;
    }
    else
    {
        printError("Invalid packet received.");
        return false;
    }
    return true;
}

void receiveFileProtocolMessage()
{
    size_t messageSize = radio.getPacketLength();
    uint8_t message[messageSize];
    if (!receiveMessage(message, messageSize))
    {
        printError("Error receiving message.");
        return;
    }
    printReceivedPacket(message, messageSize);
    if (!payloadType(message, messageSize))
    {
        printError("Error getting payload type.");
    }
}

// --------------------------------------------- //
//         FILE TRANSFER - RECEIVER              //
// --------------------------------------------- //

bool ACKContent(uint16_t packetNumber)
{
    uint8_t message[3];
    message[0] = (uint8_t)0b10000000;
    message[1] = (uint8_t)(packetNumber >> 8);
    message[2] = (uint8_t)(packetNumber & 0x00FF);
    return transmitMessage(message, 3);
}

bool ACKMetadata()
{
    uint8_t message[1];
    message[0] = (uint8_t)0b11000000;
    return transmitMessage(message, 1);
}

void receiveContent(uint8_t *message, size_t size)
{
    uint8_t first = message[0];
    uint8_t second = message[1];
    uint16_t packetNumber = (first << 8) + second;
    printPacketNumber(packetNumber);

    message += 2;
    size -= 2;

    if ((packetNumber + 1) == lastReceivedPacket)
    {
        printInfo("Packet already received");
    }
    else
    {
        lastReceivedPacket = packetNumber + 1;
        appendToFile(message, size, filename);
    }

    printPacketContent(message, size);
    transmitMode();
    if (!ACKContent(packetNumber))
    {
        printError("Something went wrong when sending ACKContent");
        receiveMode();
        return;
    }
    while (!transmittedFlag)
    {
    }
    receiveMode();
}

void receiveMetadata(uint8_t *message, size_t size)
{
    // In order to reset for every new file sent, lastReceivedPacket is set to 0.
    lastReceivedPacket = 0;
    uint8_t first = message[0];
    uint8_t second = message[1];
    packetAmount = (first << 8) + second;

    printPacketAmount(packetAmount);
    message += 2;
    size -= 2;
    strncpy(filename, (char *)message, FILENAME_SIZE);
    printFilename(filename);

    u8g2->clearBuffer();
    u8g2->drawStr(0, 16, "Receiving file");
    u8g2->drawStr(0, 32, filename);
    u8g2->updateDisplayArea(0, 0, 16, 4);

    transmitMode();
    if (!ACKMetadata())
    {
        printError("Something went wrong when sending ACKMetadata");
        return;
    }
    while (!transmittedFlag)
    {
    }
    receiveMode();
}

// --------------------------------------------- //
//         FILE TRANSFER - TRANSMITTER           //
// --------------------------------------------- //

void receiveACK()
{
    receiveMode();

    // Timeout implementation
    unsigned long timeoutStartTime = millis();
    while (!receivedFlag)
    {
        if (millis() - timeoutStartTime > timeoutTime)
        {
            printError("Timed out!");
            packetLoss++;
            return;
        }
    }

    receiveFileProtocolMessage();
}

bool sendMetadata()
{
    unsigned long packetAmount = (file.size() + packetSize - 1) / packetSize;
    if (packetAmount > MAX_PACKET_AMOUNT)
    {
        Serial.print("File of size ");
        Serial.print(file.size());
        Serial.println(" is too big!");
        return false;
    }
    uint8_t first = packetAmount >> 8;
    uint8_t second = packetAmount & 0xFF;

    size_t messageSize = strlen(filename) + 4;
    uint8_t message[messageSize];
    message[0] = 0b01000000;
    message[1] = first;
    message[2] = second;
    strcpy((char *)message + 3, filename);

    return transmitMessage(message, messageSize);
}

bool sendContents()
{
    int packetCount = 0;
    while (file.available())
    {
        if (packetCount >= MAX_PACKET_AMOUNT)
        {
            printInfo("Packet amount exceded!");
            break;
        }

        uint8_t buffer[packetSize + 3];
        buffer[0] = 0;
        buffer[1] = packetCount >> 8;
        buffer[2] = packetCount & 0xFF;
        size_t readBytes = file.read(buffer + 3, packetSize);

        while (!packetReceived || receivedPacketCount != packetCount)
        {
            printTransmitFilePacket(buffer, readBytes + 3, packetCount, buffer + 3, readBytes);

            if (!transmitMessage(buffer, readBytes + 3, false))
            {
                return false;
            }

            while (!transmittedFlag)
            {
            }
            receiveACK();
            transmitMode();
        }
        
        bytesTransferred += readBytes + 3;
        printInfo("Packet sent and ACK received!");
        packetReceived = false;
        packetCount++;
    }
    return true;
}

void transferFile(char *name)
{
    strncpy(filename, name, FILENAME_SIZE);
    file = SD.open(filename);

    if (!file)
    {
        printErrorOpeningFile(filename);
        return;
    }
    printSendingFile(filename);

    // SENDING METADATA
    unsigned long fileTransferTimerStartingTime;
    while (!metadataReceived)
    {
        if (!sendMetadata())
        {
            printError("Error sending file metadata. Stopping file transfer.");
            file.close();
            return;
        }
        fileTransferTimerStartingTime = millis();
        while (!transmittedFlag)
        {
        }
        receiveACK();
        transmitMode();
    }

    printInfo("Metadata sent and ACK received!");
    metadataReceived = false;

    // SENDING PACKET
    if (!sendContents())
    {
        printError("Error sending content packet. Stopping file transfer.");
        file.close();
    }

    unsigned long fileTransferTime = millis() - fileTransferTimerStartingTime;
    printPacketLoss(packetLoss);
    printFileTransferTotalTime(fileTransferTime);
    printDataRate(bytesTransferred, fileTransferTime);
    packetLoss = 0;
    bytesTransferred = 0;
    file.close();
}