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
    if (useRawProtocol) {
      RawProtocol::receiveFileProtocolMessage();
    }
    else {
      ACKProtocol::receiveFileProtocolMessage();
    }
    receiveMode();
  }
}
