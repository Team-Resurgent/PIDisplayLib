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

#define I2C_TIMEOUT_US 100
#define I2C_COMMAND_MODE 0x80
#define I2C_DATA_MODE 0x40

void textDisplayDriver::initSpi(spi_inst_t* spi, uint32_t baudRate) 
{
	return displayBase::initSpi(spi, baudRate);
}

void textDisplayDriver::initI2c(i2c_inst_t* i2c, uint32_t address,  uint32_t baudRate)
{
	return displayBase::initI2c(i2c, address, baudRate);
}

int32_t textDisplayDriver::scanI2c()
{
	return displayBase::scanI2c();
}

// void pixelDisplayDriver::writeCommand(uint8_t *buff, uint32_t buff_size)
// {
// 	if (mIsSpi)
// 	{
// 		gpio_put(SPI_DISPLAY_DC, 0);
//     	spi_write_blocking(mSpi, buff, buff_size);
// 		return;
// 	}

// 	uint8_t* tempBuffer = (uint8_t*)malloc(buff_size + 1);
// 	tempBuffer[0] = I2C_COMMAND_MODE;
// 	memcpy(tempBuffer + 1, buff, buff_size);
// 	i2c_write_blocking(mI2c, mI2cAddress, tempBuffer, buff_size + 1, false);
// 	free(tempBuffer);
// }

// void pixelDisplayDriver::writeCommandByte(uint8_t cmd)
// {
// 	writeCommand(&cmd, 1);
// }

// void pixelDisplayDriver::writeData(uint8_t *buff, uint32_t buff_size)
// {
// 	if (mIsSpi)
// 	{
// 		gpio_put(SPI_DISPLAY_DC, 1);
//     	spi_write_blocking(mSpi, buff, buff_size);
// 		return;
// 	}

// 	uint8_t* tempBuffer = (uint8_t*)malloc(buff_size + 1);
// 	tempBuffer[0] = I2C_DATA_MODE;
// 	memcpy(tempBuffer + 1, buff, buff_size);
// 	i2c_write_blocking(mI2c, mI2cAddress, tempBuffer, buff_size + 1, false);
// 	free(tempBuffer);
// }

// void pixelDisplayDriver::writeDataByte(uint8_t data)
// {
//     writeData(&data, 1);
// }
