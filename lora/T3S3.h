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

// API for communicating with the radio itself, from RadioLib (https://github.com/jgromes/RadioLib)
SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

// Max amount of packets
#define MAX_PACKET_AMOUNT 65536
// Max length of the file name
#define FILENAME_SIZE 100

// Modes for the device to switch between
const int INACTIVE = 0;
const int RECEIVE_MODE = 1;
const int TRANSMIT_MODE = 2;
int mode = INACTIVE;

// Counters for amount of packets sent and received
int sendCounter = 0;
int receiveCounter = 0;

// Timeout defined in milliseconds
unsigned long timeoutTime = 500;
// Size of each packet
int packetSize = 30;

// Interrupt-driven receive flag
volatile bool receivedFlag = false;
// Interrupt-driven transmit flag
volatile bool transmittedFlag = true;

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