#include "textDisplayUS2066.h"
#include "displayConfig.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

textDisplayUS2066::textDisplayUS2066()
{
    initI2c(DISPLAY_US2066_I2C, I2C_DISPLAY_ADDRESS, DISPLAY_US2066_BAUDRATE);
}
