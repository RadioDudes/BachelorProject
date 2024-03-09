#include "T3S3.h"


void setup() {
    initialize();
    receiveMode();
}

void loop() {
    readSerial();

    if (receivedFlag) {
        handleReceivedMessage(receiveBasicMessage, receiveFailure);
        // handleReceivedMessage(receiveAndAppendToFile, receiveFailure); // Receive and add received message to /out.log
        // handleReceivedMessage(receiveFileProtocolMessage, receiveFailure); // Receive file transfer protocol message
    }
}