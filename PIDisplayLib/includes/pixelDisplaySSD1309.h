#pragma once

#include "displayBase.h"
#include "pixelDisplayDriver.h"
#include "pixelDisplayBuffer.h"
#include "fonts.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/structs/spi.h"

class pixelDisplaySSD1309 : pixelDisplayDriver
{
public:
    pixelDisplaySSD1309(uint16_t width, uint16_t height, uint16_t xShift, uint16_t yShift, uint8_t bitsPerPixel);

    void initSpi(spi_inst_t* spi, uint32_t baudRate, uint8_t rxPin, uint8_t sckPin, uint8_t csnPin, uint8_t rstPin, uint8_t dcPin, uint8_t backlightPin);
    void initI2c(i2c_inst_t* i2c, uint32_t address,  uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin, uint8_t backlightPin);
    int32_t scanI2c();

    void drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character);
    void drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message);

    void drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius);
	void drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius);

    void drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y);
    void fill(uint32_t colorR8G8B8);
    void drawDisplay();

    void brightness(uint8_t value);
    void contrast(uint8_t value);
    void invert(bool value);
    void rotate(uint16_t degrees);
private:
    void init();
};
