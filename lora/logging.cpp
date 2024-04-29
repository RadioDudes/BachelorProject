#include "logging.h"

void Logging::printAsHex(uint8_t *message, size_t size) {
  for (int i = 0; i < size; i++) {
    char s[3];
    sprintf(s, "%02x", message[i]);
    Serial.print(s);
  }
}

void Logging::printTransmitPacket(uint8_t *message, size_t size) {
  Serial.println("===============================");
  Serial.println("      TRANSMITTING PACKET      ");
  Serial.println("===============================");
  Serial.print("| MESSAGE AS HEX | ");
  Logging::printAsHex(message, size);
  Serial.println();
}

void Logging::printReceivedPacket(uint8_t *message, size_t size) {
  Serial.println("===============================");
  Serial.println("      RECEIVED NEW PACKET      ");
  Serial.println("===============================");
  Serial.print("| MESSAGE AS HEX | ");
  Logging::printAsHex(message, size);
  Serial.println();
}

void Logging::printContentType(int type) {
  Serial.print("| CONTENT TYPE   | ");
  if (type == 0b000) {
    Serial.println("Data");
  } else if (type == 0b001) {
    Serial.println("Metadata");
  } else if (type == 0b010) {
    Serial.println("ACK data");
  } else if (type == 0b011) {
    Serial.println("ACK metadata");
  } else if (type == 0b100) {
    Serial.println("End of file");
  }
  else if (type == 0b101) {
    Serial.println("SF sync request");
  }
  else if (type == 0b110) {
    Serial.println("BW sync request");
  }
  else if (type == 0b111) {
    Serial.println("BW or SF sync acknowledgment");
  }
}

void Logging::printPacketAmount(uint16_t packetAmount) {
  Serial.print("| PACKET AMOUNT  | ");
  Serial.println(packetAmount);
}

void Logging::printPacketNumber(uint16_t packetNumber) {
  Serial.print("| PACKET NUMBER  | ");
  Serial.println(packetNumber);
}

void Logging::printFilename(String filename) {
  Serial.print("| FILENAME       | ");
  Serial.println(filename);
}

void Logging::printPacketContent(uint8_t *content, size_t size) {
  Serial.print("| PACKET CONTENT | ");
  Logging::printAsHex(content, size);
  Serial.println();
}

void Logging::printInfo(String info) {
  Serial.print("| INFO           | ");
  Serial.println(info);
}

void Logging::printError(String error) {
  Serial.print("| ERROR          | ");
  Serial.println(error);
}

void Logging::printErrorCode(int code) {
  Serial.print("| ERROR          | ");
  Serial.print("SX1280 error with code ");
  Serial.println(code);
}

void Logging::printAddMessage(size_t size) {
  Serial.print("| INFO           | Adding a message of size ");
  Serial.println(size);
}

void Logging::printAddNMessages(int amount, size_t size) {
  Serial.print("| INFO           | Adding ");
  Serial.print(amount);
  Serial.print("messages of size ");
  Serial.println(size);
}

void Logging::printSendingFile(char *filename) {
  Serial.print("| INFO           | Sending file ");
  Serial.println(filename);
}

void Logging::printErrorOpeningFile(char *filename) {
  Serial.print("| ERROR          | ");
  Serial.print("Could not open file ");
  Serial.println(filename);
}

void Logging::printTransmitFilePacket(uint8_t *message, size_t size, int packetNumber, uint8_t *content, size_t contentSize) {
  Serial.println("===============================");
  Serial.println("      TRANSMITTING PACKET      ");
  Serial.println("===============================");
  Serial.print("| PACKET AS HEX  | ");
  Logging::printAsHex(message, size);
  Serial.println();
  Serial.print("| PACKET NUMBER  | ");
  Serial.println(packetNumber);
  Serial.print("| PACKET SIZE    | ");
  Serial.println(size);
  Serial.print("| CONTENT AS HEX | ");
  Logging::printAsHex(content, contentSize);
  Serial.println();
}

void Logging::printFileTransferTotalTime(unsigned long time) {
  Serial.print("| INFO           | File transfer took ");
  Serial.print(time);
  Serial.println(" milliseconds");
}

void Logging::printPacketLoss(unsigned int packetLoss) {
  Serial.print("| INFO           | ");
  Serial.print(packetLoss);
  Serial.println(" packets lost");
}

void Logging::printDataRate(unsigned long bytesTransferred, unsigned long fileTransferTime) {
  Serial.print("| INFO           | Transferred ");
  Serial.print(bytesTransferred);
  Serial.print(" bytes, giving a data rate of ");
  Serial.print(8 * bytesTransferred / (fileTransferTime / 1000.0));
  Serial.println(" bps");
}

void Logging::printAvgSNR(double totalSNR, int packetsReceived) {
  Serial.print("| INFO           | Averaged an SNR of ");
  Serial.print(totalSNR / packetsReceived);
  Serial.println(" dB per packet.");
}

void Logging::printInvalidFrequency(double freq) {
  Serial.print("| ERROR          | Invalid frequency ");
  Serial.println(freq);
}

void Logging::printInvalidBandwidth(double bw) {
  Serial.print("| ERROR          | Invalid bandwidth ");
  Serial.println(bw);
}

void Logging::printInvalidSpreadingFactor(uint8_t sf) {
  Serial.print("| ERROR          | Invalid spreading factor ");
  Serial.println(sf);
}

void Logging::printInvalidCodingRate(uint8_t cr) {
  Serial.print("| ERROR          | Invalid coding rate ");
  Serial.println(cr);
}

void Logging::printSetSpreadingFactor(uint8_t sf) {
  Serial.print("| INFO           | Set spreading factor to ");
  Serial.println(sf);
}

void Logging::printSetCodingRate(uint8_t cr) {
  Serial.print("| INFO           | Set coding rate to ");
  Serial.println(cr);
}

void Logging::printSetBandwidth(double bw) {
  Serial.print("| INFO           | Set bandwidth to ");
  Serial.println(bw);
}

void Logging::printSetFrequency(double freq) {
  Serial.print("| INFO           | Set frequency to ");
  Serial.println(freq);
}

void Logging::printSetTimeout(unsigned long timeout) {
  Serial.print("| INFO           | Set timeout to ");
  Serial.println(timeout);
}

void Logging::printSetPacketSize(int size) {
  Serial.print("| INFO           | Set packet size to ");
  Serial.println(size);
}


void Logging::logFinishTransfer(char *filename, unsigned long bytesTransferred, unsigned long fileTransferTime, unsigned int packetsLost, char *logFile) {
  File file = SD.open(logFile, FILE_APPEND);
  double dataRate = 8 * bytesTransferred / (fileTransferTime / 1000.0);
  char finishTransferString[150];
  sprintf(finishTransferString, "Finished transfer of %s with %i packets lost \n", filename, packetsLost);
  file.write((uint8_t *) finishTransferString, strlen(finishTransferString));
  sprintf(finishTransferString, "Transferred %i bytes in %i milliseconds, with a data rate of %f bps \n", bytesTransferred, fileTransferTime, dataRate);
  file.write((uint8_t *) finishTransferString, strlen(finishTransferString));
  file.close();
}


void Logging::logStartTransfer(char *filename, unsigned long size, uint8_t sf, uint8_t cr, double freq, double bw, int packetSize, unsigned long timeout, char *logFile) {
  File file = SD.open(logFile, FILE_APPEND);
  char startTransferString[120];
  sprintf(startTransferString, "Starting transfer of %s, with size %i bytes.\n", filename, size);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Spreading factor is %i.\n", sf);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Bandwidth is %f.\n", bw);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Coding rate is %i.\n", cr);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Packet size is %i.\n", packetSize);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Frequency is %f.\n", freq);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Timeout is %i.\n", timeout);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  file.close();
}


void Logging::logFinishReceiving(char *filename, double totalSNR, double totalRSSI, int packetsReceived, char *logFile) {
  File file = SD.open(logFile, FILE_APPEND);
  char finishReceivingString[120];
  sprintf(finishReceivingString, "Finished receiving %s.\n", filename);
  file.write((uint8_t *) finishReceivingString, strlen(finishReceivingString));
  sprintf(finishReceivingString, "Avg SNR per packet is %f.\n", (totalSNR /  packetsReceived));
  file.write((uint8_t *) finishReceivingString, strlen(finishReceivingString));
  sprintf(finishReceivingString, "Avg RSSI per packet is %f.\n", (totalRSSI /  packetsReceived));
  file.write((uint8_t *) finishReceivingString, strlen(finishReceivingString));
  file.close();
}

void Logging::logStartReceiving(char *filename, uint16_t packetAmount, uint8_t sf, uint8_t cr, double freq, double bw, char *logFile) {
  File file = SD.open(logFile, FILE_APPEND);
  char startTransferString[120];
  sprintf(startTransferString, "Starting to receive %s, with %i packets.\n", filename, packetAmount);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Spreading factor is %i.\n", sf);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Bandwidth is %f.\n", bw);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Coding rate is %i.\n", cr);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  sprintf(startTransferString, "Frequency is %f.\n", freq);
  file.write((uint8_t *) startTransferString, strlen(startTransferString));
  file.close();
}

void Logging::logResetDevice(char *logFile) {
  File file = SD.open(logFile, FILE_APPEND);
  char *resetDeviceString = "Device was reset.\n";
  file.write((uint8_t *) resetDeviceString, strlen(resetDeviceString));
  file.close();
}

void Logging::printSyncErrorSF(uint8_t expectedSF, uint8_t actualSF) {
  Serial.print("| ERROR          | SF sync error. Expected SF ");
  Serial.print(expectedSF);
  Serial.print(" but received ");
  Serial.println(actualSF);
}

void Logging::printSyncErrorBW(double expectedBW, double actualBW) {
  Serial.print("| ERROR          | BW sync error. Expected BW ");
  Serial.print(expectedBW);
  Serial.print(" but received ");
  Serial.println(actualBW);
}
