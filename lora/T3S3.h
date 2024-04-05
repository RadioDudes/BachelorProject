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

#define DISPLAY_MODEL U8G2_SSD1306_128X64_NONAME_F_HW_I2C

// API for communicating with the radio itself, from RadioLib (https://github.com/jgromes/RadioLib)
extern SX1280 radio;

// Screen display itself
extern DISPLAY_MODEL *u8g2;

// Counters for amount of packets sent and received
extern int sendCounter;
extern int receiveCounter;

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