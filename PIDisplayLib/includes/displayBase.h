#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/structs/spi.h"

enum displayMode
{
    displayModeSpi,
    displayModeI2c
};

class displayBase
{
public:
    void initSpi(spi_inst_t* spi, uint32_t baudRate);
    void initI2c(i2c_inst_t* i2c, uint32_t address,  uint32_t baudRate);
    int32_t scanI2c();
public:
    bool mIsSpi;
    spi_inst_t* mSpi;
    i2c_inst_t* mI2c;
    uint32_t mI2cAddress;
};
