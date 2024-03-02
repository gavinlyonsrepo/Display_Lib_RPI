/*!
	@file     TM1638plus_common_RDL.cpp
	@author   Gavin Lyons
	@brief    RPI library Tm1638plus, source file for common data and functions between model classes. 
*/

#include "../../include/tm1638/TM1638plus_common_RDL.hpp"

/*!
	@brief Constructor for class TM1638plus_common
	@param strobe STB pin
	@param clock CLk pin
	@param data DIO pin 
*/
TM1638plus_common::TM1638plus_common(uint8_t strobe, uint8_t clock, uint8_t data)
{
	_STROBE_IO = strobe;
	_DATA_IO = data;
	_CLOCK_IO = clock;
}

/*!
	@brief Reset / clear the display
	@note The display is cleared by writing zero to all data segment  addresses.
*/
void TM1638plus_common::reset() {
	sendCommand(TM_WRITE_INC); // set auto increment mode
	bcm2835_gpio_write(_STROBE_IO, LOW);
	sendData(TM_SEG_ADR);   // set starting address to 0
	for (uint8_t position = 0; position < 16; position++)
	{
		sendData(0x00); //clear all segments
	}
	bcm2835_gpio_write(_STROBE_IO, HIGH);
}

/*!
	@brief  Sets the brightness level of segments in display on a scale of brightness
	@param brightness byte with value 0 to 7 The DEFAULT_BRIGHTNESS = 0x02
*/
void TM1638plus_common::brightness(uint8_t brightness)
{
	uint8_t  value = 0;
	value = TM_BRIGHT_ADR + (TM_BRIGHT_MASK & brightness);
	sendCommand(value);
}

/*!
	@brief Begin method , sets pin modes and activate display.
	@note Call in Setup
*/
void TM1638plus_common::displayBegin(void)
{
	bcm2835_gpio_fsel( _STROBE_IO, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(_DATA_IO, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(_CLOCK_IO, BCM2835_GPIO_FSEL_OUTP);
	sendCommand(TM_ACTIVATE);
	brightness(_TMDefaultBrightness);
	reset();
}

/*!
	@brief Send command to display
	@param value command byte to send
*/
void TM1638plus_common::sendCommand(uint8_t value)
{
	bcm2835_gpio_write(_STROBE_IO, LOW);
	sendData(value);
	bcm2835_gpio_write(_STROBE_IO, HIGH);
}

/*!
	@brief Send Data to display
	@param data  Data byte to send
*/
void TM1638plus_common::sendData(uint8_t data)
{
	HighFreqshiftOut(_DATA_IO, _CLOCK_IO, data);
}


/*!
	@brief Shifts in a byte of data from the Tm1638 SPI-like bus
	@param dataPin Tm1638 Data GPIO
	@param clockPin Tm1638 Clock GPIO
	@return  Data byte
	@note _TMCommDelay microsecond delay may have to be adjusted depending on processor
*/
uint8_t  TM1638plus_common::HighFreqshiftin(uint8_t dataPin, uint8_t clockPin) 
{
	uint8_t value = 0;
	uint8_t i = 0;

	for(i = 0; i < 8; ++i) 
	{
		value |= bcm2835_gpio_lev(dataPin) << i;
		bcm2835_gpio_write(clockPin, HIGH);
		delayMicroSecRDL(_TMCommDelay);
		bcm2835_gpio_write(clockPin, LOW);
		delayMicroSecRDL(_TMCommDelay);
	}
	return value;
}

 /*!
	@brief Shifts out a byte of data on to the Tm1638 SPI-like bus
	@param dataPin Tm1638 Data GPIO
	@param clockPin Tm1638 Clock GPIO
	@param val The byte of data to shift out 
	@note _TMCommDelay microsecond delay may have to be adjusted depending on processor
*/
void TM1638plus_common::HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  
	{
		bcm2835_gpio_write(dataPin, !!(val & (1 << i)));
		bcm2835_gpio_write(clockPin, HIGH);
		delayMicroSecRDL(_TMCommDelay);
		bcm2835_gpio_write (clockPin, LOW);
		delayMicroSecRDL(_TMCommDelay);
	}
}

/*!
	@brief get the TM comm delay.
	@return The TM comm delay in uS
*/
uint16_t TM1638plus_common::TMCommDelayGet(void){return  _TMCommDelay;}

/*!
	@brief set the TM comm delay.
	@param CommDelay The TM comm delay in uS
*/
void TM1638plus_common::TMCommDelayset(uint16_t CommDelay) {_TMCommDelay = CommDelay;}

