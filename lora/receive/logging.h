void printAsHex(uint8_t *message, size_t size) {
  for (int i = 0; i < size; i++) {
    char s[3];
    sprintf(s, "%x", message[i]);
    Serial.print(s);
  }
}

void printTransmitPacket(uint8_t *message, size_t size) {
  Serial.println("===============================");
  Serial.println("      TRANSMITTING PACKET      ");
  Serial.println("===============================");
  Serial.print("| MESSAGE AS HEX | ");
  printAsHex(message, size);
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
    Serial.println("Metadata");
  } else if (type == 1) {
    Serial.println("Data");
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
  Serial.print("| PAKCET CONTENT | ");
  printAsHex(content, size);
  Serial.println();
}

void printInfo(String info) {
  Serial.print("| INFO             | ");
  Serial.println(info);
}

void printError(String error) {
  Serial.print("| ERROR            | ");
  Serial.println(error);
}

void printErrorCode(int code) {
  Serial.print("| ERROR            | ");
  Serial.print("SX1280 error with code ");
  Serial.println(code);
}

void printAddMessage(size_t size) {
  Serial.print("| INFO             | Adding a message of size ");
  Serial.println(size);
}

void printAddNMessages(int amount, size_t size) {
  Serial.print("| INFO             | Adding ");
  Serial.print(amount);
  Serial.print("messages of size ");
  Serial.println(size);
}

void printSendingFile(char *filename) {
  Serial.print("| INFO             | Sending file ");
  Serial.println(filename);
}

void printErrorOpeningFile(char *filename) {
  Serial.print("| ERROR            | ");
  Serial.print("Could not open file ");
  Serial.println(filename);
}
