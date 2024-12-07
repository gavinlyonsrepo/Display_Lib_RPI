/*!
	@file   TM1638plus_Model2_RDL.hpp
	@author  Gavin Lyons
	@brief RPI library Tm1638plus_RPI, Header file for TM1638 module(16 KEY 16 pushbuttons. Model 2.
*/

#pragma once

#include "TM1638plus_common_RDL.hpp"

/*!
	@brief Class for Model 2
*/
class TM1638plus_Model2 : public TM1638plus_common {

public:
	// Constructor 
	TM1638plus_Model2(uint8_t strobe, uint8_t clock, uint8_t data, int gpioDev, bool swap_nibbles= false);

	// Methods

	uint8_t ReadKey16(void);
	uint16_t ReadKey16Two(void);

	void DisplaySegments(uint8_t segment, uint8_t segmentValue);
	void DisplayHexNum(uint16_t  numberUpper, uint16_t numberLower, uint8_t dots,  AlignTextType_e = TMAlignTextLeft);
	void DisplayDecNum(unsigned long number, uint8_t dots,  AlignTextType_e = TMAlignTextLeft);
	void DisplayStr(const char* string, uint16_t dots = 0);
	void ASCIItoSegment(const uint8_t values[]);
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, uint8_t dots,  AlignTextType_e = TMAlignTextLeft);

private:

	bool _SWAP_NIBBLES= false; /**< Swap the nibbles on display byte */

};

