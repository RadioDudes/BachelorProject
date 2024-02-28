#include "T3S3.h"

T3S3 t3s3;

void setup() {
    t3s3.initialize();
    t3s3.receiveMode();
}

void loop() {
    t3s3.readSerial();

    if (t3s3.receivedFlag) {
        t3s3.handleReceivedMessage(t3s3.receiveBasicMessage, t3s3.receiveFailure);
    }
}