#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include "utilities.h"
#include <RadioLib.h>
#include <SD.h>
#include <FS.h>
#include <U8g2lib.h>
#include "logging.h"

// Max amount of packets
#define MAX_PACKET_AMOUNT 65536
// Max length of the file name
#define FILENAME_SIZE 100

// API for communicating with the radio itself, from RadioLib (https://github.com/jgromes/RadioLib)
extern SX1280 radio;

// Modes for the device to switch between
extern const int INACTIVE;
extern const int RECEIVE_MODE;
extern const int TRANSMIT_MODE;
extern int mode;

// Counters for amount of packets sent and received
extern int sendCounter;
extern int receiveCounter;

// Timeout defined in milliseconds
extern unsigned long timeoutTime;
// Size of each packet
extern int packetSize;

// Interrupt-driven receive flag
extern volatile bool receivedFlag;
// Interrupt-driven transmit flag
extern volatile bool transmittedFlag;

// --------------------------------------------- //
//              FUNCTIONALITY CODE               //
// --------------------------------------------- //

void setSpreadingFactor(uint8_t sf);
void setCodingRate(uint8_t cr);
void setFrequency(double freq);
void setBandwidth(double bw);
void setTimeout(unsigned long time);
void setPacketSize(int size);
void initialize();

// --------------------------------------------- //
//              TRANSMITTING CODE                //
// --------------------------------------------- //

void transmitMode();
bool transmitMessage(uint8_t *message, size_t len);

// --------------------------------------------- //
//                RECEIVING CODE                 //
// --------------------------------------------- //

void receiveMode();
bool appendToFile(uint8_t *message, size_t size, char *path);
bool receiveMessage(uint8_t *buffer, size_t messageSize);