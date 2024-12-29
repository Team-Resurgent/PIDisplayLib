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

void displayBase::initSpi(spi_inst_t* spi, uint32_t baudRate, uint8_t rxPin, uint8_t sckPin, uint8_t csnPin, uint8_t rstPin, uint8_t dcPin, uint8_t backlightPin)
{
	mIsSpi = true;
	mSpi = spi;
	mDcPin = dcPin;

	if (backlightPin >= 0 && backlightPin <= 30)
	{
    	gpio_init(backlightPin);
    	gpio_put(backlightPin, 1);
    	gpio_set_dir(backlightPin, GPIO_OUT);
	}

    gpio_init(rstPin);
    gpio_put(rstPin, 1);
    gpio_set_dir(rstPin, GPIO_OUT);

    gpio_init(dcPin);
    gpio_put(dcPin, 0);
    gpio_set_dir(dcPin, GPIO_OUT);

    spi_init(mSpi, baudRate);
	spi_set_slave(mSpi, false);
    gpio_set_function(rxPin, GPIO_FUNC_SPI);
    gpio_set_function(sckPin, GPIO_FUNC_SPI);
    gpio_set_function(csnPin, GPIO_FUNC_SPI);

    spi_set_format(mSpi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);

    sleep_ms(10);
	gpio_put(rstPin, 0);
	sleep_ms(10);
	gpio_put(rstPin, 1);
}

void displayBase::initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin, uint8_t backlightPin)
{
	mIsSpi = false;
	mI2c = i2c;
	mI2cAddress = address == -1 ? scanI2c() : address;

	if (backlightPin >= 0 && backlightPin <= 30)
	{
    	gpio_init(backlightPin);
    	gpio_put(backlightPin, 1);
    	gpio_set_dir(backlightPin, GPIO_OUT);
	}

	i2c_init(mI2c, baudRate);
    gpio_set_function(sdaPin, GPIO_FUNC_I2C);
    gpio_set_function(sclPin, GPIO_FUNC_I2C);
    gpio_pull_up(sdaPin);
    gpio_pull_up(sclPin);

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