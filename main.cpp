#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fonts.h"
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
	uint8_t message[] = {'H', 'e', 'l', 'l', 'o', '\n'};
    while (true)
	{
		//spiTest::writeSpi(message, sizeof(message));
		//spiTest::readSpi();
		sleep_ms(5000);
	}
}

int main() 
{
    stdio_init_all();
	multicore_launch_core1(core1_entry);

	printf("Initializing Display\n");

	//spiTest::initSpi(spi1, 1 * 1024 * 1024);

	//displayDriver* display = (displayDriver*)new displayGC9A01A();
	//displayDriver* display = (displayDriver*)new displayST7789();
	//displayDriver* display = (displayDriver*)new displaySH1122();
	//displayDriver* display = (displayDriver*)new displaySH1106();
	//display->contrast(0x80);
	//display->brightness(0xff);
	//display->invert(false);
	//display->rotate(270);

	while (true)
	{
		// display->fill(0x000000);
		// display->drawString(0xffffff, fonts::Font_12x16(), 8, 0, "PrometheOS: V1.3.0");
		// display->drawString(0xffffff, fonts::Font_12x16(), 8, 16, "Free Mem: 112MB");
		// display->drawString(0xffffff, fonts::Font_12x16(), 8, 32, "IP: 192.168.0.100");
		// display->drawString(0xffffff, fonts::Font_12x16(), 8, 48, "FAN: 40% CPU: 35c");
		// display->drawString(0xffffff, fonts::Font_8x8(), 2, 42, "Encoder: Focus");
		// display->drawString(0xffffff, fonts::Font_8x8(), 2, 52, "Video Mode: 480p");
		// display->drawDisplay();
		sleep_ms(1000);
	}
}
