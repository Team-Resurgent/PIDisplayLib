#pragma once

#include "pico/stdlib.h"

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;

class fonts
{
private:
    /* data */
public:
    static FontDef Font_7x10();
    static FontDef Font_11x18();
    static FontDef Font_16x26();
    static uint16_t* Saber();
};