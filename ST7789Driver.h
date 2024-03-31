#pragma once

#include "pico/stdlib.h"
#include "fonts.h"

#define DISPLAY_BACKLIGHT 22
#define DISPLAY_RST 21 
#define DISPLAY_DC  20
#define DISPLAY_CS  PICO_DEFAULT_SPI_CSN_PIN
#define DISPLAY_MOSI PICO_DEFAULT_SPI_TX_PIN 
#define DISPLAY_SCLK PICO_DEFAULT_SPI_SCK_PIN

#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define GRAY        0X8430
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458

#define LIGHTGREEN  0X841F
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12

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

// static void ST7789Driver_WriteCommand(uint8_t cmd);
// static void ST7789Driver_WriteData(uint8_t *buff, uint32_t buff_size);
// static void ST7789Driver_WriteSmallData(uint8_t data);
// static void ST7789Driver_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
// static void ST7789Driver_SetRotation(uint8_t m);
// static void ST7789Driver_Fill_Color(uint16_t color);
// static void ST7789Driver_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
// static void ST7789Driver_Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color);
// static void ST7789Driver_DrawPixel_4px(uint16_t x, uint16_t y, uint16_t color);
// static void ST7789Driver_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
// static void ST7789Driver_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
// static void ST7789Driver_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
// static void ST7789Driver_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
// static void ST7789Driver_InvertColors(uint8_t invert);
// static void ST7789Driver_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
// static void ST7789Driver_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor);
// static void ST7789Driver_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
// static void ST7789Driver_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
// static void ST7789Driver_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
// static void ST7789Driver_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
// static void ST7789Driver_TearEffect(uint8_t tear);
// static void ST7789Driver_Init(void);

//https://blog.embeddedexpert.io/?p=1215



static void ST7789Driver_WriteCommand(uint8_t cmd)
{
	gpio_put(DISPLAY_DC, 0);
    spi_write_blocking(0, &cmd, 1);
}

static void ST7789Driver_WriteData(uint8_t *buff, uint32_t buff_size)
{
	gpio_put(DISPLAY_DC, 1);
    spi_write_blocking(0, buff, buff_size);
}

static void ST7789Driver_WriteSmallData(uint8_t data)
{
    ST7789Driver_WriteData(&data, 1);
}

static void ST7789Driver_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t x_start = x0 + ST7789_X_SHIFT, x_end = x1 + ST7789_X_SHIFT;
	uint16_t y_start = y0 + ST7789_Y_SHIFT, y_end = y1 + ST7789_Y_SHIFT;

	/* Column Address set */
	ST7789Driver_WriteCommand(ST7789_CASET);
	{
		uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
		ST7789Driver_WriteData(data, sizeof(data));
	}

	/* Row Address set */
	ST7789Driver_WriteCommand(ST7789_RASET);
	{
		uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
		ST7789Driver_WriteData(data, sizeof(data));
	}
	/* Write to RAM */
	ST7789Driver_WriteCommand(ST7789_RAMWR);
}

static void ST7789Driver_SetRotation(uint8_t m)
{
	ST7789Driver_WriteCommand(ST7789_MADCTL);	// MADCTL
	switch (m) {
	case 0:
		ST7789Driver_WriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
		break;
	case 1:
		ST7789Driver_WriteSmallData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		break;
	case 2:
		ST7789Driver_WriteSmallData(ST7789_MADCTL_RGB);
		break;
	case 3:
		ST7789Driver_WriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		break;
	default:
		break;
	}
}

static void ST7789Driver_Fill_Color(uint16_t color)
{
	uint16_t i;
    uint16_t offset;
	ST7789Driver_SetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);

    uint8_t* row = (uint8_t*)malloc(ST7789_WIDTH * sizeof(uint16_t));

    offset = 0;
    for (i = 0; i < ST7789_WIDTH; i++)
    {
        row[offset + 0] = color >> 8;
        row[offset + 1] = color & 0xFF;
        offset += 2;
    }

    uint16_t j;
    for (j = 0; j < ST7789_HEIGHT; j++) 
    {
        ST7789Driver_WriteData(row, ST7789_WIDTH * sizeof(uint16_t));
    }

    free(row);
}

static void ST7789Driver_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x < 0) || (x >= ST7789_WIDTH) ||
		 (y < 0) || (y >= ST7789_HEIGHT))	return;

	ST7789Driver_SetAddressWindow(x, y, x, y);
	uint8_t data[] = {color >> 8, color & 0xFF};
	ST7789Driver_WriteData(data, sizeof(data));
}

static void ST7789Driver_Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
	if ((xEnd < 0) || (xEnd >= ST7789_WIDTH) ||
		 (yEnd < 0) || (yEnd >= ST7789_HEIGHT))	return;
	uint16_t i, j;
	ST7789Driver_SetAddressWindow(xSta, ySta, xEnd, yEnd);
	for (i = ySta; i <= yEnd; i++)
		for (j = xSta; j <= xEnd; j++) {
			uint8_t data[] = {color >> 8, color & 0xFF};
			ST7789Driver_WriteData(data, sizeof(data));
		}
}

static void ST7789Driver_DrawPixel_4px(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x <= 0) || (x > ST7789_WIDTH) ||
		 (y <= 0) || (y > ST7789_HEIGHT))	return;

	ST7789Driver_Fill(x - 1, y - 1, x + 1, y + 1, color);

}

static void ST7789Driver_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) 
{
	uint16_t swap;
    uint16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
		swap = x0;
		x0 = y0;
		y0 = swap;

		swap = x1;
		x1 = y1;
		y1 = swap;
        //_swap_int16_t(x0, y0);
        //_swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
		swap = x0;
		x0 = x1;
		x1 = swap;

		swap = y0;
		y0 = y1;
		y1 = swap;
        //_swap_int16_t(x0, x1);
        //_swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            ST7789Driver_DrawPixel(y0, x0, color);
        } else {
            ST7789Driver_DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

static void ST7789Driver_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	ST7789Driver_DrawLine(x1, y1, x2, y1, color);
	ST7789Driver_DrawLine(x1, y1, x1, y2, color);
	ST7789Driver_DrawLine(x1, y2, x2, y2, color);
	ST7789Driver_DrawLine(x2, y1, x2, y2, color);
}

static void ST7789Driver_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;


	ST7789Driver_DrawPixel(x0, y0 + r, color);
	ST7789Driver_DrawPixel(x0, y0 - r, color);
	ST7789Driver_DrawPixel(x0 + r, y0, color);
	ST7789Driver_DrawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		ST7789Driver_DrawPixel(x0 + x, y0 + y, color);
		ST7789Driver_DrawPixel(x0 - x, y0 + y, color);
		ST7789Driver_DrawPixel(x0 + x, y0 - y, color);
		ST7789Driver_DrawPixel(x0 - x, y0 - y, color);

		ST7789Driver_DrawPixel(x0 + y, y0 + x, color);
		ST7789Driver_DrawPixel(x0 - y, y0 + x, color);
		ST7789Driver_DrawPixel(x0 + y, y0 - x, color);
		ST7789Driver_DrawPixel(x0 - y, y0 - x, color);
	}

}

static void ST7789Driver_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)
{
	if ((x >= ST7789_WIDTH) || (y >= ST7789_HEIGHT))
		return;
	if ((x + w - 1) >= ST7789_WIDTH)
		return;
	if ((y + h - 1) >= ST7789_HEIGHT)
		return;


	ST7789Driver_SetAddressWindow(x, y, x + w - 1, y + h - 1);
	ST7789Driver_WriteData((uint8_t *)data, sizeof(uint16_t) * w * h);
}

static void ST7789Driver_InvertColors(uint8_t invert)
{
	ST7789Driver_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
}

static void ST7789Driver_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
	uint32_t i, b, j;

	ST7789Driver_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);

	for (i = 0; i < font.height; i++) {
		b = font.data[(ch - 32) * font.height + i];
		for (j = 0; j < font.width; j++) {
			if ((b << j) & 0x8000) {
				uint8_t data[] = {color >> 8, color & 0xFF};
				ST7789Driver_WriteData(data, sizeof(data));
			}
			else {
				uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
				ST7789Driver_WriteData(data, sizeof(data));
			}
		}
	}
}

static void ST7789Driver_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor)
{
	while (*str) {
		if (x + font.width >= ST7789_WIDTH) {
			x = 0;
			y += font.height;
			if (y + font.height >= ST7789_HEIGHT) {
				break;
			}

			if (*str == ' ') {
				// skip spaces in the beginning of the new line
				str++;
				continue;
			}
		}
		ST7789Driver_WriteChar(x, y, *str, font, color, bgcolor);
		x += font.width;
		str++;
	}

}

static void ST7789Driver_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{

	uint8_t i;

	/* Check input parameters */
	if (x >= ST7789_WIDTH ||
		y >= ST7789_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= ST7789_WIDTH) {
		w = ST7789_WIDTH - x;
	}
	if ((y + h) >= ST7789_HEIGHT) {
		h = ST7789_HEIGHT - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		ST7789Driver_DrawLine(x, y + i, x + w, y + i, color);
	}

}

static void ST7789Driver_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	ST7789Driver_DrawLine(x1, y1, x2, y2, color);
	ST7789Driver_DrawLine(x2, y2, x3, y3, color);
	ST7789Driver_DrawLine(x3, y3, x1, y1, color);
}

static void ST7789Driver_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
			yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
			curpixel = 0;

	deltax = abs(x2 - x1);
	deltay = abs(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	}
	else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	}
	else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
	else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		ST7789Driver_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}

}

static void ST7789Driver_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	ST7789Driver_DrawPixel(x0, y0 + r, color);
	ST7789Driver_DrawPixel(x0, y0 - r, color);
	ST7789Driver_DrawPixel(x0 + r, y0, color);
	ST7789Driver_DrawPixel(x0 - r, y0, color);
	ST7789Driver_DrawLine(x0 - r, y0, x0 + r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		ST7789Driver_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
		ST7789Driver_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

		ST7789Driver_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
		ST7789Driver_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
	}

}

static void ST7789Driver_TearEffect(uint8_t tear)
{
	ST7789Driver_WriteCommand(tear ? 0x35 /* TEON */ : 0x34 /* TEOFF */);
}

static void ST7789Driver_Init(void)
{
	//spi_init();
	sleep_ms(25);
    gpio_put(DISPLAY_RST, 0);
	sleep_ms(50);
    gpio_put(DISPLAY_RST, 1);
    sleep_ms(50);
    ST7789Driver_WriteCommand(ST7789_SWRESET);
    sleep_ms(100);
    ST7789Driver_WriteCommand(ST7789_COLMOD);		//	Set color mode
    ST7789Driver_WriteSmallData(ST7789_COLOR_MODE_16bit);
  	ST7789Driver_WriteCommand(0xB2);				//	Porch control
	uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
	ST7789Driver_WriteData(data, sizeof(data));

	/* Internal LCD Voltage generator settings */
    ST7789Driver_WriteCommand(0XB7);				//	Gate Control
    ST7789Driver_WriteSmallData(0x35);			//	Default value
    ST7789Driver_WriteCommand(0xBB);				//	VCOM setting
    ST7789Driver_WriteSmallData(0x20);			//	0.725v (default 0.75v for 0x20)
    ST7789Driver_WriteCommand(0xC0);				//	LCMCTRL
    ST7789Driver_WriteSmallData (0x2C);			//	Default value
    ST7789Driver_WriteCommand (0xC2);				//	VDV and VRH command Enable
    ST7789Driver_WriteSmallData (0x01);			//	Default value
    ST7789Driver_WriteCommand (0xC3);				//	VRH set
    ST7789Driver_WriteSmallData (0x0b);			//	+-4.45v (defalut +-4.1v for 0x0B)
    ST7789Driver_WriteCommand (0xC4);				//	VDV set
    ST7789Driver_WriteSmallData (0x20);			//	Default value
    ST7789Driver_WriteCommand (0xC6);				//	Frame rate control in normal mode
    ST7789Driver_WriteSmallData (0x0F);			//	Default value (60HZ)
    ST7789Driver_WriteCommand (0xD0);				//	Power control
    ST7789Driver_WriteSmallData (0xA4);			//	Default value
    ST7789Driver_WriteSmallData (0xA1);			//	Default value
	/**************** Division line ****************/

	ST7789Driver_WriteCommand(0xE0);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
		ST7789Driver_WriteData(data, sizeof(data));
	}

    ST7789Driver_WriteCommand(0xE1);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
		ST7789Driver_WriteData(data, sizeof(data));
	}
    ST7789Driver_WriteCommand (ST7789_INVON);		//	Inversion ON
	ST7789Driver_WriteCommand (ST7789_SLPOUT);	//	Out of sleep mode
  	ST7789Driver_WriteCommand (ST7789_NORON);		//	Normal Display on
  	ST7789Driver_WriteCommand (ST7789_DISPON);	//	Main screen turned on

  	//sleep_ms(50);
	//Fill_Color(WHITE);				//	Fill with Black.

}