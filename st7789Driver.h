#pragma once

#include "displayConfig.h"
#include "pico/stdlib.h"
#include "fonts.h"

#define ST7789_WHITE       0xFFFF
#define ST7789_BLACK       0x0000
#define ST7789_BLUE        0x001F
#define ST7789_RED         0xF800
#define ST7789_MAGENTA     0xF81F
#define ST7789_GREEN       0x07E0
#define ST7789_CYAN        0x7FFF
#define ST7789_YELLOW      0xFFE0
#define ST7789_GRAY        0X8430
#define ST7789_BRED        0XF81F
#define ST7789_GRED        0XFFE0
#define ST7789_GBLUE       0X07FF
#define ST7789_BROWN       0XBC40
#define ST7789_BRRED       0XFC07
#define ST7789_DARKBLUE    0X01CF
#define ST7789_LIGHTBLUE   0X7D7C
#define ST7789_GRAYBLUE    0X5458

#define ST7789_LIGHTGREEN  0X841F
#define ST7789_LGRAY       0XC618
#define ST7789_LGRAYBLUE   0XA651
#define ST7789_LBBLUE      0X2B12

/* Control Registers and constant codes */
#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09
#define ST7789_RDDPM   0x0A

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

/**
 * Memory Data Access Control Register (0x36H)
 * MAP:     D7  D6  D5  D4  D3  D2  D1  D0
 * param:   MY  MX  MV  ML  RGB MH  -   -
 *
 */

/* Page Address Order ('0': Top to Bottom, '1': the opposite) */
#define ST7789_MADCTL_MY  0x80
/* Column Address Order ('0': Left to Right, '1': the opposite) */
#define ST7789_MADCTL_MX  0x40
/* Page/Column Order ('0' = Normal Mode, '1' = Reverse Mode) */
#define ST7789_MADCTL_MV  0x20
/* Line Address Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define ST7789_MADCTL_ML  0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD

#define ST7789_COLOR_MODE_16bit 0x55    //  RGB565 (16bit)
#define ST7789_COLOR_MODE_18bit 0x66    //  RGB666 (18bit)

#define ST7789_ROTATION 1
#define ST7789_WIDTH 280
#define ST7789_HEIGHT 240
#define ST7789_X_SHIFT 20
#define ST7789_Y_SHIFT 0

//https://blog.embeddedexpert.io/?p=1215

class st7789Driver
{
public:
	static void WriteCommand(uint8_t cmd);
	static void WriteData(uint8_t *buff, uint32_t buff_size);
	static void WriteSmallData(uint8_t data);
	static void SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	static void SetRotation(uint8_t m);
	static void FillColor(uint16_t color);
	static void DrawPixel(uint16_t x, uint16_t y, uint16_t color);
	static void Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color);
	static void DrawPixel_4px(uint16_t x, uint16_t y, uint16_t color);
	static void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	static void DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
	static void DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
	static void DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
	static void InvertColors(uint8_t invert);
	static void drawChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
	static void drawString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor);
	static void DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	static void DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
	static void DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
	static void DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	static void TearEffect(uint8_t tear);
	static void Init(void);
};