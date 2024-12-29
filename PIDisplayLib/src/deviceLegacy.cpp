#include "deviceLegacy.h"

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/structs/spi.h"

#include <cstdio>
#include <cstring>
#include <malloc.h>

#define LEGACY_CURSOR_UP 65
#define LEGACY_CURSOR_DOWN 66
#define LEGACY_CURSOR_RIGHT 67
#define LEGACY_CURSOR_LEFT 68

#define LEGACY_CURSOR_HOME 1
#define LEGACY_HIDE_DISPLAY 2
#define LEGACY_SHOW_DISPLAY 3
#define LEGACY_HIDE_CURSOR 4
#define LEGACY_SHOW_UNDERLINE_CURSOR 5
#define LEGACY_SHOW_BLOCK_CURSOR 6
#define LEGACY_SHOW_INVERTED_CURSOR 7
#define LEGACY_BACKSPACE 8
#define LEGACY_MODULE_CONFIG 9
#define LEGACY_LINE_FEED 10
#define LEGACY_DELETE_IN_PLACE 11
#define LEGACY_FORM_FEED 12
#define LEGACY_CARRIAGE_RETURN 13
#define LEGACY_SET_BACKLIGHT 14
#define LEGACY_SET_CONTRAST 15
#define LEGACY_SET_CURSOR_POSITION 17
#define LEGACY_DRAW_BAR_GRAPH 18
#define LEGACY_SCROLL_ON 19
#define LEGACY_SCROLL_OFF 20
#define LEGACY_WRAP_ON 23
#define LEGACY_WRAP_OFF 24
#define LEGACY_CUSTOM_CHARACTER 25
#define LEGACY_REBOOT 26
#define LEGACY_CURSOR_MOVE 27
#define LEGACY_LARGE_NUMBER 28

#define SPI_LEGACY_TX 15
#define SPI_LEGACY_SCK 14
#define SPI_LEGACY_CSN 13
#define SPI_LEGACY_RX 12

deviceLegacy::deviceLegacy(uint8_t rows, uint8_t cols)
{
    memset(mBuffer, -1, sizeof(mBuffer));
    mBufferRxPos = 0;
    mBufferTxPos = 0;

    mBrightness = 100;
    mContrast = 0;

    mRows = rows;
    mCols = cols;

    mDisplayBuffer = (uint8_t *)malloc(mRows * mCols);

    reset();
}

deviceLegacy::~deviceLegacy()
{
    free(mDisplayBuffer);
}

void deviceLegacy::initSpi(spi_inst_t *spi, uint32_t baudRate, uint8_t rxPin, uint8_t sckPin, uint8_t csnPin)
{
    mSpi = spi;

    spi_init(mSpi, baudRate);
    spi_set_slave(mSpi, true);
    gpio_set_function(rxPin, GPIO_FUNC_SPI);
    gpio_set_function(sckPin, GPIO_FUNC_SPI);
    gpio_set_function(csnPin, GPIO_FUNC_SPI);
    bi_decl(bi_3pins_with_func((uint32_t)rxPin, (uint32_t)sckPin, (uint32_t)csnPin, GPIO_FUNC_SPI));

    spi_set_format(mSpi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
}

int16_t deviceLegacy::peekCommand(uint16_t index)
{
    return mBuffer[(mBufferTxPos + index) % LEGACY_BUFFER_SIZE];
}

void deviceLegacy::completeCommand()
{
    mBuffer[mBufferTxPos];
    mBufferTxPos = (mBufferTxPos + 1) % LEGACY_BUFFER_SIZE;
}

void deviceLegacy::reset()
{
    mCursorPosRow = 0;
    mCursorPosCol = 0;
    mShowDisplay = false;
    mShowCursor = true;
    mWrapping = false;
    mScrolling = false;
    memset(mDisplayBuffer, 0x20, mRows * mCols);
}

bool deviceLegacy::poll()
{
    while (spi_is_readable(mSpi))
    {
        uint8_t value = 0;
        spi_read_blocking(mSpi, 0, &value, 1);
        mBuffer[mBufferRxPos] = value;
        mBufferRxPos = (mBufferRxPos + 1) % LEGACY_BUFFER_SIZE;
    }

    bool result = false;
    while (mBufferRxPos != mBufferTxPos)
    {
        int16_t peekedCommand = peekCommand(0);
        switch (peekedCommand)
        {
        case -1:
            break;
        case LEGACY_CURSOR_HOME:
            mCursorPosRow = 0;
            mCursorPosCol = 0;
            result = true;
            completeCommand();
            break;
        case LEGACY_HIDE_DISPLAY:
            mShowDisplay = false;
            result = true;
            completeCommand();
            break;
        case LEGACY_SHOW_DISPLAY:
            mShowDisplay = true;
            result = true;
            completeCommand();
            break;
        case LEGACY_HIDE_CURSOR:
            mShowCursor = false;
            result = true;
            completeCommand();
            break;
        case LEGACY_SHOW_UNDERLINE_CURSOR:
        case LEGACY_SHOW_BLOCK_CURSOR:
        case LEGACY_SHOW_INVERTED_CURSOR:
            mShowCursor = true;
            result = true;
            completeCommand();
            break;
        case LEGACY_BACKSPACE:
            if (mCursorPosCol > 0)
            {
                mCursorPosCol--;
                mDisplayBuffer[(mCursorPosRow * mCols) + mCursorPosCol] = ' ';
                result = true;
            }
            completeCommand();
            break;
        case LEGACY_LINE_FEED:
            if (mCursorPosRow < mRows - 1)
            {
                mCursorPosRow++;
                result = true;
            }
            completeCommand();
            break;
        case LEGACY_DELETE_IN_PLACE:
            mDisplayBuffer[(mCursorPosRow * mCols) + mCursorPosCol] = ' ';
            result = true;
            completeCommand();
            break;
        case LEGACY_FORM_FEED:
            memset(mDisplayBuffer, 0x20, mRows * mCols);
            mCursorPosRow = 0;
            mCursorPosCol = 0;
            result = true;
            completeCommand();
            break;
        case LEGACY_CARRIAGE_RETURN:
            mCursorPosCol = 0;
            result = true;
            completeCommand();
            break;
        case LEGACY_SET_CURSOR_POSITION:
            if (peekCommand(2) != -1)
            {
                uint8_t col = mBuffer[(uint8_t)(mBufferTxPos + 1)];
                uint8_t row = mBuffer[(uint8_t)(mBufferTxPos + 2)];
                if (col < mCols && row < mRows)
                {
                    mCursorPosCol = col;
                    mCursorPosRow = row;
                    result = true;
                }
                completeCommand();
                completeCommand();
                completeCommand();
            }
            break;
        case LEGACY_SET_BACKLIGHT:
            if (peekCommand(1) != -1)
            {
                uint8_t brightness = mBuffer[(uint8_t)(mBufferTxPos + 1)];
                if (brightness >= 0 && brightness <= 100)
                {
                    mBrightness = brightness;
                    result = true;
                }
                completeCommand();
                completeCommand();
            }
            break;
        case LEGACY_SET_CONTRAST:
            if (peekCommand(1) != -1)
            {
                uint8_t contrast = mBuffer[(uint8_t)(mBufferTxPos + 1)];
                if (contrast >= 0 && contrast <= 100)
                {
                    mContrast = contrast;
                    result = true;
                }
                completeCommand();
                completeCommand();
            }
            break;
        case LEGACY_REBOOT:
            reset();
            result = true;
            completeCommand();
            break;
        case LEGACY_CURSOR_MOVE:
            if (peekCommand(1) == 27 && peekCommand(2) != -1)
            {
                switch (peekCommand(2))
                {
                case LEGACY_CURSOR_UP:
                    if (mCursorPosRow > 0)
                    {
                        mCursorPosRow--;
                        result = true;
                    }
                    break;
                case LEGACY_CURSOR_DOWN:
                    if (mCursorPosRow < (mRows - 1))
                    {
                        mCursorPosRow++;
                        result = true;
                    }
                    break;
                case LEGACY_CURSOR_RIGHT:
                    if (mCursorPosCol < (mCols - 1))
                    {
                        mCursorPosCol++;
                        result = true;
                    }
                    break;
                case LEGACY_CURSOR_LEFT:
                    if (mCursorPosCol > 0)
                    {
                        mCursorPosCol--;
                        result = true;
                    }
                    break;
                default:
                    break;
                }
                completeCommand();
                completeCommand();
                completeCommand();
            }
            break;
        case LEGACY_WRAP_OFF:
            mWrapping = false;
            result = true;
            completeCommand();
            break;
        case LEGACY_WRAP_ON:
            mWrapping = true;
            result = true;
            completeCommand();
            break;
        case LEGACY_SCROLL_OFF:
            mScrolling = false;
            result = true;
            completeCommand();
            break;
        case LEGACY_SCROLL_ON:
            mScrolling = true;
            result = true;
            completeCommand();
            break;
        case 32 ... 255:
            mDisplayBuffer[(mCursorPosRow * mCols) + mCursorPosCol] = peekedCommand;
            if (mCursorPosCol < mCols)
            {
                mCursorPosCol++;
            }
            result = true;
            completeCommand();
            break;
        case LEGACY_LARGE_NUMBER:
        case LEGACY_DRAW_BAR_GRAPH:
        case LEGACY_MODULE_CONFIG:
        case LEGACY_CUSTOM_CHARACTER:
        default:
            completeCommand();
            break;
        }
    }

    if (result == true)
    {
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 20; j++)
            {
                printf("%c", (char)mDisplayBuffer[(i * 20) +j]);
            }
            printf("\n");
        }
    }
    return result;
}

uint8_t deviceLegacy::getRows()
{
    return mRows;
}

uint8_t deviceLegacy::getCols()
{
    return mCols;
}

uint8_t deviceLegacy::getCursorRow()
{
    return mCursorPosRow;
}

uint8_t deviceLegacy::getCursorCol()
{
    return mCursorPosCol;
}

uint8_t deviceLegacy::getBrightness()
{
    return mBrightness;
}

uint8_t deviceLegacy::getContrast()
{
    return mContrast;
}

uint8_t deviceLegacy::getDisplayChar(uint8_t row, uint8_t col)
{
    return mDisplayBuffer[(row * mCols) + col];
}

bool deviceLegacy::getShowDisplay()
{
    return mShowDisplay;
}

bool deviceLegacy::getShowCursor()
{
    return mShowCursor;
}