#include "displaySH1122.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define SH1122_CMD_SET_LOW_COLUMN_ADDR 0x00
#define SH1122_CMD_SET_HIGH_COLUMN_ADDR 0x10
#define SH1122_CMD_SET_DISPLAY_START_LINE 0x40 
#define SH1122_CMD_SET_CONTRAST_CURRENT 0x81 // 2 Byte Command
#define SH1122_CMD_SET_SEGMENT_REMAP 0xA0
#define SH1122_CMD_SET_ENTIRE_DISPLAY_OFF 0xA4
#define SH1122_CMD_SET_ENTIRE_DISPLAY_ON 0xA5
#define SH1122_CMD_SET_NORMAL_DISPLAY 0xA6
#define SH1122_CMD_SET_REVERSE_DISPLAY 0xA7
#define SH1122_CMD_SET_MULTIPLEX_RATIO 0xA8 // 2 Byte Command
#define SH1122_CMD_SET_DCDC_SETTING 0xAD // 2 Byte Command
#define SH1122_CMD_SET_DISPLAY_OFF 0xAE
#define SH1122_CMD_SET_DISPLAY_ON 0xAF
#define SH1122_CMD_SET_ROW_ADDR 0xB0 // 2 Byte Command
#define SH1122_CMD_SET_SCAN_DIRECTION 0xC0
#define SH1122_CMD_SET_DISPLAY_OFFSET 0xD3 // 2 Byte Command
#define SH1122_CMD_SET_CLOCK_DIVIDER 0xD5 // 2 Byte Command
#define SS1122_CMD_SET_DISCHARGE_PRECHARGE_PERIOD 0xD9 // 2 Byte Command
#define SH1122_CMD_SET_VCOM_DESELECT_LEVEL 0xDB // 2 Byte Command
#define SH1122_CMD_SET_VSEGM_LEVEL 0xDC // 2 Byte Command
#define SH1122_CMD_SET_DISCHARGE_VSL_LEVEL 0x30
#define SH1122_CMD_SET_READ_MODIFY_WRITE 0xE0
#define SH1122_CMD_SET_END 0xEE
#define SH1122_CMD_SET_NOP 0xE3

displaySH1122::displaySH1122()
{
    initDisplayBuffer(
        DISPLAY_SH1122_WIDTH, 
        DISPLAY_SH1122_HEIGHT, 
        DISPLAY_SH1122_BITS_PER_PIXEL
    );

    initSpi(DISPLAY_SH1122_BAUDRATE);

    writeSpiCommand(SH1122_CMD_SET_DISPLAY_OFF);
    writeSpiCommand(SH1122_CMD_SET_ENTIRE_DISPLAY_OFF);

    writeSpiCommand(SH1122_CMD_SET_DISPLAY_START_LINE | 0);
    writeSpiDualCommand(SH1122_CMD_SET_CONTRAST_CURRENT, 0x80);
    writeSpiCommand(SH1122_CMD_SET_SEGMENT_REMAP | 0x00);
    writeSpiCommand(SH1122_CMD_SET_NORMAL_DISPLAY);
    writeSpiDualCommand(SH1122_CMD_SET_MULTIPLEX_RATIO, DISPLAY_SH1122_HEIGHT - 1);
    writeSpiDualCommand(SH1122_CMD_SET_DCDC_SETTING, 0x81);
    writeSpiCommand(SH1122_CMD_SET_SCAN_DIRECTION | 0x00);
    writeSpiDualCommand(SH1122_CMD_SET_DISPLAY_OFFSET, 0x00);
    writeSpiDualCommand(SH1122_CMD_SET_CLOCK_DIVIDER, 0x50);
    writeSpiDualCommand(SS1122_CMD_SET_DISCHARGE_PRECHARGE_PERIOD, 0x22);
    writeSpiDualCommand(SH1122_CMD_SET_VCOM_DESELECT_LEVEL, 0x35);
    writeSpiDualCommand(SH1122_CMD_SET_VSEGM_LEVEL, 0x35);
    writeSpiCommand(SH1122_CMD_SET_DISCHARGE_VSL_LEVEL | 0x00);

    writeSpiDualCommand(SH1122_CMD_SET_ROW_ADDR, 0x00);
    writeSpiCommand(SH1122_CMD_SET_LOW_COLUMN_ADDR);
    writeSpiCommand(SH1122_CMD_SET_HIGH_COLUMN_ADDR);
    writeSpiCommand(SH1122_CMD_SET_DISPLAY_ON);

    drawDisplay();
}

void displaySH1122::drawChar(uint32_t colorRgb, FontDef font, uint16_t x, uint16_t y, char character)
{
    displayDriver::drawChar(colorRgb, font, x, y, character);
}

void displaySH1122::drawString(uint32_t colorRgb, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    displayDriver::drawString(colorRgb, font, x, y, message);
}

void displaySH1122::drawLine(uint32_t colorRgb, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    displayDriver::drawLine(colorRgb, x0, y0, x1, y1);
}

void displaySH1122::drawRectangle(uint32_t colorRgb, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawRectangle(colorRgb, x, y, width, height);
}

void displaySH1122::drawTriangle(uint32_t colorRgb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawTriangle(colorRgb, x1, y1, x2, y2, x3, y3);
}

void displaySH1122::drawCircle(uint32_t colorRgb, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawCircle(colorRgb, x, y, radius);
}

void displaySH1122::drawFilledRectangle(uint32_t colorRgb, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawFilledRectangle(colorRgb, x, y, width, height);
}

void displaySH1122::drawFilledTriangle(uint32_t colorRgb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawFilledTriangle(colorRgb, x1, y1, x2, y2, x3, y3);
}

void displaySH1122::drawFilledCircle(uint32_t colorRgb, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawFilledCircle(colorRgb, x, y, radius);
}

void displaySH1122::drawPixel(uint32_t colorRgb, uint16_t x, uint16_t y)
{
    if (x >= DISPLAY_SH1122_WIDTH || y >= DISPLAY_SH1122_HEIGHT)
    {
        return;
    }

    uint8_t gray4 = color::convertRGB8toGray4(colorRgb);
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pixelOffset = (y * (DISPLAY_SH1122_WIDTH >> 1)) + (x >> 1);
    uint8_t currentPixel = buffer[pixelOffset];
    if ((x & 1) == 0) 
    {
        currentPixel &= 0x0f;
        currentPixel |= (gray4 << 4);
    }
    else
    {
        currentPixel &= 0xf0;
        currentPixel |= gray4;
    }
    buffer[pixelOffset] = currentPixel;
}

void displaySH1122::fill(uint32_t colorRgb)
{
    uint8_t gray4 = color::convertRGB8toGray4(colorRgb);
    memset(getDisplayBuffer()->getBuffer(), (gray4 << 4) | gray4, getDisplayBuffer()->getBufferSize());
}

void displaySH1122::drawDisplay()
{
    writeSpiDualCommand(SH1122_CMD_SET_ROW_ADDR, 0x00);
    writeSpiCommand(SH1122_CMD_SET_LOW_COLUMN_ADDR);
    writeSpiCommand(SH1122_CMD_SET_HIGH_COLUMN_ADDR);
    writeSpiData(getDisplayBuffer()->getBuffer(), getDisplayBuffer()->getBufferSize());
}

void displaySH1122::contrast(uint8_t value)
{
    writeSpiDualCommand(SH1122_CMD_SET_CONTRAST_CURRENT, value);
}

void displaySH1122::invert(bool value)
{
    writeSpiCommand(value ? SH1122_CMD_SET_REVERSE_DISPLAY : SH1122_CMD_SET_NORMAL_DISPLAY);
}

void displaySH1122::rotate(uint16_t degrees)
{
    if (degrees == 0)
    {
        writeSpiCommand(SH1122_CMD_SET_DISPLAY_START_LINE | 0x00 );
	    writeSpiCommand(SH1122_CMD_SET_SEGMENT_REMAP | 0x00);
	    writeSpiCommand(SH1122_CMD_SET_SCAN_DIRECTION | 0x00);
    }
    else if (degrees == 180)
    {
        writeSpiCommand(SH1122_CMD_SET_DISPLAY_START_LINE | (uint8_t)DISPLAY_SH1122_WIDTH);
	    writeSpiCommand(SH1122_CMD_SET_SEGMENT_REMAP | 0x01);
	    writeSpiCommand(SH1122_CMD_SET_SCAN_DIRECTION | 0x08);
    }
}