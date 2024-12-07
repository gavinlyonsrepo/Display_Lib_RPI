/*!
	@file     TM1638plus_Model1_RDL.hpp
	@author   Gavin Lyons
	@brief    RPI library Tm1638plus, Header file for for TM1638 module(LED & KEY). Model 1
*/

#pragma once

#include "TM1638plus_common_RDL.hpp"

/*!
	@brief Class for Model 1
*/
class TM1638plus_Model1 : public TM1638plus_common  {

public:

	TM1638plus_Model1 (uint8_t strobe, uint8_t clock, uint8_t data ,int gpioDev) ;
	
	uint8_t readButtons(void);
	
	void displayText(const char *text);
	void displayASCII(uint8_t position, uint8_t ascii);
	void displayASCIIwDot(uint8_t position, uint8_t ascii) ;
	void displayHex(uint8_t position, uint8_t hex);
	void display7Seg(uint8_t position, uint8_t value);
	void displayIntNum(unsigned long number, AlignTextType_e = TMAlignTextLeft);
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower,AlignTextType_e = TMAlignTextLeft);
	
	// LED methods defined as overridden in sub-class for Model 3
	virtual void setLEDs(uint16_t leds); 
	virtual rpiDisplay_Return_Codes_e setLED(uint8_t position, uint8_t value); 
	
};
