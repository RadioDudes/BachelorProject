#include "T3S3.h"

class Receiver
{
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
    // Following methods implement a simple protocol for file transfer

    //      16 bits
    // +---------------+---------+
    // | packet number | content |
    // +---------------+---------+

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
    //      2 bit         6 bits         16 bits
    // +--------------+-------------+---------------+-----------+
    // | payload type | file number | packet amount | file name |
    // +--------------+-------------+---------------+-----------+

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
    //     2 bits          6 bits
    // +--------------+-------------+---------+
    // | payload type | file number | payload |
    // +--------------+-------------+---------+
    // 00 = file data
    // 01 = file meta data
    // 10 = ACK file data
    // 11 = ACK file meta data

    void payloadType(uint8_t *message, size_t size)
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
        else
        {
            printError("Invalid packet received.");
        }
    }

public:
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
        payloadType(message, messageSize);
    }
};