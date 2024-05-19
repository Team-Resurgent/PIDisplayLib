#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fonts.h"
#include "deviceLegacy.h"
#include "displayDriver.h"
#include "displayGC9A01A.h"
#include "displayILI9341.h"
#include "displaySH1106.h"
#include "displaySH1122.h"
#include "displayST7789.h"
#include "spiTest.h"
#include "color.h"
#include <math.h>

#define DISPLAY SH1122
//#define DISPLAY ST7789 

#define FLAG_VALUE 123

void core1_entry() 
{
    while (true)
	{
		spiTest::process();
		sleep_ms(5000);
	}
}

int main() 
{
    stdio_init_all();
	multicore_launch_core1(core1_entry);

	printf("Initializing Device\n");

	const int testConfig = 1;

	if (testConfig == 0)
	{
		spiTest::initSpi(spi1, 1 * 1024 * 1024, false);
		while (true)
		{
			spiTest::writeSpi();
			sleep_ms(1000);
		}
	}

	if (testConfig == 1)
	{
		displayDriver* display = (displayDriver*)new displayILI9341();
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
			sleep_ms(1000);
		}
	}

	if (testConfig == 2)
	{
		deviceLegacy* device = new deviceLegacy();
		device->initSpi(spi1, 10 * 1024 * 1024);

		displayDriver* display = (displayDriver*)new displaySH1122();
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
}
