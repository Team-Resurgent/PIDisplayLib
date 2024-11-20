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
#include <math.h>

#define DISPLAY SH1122
//#define DISPLAY ST7789 

#define FLAG_VALUE 123

void core1_entry() 
{
    while (true)
	{
		sleep_ms(5000);
	}
}

int main() 
{
    stdio_init_all();
	multicore_launch_core1(core1_entry);

	sleep_ms(2000);

	printf("Initializing Device\n");

	const int testConfig = 0;

	if (testConfig == 0)
	{
		printf("testconfig 0\n");
		textDisplayDriver* display = (textDisplayDriver*)new textDisplayUS2066();
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
		deviceTouch* touch = new deviceTouch();
		touch->initSpi(spi1, 3 * 1000 * 1000);

		pixelDisplayDriver* display = (pixelDisplayDriver*)new pixelDisplayILI9341();
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

			uint16_t x = 0xffff;
			uint16_t y = 0xffff;
			touch->readTouchPos(x, y);
			sleep_ms(10);
		}
	}

	if (testConfig == 2)
	{
		deviceLegacy* device = new deviceLegacy();
		device->initSpi(spi1, 10 * 1024 * 1024);

		pixelDisplayDriver* display = (pixelDisplayDriver*)new pixelDisplaySH1122();
		display->fill(0x000000);
		display->drawDisplay();

		uint32_t counter = 0;
		while (true)
		{
			device->poll();
			counter++;

			if (counter == 1000)
			{
				counter = 0;
				display->fill(0x000000);
				for (int y = 0; y < device->getRows(); y++)
				{
					for (int x = 0; x < device->getCols(); x++)
					{
						display->drawChar(0xffffff, fonts::Font_12x16(), (x * 12) + 8, y << 4, device->getDisplayChar(y, x));
					}
				}
				display->drawDisplay();
			}

			sleep_ms(1);
		}
	}

	if (testConfig == 3)
	{
		pixelDisplayDriver* display = (pixelDisplayDriver*)new pixelDisplaySSD1306();
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
		pixelDisplayDriver* display = (pixelDisplayDriver*)new pixelDisplaySSD1309(displayModeSpi);
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
		pixelDisplayDriver* display = (pixelDisplayDriver*)new pixelDisplayST7789();
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
		deviceRTC* rtc = new deviceRTC();
		rtc->initI2c(0x68);

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

	if (testConfig == 7)
	{
		printf("scanning\n");
		deviceEEPROM* eeprom = new deviceEEPROM();
		eeprom->initI2c(0x54);
		eeprom->scanI2c();
		printf("scan done\n");
		eeprom->read();
	}
}
