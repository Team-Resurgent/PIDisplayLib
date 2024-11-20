#pragma once

#include "displayBase.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/structs/spi.h"

class textDisplayDriver : displayBase
{
public:

    void initSpi(spi_inst_t* spi, uint32_t baudRate);
    void initI2c(i2c_inst_t* i2c, uint32_t address,  uint32_t baudRate);
    int32_t scanI2c();
    int32_t getI2cAddress();

    void writeCommand(uint8_t *buff, uint32_t buff_size);
    void writeCommandByte(uint8_t cmd);
    void writeData(uint8_t *buff, uint32_t buff_size);
    void writeDataByte(uint8_t data);

    virtual void setCursor(uint16_t row, uint16_t col) = 0;
    virtual void printChar(uint8_t character) = 0;
    
    void printMessage(const char* message);
    void printFormat(const char* message, ...);
};
