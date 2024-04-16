#pragma once
#include <stdint.h>
#include <stddef.h>
#include "T3S3.h"

namespace Logging
{
    void printAsHex(uint8_t *message, size_t size);

    void printTransmitPacket(uint8_t *message, size_t size);

    void printReceivedPacket(uint8_t *message, size_t size);

    void printContentType(int type);

    void printPacketAmount(uint16_t packetAmount);

    void printPacketNumber(uint16_t packetNumber);

    void printFilename(String filename);

    void printPacketContent(uint8_t *content, size_t size);

    void printInfo(String info);

    void printError(String error);

    void printErrorCode(int code);

    void printAddMessage(size_t size);

    void printAddNMessages(int amount, size_t size);

    void printSendingFile(char *filename);

    void printErrorOpeningFile(char *filename);

    void printTransmitFilePacket(uint8_t *message, size_t size, int packetNumber, uint8_t *content, size_t contentSize);

    void printFileTransferTotalTime(unsigned long time);

    void printPacketLoss(unsigned int packetLoss);

    void printDataRate(unsigned long bytesTransferred, unsigned long fileTransferTime);

    void printInvalidFrequency(double freq);

    void printInvalidBandwidth(double bw);

    void printInvalidSpreadingFactor(uint8_t sf);

    void printInvalidCodingRate(uint8_t cr);

    void printSetSpreadingFactor(uint8_t sf);

    void printSetCodingRate(uint8_t cr);

    void printSetBandwidth(double bw);

    void printSetFrequency(double freq);

    void printSetTimeout(unsigned long timeout);

    void printSetPacketSize(int size);

    void logFinishTransfer(char *filename, unsigned long bytesTransferred, unsigned long fileTransferTime, unsigned int packetsLost, char *logFile);

    void logStartTransfer(char *filename, unsigned long size, uint8_t sf, uint8_t cr, double freq, double bw, int packetSize, unsigned long timeout, char *logFile);

    void logStartReceiving(char *filename, uint16_t packetAmount, uint8_t sf, uint8_t cr, double freq, double bw, char *logFile);

    void logFinishReceiving(char *filename, char *logFile);

    void logResetDevice(char *logFile);

    void printSyncErrorSF(uint8_t expectedSF, uint8_t actualSF);
    void printSyncErrorBW(double expectedBW, double actualBW);
}