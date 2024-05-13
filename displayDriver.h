#pragma once

#include "displayBuffer.h"
#include "fonts.h"
#include "pico/stdlib.h"

class displayDriver
{

public:

    ~displayDriver();

    void initDisplayBuffer(uint16_t width, uint16_t height, uint8_t bitsPerPixel);
    displayBuffer* getDisplayBuffer();

    void initSpi(uint32_t baudRate);
    void writeSpiCommand(uint8_t cmd);
    void writeSpiDualCommand(uint8_t cmd1, uint8_t cmd2);
    void writeSpiData(uint8_t *buff, uint32_t buff_size);
    void writeSpiDataByte(uint8_t data);

    void drawChar(uint32_t colorRgb, FontDef font, uint16_t x, uint16_t y, char character);
    void drawString(uint32_t colorRgb, FontDef font, uint16_t x, uint16_t y, const char *message);

    void drawLine(uint32_t colorRgb, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void drawRectangle(uint32_t colorRgb, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void drawTriangle(uint32_t colorRgb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void drawCircle(uint32_t colorRgb, int16_t x, int16_t y, int16_t radius);
	void drawFilledRectangle(uint32_t colorRgb, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void drawFilledTriangle(uint32_t colorRgb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void drawFilledCircle(uint32_t colorRgb, int16_t x, int16_t y, int16_t radius);

    virtual void drawPixel(uint32_t colorRgb, uint16_t x, uint16_t y) = 0;
    virtual void fill(uint32_t colorRgb) = 0;
    virtual void drawDisplay() = 0;

    virtual void contrast(uint8_t value) = 0;
    virtual void invert(bool value) = 0;
    virtual void rotate(uint16_t degrees) = 0;

    // todo
    // draw image

public:
    displayBuffer* mDisplayBuffer;
};
