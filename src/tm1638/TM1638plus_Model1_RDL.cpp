/*!
	@file     TM1638plus_Model1_RDL.cpp
	@author   Gavin Lyons
	@brief    RPI library Tm1638plus, Source file for for TM1638 module(LED & KEY). Model 1
*/

#include "../../include/tm1638/TM1638plus_Model1_RDL.hpp"


/*!
	@brief Constructor for class TM1638plus_Model1
	@param strobe  GPIO STB pin
	@param clock  GPIO CLK pin
	@param data  GPIO DIO pin
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
*/
TM1638plus_Model1::TM1638plus_Model1(uint8_t strobe, uint8_t clock, uint8_t data, int gpioDev) : TM1638plus_common(strobe, clock, data, gpioDev) {
 // Blank constructor
}

/*!
	@brief Set one LED on or off Model 1 & 3
	@param position  0-7  == L1-L8 on PCB
	@param  value  0 off 1 on
	@return 
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinClaim
*/
rpiDisplay_Return_Codes_e TM1638plus_Model1::setLED(uint8_t position, uint8_t value)
{
	int GpioDataErrorstatus = 0;
	GpioDataErrorstatus = TM163X_SET_OUTPUT_DATA;
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
		return rpiDisplay_GpioPinClaim;
	}
	sendCommand(TM_WRITE_LOC);
	TM163X_STROBE_SetLow;
	sendData(TM_LEDS_ADR + (position << 1));
	sendData(value);
	TM163X_STROBE_SetHigh;
	return rpiDisplay_Success;
}

/*!
	@brief Set all LED's  on or off  Model 1 & 3
	@param  ledvalues 1 on , 0 off , 0xXXLL where LL = L8-L1
	@note MODEL 1:
		-# Upper byte ignored this byte/method is used by sub-class Model 3 
		-# Lower byte LED data model 1
		-# setLEDs(0xF0) Displays as XXXX LLLL (L1-L8),NOTE on display L8 is on right hand side.
*/
void TM1638plus_Model1::setLEDs(uint16_t ledvalues)
{
	uint8_t colour = 0;
	ledvalues = ledvalues & 0x00FF;
	for (uint8_t LEDposition = 0;  LEDposition < 8; LEDposition++) {
		if ((ledvalues & (1 << LEDposition)) != 0) {
			colour |= 0x01; 
		} 
		setLED(LEDposition, colour);
		colour = 0;
	}
}

/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32 
	@param TextAlignment enum text alignment, left or right alignment or leading zeros
*/
void TM1638plus_Model1::displayIntNum(unsigned long number, AlignTextType_e TextAlignment)
{
	char values[_TMDisplaySize + 1];
	char TextDisplay[6] = "%";
	char TextRight[4] = "8ld";
	char TextLeft[3] = "ld";
	char TextLeadZero[5] = "08ld";
	
	switch(TextAlignment) 
	{
		case TMAlignTextRight: strcat(TextDisplay ,TextRight); break; // %8ld
		case TMAlignTextLeft: strcat(TextDisplay ,TextLeft); break;  // %ld
		case TMAlignTextZeros: strcat(TextDisplay ,TextLeadZero); break; // %08ld
	}
	snprintf(values, _TMDisplaySize + 1, TextDisplay, number); 
	displayText(values);
}


/*!
	@brief Display an integer in a nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param TextAlignment  left or right alignment or leading zeros
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble.
*/
void TM1638plus_Model1::DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, AlignTextType_e TextAlignment)
{
	char valuesUpper[_TMDisplaySize + 1];
	char valuesLower[_TMDisplaySize/2 + 1];
	char TextDisplay[5] = "%";
	char TextRight[3] = "4d";
	char TextLeft[4] = "-4d";
	char TextLeadZero[4] = "04d";
	
	switch(TextAlignment) 
	{
		case TMAlignTextRight: strcat(TextDisplay ,TextRight); break; // %4d
		case TMAlignTextLeft: strcat(TextDisplay ,TextLeft); break;  // %-4d
		case TMAlignTextZeros: strcat(TextDisplay ,TextLeadZero); break; // %04d
	}
	
	snprintf(valuesUpper, _TMDisplaySize/2 + 1, TextDisplay, numberUpper);
	snprintf(valuesLower, _TMDisplaySize/2 + 1, TextDisplay, numberLower); 
	strcat(valuesUpper ,valuesLower);
	displayText(valuesUpper);
}


/*!
	@brief Display a text string  on display
	@param text pointer to a character array
	@note 
		Dots are removed from string and dot on preceding digit switched on
		"abc.def" will be shown as "abcdef" with c decimal point turned on.
*/
void TM1638plus_Model1::displayText(const char *text) {
	char c, pos;
	pos = 0;
		while ((c = (*text++)) && pos < _TMDisplaySize)  {
		if (*text == '.' && c != '.') {
			displayASCIIwDot(pos++, c);

			text++;
		}  else {
			displayASCII(pos++, c);
		}
		}
}

/*!
	@brief Display an ASCII character with decimal point turned on
	@param position The position on display 0-7 
	@param ascii The ASCII value from font table to display 
*/
void TM1638plus_Model1::displayASCIIwDot(uint8_t position, uint8_t ascii) { 
		// add 128 or 0x080 0b1000000 to turn on decimal point/dot in seven seg
	display7Seg(position, pFontSevenSegptr[ascii- TM_ASCII_OFFSET] + TM_DOT_MASK_DEC);
}

/*!
	@brief  Send seven segment value to seven segment
	@param position The position on display 0-7  
	@param value  byte of data corresponding to segments (dp)gfedcba 
	@note 	0b01000001 in value will set g and a on.
*/
void TM1638plus_Model1::display7Seg(uint8_t position, uint8_t value) { // call 7-segment
	sendCommand(TM_WRITE_LOC);
	TM163X_STROBE_SetLow;
	sendData(TM_SEG_ADR + (position << 1));
	sendData(value);
	TM163X_STROBE_SetHigh;
}

/*!
	@brief Display an ASCII character on display
	@param position The position on display 0-7  
	@param ascii The ASCII value from font table  to display 
*/
void TM1638plus_Model1::displayASCII(uint8_t position, uint8_t ascii) {
	display7Seg(position, pFontSevenSegptr[ascii - TM_ASCII_OFFSET]);
}
 
/*!
	@brief  Send Hexadecimal value to seven segment
	@param position The position on display 0-7  
	@param hex  hexadecimal  value (DEC) 0-15  (0x00 - 0x0F)
*/
void TM1638plus_Model1::displayHex(uint8_t position, uint8_t hex) 
{
		uint8_t offset = 0;
		hex = hex % 16;
		if (hex <= 9)
		{
			display7Seg(position, pFontSevenSegptr[hex + TM_HEX_OFFSET]);
			// 16 is offset in reduced ASCII table for 0 
		}else if ((hex >= 10) && (hex <=15))
		{
			// Calculate offset in reduced ASCII table for AbCDeF
			switch(hex) 
			{
			 case 10: offset = 'A'; break;
			 case 11: offset = 'b'; break;
			 case 12: offset = 'C'; break;
			 case 13: offset = 'd'; break;
			 case 14: offset = 'E'; break;
			 case 15: offset = 'F'; break;
			}
			display7Seg(position, pFontSevenSegptr[offset-TM_ASCII_OFFSET]);
		}
		
}

 /*!
	@brief  Read buttons values from display
	@return byte with value of buttons 1-8 b7b6b5b4b3b2b1b0 1 pressed, 0 not pressed.
	@note User may have to debounce buttons depending on application.
*/
uint8_t TM1638plus_Model1::readButtons()
{
	uint8_t buttons = 0;
	uint8_t v =0;
	int GpioDataErrorstatus = 0;

	TM163X_STROBE_SetLow;
	sendData(TM_BUTTONS_MODE); 
	GpioDataErrorstatus = TM163X_SET_INPUT_DATA;
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for input (%s)\n", lguErrorText(GpioDataErrorstatus));
	}
	for (uint8_t i = 0; i < 4; i++)
	{
		v = HighFreqshiftin() << i;
		buttons |= v;
	}

	GpioDataErrorstatus = TM163X_SET_OUTPUT_DATA;
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
	}
	TM163X_STROBE_SetHigh;
	return buttons;
}
