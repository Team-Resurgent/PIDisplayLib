#include "textDisplayUS2066.h"
#include "displayConfig.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define US2066_DISPLAY_MODE 0x08
#define US2066_DISPLAY_MODE_OFF_FLAG 0x00
#define US2066_DISPLAY_MODE_ON_FLAG 0x04
#define US2066_DISPLAY_MODE_CURSOR_FLAG 0x02
#define US2066_DISPLAY_MODE_BLINK_FLAG 0x01

textDisplayUS2066::textDisplayUS2066()
{
    printf("scan i2c\n");

    uint8_t i2cAddress = I2C_DISPLAY_ADDRESS == -1 ? scanI2c() : I2C_DISPLAY_ADDRESS;
        printf("Using address %i\n", i2cAddress);
    initI2c(DISPLAY_US2066_I2C, i2cAddress, DISPLAY_US2066_BAUDRATE);

    printf("Using address %i\n", i2cAddress);

    writeCommandByte(US2066_DISPLAY_MODE | US2066_DISPLAY_MODE_OFF_FLAG);

    //Set display clock devide ratio, oscillator freq
    writeCommandByte(0x2a); //RE=1
    writeCommandByte(0x79); //SD=1
    writeCommandByte(0xd5);
    writeCommandByte(0x70);
    writeCommandByte(0x78); //SD=0

    //Set display mode
    writeCommandByte(0x08);

    //Set remap
    writeCommandByte(0x06);

    //CGROM/CGRAM Management
    writeCommandByte(0x72);
    writeDataByte(0x01);    //ROM A
    
    //Set OLED Characterization
    writeCommandByte(0x2a); //RE=1
    writeCommandByte(0x79); //SD=1
    
    //Set SEG pins Hardware configuration
    writeCommandByte(0xda);
    writeCommandByte(0x10);

    //Set contrast control
    writeCommandByte(0x81);
    writeCommandByte(0xff);

    //Set precharge period
    writeCommandByte(0xd9);
    writeCommandByte(0xf1);

    //Set VCOMH Deselect level
    writeCommandByte(0xdb); 
    writeCommandByte(0x30);

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

void textDisplayUS2066::setCursor(uint16_t row, uint16_t col)
{
    if (row >= 4)
    {
        return;
    }

    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    writeCommandByte(0x80 | row_offsets[row] | col);
}

void textDisplayUS2066::printChar(uint8_t character)
{
    writeDataByte(character);
}

//https://github.com/MrFrangipane/Arduino_I2C_US2066_OLED
//https://github.com/pedro11x/Arduino-US2066-OLED/blob/3002dac0c1a837a7510af82a8a111fa745f4ac30/US2066.cpp