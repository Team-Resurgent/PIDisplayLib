#pragma once

#include "textDisplayDriver.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/structs/spi.h"

// Config
#define DISPLAY_US2066_I2C i2c1
#define DISPLAY_US2066_BAUDRATE (1 * 1024 * 1024)
#define DISPLAY_US2066_COLS 20
#define DISPLAY_US2066_ROWS 4

class textDisplayUS2066 : textDisplayDriver
{
public:
    textDisplayUS2066();
    void initSpi(spi_inst_t* spi, uint32_t baudRate, uint8_t txPin, uint8_t sckPin, uint8_t csnPin, uint8_t rstPin, uint8_t dcPin, uint8_t backlightPin); 
    void initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin, uint8_t backlightPin);
    int32_t scanI2c();
    void setCursor(uint16_t row, uint16_t col);
    void printChar(uint8_t character);
    void printMessage(const char* message);
private:
    void init();
};
