#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/structs/spi.h"

class displayBase
{
public:
    void initSpi(spi_inst_t* spi, uint32_t baudRate, uint8_t txPin, uint8_t sckPin, uint8_t csnPin, uint8_t rstPin, uint8_t dcPin, uint8_t backlightPin);
    void initI2c(i2c_inst_t* i2c, uint32_t address,  uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin, uint8_t backlightPin);
    int32_t scanI2c();
    int32_t getI2cAddress();
public:
    bool mIsSpi;
    spi_inst_t* mSpi;
    uint8_t mDcPin;
    i2c_inst_t* mI2c;
    uint32_t mI2cAddress;
};
