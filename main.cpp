#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fonts.h"
#include "deviceLegacy.h"
#include "displayDriver.h"
#include "displayGC9A01A.h"
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

	printf("Initializing Display\n");

	deviceLegacy* device = new deviceLegacy();
	device->initSpi(spi1, 1 * 1024 * 1024);

	displayDriver* display = (displayDriver*)new displaySH1122();
	display->fill(0x000000);
	display->drawDisplay();

	uint32_t counter = 0;
	while (true)
	{
		device->poll();
		counter++;

		if (counter == 100)
		{
			counter = 0;
			display->fill(0x000000);
			for (int y = 0; y < device->getRows(); y++)
			{
				for (int x = 0; x < device->getCols(); x++)
				{
					display->drawChar(0xffffff, fonts::Font_12x16(), x << 3, y << 3, device->getDisplayChar(y, x));
				}
			}
			display->drawDisplay();
		}
		
		sleep_ms(10);
		counter++;
	}
}
