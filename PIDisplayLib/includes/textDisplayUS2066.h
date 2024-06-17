#pragma once

#include "textDisplayDriver.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/structs/spi.h"

// Config
#define DISPLAY_US2066_I2C i2c0
#define DISPLAY_US2066_BAUDRATE (10 * 1024 * 1024)
#define DISPLAY_US2066_COLS 20
#define DISPLAY_US2066_ROWS 4

class textDisplayUS2066 : textDisplayDriver
{
public:
    textDisplayUS2066();

};
