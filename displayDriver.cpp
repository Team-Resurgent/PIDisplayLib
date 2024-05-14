#include "displayDriver.h"
#include "displayConfig.h"
#include "fonts.h"

#include "hardware/spi.h"
#include "hardware/structs/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <cstdio>
#include <stdlib.h>

displayDriver::~displayDriver()
{
    delete(mDisplayBuffer);
}

void displayDriver::initDisplayBuffer(uint16_t width, uint16_t height, uint16_t xShift, uint16_t yShift, uint8_t bitsPerPixel)
{
    mDisplayBuffer = new displayBuffer(
        width, 
        height, 
		xShift,
		yShift,
        bitsPerPixel
    );
}

displayBuffer* displayDriver::getDisplayBuffer()
{
    return mDisplayBuffer;
}

void displayDriver::initSpi(uint32_t baudRate, bool hasBacklight)
{
	if (hasBacklight)
	{
    	gpio_init(SPI_DISPLAY_BACKLIGHT);
    	gpio_put(SPI_DISPLAY_BACKLIGHT, 1);
    	gpio_set_dir(SPI_DISPLAY_BACKLIGHT, GPIO_OUT);
	}

    gpio_init(SPI_DISPLAY_RST);
    gpio_put(SPI_DISPLAY_RST, 1);
    gpio_set_dir(SPI_DISPLAY_RST, GPIO_OUT);

    gpio_init(SPI_DISPLAY_DC);
    gpio_put(SPI_DISPLAY_DC, 0);
    gpio_set_dir(SPI_DISPLAY_DC, GPIO_OUT);

    spi_init(spi_default, baudRate);
    gpio_set_function(SPI_DISPLAY_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_DISPLAY_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_DISPLAY_TX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_DISPLAY_CSN, GPIO_FUNC_SPI);
    bi_decl(bi_4pins_with_func((uint32_t)SPI_DISPLAY_RX, (uint32_t)SPI_DISPLAY_TX, (uint32_t)SPI_DISPLAY_SCK, (uint32_t)SPI_DISPLAY_CSN, GPIO_FUNC_SPI));

    spi_set_format(spi_default, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);

    sleep_ms(10);
	gpio_put(SPI_DISPLAY_RST, 0);
	sleep_ms(10);
	gpio_put(SPI_DISPLAY_RST, 1);
}

void displayDriver::writeSpiCommand(uint8_t cmd)
{
	gpio_put(SPI_DISPLAY_DC, 0);
    spi_write_blocking(spi_default, &cmd, 1);
}

void displayDriver::writeSpiDualCommand(uint8_t cmd1, uint8_t cmd2)
{
    uint8_t cmds[2];
    cmds[0] = cmd1;
    cmds[1] = cmd2;
	gpio_put(SPI_DISPLAY_DC, 0);
    spi_write_blocking(spi_default, &cmds[0], 2);
}

void displayDriver::writeSpiData(uint8_t *buff, uint32_t buff_size)
{
	gpio_put(SPI_DISPLAY_DC, 1);
    spi_write_blocking(spi_default, buff, buff_size);
}

void displayDriver::writeSpiDataByte(uint8_t data)
{
    writeSpiData(&data, 1);
}

void displayDriver::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    if (x > mDisplayBuffer->getWidth() || y > mDisplayBuffer->getHeight())
    {
        return;
    }

    uint16_t charOffset = (character - 32) * font.height;

    uint16_t yOffset = y;
	for (uint16_t i = 0; i < font.height; i++) 
    {
		uint16_t fontValue = font.data[charOffset];
		for (uint16_t j = 0; j < font.width; j++) 
        {
			if ((fontValue & 0x8000) == 0x8000) 
            {
                drawPixel(colorR8G8B8, x + j, yOffset);
			}
            fontValue <<= 1;
		}
        yOffset++;
        charOffset++;
	}
}

void displayDriver::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
	while (*message) 
    {
		drawChar(colorR8G8B8, font, x, y, *message);
		x += font.width;
		message++;
	}
}

void displayDriver::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
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
    }

    if (x0 > x1) {
		swap = x0;
		x0 = x1;
		x1 = swap;
		swap = y0;
		y0 = y1;
		y1 = swap;
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

    for (; x0<=x1; x0++) 
    {
        if (steep) 
        {
            drawPixel(colorR8G8B8, y0, x0);
        } 
        else 
        {
            drawPixel(colorR8G8B8, x0, y0);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void displayDriver::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	drawLine(colorR8G8B8, x, y, x + width, y);
	drawLine(colorR8G8B8, x, y, x, y + height);
	drawLine(colorR8G8B8, x, y + height, x + width, y + height);
	drawLine(colorR8G8B8, x + width, y, x + width, y + height);
}

void displayDriver::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
	drawLine(colorR8G8B8, x1, y1, x2, y2);
	drawLine(colorR8G8B8, x2, y2, x3, y3);
	drawLine(colorR8G8B8, x3, y3, x1, y1);
}

void displayDriver::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t xx = 0;
	int16_t yy = radius;

	drawPixel(colorR8G8B8, x, y + radius);
	drawPixel(colorR8G8B8, x, y - radius);
	drawPixel(colorR8G8B8, x + radius, y);
	drawPixel(colorR8G8B8, x - radius, y);

	while (xx < yy) {
		if (f >= 0) {
			yy--;
			ddF_y += 2;
			f += ddF_y;
		}
		xx++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(colorR8G8B8, x + xx, y + yy);
		drawPixel(colorR8G8B8, x - xx, y + yy);
		drawPixel(colorR8G8B8, x + xx, y - yy);
		drawPixel(colorR8G8B8, x - xx, y - yy);

		drawPixel(colorR8G8B8, x + yy, y + xx);
		drawPixel(colorR8G8B8, x - yy, y + xx);
		drawPixel(colorR8G8B8, x + yy, y - xx);
		drawPixel(colorR8G8B8, x - yy, y - xx);
	}
}

void displayDriver::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint8_t i;

    uint16_t displayWidth =  mDisplayBuffer->getWidth();
    uint16_t displayHeight =  mDisplayBuffer->getHeight();

	if (x >= displayWidth || y >= displayHeight) {
		return;
	}

	if ((x + width) >= displayWidth) 
    {
		width = displayWidth - x;
	}

	if ((y + height) >= displayHeight) 
    {
		height = displayHeight - y;
	}

	for (i = 0; i <= height; i++) 
    {
		drawLine(colorR8G8B8, x, y + i, x + width, y + i);
	}
}

void displayDriver::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
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
		drawLine(colorR8G8B8, x, y, x3, y3);
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

void displayDriver::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t xx = 0;
	int16_t yy = radius;

	drawPixel(colorR8G8B8, x, y + radius);
	drawPixel(colorR8G8B8, x, y - radius);
	drawPixel(colorR8G8B8, x + radius, y);
	drawPixel(colorR8G8B8, x - radius, y);
	drawLine(colorR8G8B8, x - radius, y, x + radius, y);

	while (xx < yy) {
		if (f >= 0) {
			yy--;
			ddF_y += 2;
			f += ddF_y;
		}
		xx++;
		ddF_x += 2;
		f += ddF_x;

		drawLine(colorR8G8B8, x - xx, y + yy, x + xx, y + yy);
		drawLine(colorR8G8B8, x + xx, y - yy, x - xx, y - yy);
		drawLine(colorR8G8B8, x + yy, y + x, x - yy, y + xx);
		drawLine(colorR8G8B8, x + yy, y - x, x - yy, y - xx);
	}
}