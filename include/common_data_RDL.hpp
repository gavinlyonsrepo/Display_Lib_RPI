/*!
	@file common_data_RDL.hpp
	@author Gavin Lyons
	@brief file to hold common data used by many Displays
	Project Name: Display_Lib_RPI
*/

#pragma once

#include <cstdint>
#include <chrono>
#include <thread>
#include <lgpio.h>

uint16_t GetRDLibVersionNum(void);

// Delay abstractions Macros  
#define delaySecRDL lguSleep   /**<  second delay abstraction */
#define delayMilliSecRDL(x) std::this_thread::sleep_for(std::chrono::milliseconds(x)) /**< milli second delay abstraction */
#define delayMicroSecRDL(x) std::this_thread::sleep_for(std::chrono::microseconds(x)) /**<  micro second delay abstraction */

/*! Enum to define a standard return code for most functions that return failures*/
enum rpiDisplay_Return_Codes_e : uint8_t
{
	rpiDisplay_Success = 0,                /**< Success, Function ran without defined Error :)*/
	rpiDisplay_Reserved = 1,               /**< Reserved for future use*/
	rpiDisplay_WrongFont = 2,              /**< Wrong Font selected*/
	rpiDisplay_CharScreenBounds = 3,       /**< Text Character is out of Screen bounds, Check x and y*/
	rpiDisplay_CharFontASCIIRange = 4,     /**< Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range.*/
	rpiDisplay_CharArrayNullptr = 5,       /**< Text Character Array is an invalid pointer object*/
	rpiDisplay_FontPtrNullptr = 6,         /**< Pointer to the font is an invalid pointer object*/
	rpiDisplay_BitmapNullptr = 7,          /**< The Bitmap data array is an invalid pointer object*/
	rpiDisplay_BitmapScreenBounds = 8,     /**< The Bitmap starting point is outside screen bounds, check x and y*/
	rpiDisplay_BitmapLargerThanScreen = 9, /**< The Bitmap is larger than screen, check  w and h*/
	rpiDisplay_BitmapVerticalSize = 10,    /**< A vertical Bitmap's height must be divisible by 8.*/
	rpiDisplay_BitmapHorizontalSize = 11,  /**< A horizontal Bitmap's width  must be divisible by 8*/
	rpiDisplay_BitmapSize = 12,            /**< Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8),BitmapSize(horizontal)!=(w/8)*h*/
	rpiDisplay_CustomCharLen = 13,         /**< CustomChar array must always be 5 bytes long*/
	rpiDisplay_BufferSize = 14,            /**< Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)*/
	rpiDisplay_BufferNullptr = 15,         /**< The Buffer data array is an invalid pointer object*/
	rpiDisplay_SPIOpenFailure = 16,        /**< Failed to open HW SPI , lgpio*/
	rpiDisplay_SPICloseFailure = 17,       /**< Failed to close HW SPI , lgpio */
	rpiDisplay_I2CbeginFail = 18,          /**< Failed to open I2C , lgpio*/
	rpiDisplay_I2CcloseFail = 19,          /**< Failed to close I2C , lgpio*/
	rpiDisplay_ShapeScreenBounds = 20,     /**< Shape is outside screen bounds, check x and y */
	rpiDisplay_MemoryAError = 21,          /**< Could not assign memory*/
	rpiDisplay_WrongInputPCBType = 22,     /**< Wrong input PCB type chosen */
	rpiDisplay_GpioChipDevice = 23,        /**< Failed to open or close GPIO chip device, lgpio*/
	rpiDisplay_GpioPinClaim = 24,          /**< Failed to claim a GPIO for output or input, lgpio*/
	rpiDisplay_GpioPinFree = 25,           /**< Failed to free a GPIO for output or input, lgpio*/
	rpiDisplay_WrongModeChosen = 26,       /**< Wrong SPI communication mode chosen by user */
	rpiDisplay_GenericError = 27           /**< Generic Error message */
};


// lg library GLOBAL ABSTRACTION
// (this makes porting or changing the library much easier across all devices)

// GPIO
// GPIO levels
#define Display_DC_SetHigh  lgGpioWrite(_GpioHandle, _Display_DC, 1)
#define Display_DC_SetLow  lgGpioWrite(_GpioHandle ,_Display_DC, 0)
#define Display_RST_SetHigh  lgGpioWrite(_GpioHandle, _Display_RST, 1)
#define Display_RST_SetLow  lgGpioWrite(_GpioHandle, _Display_RST, 0)
#define Display_CS_SetHigh  lgGpioWrite(_GpioHandle ,_Display_CS, 1)
#define Display_CS_SetLow  lgGpioWrite(_GpioHandle, _Display_CS, 0)
#define Display_SCLK_SetHigh  lgGpioWrite(_GpioHandle, _Display_SCLK, 1)
#define Display_SCLK_SetLow  lgGpioWrite(_GpioHandle, _Display_SCLK, 0)
#define Display_SDATA_SetHigh lgGpioWrite(_GpioHandle, _Display_SDATA, 1)
#define Display_SDATA_SetLow  lgGpioWrite(_GpioHandle, _Display_SDATA,0)
#define Display_MISO_Read  lgGpioRead(_GpioHandle, _Display_MISO)
// GPIO claim IO
#define Display_RST_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_RST,  0);
#define Display_DC_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_DC,  0);
#define Display_CS_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_CS,  0);
#define Display_SCLK_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_SCLK,  0);
#define Display_SDATA_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_SDATA,  0);
#define Display_MISO_SetDigitalInput lgGpioClaimInput(_GpioHandle, 0,_Display_MISO);
// GPIO open and close
#define Display_OPEN_GPIO_CHIP lgGpiochipOpen(_DeviceNumGpioChip)
#define Display_CLOSE_GPIO_HANDLE lgGpiochipClose(_GpioHandle)
// GPIO free modes
#define Display_GPIO_FREE_DC lgGpioFree(_GpioHandle , _Display_DC)
#define Display_GPIO_FREE_RST lgGpioFree(_GpioHandle , _Display_RST)
#define Display_GPIO_FREE_CS lgGpioFree(_GpioHandle , _Display_CS)
#define Display_GPIO_FREE_CLK lgGpioFree(_GpioHandle , _Display_SCLK)
#define Display_GPIO_FREE_SDATA lgGpioFree(_GpioHandle , _Display_SDATA)

//HW SPI related
#define Display_OPEN_SPI lgSpiOpen(_spiDev, _spiChan, _spiBaud, _spiFlags)
#define Display_CLOSE_SPI lgSpiClose(_spiHandle)
#define Display_SPI_WRITE lgSpiWrite
#define Display_SPI_BLK_SIZE 65536 /**< This is maximum block size of SPI Transaction that lgpio library handles by default*/
#define Display_SPI_TRANSFER lgSpiXfer

// I2C 
#define DISPLAY_RDL_I2C_OPEN  lgI2cOpen
#define DISPLAY_RDL_I2C_CLOSE lgI2cClose
#define DISPLAY_RDL_I2C_WRITE lgI2cWriteDevice
#define DISPLAY_RDL_I2C_READ  lgI2cReadDevice


// TM163X modules only
// GPIO Write
#define TM163X_STROBE_SetHigh lgGpioWrite(_GpioHandle, _STROBE_IO, 1)
#define TM163X_STROBE_SetLow  lgGpioWrite(_GpioHandle, _STROBE_IO, 0)
#define TM163X_DATA_SetHigh   lgGpioWrite(_GpioHandle, _DATA_IO, 1)
#define TM163X_DATA_SetLow    lgGpioWrite(_GpioHandle, _DATA_IO, 0)
#define TM163X_CLOCK_SetHigh  lgGpioWrite(_GpioHandle, _CLOCK_IO, 1)
#define TM163X_CLOCK_SetLow   lgGpioWrite(_GpioHandle, _CLOCK_IO, 0)
// GPIO Read
#define TM163X_DATA_READ lgGpioRead(_GpioHandle, _DATA_IO)
// GPIO claim modes 
#define TM163X_SET_OUTPUT_STROBE lgGpioClaimOutput(_GpioHandle, 0, _STROBE_IO, 0)
#define TM163X_SET_OUTPUT_DATA   lgGpioClaimOutput(_GpioHandle, 0, _DATA_IO, 0)
#define TM163X_SET_INPUT_DATA    lgGpioClaimInput(_GpioHandle,  0, _DATA_IO)
#define TM163X_SET_OUTPUT_CLOCK  lgGpioClaimOutput(_GpioHandle, 0, _CLOCK_IO, 0)
#define TM163X_SET_INPUT_CLOCK   lgGpioClaimInput(_GpioHandle, 0, _CLOCK_IO)
// GPIO open and close
#define TM163X_OPEN_GPIO_CHIP lgGpiochipOpen(_DeviceNumGpioChip)
#define TM163X_CLOSE_GPIO_HANDLE lgGpiochipClose(_GpioHandle)
// GPIO free modes
#define TM163X_GPIO_FREE_STROBE lgGpioFree(_GpioHandle , _STROBE_IO)
#define TM163X_GPIO_FREE_CLOCK lgGpioFree(_GpioHandle , _CLOCK_IO)
#define TM163X_GPIO_FREE_DATA lgGpioFree(_GpioHandle , _DATA_IO)
