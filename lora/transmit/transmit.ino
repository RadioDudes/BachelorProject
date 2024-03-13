#include "T3S3.h"

char message[] = "Hello, World!";

void setup() {
  initialize();
  transmitMode();
}

void loop() {
  readSerial();

  if (!stopFlag) {
    int state = transmitNextInQueue();

    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("transmission finished!"));

      u8g2->clearBuffer();
      u8g2->drawStr(0, 12, "Transmitting: OK!");
      u8g2->drawStr(0, 30, ("TX:" + String(sendCounter)).c_str());
      u8g2->sendBuffer();

    } else if (state != 1 && state != 2) {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
  }

  delay(2);
}
