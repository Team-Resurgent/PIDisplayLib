#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fonts.h"
#include "deviceLegacy.h"
#include "deviceRTC.h"
#include "deviceEEPROM.h"
#include "deviceTouch.h"
#include "pixelDisplayDriver.h"
#include "pixelDisplayGC9A01A.h"
#include "pixelDisplayILI9341.h"
#include "pixelDisplaySH1106.h"
#include "pixelDisplaySH1122.h"
#include "pixelDisplayST7789.h"
#include "pixelDisplaySSD1306.h"
#include "pixelDisplaySSD1309.h"
#include "textDisplayDriver.h"
#include "textDisplayUS2066.h"
#include "color.h"

auto_init_mutex(mutex);

static deviceLegacy* device = NULL;
static bool refresh = false;

void render_loop() 
{
	const uint16_t width = 128;
	const uint16_t height = 64;
	const uint16_t xShift = 0;
	const uint16_t yShift = 0;
	const uint16_t bitsPerPixel = 1;

	spi_inst_t* spi = spi0;
	const uint32_t baudRate = 100 * 1000;
	const uint8_t rxPin = 13;
	const uint8_t sckPin = 10;
	const uint8_t csnPin = 9;
	const uint8_t rstPin = 12;
	const uint8_t dcPin = 8;
	const uint8_t backlightPin = 20;

	pixelDisplaySSD1309* display = new pixelDisplaySSD1309(width, height, xShift, yShift, bitsPerPixel);
	display->initSpi(spi, baudRate, rxPin, sckPin, csnPin, rstPin, dcPin, backlightPin);
	display->fill(0x000000);
	display->drawDisplay();

    while (true)
	{
		mutex_enter_blocking(&mutex);
		bool needRefresh = refresh;
		refresh = false;
		mutex_exit(&mutex);

		if (needRefresh == true)
		{
			display->fill(0x000000);
			for (int y = 0; y < device->getRows(); y++)
			{
				for (int x = 0; x < device->getCols(); x++)
				{
					display->drawChar(0xffffff, fonts::Font_6x8(), (x * 6) + 2, y << 3, device->getDisplayChar(y, x));
				}
			}
			display->drawDisplay();
		}
		sleep_ms(1000);
	}
}

int main() 
{
    stdio_init_all();

	const int testConfig = 2;

	if (testConfig == 0)
	{
		printf("testconfig 0\n");
		textDisplayUS2066* display = new textDisplayUS2066();
		while (true)
		{
			display->setCursor(0, 0);
			display->printMessage("Hello");
			display->setCursor(1, 0);
			display->printMessage("World");
			display->setCursor(2, 0);
			display->printMessage("Yet");
			display->setCursor(3, 0);
			display->printMessage("Again");

			sleep_ms(1000);
			
			//printf("testconfig 0 done\n");
		}
	}

	if (testConfig == 1)
	{
		const uint16_t width = 240;
		const uint16_t height = 320;
		const uint16_t xShift = 0;
		const uint16_t yShift = 0;
		const uint16_t bitsPerPixel = 16;

		spi_inst_t* spi = spi0;
		const uint32_t baudRate = 100 * 1000;
		const uint8_t rxPin = 13;
		const uint8_t sckPin = 10;
		const uint8_t csnPin = 9;
		const uint8_t rstPin = 12;
		const uint8_t dcPin = 8;
		const uint8_t backlightPin = 20;

		pixelDisplayILI9341* display = new pixelDisplayILI9341(width, height, xShift, yShift, bitsPerPixel);
		display->initSpi(spi, baudRate, rxPin, sckPin, csnPin, rstPin, dcPin, backlightPin);

		display->fill(0xff0000);
		display->rotate(270);
		display->brightness(20);
		while (true)
		{
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 8, "PrometheOS: V1.3.0");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 24, "Free Mem: 112MB");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 40, "IP: 192.168.0.100");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 56, "FAN: 40% CPU: 35c");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 72, "Encoder: Focus");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 88, "Video Mode: 480p");
			display->drawDisplay();

			sleep_ms(10);
		}
	}

	// Test of spi2par
	if (testConfig == 2)
	{
		spi_inst_t* spi = spi0;
		const uint32_t baudRate = 100 * 1000;
		const uint8_t rxPin = 0;
		const uint8_t csnPin = 1;
		const uint8_t sckPin = 2;

		device = new deviceLegacy(4, 20);
		device->initSpi(spi, baudRate, rxPin, sckPin, csnPin);
		multicore_launch_core1(render_loop);

		while (true)
		{
			bool needRefresh = device->poll();
			if (needRefresh == true)
			{
				mutex_enter_blocking(&mutex);
				refresh = true;
				mutex_exit(&mutex);
			}
			sleep_ms(1);
		}
	}

	if (testConfig == 3)
	{
		const uint16_t width = 128;
		const uint16_t height = 64;
		const uint16_t xShift = 0;
		const uint16_t yShift = 0;
		const uint16_t bitsPerPixel = 1;

		i2c_inst_t* i2c = i2c0;
		const uint32_t baudRate = 100 * 1000;
		const uint8_t address = -1;
		const uint8_t sdaPin = 13;
		const uint8_t sclPin = 10;
		const uint8_t backlightPin = 20;

		pixelDisplaySSD1306* display = new pixelDisplaySSD1306(width, height, xShift, yShift, bitsPerPixel);
		display->initI2c(i2c, address, baudRate, sdaPin, sclPin, backlightPin);
		printf("i2c Addres %i\n", display->scanI2c());

		display->fill(0x000000);
		display->rotate(180);
		//display->invert(true);
		while (true)
		{
			display->drawString(0xffffff, fonts::Font_8x8(), 8, 8, "Hello");
			display->drawString(0xffffff, fonts::Font_8x8(), 8, 56, "World");
			display->drawDisplay();
		}
	}

	if (testConfig == 4)
	{
		const uint16_t width = 128;
		const uint16_t height = 64;
		const uint16_t xShift = 0;
		const uint16_t yShift = 0;
		const uint16_t bitsPerPixel = 1;

		spi_inst_t* spi = spi0;
		const uint32_t baudRate = 100 * 1000;
		const uint8_t rxPin = 13;
		const uint8_t sckPin = 10;
		const uint8_t csnPin = 9;
		const uint8_t rstPin = 12;
		const uint8_t dcPin = 8;
		const uint8_t backlightPin = 20;

		pixelDisplaySSD1309* display = new pixelDisplaySSD1309(width, height, xShift, yShift, bitsPerPixel);
		display->initSpi(spi, baudRate, rxPin, sckPin, csnPin, rstPin, dcPin, backlightPin);

		display->fill(0x000000);
		//display->rotate(180);
		//display->invert(true);
		while (true)
		{
			display->drawString(0xffffff, fonts::Font_8x8(), 8, 4, "Transparent");
			display->drawString(0xffffff, fonts::Font_8x8(), 8, 20, "velcro...?");
			display->drawString(0xffffff, fonts::Font_8x8(), 8, 42, "Can't see that");
			display->drawString(0xffffff, fonts::Font_8x8(), 8, 52, "catching on...");
			display->drawDisplay();
		}
	}

	if (testConfig == 5)
	{
		const uint16_t width = 240;
		const uint16_t height = 320;
		const uint16_t xShift = 0;
		const uint16_t yShift = 0;
		const uint16_t bitsPerPixel = 16;

		spi_inst_t* spi = spi0;
		const uint32_t baudRate = 100 * 1000;
		const uint8_t rxPin = 13;
		const uint8_t sckPin = 10;
		const uint8_t csnPin = 9;
		const uint8_t rstPin = 12;
		const uint8_t dcPin = 8;
		const uint8_t backlightPin = 20;

		pixelDisplayST7789* display = new pixelDisplayST7789(width, height, xShift, yShift, bitsPerPixel);
		display->initSpi(spi, baudRate, rxPin, sckPin, csnPin, rstPin, dcPin, backlightPin);

		display->fill(0xff0000);
		display->rotate(270);
		//display->brightness(20);
		while (true)
		{
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 8, "PrometheOS: V1.3.0");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 24, "Free Mem: 112MB");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 40, "IP: 192.168.0.100");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 56, "FAN: 40% CPU: 35c");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 72, "Encoder: Focus");
			display->drawString(0xffffff, fonts::Font_12x16(), 8, 88, "Video Mode: 480p");
			display->drawDisplay();
		}
	}

	if (testConfig == 6)
	{
		i2c_inst_t* i2c = i2c0;
		const uint32_t baudRate = 100 * 1000;
		const uint8_t address = 0x68;
		const uint8_t sdaPin = 13;
		const uint8_t sclPin = 10;
		const uint8_t backlightPin = 20;

		deviceRTC* rtc = new deviceRTC();
		rtc->initI2c(i2c, address, baudRate, sdaPin, sclPin);

		datetime_t datetime;
		datetime.day = 21;
		datetime.month = 6;
		datetime.year = 2024;
		datetime.dotw = 5;
		datetime.hour = 14;
		datetime.min = 44;
		datetime.sec = 0;
		// bool setOk = rtc->setDateTime(&datetime);
		// printf("Date set ok = %i\n", setOk);

		while (true)
		{
			bool getOk = rtc->getDateTime(&datetime);
			if (getOk)
			{
				printf("Datetime Day = %i\n", datetime.day);
				printf("Datetime Month = %i\n", datetime.month);
				printf("Datetime Year = %i\n", datetime.year);
				printf("Datetime Dotw = %i\n", datetime.dotw);
				printf("Datetime Hour = %i\n", datetime.hour);
				printf("Datetime Min = %i\n", datetime.min);
				printf("Datetime Sec = %i\n", datetime.sec);
			}

			float temperature;
			bool getTemp = rtc->getTemperature(&temperature);
			if (getTemp)
			{
				printf("Temp Degrees C = %f\n", temperature);
			}

			sleep_ms(1000);
		}
		delete rtc;
	}
}
