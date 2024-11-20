#pragma once

#include "textDisplayDriver.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/structs/spi.h"

// Config
#define DISPLAY_US2066_I2C i2c1
#define DISPLAY_US2066_BAUDRATE (1 * 1024 * 1024)
#define DISPLAY_US2066_COLS 20
#define DISPLAY_US2066_ROWS 4

class textDisplayUS2066 : textDisplayDriver
{
public:
    textDisplayUS2066();

    void setCursor(uint16_t row, uint16_t col);
    void printChar(uint8_t character);
};
