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

int transmitMessage(uint8_t *message, size_t len) {
    if (!transmittedFlag) {
        return NOT_FINISHED_TRANSMITTING;
    }
    enableInterrupt = false;
    transmittedFlag = false;
    Serial.print(F("[SX1280] Sending message "));
    char readMessage[255];
    memcpy(readMessage, message, len);
    readMessage[len] = '\0';
    Serial.println(readMessage);
    sendCounter++;
    int state = radio.startTransmit(message, len);
    enableInterrupt = true;
    return state;
}

int transmitNextInQueue() {
  if (!transmittedFlag) {
        return NOT_FINISHED_TRANSMITTING;
  }
  uint8_t message[255];
  size_t length = dequeue(messageQueue, message);
  if (length == -1) {
    return QUEUE_IS_EMPTY;
  }
  return transmitMessage(message, length);
}

void receiveMode() {
    enableInterrupt = false;
    mode = RECEIVE_MODE;
    radio.startReceive();
    enableInterrupt = true;
}

void handleReceivedMessage(void (*sucessCallback)(String), void (*failureCallback)(int)) {
    enableInterrupt = false;
    receivedFlag = false;
    receiveCounter++;
    
    String str;
    int state = radio.readData(str);

    if (state == RADIOLIB_ERR_NONE) {
        (*sucessCallback)(str);
    } else {
        (*failureCallback)(state);
    }

    radio.startReceive();
    enableInterrupt = true;
}

void receiveBasicMessage(String message) {
    Serial.println(F("[SX1280] Received packet!"));
    Serial.println(message);
    Serial.println(receiveCounter);
    Serial.println(radio.getRSSI());
    Serial.println(radio.getSNR());
    Serial.println(radio.getFrequencyError());
    Serial.println(radio.getPacketLength());
    u8g2->clearBuffer();
    char buf[256];
    u8g2->drawStr(0, 12, "Received OK!");
    snprintf(buf, sizeof(buf), "%s", message);
    u8g2->drawStr(0, 26, buf);
    snprintf(buf, sizeof(buf), "RSSI:%.2f", radio.getRSSI());
    u8g2->drawStr(0, 40, buf);
    snprintf(buf, sizeof(buf), "SNR:%.2f", radio.getSNR());
    u8g2->drawStr(0, 54, buf);
    u8g2->sendBuffer();
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

bool addMessage(uint8_t *message, size_t len) {
  Serial.print("Adding a message of size ");
  Serial.println(len);
  return enqueue(messageQueue, message, len);
}

bool addMessageN(uint8_t *message, size_t len, int amount) {
  bool result = true;
  while (amount > 0) {
    result = result && addMessage(message, len);
    amount--;
  }
  return result;
}


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
    if (strcmp(next, "sendfile") == 0) {
      next = strtok(NULL, " ");
      File file = SD.open(next);

      if (!file) {
          Serial.print("Could not open file ");
          Serial.println(next);
          return;
      }
      Serial.print("Sending file: ");
      Serial.println(next);

      int packetCount = 0;
      int MAX_PACKET_AMOUNT = 65535;
      while (file.available()) {
        if (packetCount >= MAX_PACKET_AMOUNT) {
          Serial.println("Packet amount exceded!");
          break;
        }

        uint8_t buffer[30];
        size_t readBytes = file.read(buffer, 30);

        if (!addMessage(buffer, readBytes)) {
          Serial.println("Could not add file chunk to queue");
        } else {
          Serial.println("Added chunk to queue.");
        }
        packetCount++;
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

      if (!addMessageN((uint8_t*) message, strlen(message), amount)) {
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