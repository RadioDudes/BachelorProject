#include "T3S3.h"

SPIClass SDSPI(HSPI);
#define DISPLAY_MODEL U8G2_SSD1306_128X64_NONAME_F_HW_I2C

fs::SDFS sd = SD;
DISPLAY_MODEL *u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
Ticker ledTicker;

// API for communicating with the radio itself, from RadioLib (https://github.com/jgromes/RadioLib)
SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

// Modes for the device to switch between
const int INACTIVE = 0;
const int RECEIVE_MODE = 1;
const int TRANSMIT_MODE = 2;
int mode = INACTIVE;

// Counters for amount of packets sent and received
int sendCounter = 0;
int receiveCounter = 0;

// Timeout defined in milliseconds
unsigned long timeoutTime = 500;
// Size of each packet
int packetSize = 30;

// Interrupt-driven receive flag
volatile bool receivedFlag = false;
// Interrupt-driven transmit flag
volatile bool transmittedFlag = true;

// ---------------------------------------------------------- //

void setSpreadingFactor(uint8_t sf)
{
  if (radio.setSpreadingFactor(sf) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR)
  {
    printInvalidSpreadingFactor(sf);
    return;
  }
  printSetSpreadingFactor(sf);
}

void setCodingRate(uint8_t cr)
{
  if (radio.setCodingRate(cr) == RADIOLIB_ERR_INVALID_CODING_RATE)
  {
    printInvalidCodingRate(cr);
    return;
  }
  printSetCodingRate(cr);
}

void setFrequency(double freq)
{
  if (radio.setFrequency(freq) == RADIOLIB_ERR_INVALID_FREQUENCY)
  {
    printInvalidFrequency(freq);
    return;
  }
  printSetFrequency(freq);
}

void setBandwidth(double bw)
{
  if (radio.setBandwidth(bw) == RADIOLIB_ERR_INVALID_BANDWIDTH)
  {
    printInvalidBandwidth(bw);
    return;
  }
  printSetBandwidth(bw);
}

void setTimeout(unsigned long time)
{
  timeoutTime = time;
  printSetTimeout(time);
}

void setPacketSize(int size)
{
  packetSize = size;
  printSetPacketSize(size);
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
    printError("Selected output power is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setFrequency(2400.0) == RADIOLIB_ERR_INVALID_FREQUENCY)
  {
    printError("Selected frequency is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH)
  {
    printError("Selected bandwidth is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR)
  {
    printError("Selected spreading factor is invalid for this module!");
    while (true)
      ;
  }

  if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE)
  {
    printError("Selected coding rate is invalid for this module!");
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
}

bool transmitMessage(uint8_t *message, size_t len)
{
  if (!transmittedFlag)
  {
    return false;
  }

  transmittedFlag = false;
  printTransmitPacket(message, len);
  sendCounter++;
  int state = radio.startTransmit(message, len);

  if (state != RADIOLIB_ERR_NONE)
  {
    return false;
  }

  return true;
}

// --------------------------------------------- //
//                RECEIVING CODE                 //
// --------------------------------------------- //

void receiveMode()
{
  mode = RECEIVE_MODE;
  radio.startReceive();
  printInfo("RECEIVE MODE ACTIVATED");
}

bool appendToFile(uint8_t *message, size_t size, char *path)
{
  printInfo(String("Opening file ") + String(path));
  File file = SD.open(path, FILE_APPEND);

  if (!file)
  {
    printError(String("Could not open file ") + String(path));
    return false;
  }

  if (!file.write(message, size))
  {
    printError(String("Could not write to file ") + String(path));
    return false;
  }

  file.close();
  return true;
}

bool receiveMessage(uint8_t *buffer, size_t messageSize)
{
  receivedFlag = false;

  int state = radio.readData(buffer, messageSize);

  if (state == RADIOLIB_ERR_NONE)
  {
    receiveCounter++;
    return true;
  }
  else
  {
    return false;
  }
}