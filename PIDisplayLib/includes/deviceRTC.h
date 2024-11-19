#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"

class deviceRTC
{
public:
    void initI2c(uint32_t address);
    void scanI2c();

    bool setDateTime(datetime_t* dataTime);
    bool getDateTime(datetime_t* dataTime);
    bool getTemperature(float *val);

public:
    uint32_t mI2cAddress;
};