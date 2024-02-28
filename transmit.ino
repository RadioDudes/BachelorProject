#include "T3S3.h"

T3S3 t3s3;
char message[] = "Hello, World!";


void setup() {
    t3s3.initialize();
    t3s3.transmitMode();
}

void loop() {
    t3s3.readSerial();

    if (transmissionState == RADIOLIB_ERR_NONE) {
        Serial.println(F("transmission finished!"));

        u8g2->clearBuffer();
        u8g2->drawStr(0, 12, "Transmitting: OK!");
        u8g2->drawStr(0, 30, ("TX:" + String(counter)).c_str());
        u8g2->sendBuffer();
    
    } else {
      Serial.print(F("failed, code "));
      Serial.println(transmissionState);
    }

    delay(2);

    int state = t3s3.transmitMessage(message);
}