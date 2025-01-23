/*!
	@file     TM1637plus_Model4_RDL.hpp
	@author   Gavin Lyons
	@brief    RPI library Tm1637plus, Header file for for TM1637 module. Model 4
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <lgpio.h>
#include "common_data_RDL.hpp"
#include "tm163X_font_data_RDL.hpp"


#define TM1637_COMMAND_1        0x40   /**< Automatic data incrementing */
#define TM1637_COMMAND_2        0xC0   /**< Data Data1~N: Transfer display data */
#define TM1637_COMMAND_3        0x80   /**< Display intensity */
#define TM1637_ASCII_OFFSET     0x20   /**< Offset in the ASCII table for font Start position */

/*!
	@brief Class for TM1637 Model 4
*/
class TM1637plus_Model4 {

public:

	TM1637plus_Model4 (uint8_t clock, uint8_t data ,int gpioDev, int delay, int DisplaySize) ;
	rpiDisplay_Return_Codes_e displayBegin(void);
	rpiDisplay_Return_Codes_e displayClose(void);
	void displayClear(void);
	void setBrightness(uint8_t brightness, bool on );

	void setSegments(const uint8_t segments[], uint8_t length , uint8_t pos );
	void DisplayDecimal(int num, bool leading_zero, uint8_t length , uint8_t pos );
	void DisplayDecimalwDot(int num, uint8_t dots , bool leading_zero ,uint8_t length , uint8_t pos );
	void DisplayString(const char* numStr, uint8_t dots , uint8_t length, uint8_t position);
	unsigned char encodeCharacter(unsigned char digit);

protected:

private:

	uint8_t _DATA_IO; /**<  GPIO connected to DIO on Tm1637  */
	uint8_t _CLOCK_IO; /**<  GPIO connected to CLk on Tm1637  */
	uint8_t _DisplaySize = 4; /**< size of display in digits */
	int _DeviceNumGpioChip = 0; /**< The device number of a gpiochip usually 0 , ls /dev/gpio */
	int _GpioHandle = 0; /**< This returns a handle to a gpiochip device. */
	int _BitDelayUS = 75; /**< Us second delay used in communications */
	uint8_t _brightness; /**< Brightness level 0-7*/

	void CommBitDelay(void);
	void CommStart(void);
	void CommStop(void);
	bool writeByte(uint8_t byte);

};
