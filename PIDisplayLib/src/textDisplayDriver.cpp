#include "textDisplayDriver.h"
#include "displayConfig.h"
#include "fonts.h"

#include "hardware/spi.h"
#include "hardware/structs/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdarg>

#define I2C_TIMEOUT_US 100
#define I2C_COMMAND_MODE 0x80
#define I2C_DATA_MODE 0x40

void textDisplayDriver::initSpi(spi_inst_t* spi, uint32_t baudRate, uint8_t rxPin, uint8_t sckPin, uint8_t csnPin, uint8_t rstPin, uint8_t dcPin, uint8_t backlightPin) 
{
	displayBase::initSpi(spi, baudRate, rxPin, sckPin, csnPin, rstPin, dcPin, backlightPin);
}

void textDisplayDriver::initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin, uint8_t backlightPin)
{
	displayBase::initI2c(i2c, address, baudRate, sdaPin, sclPin, backlightPin);
}

int32_t textDisplayDriver::scanI2c()
{
	return displayBase::scanI2c();
}

int32_t textDisplayDriver::getI2cAddress()
{
	return displayBase::getI2cAddress();
}

void textDisplayDriver::writeCommand(uint8_t *buff, uint32_t buff_size)
{
	if (mIsSpi)
	{
		gpio_put(mDcPin, 0);
    	spi_write_blocking(mSpi, buff, buff_size);
		return;
	}

	uint8_t* tempBuffer = (uint8_t*)malloc(buff_size + 1);
	tempBuffer[0] = I2C_COMMAND_MODE;
	memcpy(tempBuffer + 1, buff, buff_size);
	i2c_write_timeout_us(mI2c, mI2cAddress, tempBuffer, buff_size + 1, false, I2C_TIMEOUT_US);
	//i2c_write_timeout_us(mI2c, mI2cAddress, &tempBuffer[0], 1, false, I2C_TIMEOUT_US);
	//i2c_write_timeout_us(mI2c, mI2cAddress, &tempBuffer[1], 1, false, I2C_TIMEOUT_US);
	free(tempBuffer);
}

void textDisplayDriver::writeCommandByte(uint8_t cmd)
{
	writeCommand(&cmd, 1);
}

void textDisplayDriver::writeData(uint8_t *buff, uint32_t buff_size)
{
	if (mIsSpi)
	{
		gpio_put(mDcPin, 1);
    	spi_write_blocking(mSpi, buff, buff_size);
		return;
	}

	uint8_t* tempBuffer = (uint8_t*)malloc(buff_size + 1);
	tempBuffer[0] = I2C_DATA_MODE;
	memcpy(tempBuffer + 1, buff, buff_size);
	i2c_write_timeout_us(mI2c, mI2cAddress, tempBuffer, buff_size + 1, false, I2C_TIMEOUT_US);
	//i2c_write_timeout_us(mI2c, mI2cAddress, &tempBuffer[0], 1, false, I2C_TIMEOUT_US);
	//i2c_write_timeout_us(mI2c, mI2cAddress, &tempBuffer[1], 1, false, I2C_TIMEOUT_US);
	free(tempBuffer);
}

void textDisplayDriver::writeDataByte(uint8_t data)
{
    writeData(&data, 1);
}

void textDisplayDriver::printMessage(const char* message)
{
    for (uint i = 0; i < strlen(message); i++)
    {
        printChar(message[i]);
    }
}
    
void textDisplayDriver::printFormat(const char* format, ...)
{
    va_list args;
    va_start(args, format);

	uint32_t length = vsnprintf(NULL, 0, format, args);

	char* message = (char*)malloc(length + 1);
	vsnprintf(message, length, format, args);
	message[length] = 0;

    va_end(args);

    printMessage(message);

	free(message);
}