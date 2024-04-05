#include "exec.h"

void setup()
{
  initialize();
  receiveMode();
}

void loop()
{
  readSerial();
  if (receivedFlag)
  {
    receiveFileProtocolMessage();
  }
}
