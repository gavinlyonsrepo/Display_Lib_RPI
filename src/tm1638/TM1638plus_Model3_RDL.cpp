/*!
	@file     TM1638plus_Model3_RDL.cpp
	@author   Gavin Lyons
	@brief    RPI library Tm1638plus, Source file for for TM1638 module(LED & KEY). Model 3
*/

#include "../../include/tm1638/TM1638plus_Model3_RDL.hpp"

/*!
	@brief Constructor for class TM1638plus_Model3
	@param strobe  GPIO STB pin
	@param clock  GPIO CLK pin
	@param data  GPIO DIO pin
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
*/
TM1638plus_Model3::TM1638plus_Model3(uint8_t strobe, uint8_t clock, uint8_t data, int gpioDev) : TM1638plus_Model1(strobe, clock, data, gpioDev) {
 // Blank constructor
}

/*!
	@brief Set one LED on or off Model  3
	@param position  0-7  == L1-L8 on PCB
	@param  value see enum TMLEDColors red = 0x02, green = 0x01, 0 is off
	@return 
		-# rpiDisplay_GpioPinClaim
		-# rpiDisplay_Success
*/
rpiDisplay_Return_Codes_e  TM1638plus_Model3::setLED(uint8_t position, uint8_t value)
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
	@brief Set the LEDs. passed one 16bit integer.
	@param ledvalues
		MSB uint8_t for the green LEDs, LSB uint8_t for the red LEDs (0xgreenred) 
		ie. 0xE007   1110 0000 0000 0111 results in L8-L1  GGGX XRRR, NOTE L8 is RHS on display
		setLEDs(0xFE01); // L1-L8 turns on RGGG GGGG on display L1-L8 as seen IRL
*/
void TM1638plus_Model3::setLEDs(uint16_t ledvalues)
{
	for (uint8_t LEDposition = 0;  LEDposition < 8; LEDposition++) {
		uint8_t colour = 0;

		if ((ledvalues & (1 << LEDposition)) != 0) {
			colour |= TM_RED_LED; //scan lower uint8_t, set Red if one
		} 

		if ((ledvalues & (1 << (LEDposition + 8))) != 0) {
			colour |= TM_GREEN_LED; //scan upper uint8_t, set green if one
		}

		setLED(LEDposition, colour);
	}
}
