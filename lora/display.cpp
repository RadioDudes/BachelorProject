#include "display.h"

DISPLAY_MODEL *u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);

void Display::displayInfoTop(char *message)
{
    u8g2->clearBuffer();
    u8g2->drawStr(0, 16, message);
    u8g2->updateDisplayArea(0, 0, 16, 4);
}

void Display::displayInfoBot(char *message)
{
    u8g2->clearBuffer();
    u8g2->drawStr(0, 60, message);
    u8g2->updateDisplayArea(0, 5, 16, 3);
}