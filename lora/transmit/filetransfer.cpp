#include "filetransfer.h"

// For file transfer protocol
// Assumption is that only one file is being transferred at a time
File file;
char filename[FILENAME_SIZE];
uint16_t packetAmount;

// --------------------------------------------- //
//         FILE TRANSFER - SHARED                //
// --------------------------------------------- //

bool payloadType(uint8_t *message, size_t size)
{
    int type = message[0] >> 6;
    printContentType(type);
    if (type == 0)
    {
        receiveContent(message + 1, size - 1);
    }
    else if (type == 1)
    {
        receiveMetadata(message + 1, size - 1);
    }
    else if (type == 0b10)
    {
        packetReceived = true;
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
}

// --------------------------------------------- //
//         FILE TRANSFER - RECEIVER              //
// --------------------------------------------- //

uint16_t lastReceivedPacket = 0;

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

    if (packetNumber + 1 == lastReceivedPacket)
    {
        printInfo("Packet already received");
    }
    else
    {
        lastReceivedPacket = packetNumber + 1;
        appendToFile(message, size, filename);
    }

    printPacketContent(message, size);

    if (!ACKContent(packetNumber))
    {
        printError("Something went wrong when adding ACKConent to queue");
        receiveMode();
        return;
    }
    transmitMode();
}

void receiveMetadata(uint8_t *message, size_t size)
{
    uint8_t first = message[0];
    uint8_t second = message[1];
    packetAmount = (first << 8) + second;
    printPacketAmount(packetAmount);
    message += 2;
    size -= 2;
    strncpy(filename, (char *)message, FILENAME_SIZE);
    printFilename(filename);
    if (!ACKMetadata())
    {
        printError("Something went wrong when adding ACKMetadata to queue");
        return;
    }
    transmitMode();
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
//         FILE TRANSFER - TRANSMITTER           //
// --------------------------------------------- //

volatile bool metadataReceived = false;
volatile bool packetReceived = false;

bool receiveACK()
{
    receiveMode();
    size_t messageSize = radio.getPacketLength();
    uint8_t message[messageSize];

    if (!receiveMessage(message, messageSize))
    {
        return false;
    }

    return payloadType(message, messageSize);
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

        while (!packetReceived)
        {
            printTransmitFilePacket(buffer, readBytes + 3, packetCount, buffer + 3, readBytes);

            if (!transmitMessage(buffer, readBytes + 3))
            {
                return false;
            }

            while (!transmittedFlag)
            {
            }

            if (!receiveACK())
            {
                printInfo("Timeout or failure when receiving content ACK. Trying again.");
            }
            transmitMode();
        }

        printInfo("Packet sent and ACK received!");
        packetReceived = false;
        packetCount++;
    }
    return true;
}

void transferFile()
{
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
        if (!receiveACK())
        {
            printInfo("Timeout or failure when receiving metadata ACK. Trying again.");
        }
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
    printFileTransferTotalTime(fileTransferTime);
    file.close();
}