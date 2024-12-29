#include "deviceRTC.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <cstdio>
#include <cstring>

#define DEVICE_RTC_I2C_TIMEOUT_US 100000

#define DEVICE_RTC_DATETIME_REG 0x00
#define DEVICE_RTC_ALARM1_REG 0x07
#define DEVICE_RTC_ALARM2_REG 0x0B
#define DEVICE_RTC_CONTROL_REG 0x0E
#define DEVICE_RTC_STATUS_REG 0x0F
#define DEVICE_RTC_AGING_OFFSET_REG 0x10
#define DEVICE_RTC_TEMPERATURE_REG 0x11

#define EXTRACT_DATE_VALUE(value) ((((value & 0xf0) >> 4) * 10) + (value & 0x0f))
#define COMPOSE_DATE_VALUE(value) ((((value - (value % 10)) / 10) << 4) | (value % 10));

//https://github.com/antgon/pico-ds3231/blob/main/lib/ds3231.c

void deviceRTC::initI2c(i2c_inst_t* i2c, uint32_t address, uint32_t baudRate, uint8_t sdaPin, uint8_t sclPin)
{
    mI2c = i2c;
	mI2cAddress = address;

	i2c_init(mI2c, baudRate);
    gpio_set_function(sdaPin, GPIO_FUNC_I2C);
    gpio_set_function(sclPin, GPIO_FUNC_I2C);
    gpio_pull_up(sdaPin);
    gpio_pull_up(sclPin);

// while (true)
// {
//     sleep_ms(1000);

//     uint8_t reg = 0;
//     int xx = i2c_write_timeout_us(DEVICE_RTC_I2C, 0x68, &reg, 1, true, DEVICE_RTC_I2C_TIMEOUT_US);  
//     printf("r1 %i\n", xx);

//     uint8_t buffer[7];
//     memset(buffer, 0, 7);
//     int yy = i2c_read_timeout_us(DEVICE_RTC_I2C, 0x68, buffer, 7, false, DEVICE_RTC_I2C_TIMEOUT_US);
//     printf("r2 %i\n", yy);

//     for (int i = 0; i < 7; i++)
//     {
//            printf("I2C %i = %i.\n", i, buffer[i]);
//     }
// };
}

void deviceRTC::scanI2c(i2c_inst_t* i2c)
{
   	uint8_t testvalue = 0;
	for (int32_t address = 0; address < 256; address++)
	{
        uint8_t reg = 0;
		if (i2c_write_timeout_us(i2c, address, &reg, 1, false, DEVICE_RTC_I2C_TIMEOUT_US) == 1)
		{
            printf("Found I2C device on address 0x%02x.\n", address);
        }
	}
}

bool deviceRTC::setDateTime(datetime_t* dataTime)
{
    uint8_t buffer[8];
    buffer[0] = DEVICE_RTC_DATETIME_REG;
    buffer[1] = COMPOSE_DATE_VALUE(dataTime->sec);
    buffer[2] = COMPOSE_DATE_VALUE(dataTime->min);
    buffer[3] = COMPOSE_DATE_VALUE(dataTime->hour);
    buffer[4] = dataTime->dotw & 0x07;
    buffer[5] = COMPOSE_DATE_VALUE(dataTime->day);
    buffer[6] = COMPOSE_DATE_VALUE(dataTime->month);
    buffer[7] = COMPOSE_DATE_VALUE(dataTime->year % 100);

    if (i2c_write_timeout_us(mI2c, mI2cAddress, buffer, 8, false, DEVICE_RTC_I2C_TIMEOUT_US) != 8)
    {
        return false;
    }

    uint8_t reg = DEVICE_RTC_STATUS_REG;
    if (i2c_write_timeout_us(mI2c, mI2cAddress, &reg, 1, true, DEVICE_RTC_I2C_TIMEOUT_US) != 1)
    {
        return false;
    }

    uint8_t status;
    if (i2c_read_timeout_us(mI2c, mI2cAddress, &status, 1, true, DEVICE_RTC_I2C_TIMEOUT_US) != 1)
    {
        return false;
    }

    status &= ~(1 << 7);

    uint8_t buf[] = {DEVICE_RTC_STATUS_REG, status};
    if (i2c_write_timeout_us(mI2c, mI2cAddress, buf, 2, false, DEVICE_RTC_I2C_TIMEOUT_US) != 2)
    {
        return false;
    }

    return true;
}

bool deviceRTC::getDateTime(datetime_t* dataTime)
{
    uint8_t reg = DEVICE_RTC_DATETIME_REG;
    
    if (i2c_write_timeout_us(mI2c, mI2cAddress, &reg, 1, true, DEVICE_RTC_I2C_TIMEOUT_US) != 1)
    {
        return false;
    }

    uint8_t buffer[7];
    if (i2c_read_timeout_us(mI2c, mI2cAddress, buffer, 7, false, DEVICE_RTC_I2C_TIMEOUT_US) != 7)
    {
        return false;
    }

    dataTime->sec = EXTRACT_DATE_VALUE(buffer[0]);
    dataTime->min = EXTRACT_DATE_VALUE(buffer[1]);

    if ((buffer[2] & 0x40) == 0) 
    {  
        dataTime->hour = EXTRACT_DATE_VALUE(buffer[2] & 0x3f);
    } 
    else 
    { 
        dataTime->hour = ((buffer[2] & 0x20) == 0 ? 0 : 12) + EXTRACT_DATE_VALUE(buffer[2] & 0x1f);
    }

    dataTime->dotw = buffer[3] & 0x07;
    dataTime->day = EXTRACT_DATE_VALUE(buffer[4]);
    dataTime->month = EXTRACT_DATE_VALUE(buffer[5]);
    dataTime->year = 2000 + EXTRACT_DATE_VALUE(buffer[6]);
    return true;
}

bool deviceRTC::getTemperature(float *val)
{
    uint8_t reg = DEVICE_RTC_TEMPERATURE_REG;
    float frac;
    
    if (i2c_write_timeout_us(mI2c, mI2cAddress, &reg, 1, false, DEVICE_RTC_I2C_TIMEOUT_US) != 1)
    {
        return false;
    }

    uint8_t buffer[2];
    if (i2c_read_timeout_us(mI2c, mI2cAddress, buffer, 2, false, DEVICE_RTC_I2C_TIMEOUT_US) != 2)
    {
        return false;
    }

    frac = (float)(buffer[1] >> 6);
    frac *= 0.25;
    
    if ((buffer[0] >> 7) & 1) 
    {
        *val = (float)((~buffer[0]) + 1);
    } 
    else 
    {
        *val = (float)buffer[0];
    }

    *val += frac;
    return true;
}