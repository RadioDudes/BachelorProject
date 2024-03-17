#include "T3S3.h"


void setup() {
    initialize();
    receiveMode();
}

void loop() {
    readSerial();

    if (mode == RECEIVE_MODE) {
      if (receivedFlag) {
        // handleReceivedMessage(receiveBasicMessage, receiveFailure);
        // handleReceivedMessage(receiveAndAppendToFile, receiveFailure); // Receive and add received message to /out.log
        handleReceivedMessage(receiveFileProtocolMessage, receiveFailure); // Receive file transfer protocol message
      }
    }
    
    if (mode == TRANSMIT_MODE) {
      int state = transmitNextInQueue();
   
      if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("transmission finished!"));

        u8g2->clearBuffer();
        u8g2->drawStr(0, 12, "Transmitting: OK!");
        u8g2->drawStr(0, 30, ("TX:" + String(sendCounter)).c_str());
        u8g2->sendBuffer();
      } else if (state == QUEUE_IS_EMPTY) {
        receiveMode();
      } else if (state != NOT_FINISHED_TRANSMITTING) {
        Serial.print(F("failed, code "));
        Serial.println(state);
      }
    }

}