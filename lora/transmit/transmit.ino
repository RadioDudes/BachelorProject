#include "exec.h"

void setup() {
  initialize();
  transmitMode();
}

void loop() {
  readSerial();
  delay(2);
}
