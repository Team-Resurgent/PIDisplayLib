#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"

class deviceEEPROM
{
public:
    void initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin);
    void scanI2c(i2c_inst_t* i2c);
    bool read();

public:
    i2c_inst_t* mI2c;
    uint32_t mI2cAddress;
};