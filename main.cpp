#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fonts.h"
#include "st7789Driver.h"

#define FLAG_VALUE 123

void core1_entry() 
{
    multicore_fifo_push_blocking(FLAG_VALUE);
    auto g = multicore_fifo_pop_blocking();
    if (g != FLAG_VALUE)
	{
        printf("Hmm, that's not right on core 1!\n");
		return;
	}

    while (true)
	{
        printf("HELLO\n");
		sleep_ms(100);
	}
}

int main() 
{
    stdio_init_all();
	multicore_launch_core1(core1_entry);
	auto g = multicore_fifo_pop_blocking();
    if (g != FLAG_VALUE)
	{
        printf("Hmm, that's not right on core 0!\n");
	}

    st7789Driver::Init();

    while (true) 
	{
		st7789Driver::WriteString(10, 20, "Speed Test", fonts::Font_11x18(), RED, WHITE);
		sleep_ms(1000);
		st7789Driver::Fill_Color(CYAN);
		sleep_ms(500);
		st7789Driver::Fill_Color(RED);
		sleep_ms(500);
		st7789Driver::Fill_Color(BLUE);
		sleep_ms(500);
		st7789Driver::Fill_Color(GREEN);
		sleep_ms(500);
		st7789Driver::Fill_Color(YELLOW);
		sleep_ms(500);
		st7789Driver::Fill_Color(BROWN);
		sleep_ms(500);
		st7789Driver::Fill_Color(DARKBLUE);
		sleep_ms(500);
		st7789Driver::Fill_Color(MAGENTA);
		sleep_ms(500);
		st7789Driver::Fill_Color(LIGHTGREEN);
		sleep_ms(500);
		st7789Driver::Fill_Color(LGRAY);
		sleep_ms(500);
		st7789Driver::Fill_Color(LBBLUE);
		sleep_ms(500);
		st7789Driver::Fill_Color(WHITE);
		sleep_ms(500);

		st7789Driver::WriteString(10, 10, "Font test.", fonts::Font_16x26(), GBLUE, WHITE);
		st7789Driver::WriteString(10, 50, "Hello Steve!", fonts::Font_7x10(), RED, WHITE);
		st7789Driver::WriteString(10, 75, "Hello Steve!", fonts::Font_11x18(), YELLOW, WHITE);
		st7789Driver::WriteString(10, 100, "Hello Steve!", fonts::Font_16x26(), MAGENTA, WHITE);
		sleep_ms(1000);

		st7789Driver::Fill_Color(RED);
		st7789Driver::WriteString(10, 10, "Rect./Line.", fonts::Font_11x18(), YELLOW, BLACK);
		st7789Driver::DrawRectangle(30, 30, 100, 100, WHITE);
		sleep_ms(1000);

		st7789Driver::Fill_Color(RED);
		st7789Driver::WriteString(10, 10, "Filled Rect.", fonts::Font_11x18(), YELLOW, BLACK);
		st7789Driver::DrawFilledRectangle(30, 30, 50, 50, WHITE);
		sleep_ms(1000);

		st7789Driver::Fill_Color(RED);
		st7789Driver::WriteString(10, 10, "Circle.", fonts::Font_11x18(), YELLOW, BLACK);
		st7789Driver::DrawCircle(60, 60, 25, WHITE);
		sleep_ms(1000);

		st7789Driver::Fill_Color(RED);
		st7789Driver::WriteString(10, 10, "Filled Cir.", fonts::Font_11x18(), YELLOW, BLACK);
		st7789Driver::DrawFilledCircle(60, 60, 25, WHITE);
		sleep_ms(1000);

		st7789Driver::Fill_Color(RED);
		st7789Driver::WriteString(10, 10, "Triangle", fonts::Font_11x18(), YELLOW, BLACK);
		st7789Driver::DrawTriangle(30, 30, 30, 70, 60, 40, WHITE);
		sleep_ms(1000);

		st7789Driver::Fill_Color(RED);
		st7789Driver::WriteString(10, 10, "Filled Tri", fonts::Font_11x18(), YELLOW, BLACK);
		st7789Driver::DrawFilledTriangle(30, 30, 30, 70, 60, 40, WHITE);
		sleep_ms(1000);

		//	If FLASH cannot storage anymore datas, please delete codes below.
		st7789Driver::Fill_Color(WHITE);
		st7789Driver::DrawImage(0, 0, 128, 128, fonts::Saber());
		sleep_ms(3000);
    }
}
