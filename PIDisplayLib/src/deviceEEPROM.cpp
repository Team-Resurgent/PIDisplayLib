#include "deviceEEPROM.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <cstdio>
#include <cstring>

#define DEVICE_EEPROM_I2C i2c0
#define DEVICE_EEPROM_ADDRESS 0x68
#define DEVICE_EEPROM_I2C_SDA 0
#define DEVICE_EEPROM_I2C_SCL 1
#define DEVICE_EEPROM_I2C_BAUDRATE (100000)
#define DEVICE_EEPROM_I2C_TIMEOUT_US 100000

#define DEVICE_EEPROM_REG 0x00

void deviceEEPROM::initI2c(uint32_t address)
{
    mI2cAddress = address;

    i2c_init(DEVICE_EEPROM_I2C, DEVICE_EEPROM_I2C_BAUDRATE);
    gpio_set_function(DEVICE_EEPROM_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DEVICE_EEPROM_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DEVICE_EEPROM_I2C_SDA);
    gpio_pull_up(DEVICE_EEPROM_I2C_SCL);
    bi_decl(bi_2pins_with_func(DEVICE_EEPROM_I2C_SDA, DEVICE_EEPROM_I2C_SCL, GPIO_FUNC_I2C));
}

void deviceEEPROM::scanI2c()
{
    uint8_t testvalue = 0;
    for (int32_t address = 0; address < 256; address++)
    {
        uint8_t reg = 0;
        if (i2c_write_timeout_us(DEVICE_EEPROM_I2C, address, &reg, 1, false, DEVICE_EEPROM_I2C_TIMEOUT_US) == 1)
        {
            printf("Found I2C device on address 0x%02x.\n", address);
        }
    }
}

bool deviceEEPROM::read()
{
    uint8_t reg = DEVICE_EEPROM_REG;

    if (i2c_write_timeout_us(DEVICE_EEPROM_I2C, mI2cAddress, &reg, 1, true, DEVICE_EEPROM_I2C_TIMEOUT_US) != 1)
    {
        printf("Fail1\n");
        return false;
    }

    uint8_t buffer[256];
    if (i2c_read_timeout_us(DEVICE_EEPROM_I2C, mI2cAddress, buffer, 256, false, DEVICE_EEPROM_I2C_TIMEOUT_US) != 256)
    {
        printf("Fail2\n");
        return false;
    }

    printf("EEPROM Contents...\n");
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 64; i++)
        {
            printf("%02x", buffer[(j * 64) + i]);
        }
        printf("\n");
    }
    printf("\nDone.\n");
    return true;
}
