#include "displayBase.h"
#include "displayConfig.h"

#include "hardware/spi.h"
#include "hardware/structs/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <cstdio>
#include <stdlib.h>
#include <string.h>

#define I2C_TIMEOUT_US 100
#define I2C_COMMAND_MODE 0x80
#define I2C_DATA_MODE 0x40

void displayBase::initSpi(spi_inst_t* spi, uint32_t baudRate)
{
	mIsSpi = true;
	mSpi = spi;

	if (SPI_DISPLAY_BACKLIGHT >= 0)
	{
    	gpio_init(SPI_DISPLAY_BACKLIGHT);
    	gpio_put(SPI_DISPLAY_BACKLIGHT, 1);
    	gpio_set_dir(SPI_DISPLAY_BACKLIGHT, GPIO_OUT);
	}

    gpio_init(SPI_DISPLAY_RST);
    gpio_put(SPI_DISPLAY_RST, 1);
    gpio_set_dir(SPI_DISPLAY_RST, GPIO_OUT);

    gpio_init(SPI_DISPLAY_DC);
    gpio_put(SPI_DISPLAY_DC, 0);
    gpio_set_dir(SPI_DISPLAY_DC, GPIO_OUT);

    spi_init(mSpi, baudRate);
	spi_set_slave(mSpi, false);
    gpio_set_function(SPI_DISPLAY_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_DISPLAY_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_DISPLAY_TX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_DISPLAY_CSN, GPIO_FUNC_SPI);
    bi_decl(bi_3pins_with_func((uint32_t)SPI_DISPLAY_RX, (uint32_t)SPI_DISPLAY_TX, (uint32_t)SPI_DISPLAY_SCK, GPIO_FUNC_SPI));

    spi_set_format(mSpi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);

    sleep_ms(10);
	gpio_put(SPI_DISPLAY_RST, 0);
	sleep_ms(10);
	gpio_put(SPI_DISPLAY_RST, 1);
}

void displayBase::initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate)
{
	mIsSpi = false;
	mI2c = i2c;
	mI2cAddress = address;

	if (I2C_DISPLAY_BACKLIGHT >= 0)
	{
    	gpio_init(I2C_DISPLAY_BACKLIGHT);
    	gpio_put(I2C_DISPLAY_BACKLIGHT, 1);
    	gpio_set_dir(I2C_DISPLAY_BACKLIGHT, GPIO_OUT);
	}

	i2c_init(mI2c, baudRate);
    gpio_set_function(I2C_DISPLAY_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_DISPLAY_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_DISPLAY_SDA);
    gpio_pull_up(I2C_DISPLAY_SCL);
    bi_decl(bi_2pins_with_func(I2C_DISPLAY_SDA, I2C_DISPLAY_SCL, GPIO_FUNC_I2C));

	mI2cAddress = address == -1 ? scanI2c() : address;
}

int32_t displayBase::scanI2c()
{
	for (int32_t address = 0; address < 256; address++)
	{
   		uint8_t testvalue = 0;
		if (i2c_write_timeout_us(mI2c, address, &testvalue, 1, false, I2C_TIMEOUT_US) == 1)
		{
			return address;
		}
	}
	return -1;
}

int32_t displayBase::getI2cAddress()
{
	return mI2cAddress;
}