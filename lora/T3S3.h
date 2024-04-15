#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include "utilities.h"
#include <RadioLib.h>
#include <SD.h>
#include <FS.h>
#include "display.h"
#include "logging.h"

// API for communicating with the radio itself, from RadioLib (https://github.com/jgromes/RadioLib)
extern SX1280 radio;

// Boolean for enabling/disabling debug info (screen and serial prints)
extern bool enableDebug;

// Counters for amount of packets sent and received
extern int sendCounter;
extern int receiveCounter;
extern unsigned long bytesTransferred;
extern unsigned long bytesReceived;

// Interrupt-driven receive flag
extern volatile bool receivedFlag;
// Interrupt-driven transmit flag
extern volatile bool transmittedFlag;

extern double frequency;
extern double bandwidth;
extern uint8_t codingRate;
extern uint8_t spreadingFactor;
extern int packetSize;

// --------------------------------------------- //
//              FUNCTIONALITY CODE               //
// --------------------------------------------- //

void setPacketSize(int size);
void setSpreadingFactor(uint8_t sf);
void setCodingRate(uint8_t cr);
void setFrequency(double freq);
void setBandwidth(double bw);
void toggleDebug();
void initialize();

// --------------------------------------------- //
//              TRANSMITTING CODE                //
// --------------------------------------------- //

void transmitMode();
bool transmitMessage(uint8_t *message, size_t len, bool printPacket = true);

// --------------------------------------------- //
//                RECEIVING CODE                 //
// --------------------------------------------- //

void receiveMode();
bool appendToFile(uint8_t *message, size_t size, char *path, File* file);
bool receiveMessage(uint8_t *buffer, size_t messageSize);