#include "pixelDisplayBuffer.h"
#include <cstdlib>
#include <cstring>

pixelDisplayBuffer::pixelDisplayBuffer(uint16_t width, uint16_t height, uint16_t xShift, uint16_t yShift, uint8_t bitsPerPixel)
{
    mWidth = width;
    mHeight = height;
    mXShift = xShift;
    mYShift = yShift;
    mBitsPerPixel = bitsPerPixel;
    mBufferSize = (mWidth * mHeight * mBitsPerPixel) >> 3;
    mBuffer = static_cast<uint8_t*>(malloc(mBufferSize));
    mRotation = 0;
    memset(mBuffer, 0, mBufferSize);
}

pixelDisplayBuffer::~pixelDisplayBuffer()
{
    free(mBuffer);
}

uint16_t pixelDisplayBuffer::getWidth()
{
    return (mRotation == 90 || mRotation == 270) ? mHeight : mWidth;
}

uint16_t pixelDisplayBuffer::getHeight()
{
    return (mRotation == 90 || mRotation == 270) ? mWidth : mHeight;
}

uint16_t pixelDisplayBuffer::getXShift()
{
    return (mRotation == 90 || mRotation == 270) ? mYShift : mXShift;
}

uint16_t pixelDisplayBuffer::getYShift()
{
    return (mRotation == 90 || mRotation == 270) ? mXShift : mYShift;
}

uint8_t pixelDisplayBuffer::getBitsPerPixel()
{
    return mBitsPerPixel;
}

uint32_t pixelDisplayBuffer::getBufferSize()
{
    return mBufferSize;
}

uint8_t* pixelDisplayBuffer::getBuffer()
{
    return mBuffer;
}

uint16_t pixelDisplayBuffer::getRotation()
{
    return mRotation;
}

void pixelDisplayBuffer::setRotation(uint16_t degrees)
{
    mRotation = degrees;
}