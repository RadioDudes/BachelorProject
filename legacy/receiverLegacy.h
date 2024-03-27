#include "T3S3.h"

class receiverLegacy
{
    void handleReceivedMessage(void (*sucessCallback)(uint8_t *, size_t), void (*failureCallback)(int))
    {
        receivedFlag = false;
        receiveCounter++;

        size_t receivedPacketSize = radio.getPacketLength();
        uint8_t message[receivedPacketSize];
        int state = radio.readData(message, receivedPacketSize);

        if (state == RADIOLIB_ERR_NONE)
        {
            (*sucessCallback)(message, receivedPacketSize);
        }
        else
        {
            (*failureCallback)(state);
        }

        // radio.startReceive();
        // enableInterrupt = true;
    }
    // Receives message and prints it on screen as well as serial
    void receiveBasicMessage(uint8_t *message, size_t size)
    {
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
    void receiveAndAppendToFile(uint8_t *message, size_t size)
    {
        receiveBasicMessage(message, size);

        if (appendToFile(message, size, "/out.log"))
        {
            Serial.println("Wrote to /out.log");
        }
    }
    
    void receiveFailure(int state)
    {
        if (state == RADIOLIB_ERR_CRC_MISMATCH)
        {
            // packet was received, but is malformed
            printError("SX1280 CRC Error");
        }
        else
        {
            // some other error occurred
            printErrorCode(state);
        }
        receiveMode();
    }
};