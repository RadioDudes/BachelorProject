#include "T3S3.h"

class Transmitter
{
    volatile bool metadataReceived = false;
    volatile bool packetReceived = false;

    //     2 bits          6 bits
    // +--------------+-------------+---------+
    // | payload type | file number | payload |
    // +--------------+-------------+---------+
    // 00 = file data
    // 01 = file meta data
    // 10 = ACK file data
    // 11 = ACK file meta data
    bool payloadType(uint8_t *message, size_t size)
    {
        int type = message[0] >> 6;
        printContentType(type);
        if (type == 0b10)
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
        return true;
    }

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

public:
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
};