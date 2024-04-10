#include "T3S3.h"

SPIClass SDSPI(HSPI);

fs::SDFS sd = SD;
Ticker ledTicker;

// API for communicating with the radio itself, from RadioLib (https://github.com/jgromes/RadioLib)
SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

// Boolean for enabling/disabling debug info (screen and serial prints)
bool enableDebug = true;

// Counters for amount of packets sent and received
int sendCounter = 0;
int receiveCounter = 0;
// Counters for amount of bytes sent and received
unsigned long bytesTransferred = 0;
unsigned long bytesReceived = 0;

// Interrupt-driven receive flag
volatile bool receivedFlag = false;
// Interrupt-driven transmit flag
volatile bool transmittedFlag = true;

// Modes for the device to switch between
const int INACTIVE = 0;
const int RECEIVE_MODE = 1;
const int TRANSMIT_MODE = 2;
int mode = INACTIVE;

// Size of each packet
int packetSize = 30;

// ---------------------------------------------------------- //

void setPacketSize(int size)
{
    packetSize = size;
    Logging::printSetPacketSize(size);
}

void setSpreadingFactor(uint8_t sf)
{
  if (radio.setSpreadingFactor(sf) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR)
  {
    Logging::printInvalidSpreadingFactor(sf);
    return;
  }
  Logging::printSetSpreadingFactor(sf);
}

void setCodingRate(uint8_t cr)
{
  if (radio.setCodingRate(cr) == RADIOLIB_ERR_INVALID_CODING_RATE)
  {
    Logging::printInvalidCodingRate(cr);
    return;
  }
  Logging::printSetCodingRate(cr);
}

void setFrequency(double freq)
{
  if (radio.setFrequency(freq) == RADIOLIB_ERR_INVALID_FREQUENCY)
  {
    Logging::printInvalidFrequency(freq);
    return;
  }
  Logging::printSetFrequency(freq);
}

void setBandwidth(double bw)
{
  if (radio.setBandwidth(bw) == RADIOLIB_ERR_INVALID_BANDWIDTH)
  {
    Logging::printInvalidBandwidth(bw);
    return;
  }
  Logging::printSetBandwidth(bw);
}

void toggleDebug()
{
  enableDebug = !enableDebug;
  enableDebug ? Logging::printInfo("Debug mode enabled") : Logging::printInfo("Debug mode disabled");
}

void setFlag(void)
{
  if (mode == RECEIVE_MODE)
  {
    receivedFlag = true;
  }
  else if (mode == TRANSMIT_MODE)
  {
    transmittedFlag = true;
  }
}

void initialize()
{

  Serial.begin(115200);
  Serial.println("initBoard");
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

  Wire.begin(I2C_SDA, I2C_SCL);

#if LED_ON == LOW
  gpio_hold_dis(GPIO_NUM_4);
#endif

  pinMode(BOARD_LED, OUTPUT);
  ledTicker.attach_ms(500, []()
                      {
    static bool level;
    digitalWrite(BOARD_LED, level);
    level = !level; });

  Wire.beginTransmission(0x3C);
  if (Wire.endTransmission() == 0)
  {
    Serial.println("Started OLED");
    u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
    u8g2->begin();
    u8g2->clearBuffer();
    u8g2->setFlipMode(0);
    u8g2->setFontMode(1); // Transparent
    u8g2->setDrawColor(1);
    u8g2->setFontDirection(0);
    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_inb19_mr);
      u8g2->drawStr(0, 30, "LilyGo");
      u8g2->drawHLine(2, 35, 47);
      u8g2->drawHLine(3, 36, 47);
      u8g2->drawVLine(45, 32, 12);
      u8g2->drawVLine(46, 33, 12);
      u8g2->setFont(u8g2_font_inb19_mf);
      u8g2->drawStr(58, 60, "LoRa");
    } while (u8g2->nextPage());
    u8g2->sendBuffer();
    u8g2->setFont(u8g2_font_fur11_tf);
    delay(3000);
  }

  u8g2->setFont(u8g2_font_ncenB08_tr);

  pinMode(SDCARD_MISO, INPUT_PULLUP);
  SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);

  u8g2->clearBuffer();

  if (!SD.begin(SDCARD_CS, SDSPI))
  {
    Serial.println("setupSDCard FAIL");
    do
    {
      u8g2->setCursor(0, 16);
      u8g2->println("SDCard FAILED");
      ;
    } while (u8g2->nextPage());
  }
  else
  {
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("setupSDCard PASS . SIZE = ");
    Serial.print(cardSize / 1024.0);
    Serial.println(" GB");

    do
    {
      u8g2->setCursor(0, 16);
      u8g2->print("SDCard:");
      u8g2->print(cardSize / 1024.0);
      u8g2->println(" GB");
    } while (u8g2->nextPage());

    u8g2->sendBuffer();
  }
  delay(3000);

  u8g2->clearBuffer();
  do
  {
    u8g2->setCursor(0, 16);
    u8g2->println("Waiting for command");
    ;
  } while (u8g2->nextPage());

  // ↑                From boards.h               ↑
  // ↓ From SX128x receive and transmit ino files ↓

  delay(1500);

  Serial.print(F("[SX1280] Initializing ... "));
  int state = radio.begin();
  if (state != RADIOLIB_ERR_NONE)
  {
    u8g2->clearBuffer();
    u8g2->drawStr(0, 12, "Initializing: FAIL!");
    u8g2->sendBuffer();
  }
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }

#if defined(RADIO_RX_PIN) && defined(RADIO_TX_PIN)
  // Set ANT Control pins
  radio.setRfSwitchPins(RADIO_RX_PIN, RADIO_TX_PIN);
#endif

  if (radio.setOutputPower(3) == RADIOLIB_ERR_INVALID_OUTPUT_POWER)
  {
    Logging::printError("Selected output power is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setFrequency(2400.0) == RADIOLIB_ERR_INVALID_FREQUENCY)
  {
    Logging::printError("Selected frequency is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH)
  {
    Logging::printError("Selected bandwidth is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR)
  {
    Logging::printError("Selected spreading factor is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE)
  {
    Logging::printError("Selected coding rate is invalid for this module!");
    while (true)
      ;
  }

  radio.setDio1Action(setFlag);
}

// --------------------------------------------- //
//              TRANSMITTING CODE                //
// --------------------------------------------- //

void transmitMode()
{
  receivedFlag = false;
  mode = TRANSMIT_MODE;
  transmittedFlag = true;

  if (enableDebug)
  {
    char buffer[40];
    sprintf(buffer, "Transmitted packet %d", receiveCounter);
    Display::displayInfoBot(buffer);
  }
}

bool transmitMessage(uint8_t *message, size_t len, bool printPacket)
{
  if (!transmittedFlag)
  {
    return false;
  }

  transmittedFlag = false;
  if (printPacket && enableDebug)
  {
    Logging::printTransmitPacket(message, len);
  }

  int state = radio.startTransmit(message, len);

  if (state != RADIOLIB_ERR_NONE)
  {
    return false;
  }

  sendCounter++;
  bytesTransferred += len;
  return true;
}

// --------------------------------------------- //
//                RECEIVING CODE                 //
// --------------------------------------------- //

void receiveMode()
{
  mode = RECEIVE_MODE;
  radio.startReceive();

  if (enableDebug)
  {
    char buffer[40];
    sprintf(buffer, "Received packet %d", receiveCounter);
    Display::displayInfoBot(buffer);
  }
}

bool appendToFile(uint8_t *message, size_t size, char *path, File* file)
{
  if (!file->write(message, size))
  {
    if (enableDebug)
      Logging::printError(String("Could not write to file ") + String(path));
    return false;
  }

  return true;
}

bool receiveMessage(uint8_t *buffer, size_t messageSize)
{
  receivedFlag = false;

  int state = radio.readData(buffer, messageSize);

  if (state == RADIOLIB_ERR_NONE)
  {
    receiveCounter++;
    bytesReceived += messageSize;
    return true;
  }
  else
  {
    return false;
  }
}