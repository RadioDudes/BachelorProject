#include "filetransfer-raw.h"
// For file transfer protocol

namespace RawProtocol
{
// Max amount of packets
#define MAX_PACKET_AMOUNT 65536
// Max length of the file name
#define FILENAME_SIZE 100

    // Timeout defined in milliseconds
    unsigned long timeoutTime = 500;

    // Amount of packets timed out/"lost"
    unsigned int packetLoss = 0;
    unsigned long fileTransferTimerStartingTime;

    // Assumption is that only one file is being transferred at a time
    File file;
    char filename[FILENAME_SIZE];
    uint16_t packetAmount;

    uint16_t lastReceivedPacket = UINT16_MAX;
    uint16_t receivedPacketCount = UINT16_MAX;

    // Booleans for transmitter to know when packets have been ACKed
    volatile bool metadataAcked = false;
    volatile bool packetAcked = false;

    // Boolean for receiver to know if metadata has been received
    volatile bool metadataReceived = false;
}

// --------------------------------------------- //
//         FILE TRANSFER - SHARED                //
// --------------------------------------------- //

void RawProtocol::resetVars()
{
    metadataAcked = false;
    packetAcked = false;
    metadataReceived = false;
    bytesTransferred = 0;
    bytesReceived = 0;
    receiveCounter = 0;
    sendCounter = 0;
    lastReceivedPacket = UINT16_MAX;
    receivedPacketCount = UINT16_MAX;
    packetLoss = 0;
    totalSNR = 0;
    totalRSSI = 0;
}

bool RawProtocol::payloadType(uint8_t *message, size_t size)
{
    int type = message[0] >> 5;
    if (enableDebug)
        Logging::printContentType(type);
    if (type == 0b000)
    {
        if (metadataReceived)
        {
            receiveContent(message + 1, size - 1);
        }
        else
        {
            if (enableDebug)
                Logging::printError("Packet received before metadata!");
            return false;
        }
    }
    else if (type == 0b001)
    {
        receiveMetadata(message + 1, size - 1);
    }
    else if (type == 0b010)
    {
        if (size >= 3)
        {
            receivedPacketCount = (message[1] << 8) + message[2];
            packetAcked = true;
        }
        else
        {
            if (enableDebug)
                Logging::printError("Content packet too short!");
            return false;
        }
    }
    else if (type == 0b011)
    {
        metadataAcked = true;
    }
    else if (type == 0b100)
    {
        lastReceivedPacket = (message[1] << 8) + message[2];
        receiveFileEnd();
    }
    else
    {
        if (enableDebug)
            Logging::printError("Invalid packet received.");
        return false;
    }
    return true;
}

void RawProtocol::receiveFileProtocolMessage()
{
    size_t messageSize = radio.getPacketLength();
    uint8_t message[messageSize];
    if (!receiveMessage(message, messageSize))
    {
        if (enableDebug)
            Logging::printError("Error receiving message.");
        return;
    }
    if (enableDebug)
        Logging::printReceivedPacket(message, messageSize);
    if (!payloadType(message, messageSize))
    {
        if (enableDebug)
            Logging::printError("Error getting payload type.");
    }
}

// --------------------------------------------- //
//         FILE TRANSFER - RECEIVER              //
// --------------------------------------------- //

void RawProtocol::receiveFileEnd()
{
    Display::displayInfoTop("Finished file transfer!");
    file.close();

    unsigned long fileTransferTime = millis() - fileTransferTimerStartingTime;
    Logging::printPacketLoss(receiveCounter - (lastReceivedPacket + 1));
    Logging::printFileTransferTotalTime(fileTransferTime);
    Logging::printDataRate(bytesReceived, fileTransferTime);

    resetVars();
    receiveMode();
}

void RawProtocol::receiveContent(uint8_t *message, size_t size)
{
    uint8_t first = message[0];
    uint8_t second = message[1];
    uint16_t packetNumber = (first << 8) + second;
    if (enableDebug)
        Logging::printPacketNumber(packetNumber);

    message += 2;
    size -= 2;

    if (packetNumber == lastReceivedPacket)
    {
        if (enableDebug)
            Logging::printInfo("Packet already received");
    }
    else
    {
        lastReceivedPacket = packetNumber;
        appendToFile(message, size, filename, &file);
    }

    if (enableDebug)
        Logging::printPacketContent(message, size);
    receiveMode();
}

void RawProtocol::receiveMetadata(uint8_t *message, size_t size)
{
    metadataReceived = true;
    fileTransferTimerStartingTime = millis();
    message += 2;
    size -= 2;
    strncpy(filename, (char *)message, FILENAME_SIZE);

    // Opens the file named and gets ready to append
    file = SD.open(filename, FILE_APPEND);

    if (!file)
    {
        if (enableDebug)
            Logging::printError(String("Could not open file ") + String(filename));
    }

    if (enableDebug)
    {
        uint8_t first = message[0];
        uint8_t second = message[1];
        packetAmount = (first << 8) + second;
        Logging::printPacketAmount(packetAmount);
        Logging::printFilename(filename);
        char buffer[sizeof(filename) + 20] = "Receiving file ";
        strcat(buffer, filename);
        Display::displayInfoTop(buffer);
    }

    receiveMode();
}

// --------------------------------------------- //
//         FILE TRANSFER - TRANSMITTER           //
// --------------------------------------------- //

bool RawProtocol::sendMetadata()
{
    unsigned long packetAmount = (file.size() + packetSize - 1) / packetSize;
    if (packetAmount > MAX_PACKET_AMOUNT)
    {
        if (enableDebug)
        {
            char buffer[100];
            sprintf(buffer, "File of size %d is too big!", file.size());
            Logging::printError(buffer);
        }
        return false;
    }
    uint8_t first = packetAmount >> 8;
    uint8_t second = packetAmount & 0xFF;

    size_t messageSize = strlen(filename) + 4;
    uint8_t message[messageSize];
    message[0] = 0b00100000;
    message[1] = first;
    message[2] = second;
    strcpy((char *)message + 3, filename);

    return transmitMessage(message, messageSize);
}

bool RawProtocol::sendEOF(int packetCount)
{
    uint8_t message[4];
    message[0] = 0b10000000;
    message[1] = packetCount >> 8;
    message[2] = packetCount & 0xFF;
    return transmitMessage(message, 3);
}

bool RawProtocol::sendContents()
{
    int packetCount = 1;
    while (file.available())
    {
        if (packetCount >= MAX_PACKET_AMOUNT)
        {
            Logging::printInfo("Packet amount exceded!");
            break;
        }

        uint8_t buffer[packetSize + 3];
        buffer[0] = 0;
        buffer[1] = packetCount >> 8;
        buffer[2] = packetCount & 0xFF;
        size_t readBytes = file.read(buffer + 3, packetSize);

        if (enableDebug)
            Logging::printTransmitFilePacket(buffer, readBytes + 3, packetCount, buffer + 3, readBytes);

        if (!transmitMessage(buffer, readBytes + 3, false))
        {
            return false;
        }

        while (!transmittedFlag)
        {
        }
        transmitMode();

        if (enableDebug)
            Logging::printInfo("Packet sent!");
        packetAcked = false;
        packetCount++;
    }
    if (!sendEOF(packetCount))
    {
        Logging::printError("Error sending end of file!");
    }
    return true;
}

void RawProtocol::transferFile(char *name)
{
    strncpy(filename, name, FILENAME_SIZE);
    file = SD.open(filename);

    if (!file)
    {
        Logging::printErrorOpeningFile(filename);
        return;
    }

    Logging::printSendingFile(filename);

    // SENDING METADATA
    unsigned long fileTransferTimerStartingTime;
    if (!sendMetadata())
    {
        if (enableDebug)
            Logging::printError("Error sending file metadata. Stopping file transfer.");
        file.close();
        return;
    }
    fileTransferTimerStartingTime = millis();
    while (!transmittedFlag)
    {
    }
    transmitMode();

    if (enableDebug)
        Logging::printInfo("Metadata sent and ACK received!");
    metadataAcked = false;

    // SENDING PACKET
    if (!sendContents())
    {
        if (enableDebug)
            Logging::printError("Error sending content packet. Stopping file transfer.");
        file.close();
    }

    unsigned long fileTransferTime = millis() - fileTransferTimerStartingTime;
    Logging::printPacketLoss(packetLoss);
    Logging::printFileTransferTotalTime(fileTransferTime);
    Logging::printDataRate(bytesTransferred, fileTransferTime);
    file.close();

    resetVars();
}