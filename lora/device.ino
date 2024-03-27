#include "exec.h"

void setup()
{
  initialize();
}

void loop()
{
  readSerial();
  if (mode == RECEIVE_MODE)
  {
    if (receivedFlag)
    {
      receiveFileProtocolMessage();
    }
  }
}