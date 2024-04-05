#include "T3S3.h"

class transmitterLegacy {
    public:
    const int NOT_FINISHED_TRANSMITTING = 1;
    const int QUEUE_IS_EMPTY = 2;
    volatile bool stopFlag = true;
    volatile bool enableInterrupt = false;
    struct queue *messageQueue = createQueue();

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
        printTransmitPacket(message, len);
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

    bool addMessage(uint8_t *message, size_t len) {
        printAddMessage(len);
        return enqueue(messageQueue, message, len);
    }

    bool addMessageN(uint8_t *message, size_t len, int amount) {
        printAddNMessages(amount, len);
        bool result = true;
        while (amount > 0) {
            result = result && addMessage(message, len);
            amount--;
        }
        return result;
    }

    bool sendMessage(uint8_t *message, size_t len) {
        int state = transmitMessage(message, len);

        if (state == RADIOLIB_ERR_NONE) {
            Serial.println(F("transmission finished!"));

            u8g2->clearBuffer();
            u8g2->drawStr(0, 12, "Transmitting: OK!");
            u8g2->drawStr(0, 30, ("TX:" + String(sendCounter)).c_str());
            u8g2->sendBuffer();

        } else if (state != 1 && state != 2) {
            Serial.print(F("failed, code "));
            Serial.println(state);
            return false;
        }
        return true;
    }
};