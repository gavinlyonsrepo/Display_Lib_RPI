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
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
*/
TM1638plus_common::TM1638plus_common(uint8_t strobe, uint8_t clock, uint8_t data, int gpioDev)
{
	_Display_CS = strobe;
	_Display_SDATA = data;
	_Display_SCLK = clock;
	_DeviceNumGpioChip = gpioDev;
}

/*!
	@brief Reset / clear the display
	@note The display is cleared by writing zero to all data segment  addresses.
*/
void TM1638plus_common::reset() {
	sendCommand(TM_WRITE_INC); // set auto increment mode
	Display_CS_SetLow;
	sendData(TM_SEG_ADR);   // set starting address to 0
	for (uint8_t position = 0; position < 16; position++)
	{
		sendData(0x00); //clear all segments
	}
	Display_CS_SetHigh;
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
	@return 
		-# rdlib::Success
		-# rdlib::GpioChipDevice;
		-# rdlib::GpioPinClaim
		@note Call in Setup
*/
rdlib::Return_Codes_e TM1638plus_common::displayBegin(void)
{
	int GpioStrobeErrorstatus = 0;
	int GpioClockErrorstatus = 0;
	int GpioDataErrorstatus = 0;

	_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Errror : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rdlib::GpioChipDevice;
	}

	// Clain GPIO as outputs
	GpioStrobeErrorstatus = Display_CS_SetDigitalOutput;
	GpioClockErrorstatus = Display_SCLK_SetDigitalOutput;
	GpioDataErrorstatus = Display_SDATA_SetDigitalOutput;

	if (GpioStrobeErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim STB GPIO for output (%s)\n", lguErrorText(GpioStrobeErrorstatus));
		return rdlib::GpioPinClaim;
	}
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorstatus));
		return rdlib::GpioPinClaim;
	}
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
		return rdlib::GpioPinClaim;
	}

	sendCommand(TM_ACTIVATE);
	brightness(_TMDefaultBrightness);
	reset();
	return rdlib::Success;
}

/*!
	@brief Send command to display
	@param value command byte to send
*/
void TM1638plus_common::sendCommand(uint8_t value)
{
	Display_CS_SetLow;
	sendData(value);
	Display_CS_SetHigh;
}

/*!
	@brief Send Data to display
	@param data  Data byte to send
*/
void TM1638plus_common::sendData(uint8_t data)
{
	HighFreqshiftOut(data);
}


/*!
	@brief Shifts in a byte of data from the Tm1638 SPI-like bus
	@return  Data byte
	@note _TMCommDelay microsecond delay may have to be adjusted depending on processor
*/
uint8_t  TM1638plus_common::HighFreqshiftin(void)
{
	uint8_t value = 0;
	uint8_t i = 0;

	for(i = 0; i < 8; ++i)
	{
		value |= (Display_SDATA_Read << i);
		Display_SCLK_SetHigh;
		delayMicroSecRDL(_TMCommDelay);
		Display_SCLK_SetLow;
		delayMicroSecRDL(_TMCommDelay);
	}
	return value;
}

 /*!
	@brief Shifts out a byte of data on to the Tm1638 SPI-like bus
	@param val The byte of data to shift out
	@note _TMCommDelay microsecond delay may have to be adjusted depending on processor
*/
void TM1638plus_common::HighFreqshiftOut(uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		!!(val & (1 << i)) ? Display_SDATA_SetHigh : Display_SDATA_SetLow;
		Display_SCLK_SetHigh;
		delayMicroSecRDL(_TMCommDelay);
		Display_SCLK_SetLow;
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

/*!
	@brief Close method , frees GPIO and deactivate display.
	@return 
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinFree
	@note call at end of program
*/
rdlib::Return_Codes_e TM1638plus_common::displayClose(void)
{
	uint8_t ErrorFlag = 0; // Becomes > 0 in event of error

	int GpioStrobeErrorstatus = 0;
	int GpioClockErrorstatus = 0;
	int GpioDataErrorstatus = 0;
	int GpioCloseStatus = 0;

	GpioStrobeErrorstatus = Display_GPIO_FREE_CS;
	GpioClockErrorstatus =  Display_GPIO_FREE_CLK;
	GpioDataErrorstatus =   Display_GPIO_FREE_SDATA;

	if (GpioStrobeErrorstatus < 0 )
	{
		fprintf(stderr,"Error :: Can't Free STB GPIO (%s)\n", lguErrorText(GpioStrobeErrorstatus));
		ErrorFlag = 2;
	}
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error :: Can't Free CLK GPIO t (%s)\n", lguErrorText(GpioClockErrorstatus));
		ErrorFlag = 2;
	}
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error :: Can't free DATA GPIO (%s)\n", lguErrorText(GpioDataErrorstatus));
		ErrorFlag = 2;
	}

	GpioCloseStatus = Display_CLOSE_GPIO_HANDLE; // close gpiochip
	if ( GpioCloseStatus < 0)
	{
		fprintf(stderr,"Error :: Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		ErrorFlag = 3;
	}

	// 4 Check error flag (we don't want to return early for any failure)
	switch (ErrorFlag)
	{
		case 0:return rdlib::Success;break;
		case 2:return rdlib::GpioPinFree;break;
		case 3:return rdlib::GpioChipDevice;;break;
		default:fprintf(stderr, "Warning::Unknown error flag value in displayClose"); break;
	}
	return rdlib::Success;
}
