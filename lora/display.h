#pragma once
#include <U8g2lib.h>

#define DISPLAY_MODEL U8G2_SSD1306_128X64_NONAME_F_HW_I2C

// Screen display API, from u8g2 (https://github.com/olikraus/u8g2)
extern DISPLAY_MODEL *u8g2;

namespace Display {
    void displayInfoTop(char *message);
    void displayInfoSecondRow(char *message);
    void displayInfoBot(char *message);
}
