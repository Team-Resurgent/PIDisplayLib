#pragma once

#include "pico/stdlib.h"

class displayBuffer
{
public:
    displayBuffer(uint16_t width, uint16_t height, uint8_t bitsPerPixel);
    ~displayBuffer();
    uint16_t getWidth();
    uint16_t getHeight();
    uint8_t getBitsPerPixel();
    uint32_t getBufferSize();
    uint8_t* getBuffer();
private:
    uint16_t mWidth;
    uint16_t mHeight;
    uint8_t mBitsPerPixel;
    uint32_t mBufferSize;
    uint8_t* mBuffer;
};