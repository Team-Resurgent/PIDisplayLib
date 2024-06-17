#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fonts.h"
#include "deviceLegacy.h"
#include "deviceTouch.h"
#include "pixelDisplayDriver.h"
#include "pixelDisplayGC9A01A.h"
#include "pixelDisplayILI9341.h"
#include "pixelDisplaySH1106.h"
#include "pixelDisplaySH1122.h"
#include "pixelDisplayST7789.h"
#include "pixelDisplaySSD1306.h"
#include "pixelDisplaySSD1309.h"
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

	const int testConfig = 3;

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
}
