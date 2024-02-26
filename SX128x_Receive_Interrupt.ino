/*
   RadioLib SX128x Receive with Interrupts Example

   This example listens for LoRa transmissions and tries to
   receive them. Once a packet is received, an interrupt is
   triggered. To successfully receive data, the following
   settings have to be the same on both transmitter
   and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word

   Other modules from SX128x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx128x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/




#include <RadioLib.h>
#include "boards.h"

SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

int counter = 0;

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

const unsigned int MAX_MESSAGE_LENGTH = 48;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if (!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

void setup() {
  initBoard();
  // When the power is turned on, a delay is required.
  delay(1500);

  // initialize SX1280 with default settings
  Serial.print(F("[SX1280] Initializing ... "));
  int state = radio.begin();
#ifdef HAS_DISPLAY
  if (u8g2) {
    if (state != RADIOLIB_ERR_NONE) {
      u8g2->clearBuffer();
      u8g2->drawStr(0, 12, "Initializing: FAIL!");
      u8g2->sendBuffer();
    }
  }
#endif
#ifdef EDP_DISPLAY
  if (state != RADIOLIB_ERR_NONE) {
    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(0, 15);
    display.println("Initializing: FAIL!");
    display.update();
  }
#endif

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


#ifdef LILYGO_T3_S3_V1_0
  // T3 S3 V1.1 with PA Version Set output power to 3 dBm    !!Cannot be greater than 3dbm!!
  int8_t TX_Power = 3;
#else
  // T3 S3 V1.2 (No PA) Version Set output power to 3 dBm    !!Cannot be greater than 3dbm!!
  int8_t TX_Power = 3;
#endif
  if (radio.setOutputPower(TX_Power) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("Selected output power is invalid for this module!"));
    while (true)
      ;
  }

  // set carrier frequency to 2410.5 MHz
  if (radio.setFrequency(2437.0) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    Serial.println(F("Selected frequency is invalid for this module!"));
    while (true)
      ;
  }

  // set bandwidth to 203.125 kHz
  if (radio.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
    Serial.println(F("Selected bandwidth is invalid for this module!"));
    while (true)
      ;
  }

  // set spreading factor to 10
  if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
    Serial.println(F("Selected spreading factor is invalid for this module!"));
    while (true)
      ;
  }

  // set coding rate to 6
  if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
    Serial.println(F("Selected coding rate is invalid for this module!"));
    while (true)
      ;
  }
  // set the function that will be called
  // when packet transmission is finished
  radio.setDio1Action(setFlag);

  // start listening for LoRa packets
  Serial.print(F("[SX1280] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}


void loop() {
  while (Serial.available() > 0) {
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;
  
    char inByte = Serial.read();

    if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1)) {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    } else {
      message[message_pos] = '\0';
      
      Serial.print(F("Message is: "));
      Serial.println(message);

      char *next = strtok(message, " ");

      if (strcmp(next, "frequency") == 0) {
        next = strtok(NULL, " ");
        if (next == NULL) {
          Serial.println(F("No frequency specified"));
          message_pos = 0;
          break;
        }

        float freq = atof(next);
        if (freq == 0) {
          Serial.print(next);
          Serial.println(F(" is an invalid frequency"));
          message_pos = 0;
          break;
        }

        if (radio.setFrequency(freq) == RADIOLIB_ERR_INVALID_FREQUENCY) {
          Serial.print(F("Frequency "));
          Serial.print(freq);
          Serial.println(F(" is invalid for this module!"));

          message_pos = 0;
          break;
        }

        Serial.print(F("Changed frequency to "));
        Serial.println(next);

      } else if (strcmp(next, "bandwidth") == 0 ) {
        next = strtok(NULL, " ");
        if (next == NULL) {
          Serial.println(F("No bandwidth specified"));
          message_pos = 0;
          break;
        }

        float bw = atof(next);
        Serial.println(next);
        Serial.println(bw);
        if (bw == 0) {
          Serial.print(next);
          Serial.println(F(" is an invalid bandwidth"));
          message_pos = 0;
          break;
        }

        if (radio.setBandwidth(bw) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
          Serial.print(F("Bandwidth "));
          Serial.print(bw);
          Serial.println(F(" is invalid for this module!"));
          message_pos = 0;
          break;
        }

        Serial.print(F("Changed bandwidth to "));
        Serial.println(next);
      } else if (strcmp(next, "sf") == 0 ) {
        next = strtok(message, " ");
        if (next == NULL) {
          Serial.println(F("No spreading factor specified"));
          message_pos = 0;
          break;
        }

        uint8_t sf = atoi(next);
        if (sf = 0) {
          Serial.print(next);
          Serial.println(F(" is an invalid spreading factor"));
          message_pos = 0;
          break;
        }

        if (radio.setSpreadingFactor(sf) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
          Serial.print(F("Spreading factor "));
          Serial.print(sf);
          Serial.println(F(" is invalid for this module!"));
          message_pos = 0;
          break;
        }

        Serial.print(F("Changed spreading factor to "));
        Serial.println(next);
      } else if (strcmp(next, "reset") == 0 ) {
        counter = 0;
        Serial.println(F("Reset counter to 0"));
      } else if (strcmp(next, "download") == 0) {
        next = strtok(NULL, " ");
        File file = SD.open(next);

        if (!file) {
          Serial.print("Filename by the name of ");
          Serial.print(next);
          Serial.println(" was not found.");
          message_pos = 0;
          break;
        }

        while (file.available()) {
          Serial.write(file.read());
        }
        file.close();
      } else if (strcmp(next, "upload") == 0 ) {
        next = strtok(NULL, " ");

        File file = SD.open(next, FILE_WRITE);
        if (!file) {
          Serial.print("Could not open file ");
          Serial.println(next);
          message_pos = 0;
          break;
        }

        next = strtok(NULL, "");

        if (!file.print(next)) {
          Serial.println("Could not write to file.");
          message_pos = 0;
          break;
        }

        file.close();

        // USED CODE FROM: https://randomnerdtutorials.com/esp32-microsd-card-arduino/#readFile
      } else if (strcmp(next, "rm") == 0 ) {
        next = strtok(NULL, " ");

        if (!file.remove(next)) {
          Serial.print("Could not remove file by the name of ");
          Serial.println(next);
          message_pos = 0;
          break;
        }

      } else {
        Serial.print(F("Did not understand command: "));
        Serial.println(message);
      }

      message_pos = 0;
    }
  }

  // check if the flag is set
  if (receivedFlag) {
    // disable the interrupt service routine while
    // processing the data
    enableInterrupt = false;

    // reset flag
    receivedFlag = false;

    // increase counter
    counter++;

    // you can read received data as an Arduino String
    String str;
    int state = radio.readData(str);

    //uint32_t counter;
    //int state = radio.readData((uint8_t *)&counter, 4);

    // you can also read received data as byte array
    /*
          byte byteArr[8];
          int state = radio.readData(byteArr, 8);
        */

    if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      Serial.println(F("[SX1280] Received packet!"));
      Serial.println(str);
      Serial.println(counter);
      Serial.println(radio.getRSSI());
      Serial.println(radio.getSNR());
      Serial.println(radio.getFrequencyError());
      Serial.println(radio.getPacketLength());

#ifdef HAS_DISPLAY
      if (u8g2) {
        u8g2->clearBuffer();
        char buf[256];
        u8g2->drawStr(0, 12, "Received OK!");
        snprintf(buf, sizeof(buf), "%s", str);
        u8g2->drawStr(0, 26, buf);
        snprintf(buf, sizeof(buf), "RSSI:%.2f", radio.getRSSI());
        u8g2->drawStr(0, 40, buf);
        snprintf(buf, sizeof(buf), "SNR:%.2f", radio.getSNR());
        u8g2->drawStr(0, 54, buf);
        u8g2->sendBuffer();
      }
#endif

#ifdef EDP_DISPLAY
      display.setRotation(1);
      display.fillScreen(GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
      display.setFont(&FreeMonoBold9pt7b);
      display.setCursor(0, 15);
      display.println("[SX128x] Received:");
      display.setCursor(0, 35);
      display.println("DATA:");
      display.setCursor(55, 35);
      display.println(counter);
      display.setCursor(0, 55);
      display.println("RSSI:");
      display.setCursor(55, 55);
      display.println(radio.getRSSI());
      display.setCursor(0, 75);
      display.println("SNR :");
      display.setCursor(55, 75);
      display.println(radio.getSNR());
      display.update();
#endif

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("[SX1280] CRC error!"));

    } else {
      // some other error occurred
      Serial.print(F("[SX1280] Failed, code "));
      Serial.println(state);
    }

    // put module back to listen mode
    radio.startReceive();

    // we're ready to receive more packets,
    // enable interrupt service routine
    enableInterrupt = true;
  }
}
