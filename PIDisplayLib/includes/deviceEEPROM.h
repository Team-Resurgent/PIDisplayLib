#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"

class deviceEEPROM
{
public:
    void initI2c(uint32_t address);
    void scanI2c();
    bool read();

public:
    uint32_t mI2cAddress;
};