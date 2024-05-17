#include "spiTest.h"

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/structs/spi.h"

#include <cstdio>

#define SPI_TEST_TX 15
#define SPI_TEST_SCK 14
#define SPI_TEST_CSN 13
#define SPI_TEST_RX 12

namespace
{
    spi_inst_t* mSpi;
    bool mSlave;
}

void spiTest::initSpi(spi_inst_t* spi, uint32_t baudRate, bool slave)
{
    mSpi = spi;
    mSlave = slave;

    spi_init(mSpi, baudRate);
    spi_set_slave(mSpi, slave);
    gpio_set_function(SPI_TEST_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TEST_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TEST_TX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TEST_CSN, GPIO_FUNC_SPI);
    bi_decl(bi_4pins_with_func((uint32_t)SPI_TEST_RX, (uint32_t)SPI_TEST_TX, (uint32_t)SPI_TEST_SCK, (uint32_t)SPI_TEST_CSN, GPIO_FUNC_SPI));

    spi_set_format(mSpi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
}

void spiTest::writeSpi()
{
    printf("sending message\n");
    uint8_t message[] = {'H', 'e', 'l', 'l', 'o', '\n'};
    spi_write_blocking(mSpi, message, sizeof(message));
}

void spiTest::readSpi()
{
    printf("recieving message\n");
    uint8_t character;
    while (spi_is_readable(mSpi)) 
    {
        uint8_t buffer[1];
        spi_read_blocking(mSpi,  0, buffer, 1);
        printf("spi received %i\n", buffer[0]);
    }
}

void spiTest::process()
{
    if (mSlave)
    {
        readSpi();
    }
    else
    {
        writeSpi();
    }
}