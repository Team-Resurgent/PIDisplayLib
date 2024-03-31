#include "st7789Driver.h"
#include "hardware/spi.h"
#include "hardware/structs/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "fonts.h"

#include <stdlib.h>

void st7789Driver::WriteCommand(uint8_t cmd)
{
	gpio_put(DISPLAY_DC, 0);
    spi_write_blocking(spi_default, &cmd, 1);
}

void st7789Driver::WriteData(uint8_t *buff, uint32_t buff_size)
{
	gpio_put(DISPLAY_DC, 1);
    spi_write_blocking(spi_default, buff, buff_size);
}

void st7789Driver::WriteSmallData(uint8_t data)
{
    WriteData(&data, 1);
}

void st7789Driver::SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t x_start = x0 + ST7789_X_SHIFT, x_end = x1 + ST7789_X_SHIFT;
	uint16_t y_start = y0 + ST7789_Y_SHIFT, y_end = y1 + ST7789_Y_SHIFT;

	/* Column Address set */
	WriteCommand(ST7789_CASET);
	{
		uint8_t data[] = {(uint8_t)(x_start >> 8), (uint8_t)(x_start & 0xFF), (uint8_t)(x_end >> 8), (uint8_t)(x_end & 0xFF)};
		WriteData(data, sizeof(data));
	}

	/* Row Address set */
	WriteCommand(ST7789_RASET);
	{
		uint8_t data[] = {(uint8_t)(y_start >> 8), (uint8_t)(y_start & 0xFF), (uint8_t)(y_end >> 8), (uint8_t)(y_end & 0xFF)};
		WriteData(data, sizeof(data));
	}
	/* Write to RAM */
	WriteCommand(ST7789_RAMWR);
}

void st7789Driver::SetRotation(uint8_t m)
{
	WriteCommand(ST7789_MADCTL);	// MADCTL
	switch (m) {
	case 0:
		WriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
		break;
	case 1:
		WriteSmallData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		break;
	case 2:
		WriteSmallData(ST7789_MADCTL_RGB);
		break;
	case 3:
		WriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		break;
	default:
		break;
	}
}

void st7789Driver::Fill_Color(uint16_t color)
{
	uint16_t i;
    uint16_t offset;
	SetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);

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
        WriteData(row, ST7789_WIDTH * sizeof(uint16_t));
    }

    free(row);
}

void st7789Driver::DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x < 0) || (x >= ST7789_WIDTH) ||
		 (y < 0) || (y >= ST7789_HEIGHT))	return;

	SetAddressWindow(x, y, x, y);
	uint8_t data[] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
	WriteData(data, sizeof(data));
}

void st7789Driver::Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
	if ((xEnd < 0) || (xEnd >= ST7789_WIDTH) ||
		 (yEnd < 0) || (yEnd >= ST7789_HEIGHT))	return;
	uint16_t i, j;
	SetAddressWindow(xSta, ySta, xEnd, yEnd);
	for (i = ySta; i <= yEnd; i++)
		for (j = xSta; j <= xEnd; j++) {
			uint8_t data[] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
			WriteData(data, sizeof(data));
		}
}

void st7789Driver::DrawPixel_4px(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x <= 0) || (x > ST7789_WIDTH) ||
		 (y <= 0) || (y > ST7789_HEIGHT))	return;

	Fill(x - 1, y - 1, x + 1, y + 1, color);

}

void st7789Driver::DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) 
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
            DrawPixel(y0, x0, color);
        } else {
            DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void st7789Driver::DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	DrawLine(x1, y1, x2, y1, color);
	DrawLine(x1, y1, x1, y2, color);
	DrawLine(x1, y2, x2, y2, color);
	DrawLine(x2, y1, x2, y2, color);
}

void st7789Driver::DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;


	DrawPixel(x0, y0 + r, color);
	DrawPixel(x0, y0 - r, color);
	DrawPixel(x0 + r, y0, color);
	DrawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		DrawPixel(x0 + x, y0 + y, color);
		DrawPixel(x0 - x, y0 + y, color);
		DrawPixel(x0 + x, y0 - y, color);
		DrawPixel(x0 - x, y0 - y, color);

		DrawPixel(x0 + y, y0 + x, color);
		DrawPixel(x0 - y, y0 + x, color);
		DrawPixel(x0 + y, y0 - x, color);
		DrawPixel(x0 - y, y0 - x, color);
	}

}

void st7789Driver::DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)
{
	if ((x >= ST7789_WIDTH) || (y >= ST7789_HEIGHT))
		return;
	if ((x + w - 1) >= ST7789_WIDTH)
		return;
	if ((y + h - 1) >= ST7789_HEIGHT)
		return;


	SetAddressWindow(x, y, x + w - 1, y + h - 1);
	WriteData((uint8_t *)data, sizeof(uint16_t) * w * h);
}

void st7789Driver::InvertColors(uint8_t invert)
{
	WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
}

void st7789Driver::WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
	uint32_t i, b, j;

	SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);

	for (i = 0; i < font.height; i++) {
		b = font.data[(ch - 32) * font.height + i];
		for (j = 0; j < font.width; j++) {
			if ((b << j) & 0x8000) {
				uint8_t data[] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
				WriteData(data, sizeof(data));
			}
			else {
				uint8_t data[] = {(uint8_t)(bgcolor >> 8), (uint8_t)(bgcolor & 0xFF)};
				WriteData(data, sizeof(data));
			}
		}
	}
}

void st7789Driver::WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor)
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
		WriteChar(x, y, *str, font, color, bgcolor);
		x += font.width;
		str++;
	}

}

void st7789Driver::DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
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
		DrawLine(x, y + i, x + w, y + i, color);
	}

}

void st7789Driver::DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void st7789Driver::DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
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
		DrawLine(x, y, x3, y3, color);

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

void st7789Driver::DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	DrawPixel(x0, y0 + r, color);
	DrawPixel(x0, y0 - r, color);
	DrawPixel(x0 + r, y0, color);
	DrawPixel(x0 - r, y0, color);
	DrawLine(x0 - r, y0, x0 + r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
		DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

		DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
		DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
	}

}

void st7789Driver::TearEffect(uint8_t tear)
{
	WriteCommand(tear ? 0x35 /* TEON */ : 0x34 /* TEOFF */);
}

void st7789Driver::Init(void)
{
    gpio_init(DISPLAY_BACKLIGHT);
    gpio_set_dir(DISPLAY_BACKLIGHT, GPIO_OUT);
    gpio_put(DISPLAY_BACKLIGHT, 1);

    gpio_init(DISPLAY_RST);
    gpio_set_dir(DISPLAY_RST, GPIO_OUT);
    gpio_put(DISPLAY_RST, 0);

    gpio_init(DISPLAY_DC);
    gpio_set_dir(DISPLAY_DC, GPIO_OUT);
    gpio_put(DISPLAY_DC, 0);

    // Enable SPI 0 at 1 MHz and connect to GPIOs
    spi_init(spi_default, 80 * 1000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
    bi_decl(bi_4pins_with_func((uint32_t)PICO_DEFAULT_SPI_RX_PIN, (uint32_t)PICO_DEFAULT_SPI_TX_PIN, (uint32_t)PICO_DEFAULT_SPI_SCK_PIN, (uint32_t)PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));

    spi_set_format(spi_default, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);

	sleep_ms(25);
    gpio_put(DISPLAY_RST, 0);
	sleep_ms(50);
    gpio_put(DISPLAY_RST, 1);
    sleep_ms(50);
    WriteCommand(ST7789_SWRESET);
    sleep_ms(100);
    WriteCommand(ST7789_COLMOD);		//	Set color mode
    WriteSmallData(ST7789_COLOR_MODE_16bit);
  	WriteCommand(0xB2);				//	Porch control
	uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
	WriteData(data, sizeof(data));

	/* Internal LCD Voltage generator settings */
    WriteCommand(0XB7);				//	Gate Control
    WriteSmallData(0x35);			//	Default value
    WriteCommand(0xBB);				//	VCOM setting
    WriteSmallData(0x20);			//	0.725v (default 0.75v for 0x20)
    WriteCommand(0xC0);				//	LCMCTRL
    WriteSmallData (0x2C);			//	Default value
    WriteCommand (0xC2);				//	VDV and VRH command Enable
    WriteSmallData (0x01);			//	Default value
    WriteCommand (0xC3);				//	VRH set
    WriteSmallData (0x0b);			//	+-4.45v (defalut +-4.1v for 0x0B)
    WriteCommand (0xC4);				//	VDV set
    WriteSmallData (0x20);			//	Default value
    WriteCommand (0xC6);				//	Frame rate control in normal mode
    WriteSmallData (0x0F);			//	Default value (60HZ)
    WriteCommand (0xD0);				//	Power control
    WriteSmallData (0xA4);			//	Default value
    WriteSmallData (0xA1);			//	Default value
	/**************** Division line ****************/

	WriteCommand(0xE0);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
		WriteData(data, sizeof(data));
	}

    WriteCommand(0xE1);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
		WriteData(data, sizeof(data));
	}
    WriteCommand (ST7789_INVON);		//	Inversion ON
	WriteCommand (ST7789_SLPOUT);	//	Out of sleep mode
  	WriteCommand (ST7789_NORON);		//	Normal Display on
  	WriteCommand (ST7789_DISPON);	//	Main screen turned on

    SetRotation(ST7789_ROTATION);

  	sleep_ms(50);
	Fill_Color(BLACK);				//	Fill with Black.
}