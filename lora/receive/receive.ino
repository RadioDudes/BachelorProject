#include "T3S3.h"


void setup() {
    initialize();
    receiveMode();
}

void loop() {
    readSerial();

    if (receivedFlag) {
        handleReceivedMessage(receiveBasicMessage, receiveFailure);
    }
}