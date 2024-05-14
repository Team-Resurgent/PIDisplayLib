#include "displayGC9A01A.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

//https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp

#define GC9A01A_NOP 0x00
#define GC9A01A_SOFTWARE_RESET 0x01
#define GC9A01A_SLEEP_IN 0x10
#define GC9A01A_SLEEP_OUT 0x11
#define GC9A01A_PARTIAL_MODE_ON 0x12
#define GC9A01A_PARTIAL_MODE_OFF 0x13
#define GC9A01A_DISPLAY_INVERSION_OFF 0x20
#define GC9A01A_DISPLAY_INVERSION_ON 0x21
#define GC9A01A_DISPLAY_OFF 0x28
#define GC9A01A_DISPLAY_ON 0x29
#define GC9A01A_COLUMN_ADDRESS_SET 0x2A // 5 Byte Command
#define GC9A01A_ROW_ADDRESS_SET 0x2B // 5 Byte Command
#define GC9A01A_MEMORY_WRITE 0x2C // 2+ Byte Command
#define GC9A01A_PARTIAL_START_END 0x30 // 5 Byte Command
#define GC9A01A_VERTICAL_SCROLLING_DIRECTION 0x33 // 7 Byte Command
#define GC9A01A_TEARING_EFFECT_LINE_OFF 0x34
#define GC9A01A_TEARING_EFFECT_LINE_ON 0x35 
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL 0x36 // 2 Byte Command
#define GC9A01A_VERTICAL_SCROLLING_START_ADDRESS 0x37 // 3 Byte Command
#define GC9A01A_IDLE_MODE_OFF 0x38
#define GC9A01A_IDLE_MODE_ON 0x39
#define GC9A01A_INTERFACE_PIXEL_FORMAT 0x3A // 2 Byte Command
#define GC9A01A_MEMORY_WRITE_CONTINUE 0x3C // 2+ Byte Command
#define GC9A01A_SET_TEAR_SCANLINE 0x44 // 3 Byte Command
#define GC9A01A_WRITE_DISPLAY_BRIGHTNESS 0x51 // 2 Byte Command
#define GC9A01A_WRITE_CTRL_DISPLAY 0x53 // 2 Byte Command

#define GC9A01A_RGB_CONTROL 0xB0 // 2 Byte Command
#define GC9A01A_PORCH_CONTROL 0xB5 // 4 Byte Command
#define GC9A01A_DISPLAY_FUNCTION_CONTROL 0xB6 // 3 Byte Command
#define GC9A01A_TE_CONTROL 0xBA // 2 Byte Command
#define GC9A01A_INTERFACE_CONTROL1 0xF6 // 2 Byte Command

#define GC9A01A_POWER_CRITERION_CONTROL 0xC1 // 2 Byte Command
#define GC9A01A_VCORE_VOLTAGE_CONTROL 0xA7 // 2 Byte Command
#define GC9A01A_VREG1A_VOLTAGE_CONTROL 0xC3 // 2 Byte Command
#define GC9A01A_VREG1B_VOLTAGE_CONTROL 0xC4 // 2 Byte Command
#define GC9A02A_VREG1A_VOLTAGE_CONTROL 0xC9 // 2 Byte Command
#define GC9A01A_FRAME_RATE_CONTROL 0xE8 // 3 Byte Command
#define GC9A01A_SPI2_DATA_CONTROL 0xE9 // 2 Byte Command

#define GC9A01A_CHARGE_PUMP_FREQUENT_CONTROL 0xEC // 4 Byte Command
#define GC9A01A_INNER_REGISTER_ENABLE1 0xFE // 1 Byte Command
#define GC9A01A_INNER_REGISTER_ENABLE2 0xEF // 1 Byte Command
#define GC9A01A_SET_GAMMA1 0xF0 // 7 Byte Command
#define GC9A01A_SET_GAMMA2 0xF1 // 7 Byte Command
#define GC9A01A_SET_GAMMA3 0xF2 // 7 Byte Command
#define GC9A01A_SET_GAMMA4 0xF3 // 7 Byte Command

#define GC9A01A_INTERFACE_PIXEL_FORMAT_16BIT 0x05
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MY 0x80
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MX 0x40
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MV 0x20
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_ML 0x10
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_BGR 0x08
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB 0x00

displayGC9A01A::displayGC9A01A()
{
    initDisplayBuffer(
        DISPLAY_GC9A01A_WIDTH, 
        DISPLAY_GC9A01A_HEIGHT, 
        DISPLAY_GC9A01A_X_SHIFT,
        DISPLAY_GC9A01A_Y_SHIFT,
        DISPLAY_GC9A01A_BITS_PER_PIXEL
    );

    initSpi(DISPLAY_GC9A01A_BAUDRATE, true);

    writeSpiCommandByte(GC9A01A_SOFTWARE_RESET);
    sleep_ms(100);

    // writeSpiCommand(GC9A01A_INTERFACE_PIXEL_FORMAT);	
    // writeSpiDataByte(GC9A01A_INTERFACE_PIXEL_FORMAT_16BIT);

  	// writeSpiCommand(GC9A01A_PORCH_CONTROL);	
	// uint8_t porchData[] = { 0x0c, 0x0c, 0x00, 0x33, 0x33 };
	// writeSpiData(porchData, sizeof(porchData));

//https://github.com/Exboom/GC9A01/blob/master/Core/Src/GC9A01.c

//https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp

    // Unknown
    writeSpiCommandByte(0xEB);
    writeSpiDataByte(0x14);
    
    writeSpiCommandByte(GC9A01A_INNER_REGISTER_ENABLE1);
    writeSpiCommandByte(GC9A01A_INNER_REGISTER_ENABLE2);
    
    // Unknown
    writeSpiCommandByte(0xEB);
    writeSpiDataByte(0x14);
    
    // Unknown
    writeSpiCommandByte(0x84);
    writeSpiDataByte(0x40);
    
    // Unknown
    writeSpiCommandByte(0x85);
    writeSpiDataByte(0xFF);
    
    // Unknown
    writeSpiCommandByte(0x86);
    writeSpiDataByte(0xFF);
    
    // Unknown
    writeSpiCommandByte(0x87);
    writeSpiDataByte(0xFF);
    
    // Unknown
    writeSpiCommandByte(0x88);
    writeSpiDataByte(0x0A);
    
    // Unknown
    writeSpiCommandByte(0x89);
    writeSpiDataByte(0x21);
    
    // Unknown
    writeSpiCommandByte(0x8A);
    writeSpiDataByte(0x00);
    
    // Unknown
    writeSpiCommandByte(0x8B);
    writeSpiDataByte(0x80);
    
    // Unknown
    writeSpiCommandByte(0x8C);
    writeSpiDataByte(0x01);
    
    // Unknown
    writeSpiCommandByte(0x8D);
    writeSpiDataByte(0x01);
    
    // Unknown
    writeSpiCommandByte(0x8E);
    writeSpiDataByte(0xFF);
    
    // Unknown
    writeSpiCommandByte(0x8F);
    writeSpiDataByte(0xFF);
    
    writeSpiCommandByte(GC9A01A_DISPLAY_FUNCTION_CONTROL);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x00);
    
    writeSpiCommandByte(GC9A01A_INTERFACE_PIXEL_FORMAT); 
    writeSpiDataByte(GC9A01A_INTERFACE_PIXEL_FORMAT_16BIT);
    
    // Unknown
    writeSpiCommandByte(0x90);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x08);
    
    // Unknown
    writeSpiCommandByte(0xBD);
    writeSpiDataByte(0x06);
    
    // Unknown
    writeSpiCommandByte(0xBC);
    writeSpiDataByte(0x00);
    
    // Unknown
    writeSpiCommandByte(0xFF);
    writeSpiDataByte(0x60);
    writeSpiDataByte(0x01);
    writeSpiDataByte(0x04);
    
    writeSpiCommandByte(GC9A01A_VREG1A_VOLTAGE_CONTROL);
    writeSpiDataByte(0x13);

    writeSpiCommandByte(GC9A01A_VREG1B_VOLTAGE_CONTROL);
    writeSpiDataByte(0x13);
    
    writeSpiCommandByte(GC9A02A_VREG1A_VOLTAGE_CONTROL);
    writeSpiDataByte(0x22);
    
    // Unknown
    writeSpiCommandByte(0xBE);
    writeSpiDataByte(0x11);
    
    // Unknown
    writeSpiCommandByte(0xE1);
    writeSpiDataByte(0x10);
    writeSpiDataByte(0x0E);
    
    // Unknown
    writeSpiCommandByte(0xDF);
    writeSpiDataByte(0x21);
    writeSpiDataByte(0x0c);
    writeSpiDataByte(0x02);
    
    writeSpiCommandByte(GC9A01A_SET_GAMMA1);
    writeSpiDataByte(0x45);
    writeSpiDataByte(0x09);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x26);
    writeSpiDataByte(0x2A);
    
    writeSpiCommandByte(GC9A01A_SET_GAMMA2);
    writeSpiDataByte(0x43);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x72);
    writeSpiDataByte(0x36);
    writeSpiDataByte(0x37);
    writeSpiDataByte(0x6F);
    
    writeSpiCommandByte(GC9A01A_SET_GAMMA3);
    writeSpiDataByte(0x45);
    writeSpiDataByte(0x09);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x26);
    writeSpiDataByte(0x2A);
    
    writeSpiCommandByte(GC9A01A_SET_GAMMA4);
    writeSpiDataByte(0x43);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x72);
    writeSpiDataByte(0x36);
    writeSpiDataByte(0x37);
    writeSpiDataByte(0x6F);
    
    // Unknown
    writeSpiCommandByte(0xED);
    writeSpiDataByte(0x1B);
    writeSpiDataByte(0x0B);
    
    // Unknown
    writeSpiCommandByte(0xAE);
    writeSpiDataByte(0x77);
    
    // Unknown
    writeSpiCommandByte(0xCD);
    writeSpiDataByte(0x63);
    
    // Unknown
    writeSpiCommandByte(0x70);
    writeSpiDataByte(0x07);
    writeSpiDataByte(0x07);
    writeSpiDataByte(0x04);
    writeSpiDataByte(0x0E);
    writeSpiDataByte(0x0F);
    writeSpiDataByte(0x09);
    writeSpiDataByte(0x07);
    writeSpiDataByte(0x08);
    writeSpiDataByte(0x03);
    
    writeSpiCommandByte(GC9A01A_FRAME_RATE_CONTROL);
    writeSpiDataByte(0x34);

    // Unknown    
    writeSpiCommandByte(0x62);
    writeSpiDataByte(0x18);
    writeSpiDataByte(0x0D);
    writeSpiDataByte(0x71);
    writeSpiDataByte(0xED);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x18);
    writeSpiDataByte(0x0F);
    writeSpiDataByte(0x71);
    writeSpiDataByte(0xEF);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x70);
    
    // Unknown
    writeSpiCommandByte(0x63);
    writeSpiDataByte(0x18);
    writeSpiDataByte(0x11);
    writeSpiDataByte(0x71);
    writeSpiDataByte(0xF1);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x18);
    writeSpiDataByte(0x13);
    writeSpiDataByte(0x71);
    writeSpiDataByte(0xF3);
    writeSpiDataByte(0x70);
    writeSpiDataByte(0x70);
    
    // Unknown
    writeSpiCommandByte(0x64);
    writeSpiDataByte(0x28);
    writeSpiDataByte(0x29);
    writeSpiDataByte(0xF1);
    writeSpiDataByte(0x01);
    writeSpiDataByte(0xF1);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x07);
    
    // Unknown
    writeSpiCommandByte(0x66);
    writeSpiDataByte(0x3C);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0xCD);
    writeSpiDataByte(0x67);
    writeSpiDataByte(0x45);
    writeSpiDataByte(0x45);
    writeSpiDataByte(0x10);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x00);
    
    // Unknown
    writeSpiCommandByte(0x67);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x3C);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x01);
    writeSpiDataByte(0x54);
    writeSpiDataByte(0x10);
    writeSpiDataByte(0x32);
    writeSpiDataByte(0x98);
    
    // Unknown
    writeSpiCommandByte(0x74);
    writeSpiDataByte(0x10);
    writeSpiDataByte(0x85);
    writeSpiDataByte(0x80);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x00);
    writeSpiDataByte(0x4E);
    writeSpiDataByte(0x00);
    
    // Unknown
    writeSpiCommandByte(0x98);
    writeSpiDataByte(0x3e);
    writeSpiDataByte(0x07);

    writeSpiCommandByte(GC9A01A_DISPLAY_INVERSION_ON);
	writeSpiCommandByte(GC9A01A_SLEEP_OUT);
  	writeSpiCommandByte(GC9A01A_PARTIAL_MODE_OFF);
  	writeSpiCommandByte(GC9A01A_DISPLAY_ON);

    rotate(0);

    drawDisplay();
}

void displayGC9A01A::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    displayDriver::drawChar(colorR8G8B8, font, x, y, character);
}

void displayGC9A01A::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    displayDriver::drawString(colorR8G8B8, font, x, y, message);
}

void displayGC9A01A::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    displayDriver::drawLine(colorR8G8B8, x0, y0, x1, y1);
}

void displayGC9A01A::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawRectangle(colorR8G8B8, x, y, width, height);
}

void displayGC9A01A::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayGC9A01A::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawCircle(colorR8G8B8, x, y, radius);
}

void displayGC9A01A::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawFilledRectangle(colorR8G8B8, x, y, width, height);
}

void displayGC9A01A::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawFilledTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayGC9A01A::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawFilledCircle(colorR8G8B8, x, y, radius);
}

void displayGC9A01A::drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y)
{
    if (x >= mDisplayBuffer->getWidth() || y >= mDisplayBuffer->getHeight())
    {
        return;
    }

    uint16_t r5g6b5 = color::convertR8G8B8toR5G6B5(colorR8G8B8);
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pixelOffset = (y * (mDisplayBuffer->getWidth() << 1)) + (x << 1);
    buffer[pixelOffset] = static_cast<uint8_t>((r5g6b5 & 0xff00) >> 8);
    buffer[pixelOffset + 1] = static_cast<uint8_t>(r5g6b5 & 0xff);
}

void displayGC9A01A::fill(uint32_t colorR8G8B8)
{
    uint16_t r5g6b5 = color::convertR8G8B8toR5G6B5(colorR8G8B8);

    uint32_t rowStride = mDisplayBuffer->getWidth() << 1;
    uint8_t* rowData = (uint8_t*)malloc(rowStride);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < mDisplayBuffer->getWidth(); i++)
    {
        rowData[offset] = static_cast<uint8_t>(r5g6b5 >> 8);
        rowData[offset + 1] = static_cast<uint8_t>(r5g6b5 & 0xff);
        offset += 2;
    }

    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    for (uint32_t i = 0; i < mDisplayBuffer->getHeight(); i++) 
    {
        memcpy(buffer, rowData, rowStride);
        buffer += rowStride;
    }
}

void displayGC9A01A::drawDisplay()
{
	uint16_t xStart = 0 + mDisplayBuffer->getXShift();
    uint16_t xEnd = mDisplayBuffer->getWidth() + mDisplayBuffer->getXShift() - 1;
	uint16_t yStart = 0 + mDisplayBuffer->getYShift();
    uint16_t yEnd = mDisplayBuffer->getHeight() + mDisplayBuffer->getYShift() - 1;

	writeSpiCommandByte(GC9A01A_COLUMN_ADDRESS_SET);
    uint8_t columnData[] = {(uint8_t)(xStart >> 8), (uint8_t)(xStart & 0xFF), (uint8_t)(xEnd >> 8), (uint8_t)(xEnd & 0xFF)};
    writeSpiData(columnData, sizeof(columnData));

	writeSpiCommandByte(GC9A01A_ROW_ADDRESS_SET);
    uint8_t rowData[] = {(uint8_t)(yStart >> 8), (uint8_t)(yStart & 0xFF), (uint8_t)(yEnd >> 8), (uint8_t)(yEnd & 0xFF)};
    writeSpiData(rowData, sizeof(rowData));
                    
	writeSpiCommandByte(GC9A01A_MEMORY_WRITE);
    writeSpiData(getDisplayBuffer()->getBuffer(), getDisplayBuffer()->getBufferSize());
}

void displayGC9A01A::brightness(uint8_t value)
{
    // Does not seem to work
    // writeSpiCommand(GC9A01A_WRITE_DISPLAY_BRIGHTNESS);
    // writeSpiDataByte(value);
}

void displayGC9A01A::contrast(uint8_t value)
{
    // NA 
}

void displayGC9A01A::invert(bool value)
{
    writeSpiCommandByte(value ? GC9A01A_DISPLAY_INVERSION_OFF : GC9A01A_DISPLAY_INVERSION_ON);
}

void displayGC9A01A::rotate(uint16_t degrees)
{
    mDisplayBuffer->setRotation(degrees);

    if (degrees == 0)
    {
        writeSpiCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
        writeSpiDataByte(
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB);
    }
    else if (degrees == 90)
    {
        writeSpiCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
        writeSpiDataByte((uint8_t)(
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 180)
    {
        writeSpiCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
	    writeSpiDataByte((uint8_t)(
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MY | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 270)
    {
        writeSpiCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
        writeSpiDataByte((uint8_t)(
            
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MY | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB));
    }
}