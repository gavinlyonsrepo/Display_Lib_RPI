/*!
	@file   HT16K33plus_Model1_RDL.hpp
	@author Gavin Lyons
	@brief  Header file for for HT16k33 module. Model 1
	@todo
			-# hexadecimal string & number function,
			-# keyscan,
			-# leading zeros option to float and string function.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <algorithm> // Required for std::fill
#include <cstring>

#include <lgpio.h>
#include "common_data_RDL.hpp"
#include "led_segment_font_data_RDL.hpp"

/*! @brief class to control Ht16K33 , supports 7 9 14 and 16 segment displays */
class HT16K33plus_Model1
{
	public:
		// enums:
		/*! Alignment of text on display */
		enum TextAlignment_e : uint8_t
		{
			AlignLeft       = 0, /**< Align text to the left on display */
			AlignRight      = 1, /**< Align text to the right on display */
			AlignRightZeros = 2  /**< Add leading zeros to the text */
		};
		/*!
		 * Display blinking frequency XXXX-XB1B0X
		 * settings Display register D10 D9 -> (B1, B0)
		*/
		enum BlinkFreq_e : uint8_t
		{
			BLINKOFF  =    0x81, /**< Blinking off*/
			BLINKON2HZ =   0x83, /**< 2 Hertz*/
			BLINKON1HZ =   0x85, /**< 1 Hertz*/
			BLINKON0_5HZ = 0x87  /**<0.5 Hertz*/
		};
		/*! Activate Decimal point segment */
		enum DecimalPoint_e : uint8_t
		{
			DecPointOff  = 0, /**< Decimal point segment off */
			DecPointOn   = 1  /**< Decimal point segment on */
		};
		/*! The Display Type */
		enum DisplayType_e : uint8_t
		{
			SegType7  = 7,  /**< 7 segment display */
			SegType9  = 9,  /**< 9 segment display */
			SegType14 = 14, /**< 14 segment display */
			SegType16 = 16  /**< 16 segment display */
		};
		// constructor
		 HT16K33plus_Model1(int I2Cdevice, int I2Caddress, int I2Cflags);
		// methods I2C related
		rdlib::Return_Codes_e Display_I2C_ON(void);
		rdlib::Return_Codes_e Display_I2C_OFF(void);
		int DisplayCheckConnection(void);
		int DisplayI2CErrorGet(void) const;
		uint16_t DisplayI2CErrorTimeoutGet(void) const;
		void DisplayI2CErrorTimeoutSet(uint16_t);
		uint8_t DisplayI2CErrorRetryNumGet(void) const;
		void DisplayI2CErrorRetryNumSet(uint8_t);

		// Device related
		void DisplayInit(uint8_t brightLevel, BlinkFreq_e  blink,
			uint8_t numOfDigits, DisplayType_e dtype);
		void DisplayOn(void);
		void DisplayOff(void);
		void DisplaySleep(void);
		void DisplayNormal(void);
		void DisplayResetDefault(void);
		void setBrightness(uint8_t value);
		uint8_t getBrightness() const;
		void  setBlink(BlinkFreq_e  value);
		BlinkFreq_e  getBlink() const;
		DisplayType_e getDisplayType() const;

		// Display data related
		void ClearDigits(void);
		//raw data segments
		void displayRawData(uint8_t digitPos, uint16_t value);
		// Display data
		rdlib::Return_Codes_e displayChar(uint8_t digitPos, char c, DecimalPoint_e dp);
		rdlib::Return_Codes_e displayText(const char *text, TextAlignment_e TextAlignment);
		rdlib::Return_Codes_e displayText(const char *text);
		rdlib::Return_Codes_e displayHexChar(uint8_t digitPos, char hex);
		rdlib::Return_Codes_e displayIntNum(int32_t number, TextAlignment_e TextAlignment);
		rdlib::Return_Codes_e displayFloatNum(float number, TextAlignment_e TextAlignment, uint8_t fractionDigits);
	protected:

	private:
		rdlib::Return_Codes_e displayMultiSegNum(uint8_t digitPos, char c, DecimalPoint_e dp);

		// methods I2C related
		void SendCmd(uint8_t cmd);
		void SendData(const unsigned char* data, size_t length);

		// Members I2C related
		int _I2CAddress = 0x70 ; /**< I2C address */
		int _I2CDevice =  1;      /**< An I2C device number. */
		int _I2CFlags =   0;      /**< Flags which modify an I2C open command. None are currently defined. */
		int _I2CHandle =  0;      /**< A number referencing an object opened by one of lgI2cOpen */
		uint16_t _I2C_ErrorDelay = 100; /**< I2C delay(in between retry attempts) in event of error in mS*/
		uint8_t _I2C_ErrorRetryNum = 3; /**< In event of I2C error number of retry attempts*/
		int _I2C_ErrorFlag =         0; /**< In event of I2C error, holds code*/

		// members device related
		//  Register Command List
		static constexpr uint8_t HT16K33_DDAPTR =     0x00; /**< Display data address pointer */
		static constexpr uint8_t HT16K33_NORMAL =     0x21; /**< System setup register turn on System oscillator, normal operation mode */
		static constexpr uint8_t HT16K33_STANDBY =    0x20; /**< Turn off System oscillator, standby mode */
		static constexpr uint8_t HT16K33_DISPLAYON =  0x81; /**< Display set register  Display on */
		static constexpr uint8_t HT16K33_DISPLAYOFF = 0x80; /**< Display set register  Display off */
		static constexpr uint8_t HT16K33_BRIGHTNESS = 0xE0; /**< Dimming set register 0-15 XXXX-BBBB*/

		BlinkFreq_e _blinkSetting = BLINKOFF; /**< Blink setting, 4 settings see enum */
		DisplayType_e  _displayType = SegType7; /**< Enum to hold chosen display type */
		uint8_t _brightness = 7;  /**< Brightness setting 0-15 */
		uint8_t _numOfDigits = 4; /**< Number of digits in display max 8 */

		// Display Offsets and Masks for ASCII Font
		static constexpr uint8_t _HT_ASCIIOffset =    0x20; /**< Offset in the ASCII table for font Start position */
		static constexpr uint8_t _HT_HEXoffset   =    0x10; /**< 16 Ascii table offset to reach number position */
		static constexpr uint8_t _HT_ASCIIEnd    =    0x7B; /**< last character+1 in defined ASCII table 'z'''*/

		static constexpr uint8_t  _HT_DEC_POINT_7_MASK =    0x80;  /**<Mask to switch on 7 seg decimal point */
		static constexpr uint16_t _HT_DEC_POINT_9_MASK =  0x0200; /**<Mask to switch on 9 seg decimal point */
		static constexpr uint16_t _HT_DEC_POINT_14_MASK = 0x4000; /**<Mask to switch on 14 seg decimal point */

};

