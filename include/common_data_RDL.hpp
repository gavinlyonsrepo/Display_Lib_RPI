/*!
	@file common_data_RDL.hpp
	@author Gavin Lyons
	@brief file to hold common data and functions used by many Displays
	Project Name: Display_Lib_RPI
*/

#pragma once

#include <cstdint>
#include <chrono>
#include <thread>
#include <array>
#include <lgpio.h>
#include "utility_data_RDL.hpp"

/*! @brief Name space for common utilities */
namespace rdlib {

/*! Enum to define a standard return code for most functions that return failures*/
enum Return_Codes_e : uint8_t
{
	Success = 0,                /**< Success, Function ran without defined Error :)*/
	Reserved = 1,               /**< Reserved for future use*/
	WrongFont = 2,              /**< Wrong Font selected*/
	CharScreenBounds = 3,       /**< Text Character is out of Screen bounds, Check x and y*/
	CharFontASCIIRange = 4,     /**< Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range.*/
	CharArrayNullptr = 5,       /**< Text Character Array is an invalid pointer object*/
	FontDataEmpty = 6,          /**< Span to the font data is an empty object*/
	BitmapDataEmpty = 7,        /**< The Bitmap span is an empty object*/
	BitmapScreenBounds = 8,     /**< The Bitmap starting point is outside screen bounds, check x and y*/
	BitmapLargerThanScreen = 9, /**< The Bitmap is larger than screen, check  w and h*/
	BitmapVerticalSize = 10,    /**< A vertical Bitmap's height must be divisible by 8.*/
	BitmapHorizontalSize = 11,  /**< A horizontal Bitmap's width must be divisible by 8*/
	BitmapSize = 12,            /**< Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8),BitmapSize(horizontal)!=(w/8)*h*/
	CustomCharLen = 13,         /**< Char array is incorrect size*/
	BufferSize = 14,            /**< Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)*/
	BufferEmpty = 15,           /**< The Buffer span is an empty object*/
	SPIOpenFailure = 16,        /**< Failed to open HW SPI , lgpio*/
	SPICloseFailure = 17,       /**< Failed to close HW SPI , lgpio */
	I2CbeginFail = 18,          /**< Failed to open I2C , lgpio*/
	I2CcloseFail = 19,          /**< Failed to close I2C , lgpio*/
	ShapeScreenBounds = 20,     /**< Shape is outside screen bounds, check x and y */
	MemoryAError = 21,          /**< Could not assign memory*/
	WrongInputPCBType = 22,     /**< Wrong input PCB type chosen */
	GpioChipDevice = 23,        /**< Failed to open or close GPIO chip device, lgpio*/
	GpioPinClaim = 24,          /**< Failed to claim a GPIO for output or input, lgpio*/
	GpioPinFree = 25,           /**< Failed to free a GPIO for output or input, lgpio*/
	WrongModeChosen = 26,       /**< Wrong SPI communication mode chosen by user */
	GenericError = 27,          /**< Generic Error message, for minor errors */
	UnknownError = 28,          /**< For Unknown Errors */
	InvalidRAMLocation = 29     /**< Invalid Display RAM location */
};

/*! String array to hold error data so user if in possession of an error code can print out message ,
 *  the message consists of the enum label and its associated comment */
constexpr std::array<const char*, 30> ReturnCodesStrings = {
	"Success, Function ran without defined Error",
	"Reserved, Reserved for future use",
	"WrongFont, Wrong Font selected",
	"CharScreenBounds, Text Character is out of Screen bounds, Check x and y",
	"CharFontASCIIRange, Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range",
	"CharArrayNullptr, Text Character Array is an invalid pointer object",
	"FontDataEmpty, Span to the font data is an empty object",
	"BitmapDataEmpty, The Bitmap span is an empty object",
	"BitmapScreenBounds, The Bitmap starting point is outside screen bounds, check x and y",
	"BitmapLargerThanScreen, The Bitmap is larger than screen, check w and h",
	"BitmapVerticalSize, A vertical Bitmap's height must be divisible by 8",
	"BitmapHorizontalSize, A horizontal Bitmap's width must be divisible by 8",
	"BitmapSize, Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8), BitmapSize(horizontal)!=(w/8)*h",
	"CustomCharLen, Char array is incorrect size",
	"BufferSize, Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)",
	"BufferEmpty, The Buffer span is an empty object",
	"SPIOpenFailure, Failed to open HW SPI, lgpio",
	"SPICloseFailure, Failed to close HW SPI, lgpio",
	"I2CbeginFail, Failed to open I2C, lgpio",
	"I2CcloseFail, Failed to close I2C, lgpio",
	"ShapeScreenBounds, Shape is outside screen bounds, check x and y",
	"MemoryAError, Could not assign memory",
	"WrongInputPCBType, Wrong input PCB type chosen",
	"GpioChipDevice, Failed to open or close GPIO chip device, lgpio",
	"GpioPinClaim, Failed to claim a GPIO for output or input, lgpio",
	"GpioPinFree, Failed to free a GPIO for output or input, lgpio",
	"WrongModeChosen, Wrong SPI communication mode chosen by user",
	"GenericError, Generic error message , for minor errors",
	"UnknownError, For unknown error events",
	"InvalidRAMLocation, Invalid RAM location"
};

std::string ReturnCodetoText(Return_Codes_e);
uint16_t LibraryVersion(void);

};

// Delay abstractions Macros
/*!  second delay abstraction */
#define delaySecRDL lguSleep
/*! milli second delay abstraction */
#define delayMilliSecRDL(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
/*! micro second delay abstraction */
#define delayMicroSecRDL(x) std::this_thread::sleep_for(std::chrono::microseconds(x))


/// @cond

// lg library GLOBAL ABSTRACTION
// (this makes porting or changing the library much easier across all devices)
// GPIO write levels
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
#define _XPT_RST_SetHigh  lgGpioWrite(_GpioHandle, _RESET_PIN , 1)
#define _XPT_RST_SetLow  lgGpioWrite(_GpioHandle, _RESET_PIN , 0)
// GPIO read levels 
#define Display_MISO_Read  lgGpioRead(_GpioHandle, _Display_MISO)
#define Display_SDATA_Read  lgGpioRead(_GpioHandle, _Display_SDATA) //tm163X
#define _XPT_IRQ_Read lgGpioRead(_GpioHandle, _IRQ_PIN)
// GPIO claim output
#define Display_RST_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_RST,  0);
#define Display_DC_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_DC,  0);
#define Display_CS_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_CS,  0);
#define Display_SCLK_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_SCLK,  0);
#define Display_SDATA_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _Display_SDATA,  0);
#define _XPT_RST_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0,_RESET_PIN,  0);
// GPIO claim input
#define Display_SCLK_SetDigitalInput lgGpioClaimInput(_GpioHandle, 0, _Display_SCLK);
#define Display_SDATA_SetDigitalInput  lgGpioClaimInput(_GpioHandle, 0, _Display_SDATA);
#define Display_MISO_SetDigitalInput lgGpioClaimInput(_GpioHandle, 0,_Display_MISO);
#define _XPT_IRQ_SetDigitalInput lgGpioClaimInput(_GpioHandle, 0,_IRQ_PIN);
// GPIO open and close
#define Display_OPEN_GPIO_CHIP lgGpiochipOpen(_DeviceNumGpioChip)
#define Display_CLOSE_GPIO_HANDLE lgGpiochipClose(_GpioHandle)
// GPIO free modes
#define Display_GPIO_FREE_DC lgGpioFree(_GpioHandle , _Display_DC)
#define Display_GPIO_FREE_RST lgGpioFree(_GpioHandle , _Display_RST)
#define Display_GPIO_FREE_CS lgGpioFree(_GpioHandle , _Display_CS)
#define Display_GPIO_FREE_CLK lgGpioFree(_GpioHandle , _Display_SCLK)
#define Display_GPIO_FREE_SDATA lgGpioFree(_GpioHandle , _Display_SDATA)
#define _XPT_GPIO_FREE_IRQ lgGpioFree(_GpioHandle , _IRQ_PIN)
#define _XPT_GPIO_FREE_RST lgGpioFree(_GpioHandle , _RESET_PIN )
// SPI
#define Display_OPEN_SPI lgSpiOpen(_spiDev, _spiChan, _spiBaud, _spiFlags)
#define Display_CLOSE_SPI lgSpiClose(_spiHandle)
#define Display_SPI_READ lgSpiRead
#define Display_SPI_WRITE lgSpiWrite
#define Display_SPI_TRANSFER lgSpiXfer
// I2C 
#define Display_RDL_I2C_OPEN  lgI2cOpen
#define Display_RDL_I2C_CLOSE lgI2cClose
#define Display_RDL_I2C_WRITE lgI2cWriteDevice
#define Display_RDL_I2C_READ  lgI2cReadDevice

/// @endcond
