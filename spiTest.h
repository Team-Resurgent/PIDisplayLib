#pragma once

#include "hardware/spi.h"
#include "hardware/structs/spi.h"

class spiTest
{
public:
    static void initSpi(spi_inst_t* spi, uint32_t baudRate, bool slave);
    static void writeSpi();
    static void readSpi();
    static void process();
};
