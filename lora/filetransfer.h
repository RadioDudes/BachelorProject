#pragma once
#include "T3S3.h"

namespace ACKProtocol
{
    // --------------------------------------------- //
    //         FILE TRANSFER - SHARED                //
    // --------------------------------------------- //

    //     3 bits          5 bits
    // +--------------+-------------+---------+
    // | payload type | file number | payload |
    // +--------------+-------------+---------+
    // 000 = file data
    // 001 = file meta data
    // 010 = ACK file data
    // 011 = ACK file meta data
    // 100 = EOF
    bool payloadType(uint8_t *message, size_t size);
    void receiveFileProtocolMessage();
    void setTimeout(unsigned long time);
    void syncBandwidth(double bw);
    void syncSpreadingFactor(uint8_t sf);
    void resetVars();

    // --------------------------------------------- //
    //         FILE TRANSFER - RECEIVER              //
    // --------------------------------------------- //

    void receiveFileEnd();
    bool ACKContent(uint16_t packetNumber);
    bool ACKMetadata();
    //      16 bits
    // +---------------+---------+
    // | packet number | content |
    // +---------------+---------+
    void receiveContent(uint8_t *message, size_t size);
    //      3 bit         6 bits         16 bits
    // +--------------+-------------+---------------+-----------+
    // | payload type | file number | packet amount | file name |
    // +--------------+-------------+---------------+-----------+
    void receiveMetadata(uint8_t *message, size_t size);

    // --------------------------------------------- //
    //         FILE TRANSFER - TRANSMITTER           //
    // --------------------------------------------- //

    void receiveACK();
    bool sendMetadata();
    bool sendEOF(int packetCount);
    bool sendContents();
    void transferFile(char *name);
}