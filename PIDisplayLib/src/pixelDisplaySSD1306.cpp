#include "pixelDisplaySSD1306.h"
#include "displayConfig.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define SSD1306_CMD_LOWER_COLUMN_START_ADDRESS 0x00
#define SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS 0x10
#define SSD1306_CMD_MEMORY_ADDRESSING_MODE 0x20
#define SSD1306_CMD_SET_COLUMN_ADDRESS 0x21 
#define SSD1306_CMD_SET_PAGE_ADDRESS 0x22 
#define SSD1306_CMD_SET_FADE_OUT_AND_BLINKING 0x23
#define SSD1306_CMD_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_CMD_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_CMD_VERTICAL_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_CMD_VERTICAL_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_CMD_DEACTIVATE_SCROLL 0x2E 
#define SSD1306_CMD_ACTIVATE_SCROLL 0x2F
#define SSD1306_CMD_DISPLAY_START_LINE 0x40 
#define SSD1306_CMD_CONTRAST_CONTROL 0x81
#define SSD1306_CMD_CHARGE_PUMP_SETTING 0x8D
#define SSD1306_CMD_COLUMN_0_MAPPED_TO_SEG0 0xA0
#define SSD1306_CMD_COLUMN_127_MAPPED_TO_SEG0 0xA1 
#define SSD1306_CMD_VERTICAL_SCROLL_AREA 0xA3 
#define SSD1306_CMD_ENTIRE_DISPLAY_OFF 0xA4 
#define SSD1306_CMD_ENTIRE_DISPLAY_ON 0xA5 
#define SSD1306_CMD_NORMAL_DISPLAY 0xA6 
#define SSD1306_CMD_INVERSE_DISPLAY 0xA7 
#define SSD1306_CMD_MULTIPLEX_RATIO 0xA8
#define SSD1306_CMD_DISPLAY_OFF 0xAE
#define SSD1306_CMD_DISPLAY_ON 0xAF
#define SSD1306_CMD_PAGE_ADDR 0xB0
#define SSD1306_CMD_SCAN_DIRECTION_COM0_START 0xC0
#define SSD1306_CMD_SCAN_DIRECTION_COM1_START 0xC8 
#define SSD1306_CMD_DISPLAY_OFFSET 0xD3
#define SSD1306_CMD_DISPLAY_CLOCK_DIVIDE 0xD5
#define SSD1306_CMD_SET_ZOOM_IN 0xD6
#define SSD1306_CMD_PRE_CHARGE_PERIOD 0xD9
#define SSD1306_CMD_COM_PINS_CONF 0xDA
#define SSD1306_CMD_COMH_DESLECT_LEVEL 0xDB
#define SSD1306_CMD_NOP 0xE3

pixelDisplaySSD1306::pixelDisplaySSD1306(uint16_t width, uint16_t height, uint16_t xShift, uint16_t yShift, uint8_t bitsPerPixel)
{
    initDisplayBuffer(
        width, 
        height, 
        xShift,
        yShift,
        bitsPerPixel
    );
}

void pixelDisplaySSD1306::initSpi(spi_inst_t* spi, uint32_t baudRate, uint8_t rxPin, uint8_t sckPin, uint8_t csnPin, uint8_t rstPin, uint8_t dcPin, uint8_t backlightPin) 
{
	pixelDisplayDriver::initSpi(spi, baudRate, rxPin, sckPin, csnPin, rstPin, dcPin, backlightPin);
    init();
}

void pixelDisplaySSD1306::initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin, uint8_t backlightPin)
{
	pixelDisplayDriver::initI2c(i2c, address, baudRate, sdaPin, sclPin, backlightPin);
    init();
}

int32_t pixelDisplaySSD1306::scanI2c()
{
	return pixelDisplayDriver::scanI2c();
}

void pixelDisplaySSD1306::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    pixelDisplayDriver::drawChar(colorR8G8B8, font, x, y, character);
}

void pixelDisplaySSD1306::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    pixelDisplayDriver::drawString(colorR8G8B8, font, x, y, message);
}

void pixelDisplaySSD1306::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    pixelDisplayDriver::drawLine(colorR8G8B8, x0, y0, x1, y1);
}

void pixelDisplaySSD1306::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    pixelDisplayDriver::drawRectangle(colorR8G8B8, x, y, width, height);
}

void pixelDisplaySSD1306::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    pixelDisplayDriver::drawTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void pixelDisplaySSD1306::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    pixelDisplayDriver::drawCircle(colorR8G8B8, x, y, radius);
}

void pixelDisplaySSD1306::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    pixelDisplayDriver::drawFilledRectangle(colorR8G8B8, x, y, width, height);
}

void pixelDisplaySSD1306::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    pixelDisplayDriver::drawFilledTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void pixelDisplaySSD1306::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    pixelDisplayDriver::drawFilledCircle(colorR8G8B8, x, y, radius);
}

void pixelDisplaySSD1306::drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y)
{
    if (x >= mDisplayBuffer->getWidth() || y >= mDisplayBuffer->getHeight())
    {
        return;
    }

    uint8_t gray1 = color::convertR8G8B8toGray1(colorR8G8B8);
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pixelOffset = x + (y >> 3) * mDisplayBuffer->getWidth();
    uint8_t currentPixel = buffer[pixelOffset];
    uint8_t pixelBit = y & 0x7;
    currentPixel &= ~(1 << pixelBit);
    currentPixel |= gray1 << pixelBit;
    buffer[pixelOffset] = currentPixel;
}

void pixelDisplaySSD1306::fill(uint32_t colorR8G8B8)
{
    uint8_t gray1 = color::convertR8G8B8toGray1(colorR8G8B8);
    uint8_t value = 
        (gray1 << 7) | (gray1 << 6) | 
        (gray1 << 5) | (gray1 << 4) | 
        (gray1 << 3) | (gray1 << 2) | 
        (gray1 << 1) | gray1; 
    memset(getDisplayBuffer()->getBuffer(), value, getDisplayBuffer()->getBufferSize());
}

void pixelDisplaySSD1306::drawDisplay()
{
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pageSize = 16;
    uint32_t bufferSize = getDisplayBuffer()->getBufferSize();

    writeCommandByte(SSD1306_CMD_LOWER_COLUMN_START_ADDRESS | 0x0); 
    writeCommandByte(SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS | 0x0);
    writeCommandByte(SSD1306_CMD_DISPLAY_START_LINE | 0x0);
    
    uint16_t pages = bufferSize / pageSize;
    for (uint16_t i = 0; i < pages; i++) 
    {
        writeData(buffer, 16);
        buffer += pageSize;
    }
}

void pixelDisplaySSD1306::brightness(uint8_t value)
{
    // NA
}

void pixelDisplaySSD1306::contrast(uint8_t value)
{
    writeCommandByte(SSD1306_CMD_CONTRAST_CONTROL);
    writeDataByte(value);
}

void pixelDisplaySSD1306::invert(bool value)
{
    writeCommandByte(value ? SSD1306_CMD_INVERSE_DISPLAY : SSD1306_CMD_NORMAL_DISPLAY);
}

void pixelDisplaySSD1306::rotate(uint16_t degrees)
{
    mDisplayBuffer->setRotation(degrees);

    if (degrees == 0)
    {
        writeCommandByte(SSD1306_CMD_COLUMN_0_MAPPED_TO_SEG0);
        writeCommandByte(SSD1306_CMD_SCAN_DIRECTION_COM0_START);
    }
    else if (degrees == 180)
    {
        writeCommandByte(SSD1306_CMD_COLUMN_127_MAPPED_TO_SEG0);
        writeCommandByte(SSD1306_CMD_SCAN_DIRECTION_COM1_START);
    }
}

// Private

void pixelDisplaySSD1306::init()
{
    writeCommandByte(SSD1306_CMD_DISPLAY_OFF);

    writeCommandByte(SSD1306_CMD_DISPLAY_CLOCK_DIVIDE);
    writeCommandByte(0x80);

    writeCommandByte(SSD1306_CMD_MULTIPLEX_RATIO); 

    writeCommandByte(mDisplayBuffer->getHeight() - 1);

    writeCommandByte(SSD1306_CMD_DISPLAY_OFFSET);
    writeCommandByte(0x00);

    writeCommandByte(SSD1306_CMD_DISPLAY_START_LINE);

    writeCommandByte(SSD1306_CMD_CHARGE_PUMP_SETTING);
    writeCommandByte(0x14); //0x22 for 64 pixel

    writeCommandByte(SSD1306_CMD_COLUMN_0_MAPPED_TO_SEG0);
    writeCommandByte(SSD1306_CMD_SCAN_DIRECTION_COM0_START);

    writeCommandByte(SSD1306_CMD_COM_PINS_CONF);
    writeCommandByte(mDisplayBuffer->getWidth() > 2 * mDisplayBuffer->getHeight() ? 0x02 : 0x12);

    writeCommandByte(SSD1306_CMD_CONTRAST_CONTROL);
    writeCommandByte(0xFF);

    writeCommandByte(SSD1306_CMD_PRE_CHARGE_PERIOD);
    writeCommandByte(0xF1);
  
    writeCommandByte(SSD1306_CMD_COMH_DESLECT_LEVEL);
    writeCommandByte(0x30);

    writeCommandByte(SSD1306_CMD_ENTIRE_DISPLAY_OFF);

    writeCommandByte(SSD1306_CMD_NORMAL_DISPLAY);

    writeCommandByte(SSD1306_CMD_DISPLAY_ON);

    writeCommandByte(SSD1306_CMD_MEMORY_ADDRESSING_MODE);
    writeCommandByte(0x10);

    drawDisplay();
}