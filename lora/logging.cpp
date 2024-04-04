#include "logging.h"

void printAsHex(uint8_t *message, size_t size) {
  for (int i = 0; i < size; i++) {
    char s[3];
    sprintf(s, "%02x", message[i]);
    Serial.print(s);
  }
}

void printTransmitPacket(uint8_t *message, size_t size) {
  Serial.println("===============================");
  Serial.println("      TRANSMITTING PACKET      ");
  Serial.println("===============================");
  Serial.print("| MESSAGE AS HEX | ");
  printAsHex(message, size);
  Serial.println();
}

void printReceivedPacket(uint8_t *message, size_t size) {
  Serial.println("===============================");
  Serial.println("      RECEIVED NEW PACKET      ");
  Serial.println("===============================");
  Serial.print("| MESSAGE AS HEX | ");
  printAsHex(message, size);
  Serial.println();
}

void printContentType(int type) {
  Serial.print("| CONTENT TYPE   | ");
  if (type == 0) {
    Serial.println("Data");
  } else if (type == 1) {
    Serial.println("Metadata");
  } else if (type == 2) {
    Serial.println("ACK data");
  } else if (type == 3) {
    Serial.println("ACK metadata");
  }
}

void printPacketAmount(uint16_t packetAmount) {
  Serial.print("| PACKET AMOUNT  | ");
  Serial.println(packetAmount);
}

void printPacketNumber(uint16_t packetNumber) {
  Serial.print("| PACKET NUMBER  | ");
  Serial.println(packetNumber);
}

void printFilename(String filename) {
  Serial.print("| FILENAME       | ");
  Serial.println(filename);
}

void printPacketContent(uint8_t *content, size_t size) {
  Serial.print("| PACKET CONTENT | ");
  printAsHex(content, size);
  Serial.println();
}

void printInfo(String info) {
  Serial.print("| INFO           | ");
  Serial.println(info);
}

void printError(String error) {
  Serial.print("| ERROR          | ");
  Serial.println(error);
}

void printErrorCode(int code) {
  Serial.print("| ERROR          | ");
  Serial.print("SX1280 error with code ");
  Serial.println(code);
}

void printAddMessage(size_t size) {
  Serial.print("| INFO           | Adding a message of size ");
  Serial.println(size);
}

void printAddNMessages(int amount, size_t size) {
  Serial.print("| INFO           | Adding ");
  Serial.print(amount);
  Serial.print("messages of size ");
  Serial.println(size);
}

void printSendingFile(char *filename) {
  Serial.print("| INFO           | Sending file ");
  Serial.println(filename);
}

void printErrorOpeningFile(char *filename) {
  Serial.print("| ERROR          | ");
  Serial.print("Could not open file ");
  Serial.println(filename);
}

void printTransmitFilePacket(uint8_t *message, size_t size, int packetNumber, uint8_t *content, size_t contentSize) {
  Serial.println("===============================");
  Serial.println("      TRANSMITTING PACKET      ");
  Serial.println("===============================");
  Serial.print("| PACKET AS HEX  | ");
  printAsHex(message, size);
  Serial.println();
  Serial.print("| PACKET NUMBER  | ");
  Serial.println(packetNumber);
  Serial.print("| PACKET SIZE    | ");
  Serial.println(size);
  Serial.print("| CONTENT AS HEX | ");
  printAsHex(content, contentSize);
  Serial.println();
}

void printFileTransferTotalTime(unsigned long time) {
  Serial.print("| INFO           | File transfer took ");
  Serial.print(time);
  Serial.println(" milliseconds");
}

void printInvalidFrequency(double freq) {
  Serial.print("| ERROR          | Invalid frequency ");
  Serial.println(freq);
}

void printInvalidBandwidth(double bw) {
  Serial.print("| ERROR          | Invalid bandwidth ");
  Serial.println(bw);
}

void printInvalidSpreadingFactor(uint8_t sf) {
  Serial.print("| ERROR          | Invalid spreading factor ");
  Serial.println(sf);
}

void printInvalidCodingRate(uint8_t cr) {
  Serial.print("| ERROR          | Invalid coding rate ");
  Serial.println(cr);
}

void printSetSpreadingFactor(uint8_t sf) {
  Serial.print("| INFO           | Set spreading factor to ");
  Serial.println(sf);
}

void printSetCodingRate(uint8_t cr) {
  Serial.print("| INFO           | Set coding rate to ");
  Serial.println(cr);
}

void printSetBandwidth(double bw) {
  Serial.print("| INFO           | Set bandwidth to ");
  Serial.println(bw);
}

void printSetFrequency(double freq) {
  Serial.print("| INFO           | Set frequency to ");
  Serial.println(freq);
}

void printSetTimeout(unsigned long timeout) {
  Serial.print("| INFO           | Set timeout to ");
  Serial.println(timeout);
}

void printSetPacketSize(int size) {
  Serial.print("| INFO           | Set packet size to ");
  Serial.println(size);
}