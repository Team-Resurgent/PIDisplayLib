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
#include "color.h"
#include <math.h>

#define DISPLAY SH1122
//#define DISPLAY ST7789 

#define FLAG_VALUE 123

void core1_entry() 
{
    while (true)
	{
		// gpio_put(DISPLAY_BACKLIGHT, 1);
		// sleep_ms(10);
		// gpio_put(DISPLAY_BACKLIGHT, 0);
		sleep_ms(10);
	}
}

int main() 
{
    stdio_init_all();
	multicore_launch_core1(core1_entry);

	printf("Initializing Display\n");

	//displayDriver* display = (displayDriver*)new displayGC9A01A();
	//displayDriver* display = (displayDriver*)new displayST7789();
	displayDriver* display = (displayDriver*)new displaySH1122();
	//displayDriver* display = (displayDriver*)new displaySH1106();
	//display->contrast(0x80);
	//display->brightness(0xff);
	//display->invert(false);
	//display->rotate(270);

	while (true)
	{
		display->fill(0x000000);
		display->drawString(0xffffff, fonts::Font_12x16(), 8, 0, "PrometheOS: V1.3.0");
		display->drawString(0xffffff, fonts::Font_12x16(), 8, 16, "Free Mem: 112MB");
		display->drawString(0xffffff, fonts::Font_12x16(), 8, 32, "IP: 192.168.0.100");
		display->drawString(0xffffff, fonts::Font_12x16(), 8, 48, "FAN: 40% CPU: 35c");
		// display->drawString(0xffffff, fonts::Font_8x8(), 2, 42, "Encoder: Focus");
		// display->drawString(0xffffff, fonts::Font_8x8(), 2, 52, "Video Mode: 480p");
		display->drawDisplay();
		sleep_ms(10);
	}

//#ifdef DRIVER
    // #if DRIVER == ST8869
    // 	st7789Driver::Init();
	// 	while (true) 
	// 	{
	// 		st7789Driver::drawString(30, 30, "PrometheOS: V1.3.0", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
	// 		st7789Driver::drawString(30, 60, "Free Mem: 112MB", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
	// 		st7789Driver::drawString(30, 90, "IP: 192.168.0.100", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
	// 		st7789Driver::drawString(30, 120, "FAN: 40%% CPU: 35c", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
	// 		st7789Driver::drawString(30, 150, "Enocder: Focus", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
	// 		st7789Driver::drawString(30, 180, "Video Mode: 480p", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
	// 		sleep_ms(1000);
	// 	}
	// #elif DRIVER == SH1122
    	// sh1122Driver::Init();
		// while (true) 
		// {
		// 	// st7789Driver::drawString(30, 30, "PrometheOS: V1.3.0", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
		// 	// st7789Driver::drawString(30, 60, "Free Mem: 112MB", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
		// 	// st7789Driver::drawString(30, 90, "IP: 192.168.0.100", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
		// 	// st7789Driver::drawString(30, 120, "FAN: 40%% CPU: 35c", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
		// 	// st7789Driver::drawString(30, 150, "Enocder: Focus", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
		// 	// st7789Driver::drawString(30, 180, "Video Mode: 480p", fonts::Font_11x18(), ST7789_WHITE, ST7789_BLACK);
		// 	sleep_ms(1000);
		// }
// 	#endif
// #endif
}
