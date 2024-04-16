#include "filetransfer.h"
// For file transfer protocol

namespace ACKProtocol
{
// Max amount of packets
#define MAX_PACKET_AMOUNT 65536
// Max length of the file name
#define FILENAME_SIZE 100

    // Timeout defined in milliseconds
    unsigned long timeoutTime = 2000;

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

    // Variables for transmitter to know if SF or BW change has been received correctly
    double ackBW = 0;
    uint8_t ackSF = 0;
}

// --------------------------------------------- //
//         FILE TRANSFER - SHARED                //
// --------------------------------------------- //

void ACKProtocol::resetVars()
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
}

void ACKProtocol::setTimeout(unsigned long time)
{
    timeoutTime = time;
    Logging::printSetTimeout(time);
}

void ACKProtocol::syncBandwidth(double bw)
{
    uint8_t message[9];
    message[0] = 0b11000000;
    double* pBW = (double*) &(message[1]);
    *pBW = bw;

    transmitMode();
    transmitMessage(message, 9);
    while (!transmittedFlag)
    {
    }

    receiveACK();

    if (ackBW == bw)
    {
        setBandwidth(bw);
    }
    else
    {
        Logging::printSyncErrorBW(bw, ackBW);
    }
}

void ACKProtocol::syncSpreadingFactor(uint8_t sf)
{
    uint8_t message[2];
    message[0] = 0b10100000;
    message[1] = sf;

    transmitMode();
    transmitMessage(message, 2);
    while (!transmittedFlag)
    {
    }

    receiveACK();

    if (ackSF == sf)
    {
        setSpreadingFactor(sf);
    }
    else
    {
        Logging::printSyncErrorSF(sf, ackSF);
    }
}

bool ACKProtocol::payloadType(uint8_t *message, size_t size)
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
    else if (type == 0b101)
    {
        uint8_t ack[2];
        ack[0] = 0b11100000;
        ack[1] = message[1];

        transmitMode();
        transmitMessage(ack, 2);
        while (!transmittedFlag)
        {
        }
        setSpreadingFactor(message[1]);
    }
    else if (type == 0b110)
    {
        double receivedBW = *((double *)&(message[1]));
        uint8_t ack[size];
        memcpy(ack, message, size);
        ack[0] = 0b11100000;

        transmitMode();
        transmitMessage(ack, size);
        while (!transmittedFlag)
        {
        }
        setBandwidth(receivedBW);
    }
    else if (type == 0b111)
    {
        if (size > 2)
        {
            ackBW = *((double *)&(message[1]));
        }
        else
        {
            ackSF = message[1];
        }
    }
    else
    {
        if (enableDebug)
            Logging::printError("Invalid packet received.");
        return false;
    }
    return true;
}

void ACKProtocol::receiveFileProtocolMessage()
{
    size_t messageSize = radio.getPacketLength();
    if (messageSize == 0)
    {
      return;
    }
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

void ACKProtocol::receiveFileEnd()
{
    Display::displayInfoTop("Finished file transfer!");
    file.close();

    unsigned long fileTransferTime = millis() - fileTransferTimerStartingTime;
    Logging::printPacketLoss(receiveCounter - (lastReceivedPacket + 1));
    Logging::printFileTransferTotalTime(fileTransferTime);
    Logging::printDataRate(bytesReceived, fileTransferTime);
    Logging::printAvgSNR(totalSNR, receiveCounter);
    Logging::logFinishReceiving(filename, "/out.log");

    resetVars();
}

bool ACKProtocol::ACKContent(uint16_t packetNumber)
{
    uint8_t message[3];
    message[0] = (uint8_t)0b01000000;
    message[1] = (uint8_t)(packetNumber >> 8);
    message[2] = (uint8_t)(packetNumber & 0x00FF);
    return transmitMessage(message, 3);
}

bool ACKProtocol::ACKMetadata()
{
    uint8_t message[1];
    message[0] = (uint8_t)0b01100000;
    return transmitMessage(message, 1);
}

void ACKProtocol::receiveContent(uint8_t *message, size_t size)
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
    transmitMode();
    if (!ACKContent(packetNumber))
    {
        if (enableDebug)
            Logging::printError("Something went wrong when sending ACKContent");
        return;
    }
    while (!transmittedFlag)
    {
    }
}

void ACKProtocol::receiveMetadata(uint8_t *message, size_t size)
{
    // In order to reset for every new file sent, lastReceivedPacket is set to 0.
    lastReceivedPacket = 0;
    metadataReceived = true;
    fileTransferTimerStartingTime = millis();

    uint8_t first = message[0];
    uint8_t second = message[1];
    packetAmount = (first << 8) + second;

    if (enableDebug)
        Logging::printPacketAmount(packetAmount);
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
        Logging::printFilename(filename);
        char buffer[sizeof(filename) + 20] = "Receiving file ";
        strcat(buffer, filename);
        Display::displayInfoTop(buffer);
    }

    Logging::logStartReceiving(filename, packetAmount, spreadingFactor, codingRate, frequency, bandwidth, "/out.log");

    transmitMode();
    if (!ACKMetadata())
    {
        if (enableDebug)
            Logging::printError("Something went wrong when sending ACKMetadata");
        return;
    }
    while (!transmittedFlag)
    {
    }
}

// --------------------------------------------- //
//         FILE TRANSFER - TRANSMITTER           //
// --------------------------------------------- //

void ACKProtocol::receiveACK()
{
    receiveMode();
    // Timeout implementation
    unsigned long timeoutStartTime = millis();
    while (!receivedFlag)
    {
        if (millis() - timeoutStartTime > timeoutTime)
        {
            if (enableDebug)
                Logging::printError("Timed out!");
            packetLoss++;
            return;
        }
    }

    receiveFileProtocolMessage();
}

bool ACKProtocol::sendMetadata()
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

bool ACKProtocol::sendEOF(int packetCount)
{
    uint8_t message[4];
    message[0] = 0b10000000;
    message[1] = packetCount >> 8;
    message[2] = packetCount & 0xFF;
    return transmitMessage(message, 3);
}

bool ACKProtocol::sendContents()
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

        while (!packetAcked || receivedPacketCount != packetCount)
        {
            if (enableDebug)
                Logging::printTransmitFilePacket(buffer, readBytes + 3, packetCount, buffer + 3, readBytes);

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

        if (enableDebug)
            Logging::printInfo("Packet sent and ACK received!");
        packetAcked = false;
        packetCount++;
    }
    if (!sendEOF(packetCount))
    {
        Logging::printError("Error sending end of file!");
    }
    return true;
}

void ACKProtocol::transferFile(char *name)
{
    strncpy(filename, name, FILENAME_SIZE);
    file = SD.open(filename);

    if (!file)
    {
        Logging::printErrorOpeningFile(filename);
        return;
    }

    Logging::printSendingFile(filename);
    Logging::logStartTransfer(filename, file.size(), spreadingFactor, codingRate, frequency, bandwidth, packetSize, timeoutTime, "/out.log");

    // SENDING METADATA
    unsigned long fileTransferTimerStartingTime;
    while (!metadataAcked)
    {
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
        receiveACK();
        transmitMode();
    }

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
    Logging::printDataRate(file.size(), fileTransferTime);
    Logging::logFinishTransfer(filename, file.size(), fileTransferTime, packetLoss, "/out.log");
    file.close();

    resetVars();
}