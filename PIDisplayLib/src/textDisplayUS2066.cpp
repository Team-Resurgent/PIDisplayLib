#include "textDisplayUS2066.h"
#include "displayConfig.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define US2066_DISPLAY_MODE 0x08
#define US2066_DISPLAY_MODE_OFF_FLAG 0x00
#define US2066_DISPLAY_MODE_ON_FLAG 0x04
#define US2066_DISPLAY_MODE_CURSOR_OFF_FLAG 0x00
#define US2066_DISPLAY_MODE_CURSOR_ON_FLAG 0x02
#define US2066_DISPLAY_MODE_BLINK_OFF_FLAG 0x00
#define US2066_DISPLAY_MODE_BLINK_ON_FLAG 0x01

#define US2066_EXTENDED_MDOE 0x08
#define US2066_EXTENDED_MDOE_5DOT_FLAG 0x00
#define US2066_EXTENDED_MDOE_6DOT_FLAG 0x04
#define US2066_EXTENDED_MDOE_INVERT_CURSOR_OFF_FLAG 0x00
#define US2066_EXTENDED_MDOE_INVERT_CURSOR_FLAG 0x02
#define US2066_EXTENDED_MDOE_1TO2ROWS_FLAG 0x00
#define US2066_EXTENDED_MDOE_3TO4ROWS_FLAG 0x01

textDisplayUS2066::textDisplayUS2066()
{
}

void textDisplayUS2066::initSpi(spi_inst_t* spi, uint32_t baudRate, uint8_t rxPin, uint8_t sckPin, uint8_t csnPin, uint8_t rstPin, uint8_t dcPin, uint8_t backlightPin) 
{
	textDisplayDriver::initSpi(spi, baudRate, rxPin, sckPin, csnPin, rstPin, dcPin, backlightPin);
    init();
}

void textDisplayUS2066::initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin, uint8_t backlightPin)
{
	textDisplayDriver::initI2c(i2c, address, baudRate, sdaPin, sclPin, backlightPin);
    init();
}

int32_t textDisplayUS2066::scanI2c()
{
	return textDisplayDriver::scanI2c();
}

void textDisplayUS2066::setCursor(uint16_t row, uint16_t col)
{
    if (row >= 4)
    {
        return;
    }

    int row_offsets[] = { 0x00, 0x20, 0x40, 0x60 };
    writeCommandByte(0x80 | row_offsets[row] | col);
}

void textDisplayUS2066::printChar(uint8_t character)
{
    writeDataByte(character);
}

void textDisplayUS2066::printMessage(const char* message)
{
    textDisplayDriver::printMessage(message);
}

// Private

void textDisplayUS2066::init()
{
    writeCommandByte(0x2a);  // function set (extended command set)
    writeCommandByte(0x71);  // function selection A, disable internal Vdd regualtor
    writeDataByte(0x00);

    writeCommandByte(0x28);  // function set (fundamental command set)
    writeCommandByte(US2066_DISPLAY_MODE | US2066_DISPLAY_MODE_OFF_FLAG);

    //Set display clock devide ratio, oscillator freq
    writeCommandByte(0x2a); //RE=1
    writeCommandByte(0x79); //SD=1
    writeCommandByte(0xd5);
    writeCommandByte(0x70);
    writeCommandByte(0x78); //SD=0
    //https://github.com/NewhavenDisplay/NHD_US2066/blob/master/NHD_US2066.cpp
    //Set display mode
    writeCommandByte(US2066_EXTENDED_MDOE | US2066_EXTENDED_MDOE_5DOT_FLAG | US2066_EXTENDED_MDOE_INVERT_CURSOR_OFF_FLAG | US2066_EXTENDED_MDOE_3TO4ROWS_FLAG);
    writeCommandByte(0x06);

    //CGROM/CGRAM Management
    writeCommandByte(0x72);
    writeDataByte(0x00);    //ROM A
    
    //Set OLED Characterization
    writeCommandByte(0x2a); //RE=1
    writeCommandByte(0x79); //SD=1
    
    //Set SEG pins Hardware configuration
    writeCommandByte(0xda);
    writeCommandByte(0x10);

    writeCommandByte(0xdc);
    writeCommandByte(0x00);

    //Set contrast control
    writeCommandByte(0x81);
    writeCommandByte(0x7f);

    //Set precharge period
    writeCommandByte(0xd9);
    writeCommandByte(0xf1);

    //Set VCOMH Deselect level
    writeCommandByte(0xdb); 
    writeCommandByte(0x40);

    //Exiting Set OLED Characterization
    writeCommandByte(0x78); //SD=0
    writeCommandByte(0x28); //RE=0, IS=0

    //Clear display
    writeCommandByte(0x01);

    //Set DDRAM Address
    writeCommandByte(0x80);

    sleep_ms(100);
    writeCommandByte(US2066_DISPLAY_MODE | US2066_DISPLAY_MODE_ON_FLAG);
}

//https://github.com/MrFrangipane/Arduino_I2C_US2066_OLED
//https://github.com/pedro11x/Arduino-US2066-OLED/blob/3002dac0c1a837a7510af82a8a111fa745f4ac30/US2066.cpp