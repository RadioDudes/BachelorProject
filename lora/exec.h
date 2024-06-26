#pragma once

#include "filetransfer.h"
#include "filetransfer-raw.h"

#define MAX_MESSAGE_LENGTH 500

// --------------------------------------------- //
//               SERIAL COMMANDS                 //
// --------------------------------------------- //

volatile bool useRawProtocol = false;

void execCommand(char *message)
{
  char *next = strtok(message, " ");

  if (strcmp(next, "frequency") == 0 || strcmp(next, "freq") == 0)
  {
    next = strtok(NULL, " ");

    if (next == NULL)
    {
      Serial.println(F("No frequency specified"));
      return;
    }

    float freq = atof(next);
    if (freq == 0)
    {
      Serial.print(next);
      Serial.println(F(" is an invalid frequency"));
      return;
    }
    setFrequency(freq);
  }
  else if (strcmp(next, "bandwidth") == 0 || strcmp(next, "bw") == 0)
  {
    next = strtok(NULL, " ");
    if (next == NULL)
    {
      Serial.println(F("No bandwidth specified"));
      return;
    }
    float bw = atof(next);
    if (bw == 0)
    {
      Serial.print(next);
      Serial.println(F(" is an invalid bandwidth"));
      return;
    }
    ACKProtocol::syncBandwidth(bw);
  }
  else if (strcmp(next, "spreadingfactor") == 0 || strcmp(next, "sf") == 0)
  {
    next = strtok(NULL, " ");
    if (next == NULL)
    {
      Serial.println(F("No spreading factor specified"));
      return;
    }

    uint8_t sf = atoi(next);
    if (sf == 0)
    {
      Serial.print(next);
      Serial.println(F(" is an invalid spreading factor"));
      return;
    }

    ACKProtocol::syncSpreadingFactor(sf);
  }
  else if (strcmp(next, "reset") == 0)
  {
    ACKProtocol::resetVars();
    RawProtocol::resetVars();
    Logging::printInfo("Variables reset.");
  }
  else if (strcmp(next, "download") == 0)
  {
    next = strtok(NULL, " ");
    File file = SD.open(next);
    if (!file)
    {
      Serial.print("Filename by the name of ");
      Serial.print(next);
      Serial.println(" was not found.");
      return;
    }

    while (file.available())
    {
      Serial.write(file.read());
    }
    file.close();
    Serial.println();
  }
  else if (strcmp(next, "upload") == 0)
  {
    next = strtok(NULL, " ");
    File file = SD.open(next, FILE_WRITE);
    if (!file)
    {
      Serial.print("Could not open file ");
      Serial.println(next);
      return;
    }
    next = strtok(NULL, "");
    if (!file.print(next))
    {
      Serial.println("Could not write to file.");
      return;
    }

    file.close();
  }
  else if (strcmp(next, "send") == 0)
  {
    next = strtok(NULL, " ");

    sendCounter = 0;
    receiveCounter = 0;

    // Print to display
    char buffer[sizeof(next) + 20] = "Sending file ";
    strcat(buffer, next);
    Display::displayInfoTop(buffer);

    transmitMode();
    if (useRawProtocol)
    {
      RawProtocol::transferFile(next);
    }
    else
    {
      ACKProtocol::transferFile(next);
    }

    Display::displayInfoTop("Finished sending file");
  }
  else if (strcmp(next, "raw") == 0)
  {
    useRawProtocol = !useRawProtocol;
    if (useRawProtocol)
    {
      Logging::printInfo("Now using raw protocol!");
    }
    else
    {
      Logging::printInfo("Now using ACK protocol!");
    }
  }
  else if (strcmp(next, "receive") == 0)
  {
    sendCounter = 0;
    receiveCounter = 0;
    receiveMode();
  }
  else if (strcmp(next, "delete") == 0 || strcmp(next, "rm") == 0)
  {
    next = strtok(NULL, " ");

    if (!SD.remove(next))
    {
      Serial.print("Could not remove file by the name of ");
      Serial.println(next);
      return;
    }
  }
  else if (strcmp(next, "packetsize") == 0)
  {
    next = strtok(NULL, " ");
    setPacketSize(atoi(next));
  }
  else if (strcmp(next, "timeout") == 0)
  {
    next = strtok(NULL, " ");
    ACKProtocol::setTimeout(atoi(next));
  }
  else if (strcmp(next, "debug") == 0)
  {
    toggleDebug();
  }
  else
  {
    Serial.print(F("Did not understand command: "));
    Serial.println(message);
  }
}

// taken from https://www.programmingelectronics.com/serial-read/
void readSerial()
{
  while (Serial.available() > 0)
  {
    static char serialMessage[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    char inByte = Serial.read();

    if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1))
    {
      // Add the incoming byte to our message
      serialMessage[message_pos] = inByte;
      message_pos++;
    }
    else
    {
      serialMessage[message_pos] = '\0';
      Serial.print(F("Message is: "));
      Serial.println(serialMessage);
      execCommand(serialMessage);
      message_pos = 0;
    }
  }
}