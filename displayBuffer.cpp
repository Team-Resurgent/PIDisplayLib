#include "displayBuffer.h"
#include <cstdlib>
#include <cstring>

displayBuffer::displayBuffer(uint16_t width, uint16_t height, uint8_t bitsPerPixel)
{
    mWidth = width;
    mHeight = height;
    mBitsPerPixel = bitsPerPixel;
    mBufferSize = (mWidth * mHeight * mBitsPerPixel) >> 3;
    mBuffer = static_cast<uint8_t*>(malloc(mBufferSize));
    memset(mBuffer, 0, mBufferSize);
}

displayBuffer::~displayBuffer()
{
    free(mBuffer);
}

uint16_t displayBuffer::getWidth()
{
    return mWidth;
}

uint16_t displayBuffer::getHeight()
{
    return mHeight;
}

uint8_t displayBuffer::getBitsPerPixel()
{
    return mBitsPerPixel;
}

uint32_t displayBuffer::getBufferSize()
{
    return mBufferSize;
}

uint8_t* displayBuffer::getBuffer()
{
    return mBuffer;
}
