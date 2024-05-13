#pragma once

#include "pico/stdlib.h"

class color
{
public:
    static uint8_t convertRGB8toGray8(uint32_t rgbColor);
    static uint8_t convertRGB8toGray4(uint32_t rgbColor);
    static uint32_t convertGray8toRGB8(uint8_t gray8);
    static uint32_t convertGray4toRGB8(uint8_t gray4);
};