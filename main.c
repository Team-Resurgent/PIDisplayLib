/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hardware/structs/spi.h"
#include "hardware/spi.h"

#include "fonts.h"
#include "ST7789Driver.h"

int main() 
{
    stdio_init_all();

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

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
    spi_init(0, 80 * 1000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
    bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));

    spi_set_format(0, 8, 1, 1, SPI_MSB_FIRST);

    ST7789Driver_Init();
    ST7789Driver_SetRotation(ST7789_ROTATION);

    while (true) {

        printf("HELLO\n");

        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);

	ST7789Driver_WriteString(10, 20, "Speed Test", Font_11x18, RED, WHITE);
	sleep_ms(1000);
	ST7789Driver_Fill_Color(CYAN);
    sleep_ms(500);
	ST7789Driver_Fill_Color(RED);
    sleep_ms(500);
	ST7789Driver_Fill_Color(BLUE);
    sleep_ms(500);
	ST7789Driver_Fill_Color(GREEN);
    sleep_ms(500);
	ST7789Driver_Fill_Color(YELLOW);
    sleep_ms(500);
	ST7789Driver_Fill_Color(BROWN);
    sleep_ms(500);
	ST7789Driver_Fill_Color(DARKBLUE);
    sleep_ms(500);
	ST7789Driver_Fill_Color(MAGENTA);
    sleep_ms(500);
	ST7789Driver_Fill_Color(LIGHTGREEN);
    sleep_ms(500);
	ST7789Driver_Fill_Color(LGRAY);
    sleep_ms(500);
	ST7789Driver_Fill_Color(LBBLUE);
    sleep_ms(500);
	ST7789Driver_Fill_Color(WHITE);
	sleep_ms(500);

	ST7789Driver_WriteString(10, 10, "Font test.", Font_16x26, GBLUE, WHITE);
	ST7789Driver_WriteString(10, 50, "Hello Steve!", Font_7x10, RED, WHITE);
	ST7789Driver_WriteString(10, 75, "Hello Steve!", Font_11x18, YELLOW, WHITE);
	ST7789Driver_WriteString(10, 100, "Hello Steve!", Font_16x26, MAGENTA, WHITE);
	sleep_ms(1000);

	ST7789Driver_Fill_Color(RED);
	ST7789Driver_WriteString(10, 10, "Rect./Line.", Font_11x18, YELLOW, BLACK);
	ST7789Driver_DrawRectangle(30, 30, 100, 100, WHITE);
	sleep_ms(1000);

	ST7789Driver_Fill_Color(RED);
	ST7789Driver_WriteString(10, 10, "Filled Rect.", Font_11x18, YELLOW, BLACK);
	ST7789Driver_DrawFilledRectangle(30, 30, 50, 50, WHITE);
	sleep_ms(1000);

	ST7789Driver_Fill_Color(RED);
	ST7789Driver_WriteString(10, 10, "Circle.", Font_11x18, YELLOW, BLACK);
	ST7789Driver_DrawCircle(60, 60, 25, WHITE);
	sleep_ms(1000);

	ST7789Driver_Fill_Color(RED);
	ST7789Driver_WriteString(10, 10, "Filled Cir.", Font_11x18, YELLOW, BLACK);
	ST7789Driver_DrawFilledCircle(60, 60, 25, WHITE);
	sleep_ms(1000);

	ST7789Driver_Fill_Color(RED);
	ST7789Driver_WriteString(10, 10, "Triangle", Font_11x18, YELLOW, BLACK);
	ST7789Driver_DrawTriangle(30, 30, 30, 70, 60, 40, WHITE);
	sleep_ms(1000);

	ST7789Driver_Fill_Color(RED);
	ST7789Driver_WriteString(10, 10, "Filled Tri", Font_11x18, YELLOW, BLACK);
	ST7789Driver_DrawFilledTriangle(30, 30, 30, 70, 60, 40, WHITE);
	sleep_ms(1000);

	//	If FLASH cannot storage anymore datas, please delete codes below.
	ST7789Driver_Fill_Color(WHITE);
	ST7789Driver_DrawImage(0, 0, 128, 128, (uint16_t *)saber);
	sleep_ms(3000);
    }
}
