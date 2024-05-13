#include "color.h"

uint8_t color::convertRGB8toGray8(uint32_t rgb8) 
{
    uint8_t r = (rgb8 >> 16) & 0xff;
    uint8_t g = (rgb8 >> 8) & 0xff;
    uint8_t b = rgb8 & 0xff;
    return static_cast<uint8_t>(0.2126 * r + 0.7152 * g + 0.0722 * b);
}

uint8_t color::convertRGB8toGray4(uint32_t rgb8) 
{
    return convertRGB8toGray8(rgb8) >> 4;
}

uint32_t color::convertGray8toRGB8(uint8_t gray8) 
{
    return 
        (static_cast<uint32_t>(gray8) << 16) | 
        (static_cast<uint32_t>(gray8) << 8) | 
        static_cast<uint32_t>(gray8);
}

uint32_t color::convertGray4toRGB8(uint8_t gray4) 
{
    return convertGray8toRGB8((gray4 & 0xf) << 4);
}