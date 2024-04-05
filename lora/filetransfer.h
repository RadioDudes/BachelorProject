#pragma once
#include "T3S3.h"

// --------------------------------------------- //
//         FILE TRANSFER - SHARED                //
// --------------------------------------------- //

void setTimeout(unsigned long time);
void setPacketSize(int size);

//     2 bits          6 bits
// +--------------+-------------+---------+
// | payload type | file number | payload |
// +--------------+-------------+---------+
// 00 = file data
// 01 = file meta data
// 10 = ACK file data
// 11 = ACK file meta data
bool payloadType(uint8_t *message, size_t size);

void receiveFileProtocolMessage();


// --------------------------------------------- //
//         FILE TRANSFER - RECEIVER              //
// --------------------------------------------- //

bool ACKContent(uint16_t packetNumber);
bool ACKMetadata();
//      16 bits
// +---------------+---------+
// | packet number | content |
// +---------------+---------+
void receiveContent(uint8_t *message, size_t size);

//      2 bit         6 bits         16 bits
// +--------------+-------------+---------------+-----------+
// | payload type | file number | packet amount | file name |
// +--------------+-------------+---------------+-----------+
void receiveMetadata(uint8_t *message, size_t size);


// --------------------------------------------- //
//         FILE TRANSFER - TRANSMITTER           //
// --------------------------------------------- //

void receiveACK();
bool sendMetadata();
bool sendContents();
void transferFile(char* name);