#pragma once

#include "hardware/spi.h"
#include "hardware/structs/spi.h"

class spiTest
{
public:
    static void initSpi(spi_inst_t* spi, uint32_t baudRate);
    static void writeSpi(uint8_t *buff, uint32_t buff_size);
    static void readSpi();
};
