#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include "utilities.h"
#include <RadioLib.h>
#include <SD.h>
#include <FS.h>
#include <U8g2lib.h>
#include "queue.h"

SPIClass SDSPI(HSPI);

#define DISPLAY_MODEL U8G2_SSD1306_128X64_NONAME_F_HW_I2C
#define MAX_MESSAGE_LENGTH 500
#define INACTIVE 0
#define RECEIVE_MODE 1
#define TRANSMIT_MODE 2
#define NOT_FINISHED_TRANSMITTING 1;
#define QUEUE_IS_EMPTY 2;


int sendCounter = 0;
int receiveCounter = 0;
int mode = INACTIVE;
volatile bool receivedFlag = false;
volatile bool enableInterrupt = false;
volatile bool transmittedFlag = false;
volatile bool stopFlag = true;
SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
fs::SDFS sd = SD;
DISPLAY_MODEL *u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
Ticker ledTicker;
struct queue *messageQueue = createQueue();

// For file transfer protocol
// Assumption is that only one file is being transferred at a time

char *filename;
uint16_t packetAmount;
uint16_t timeout = 400; // Timeout in ms

// ---------------------------------------------------------- //

void setFlag(void) {
    if (mode == RECEIVE_MODE && enableInterrupt) {
        receivedFlag = true;
    } else if (mode == TRANSMIT_MODE) {
        transmittedFlag = true;
    }
}

void initialize() {

    Serial.begin(115200);
    Serial.println("initBoard");
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

    Wire.begin(I2C_SDA, I2C_SCL);

    #if LED_ON == LOW
    gpio_hold_dis(GPIO_NUM_4);
    #endif

    pinMode(BOARD_LED, OUTPUT);
    ledTicker.attach_ms(500, []() {
        static bool level;
        digitalWrite(BOARD_LED, level);
        level = !level;
    });

    Wire.beginTransmission(0x3C);
    if (Wire.endTransmission() == 0) {
        Serial.println("Started OLED");
        u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->setFlipMode(0);
        u8g2->setFontMode(1); // Transparent
        u8g2->setDrawColor(1);
        u8g2->setFontDirection(0);
        u8g2->firstPage();
        do {
            u8g2->setFont(u8g2_font_inb19_mr);
            u8g2->drawStr(0, 30, "LilyGo");
            u8g2->drawHLine(2, 35, 47);
            u8g2->drawHLine(3, 36, 47);
            u8g2->drawVLine(45, 32, 12);
            u8g2->drawVLine(46, 33, 12);
            u8g2->setFont(u8g2_font_inb19_mf);
            u8g2->drawStr(58, 60, "LoRa");
        } while ( u8g2->nextPage() );
        u8g2->sendBuffer();
        u8g2->setFont(u8g2_font_fur11_tf);
        delay(3000);
    }

    u8g2->setFont(u8g2_font_ncenB08_tr);
    
    pinMode(SDCARD_MISO, INPUT_PULLUP);
    SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);

    u8g2->clearBuffer();

    if (!SD.begin(SDCARD_CS, SDSPI)) {
        Serial.println("setupSDCard FAIL");
        do {
            u8g2->setCursor(0, 16);
            u8g2->println("SDCard FAILED");
            ;
        } while (u8g2->nextPage());
    } else {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        Serial.print("setupSDCard PASS . SIZE = ");
        Serial.print(cardSize / 1024.0);
        Serial.println(" GB");
    
    do {
        u8g2->setCursor(0, 16);
        u8g2->print("SDCard:");
        u8g2->print(cardSize / 1024.0);
        u8g2->println(" GB");
        } while (u8g2->nextPage());

        u8g2->sendBuffer();
    }
    delay(3000);

    u8g2->clearBuffer();
    do
    {
        u8g2->setCursor(0, 16);
        u8g2->println("Waiting for command");
        ;
    } while (u8g2->nextPage());

    // ↑                From boards.h               ↑
    // ↓ From SX128x receive and transmit ino files ↓

    delay(1500);

    Serial.print(F("[SX1280] Initializing ... "));
    int state = radio.begin();
    if (state != RADIOLIB_ERR_NONE) {
        u8g2->clearBuffer();
        u8g2->drawStr(0, 12, "Initializing: FAIL!");
        u8g2->sendBuffer();
    }
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
        ;
    }

    #if defined(RADIO_RX_PIN) && defined(RADIO_TX_PIN)
    //Set ANT Control pins
    radio.setRfSwitchPins(RADIO_RX_PIN, RADIO_TX_PIN);
    #endif

    if (radio.setOutputPower(3) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
        while (true)
        ;
    }

    if (radio.setFrequency(2400.0) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true)
        ;
    }

    if (radio.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
        while (true)
        ;
    }

    if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
        while (true)
        ;
    }

    if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.println(F("Selected coding rate is invalid for this module!"));
        while (true)
        ;
    }

    radio.setDio1Action(setFlag);
}


// --------------------------------------------- //
//              TRANSMITTING CODE                //
// --------------------------------------------- //


void transmitMode() {
    enableInterrupt = false;
    receivedFlag = false;
    mode = TRANSMIT_MODE;
    transmittedFlag = true;
    enableInterrupt = true;
}

void startTransmit() {
  stopFlag = false;
}

void stopTransmit() {
  stopFlag = true;
}

int transmitMessage(char *message) {
    if (!transmittedFlag) {
        return NOT_FINISHED_TRANSMITTING;
    }
    enableInterrupt = false;
    transmittedFlag = false;
    Serial.print(F("[SX1280] Sending message "));
    Serial.println(message);
    sendCounter++;
    int state = radio.startTransmit(message);
    enableInterrupt = true;
    return state;
}

int transmitNextInQueue() {
  if (!transmittedFlag) {
        return NOT_FINISHED_TRANSMITTING;
  }
  char message[255];
  if (!dequeue(messageQueue, message, 255)) {
    return QUEUE_IS_EMPTY;
  }
  return transmitMessage(message);
}

bool addMessage(char *message, int amount) {
  Serial.print(F("Adding "));
  Serial.print(amount);
  Serial.print(F(" messages of size "));
  Serial.println(strlen(message));
  bool result = true;
  for (int i = 0; i < amount; i++) {
    result = result && enqueue(messageQueue, message, strlen(message));
  }
  return result;
}

bool ACKContent(uint16_t packetNumber) {
  uint8_t message[3];
  message[0] = (uint8_t) 0x10000000;
  message[1] = (uint8_t) (packetNumber >> 8);
  message[2] = (uint8_t) (packetNumber & 0x00FF);
  return addMessage(message, 1);
}

bool ACKMetadata() {
  return addMessage((uint8_t) 0x11000000, 1);
}

// --------------------------------------------- //
//                RECEIVING CODE                 //
// --------------------------------------------- //


void receiveMode() {
    enableInterrupt = false;
    mode = RECEIVE_MODE;
    radio.startReceive();
    enableInterrupt = true;
}

void handleReceivedMessage(void (*sucessCallback)(uint8_t *, size_t), void (*failureCallback)(int)) {
    enableInterrupt = false;
    receivedFlag = false;
    receiveCounter++;

    size_t receivedPacketSize = radio.getPacketLength();
    uint8_t message[receivedPacketSize];
    int state = radio.readData(message, receivedPacketSize);

    if (state == RADIOLIB_ERR_NONE) {
        (*sucessCallback)(message, receivedPacketSize);
    } else {
        (*failureCallback)(state);
    }

    radio.startReceive();
    enableInterrupt = true;
}

bool appendToFile(uint8_t *message, size_t size, char *path) {
  File file = SD.open(path, FILE_APPEND);

  if (!file) {
      Serial.print("Could not open ");
      Serial.println(path);
      return false;
    }

    if (!file.write(message, size)) {
      Serial.print("Could not write to file ");
      Serial.println(path);
      return false;
    }

    file.close();
    return true;
}

// Receives message and prints it on screen as well as serial
void receiveBasicMessage(uint8_t *message, size_t size) {
    Serial.println(F("[SX1280] Received packet!"));
    Serial.println((char *)message);
    Serial.println(receiveCounter);
    Serial.println(radio.getRSSI());
    Serial.println(radio.getSNR());
    Serial.println(radio.getFrequencyError());
    Serial.println(radio.getPacketLength());
    u8g2->clearBuffer();
    char buf[256];
    u8g2->drawStr(0, 12, "Received OK!");
    snprintf(buf, sizeof(buf), "%s", (char *)message);
    u8g2->drawStr(0, 26, buf);
    snprintf(buf, sizeof(buf), "RSSI:%.2f", radio.getRSSI());
    u8g2->drawStr(0, 40, buf);
    snprintf(buf, sizeof(buf), "SNR:%.2f", radio.getSNR());
    u8g2->drawStr(0, 54, buf);
    u8g2->sendBuffer();
}

// Receives and appends to file /out.log
void receiveAndAppendToFile(uint8_t *message, size_t size) {
  receiveBasicMessage(message, size);
  
  if (appendToFile(message, size, "/out.log")) {
    Serial.println("Wrote to /out.log");
  }

}

// Register that a certain packet number has been received
void registerPacketNumber(uint16_t number) {
  Serial.print("Registered packet number ");
  Serial.println(number);
}

// Following methods implement a simple protocol for file transfer

//      16 bits
// +---------------+---------+
// | packet number | content |
// +---------------+---------+

void receiveContent(uint8_t *message, size_t size) {
  uint16_t packetNumber = ((uint16_t *) message)[0];
  registerPacketNumber(packetNumber);
  message += 2;
  Serial.print("Received content ");
  Serial.println((char *)message);
  appendToFile(message, size, filename);
  ACKContent(packetNumber);
}

//      2 bit         6 bits         16 bits
// +--------------+-------------+---------------+-----------+
// | payload type | file number | packet amount | file name |
// +--------------+-------------+---------------+-----------+

void receiveMetadata(uint8_t *message, size_t size) {
  char payloadType = message[0];
  message++;
  packetAmount = ((uint16_t *) message)[0];
  Serial.print("Packet amount set to ");
  Serial.println(packetAmount);
  message += 2;
  filename = (char *) message;
  Serial.print("Filename set to ");
  Serial.print(filename);
  if (!ACKMetadata()) {

  }
}

//     2 bits          6 bits
// +--------------+-------------+---------+
// | payload type | file number | payload |
// +--------------+-------------+---------+
// 00 = file data
// 01 = file meta data
// 10 = ACK file data
// 11 = ACK file meta data

void payloadType(uint8_t *message, size_t size) {
  if (message[0] >> 6 == 0) {
    Serial.println("The packet is a content packet");
    receiveContent(message, size);
  } else if (message[0] >> 6 == 1 ){
    Serial.println("The packet is a metadata packet");
    receiveMetadata(message++, size);
  } else if (message[0] >> 6 == 2) {
    Serial.println("The packet is a content ACK packet");
  } else {
    Serial.println("The packet is a content ACK packet");
  }
}

void receiveFileProtocolMessage(uint8_t *message, size_t size) {
  Serial.print("Received message: ");
  Serial.println((char *) message);
  payloadType(message, size);
}

void receiveFailure(int state) {
    if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("[SX1280] CRC error!"));

    } else {
    // some other error occurred
    Serial.print(F("[SX1280] Failed, code "));
    Serial.println(state);
    }
}


// --------------------------------------------- //
//               SERIAL COMMANDS                 //
// --------------------------------------------- //


void execCommand(char *message) {
    char *next = strtok(message, " ");

    if (strcmp(next, "frequency") == 0 || strcmp(next, "freq") == 0) 
    {
      next = strtok(NULL, " ");

      if (next == NULL) {
          Serial.println(F("No frequency specified"));
          return;
      }

      float freq = atof(next);
      if (freq == 0) {
          Serial.print(next);
          Serial.println(F(" is an invalid frequency"));
          return;
      }

      if (radio.setFrequency(freq) == RADIOLIB_ERR_INVALID_FREQUENCY) {
          Serial.print(F("Frequency "));
          Serial.print(freq);
          Serial.println(F(" is invalid for this module!"));
          return;
      }

      Serial.print(F("Changed frequency to "));
      Serial.println(next);
    } else
    if (strcmp(next, "bandwidth") == 0 || strcmp(next, "bw") == 0) 
    {
      next = strtok(NULL, " ");
      if (next == NULL) {
          Serial.println(F("No bandwidth specified"));
          return;
      }

      float bw = atof(next);
      if (bw == 0) {
          Serial.print(next);
          Serial.println(F(" is an invalid bandwidth"));
          return;
      }

      if (radio.setBandwidth(bw) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
          Serial.print(F("Bandwidth "));
          Serial.print(bw);
          Serial.println(F(" is invalid for this module!"));
          return;
      }

      Serial.print(F("Changed bandwidth to "));
      Serial.println(next);
    } else
    if (strcmp(next, "spreadingfactor") == 0 || strcmp(next, "sf") == 0) 
    {
      next = strtok(NULL, " ");
      if (next == NULL) {
          Serial.println(F("No spreading factor specified"));
          return;
      }

      uint8_t sf = atoi(next);
      if (sf = 0) {
          Serial.print(next);
          Serial.println(F(" is an invalid spreading factor"));
          return;
      }

      if (radio.setSpreadingFactor(sf) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
          Serial.print(F("Spreading factor "));
          Serial.print(sf);
          Serial.println(F(" is invalid for this module!"));
          return;
      }

      Serial.print(F("Changed spreading factor to "));
      Serial.println(next);
    } else
    if (strcmp(next, "reset") == 0) 
    {
      receiveCounter = 0;
      sendCounter = 0;
    } else
    if (strcmp(next, "download") == 0) 
    {
      next = strtok(NULL, " ");
      File file = SD.open(next);

      if (!file) {
          Serial.print("Filename by the name of ");
          Serial.print(next);
          Serial.println(" was not found.");
          return;
      }

      while (file.available()) {
      Serial.write(file.read());
      }
      file.close();
    } else
    if (strcmp(next, "upload") == 0) 
    {
      next = strtok(NULL, " ");

      File file = SD.open(next, FILE_WRITE);
      if (!file) {
          Serial.print("Could not open file ");
          Serial.println(next);
          return;
      }

      next = strtok(NULL, "");

      if (!file.print(next)) {
          Serial.println("Could not write to file.");
          return;
      }

      file.close();
    } else
    if (strcmp(next, "delete") == 0 || strcmp(next, "rm") == 0) 
    {
      next = strtok(NULL, " ");

      if (!SD.remove(next)) {
          Serial.print("Could not remove file by the name of ");
          Serial.println(next);
          return;
      }
    } else
    if (strcmp(next, "start") == 0) 
    {
      startTransmit();
    } else
    if (strcmp(next, "stop") == 0) 
    {
      stopTransmit();
    } else
    if (strcmp(next, "send") == 0) 
    {
      char *message = "Hello, World!";
      next = strtok(NULL, " ");

      if (next == NULL) {
          Serial.println(F("No message amount specified"));
          return;
      }

      int amount = atoi(next);
      if (amount == 0) {
          Serial.println(F("No messages sent"));
          return;
      }

      next = strtok(NULL, "");
      if (next != NULL) {
        message = next;
      }

      if (!addMessage(next, amount)) {
        Serial.println(F("Not all messages were put in queue"));
      } else {
        Serial.println(F("All messages were queued"));
      }

    } else
    if (strcmp(next, "receive") == 0) 
    {

    } else
    if (strcmp(next, "remaining") == 0)
    {
      int s = size(messageQueue);
      if (s == -1) {
        Serial.println(F("Queue is null"));
        return;
      }
      Serial.print(s);
      Serial.println(F(" remaining messages in queue"));
    } else {
      Serial.print(F("Did not understand command: "));
      Serial.println(message);
    }
}

void readSerial() {
    while (Serial.available() > 0) {
      static char serialMessage[MAX_MESSAGE_LENGTH];
      static unsigned int message_pos = 0;

      char inByte = Serial.read();

      if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1)) {
          //Add the incoming byte to our message
          serialMessage[message_pos] = inByte;
          message_pos++;
      } else {
          serialMessage[message_pos] = '\0';
          Serial.print(F("Message is: "));
          Serial.println(serialMessage);
          execCommand(serialMessage);
          message_pos = 0;
      }
    }
}
    