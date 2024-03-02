/*!
	@file common_data_RDL.hpp
	@author Gavin Lyons
	@brief file to hold common data used by all Displays
	Project Name: Display_Lib_RPI
*/

#pragma once

#include <cstdint>

uint16_t GetRDLibVersionNum(void);

#define delayMicroSecRDL bcm2835_delayMicroseconds /**< bcm2835 library Micro second delay abstraction */
#define delayMilliSecRDL bcm2835_delay             /**< bcm2835 library Milli second delay abstraction */

/*! Enum to define a standard return code for most functions that return failures*/
enum rpiDisplay_Return_Codes_e : uint8_t
{
	rpiDisplay_Success = 0,                /**< Success, Function ran without defined Error*/
	rpiDisplay_Reserved = 1,               /**< Reserved */
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
	rpiDisplay_SPIbeginFail = 16,          /**< bcm2835_spi_begin() has failed, running as root?*/
	rpiDisplay_SPICEXPin = 17,             /**< SPICE_PIN value incorrect must be 0 or 1 */
	rpiDisplay_I2CbeginFail = 18,          /**< bcm2835_i2c_begin() has failed, running as root?*/
	rpiDisplay_ShapeScreenBounds = 19,     /**< Shape is outside screen bounds, check x and y */
	rpiDisplay_MallocError = 20,           /**< malloc could not assign memory*/
	rpiDisplay_WrongInputPCBType = 21      /**< Wrong input PCB type chosen */
};


