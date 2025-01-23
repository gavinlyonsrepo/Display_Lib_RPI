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
	_STROBE_IO = strobe;
	_DATA_IO = data;
	_CLOCK_IO = clock;
	_DeviceNumGpioChip = gpioDev;
}

/*!
	@brief Reset / clear the display
	@note The display is cleared by writing zero to all data segment  addresses.
*/
void TM1638plus_common::reset() {
	sendCommand(TM_WRITE_INC); // set auto increment mode
	TM163X_STROBE_SetLow;
	sendData(TM_SEG_ADR);   // set starting address to 0
	for (uint8_t position = 0; position < 16; position++)
	{
		sendData(0x00); //clear all segments
	}
	TM163X_STROBE_SetHigh;
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
		-# rpiDisplay_Success
		-# rpiDisplay_GpioChipDevice;
		-# rpiDisplay_GpioPinClaim
		@note Call in Setup
*/
rpiDisplay_Return_Codes_e TM1638plus_common::displayBegin(void)
{
	int GpioStrobeErrorstatus = 0;
	int GpioClockErrorstatus = 0;
	int GpioDataErrorstatus = 0;

	_GpioHandle = TM163X_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Errror : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rpiDisplay_GpioChipDevice;
	}

	// Clain GPIO as outputs
	GpioStrobeErrorstatus = TM163X_SET_OUTPUT_STROBE;
	GpioClockErrorstatus = TM163X_SET_OUTPUT_CLOCK;
	GpioDataErrorstatus = TM163X_SET_OUTPUT_DATA;

	if (GpioStrobeErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim STB GPIO for output (%s)\n", lguErrorText(GpioStrobeErrorstatus));
		return rpiDisplay_GpioPinClaim;
	}
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorstatus));
		return rpiDisplay_GpioPinClaim;
	}
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
		return rpiDisplay_GpioPinClaim;
	}

	sendCommand(TM_ACTIVATE);
	brightness(_TMDefaultBrightness);
	reset();
	return rpiDisplay_Success;
}

/*!
	@brief Send command to display
	@param value command byte to send
*/
void TM1638plus_common::sendCommand(uint8_t value)
{
	TM163X_STROBE_SetLow;
	sendData(value);
	TM163X_STROBE_SetHigh;
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
		value |= (TM163X_DATA_READ << i);
		TM163X_CLOCK_SetHigh;
		delayMicroSecRDL(_TMCommDelay);
		TM163X_CLOCK_SetLow;
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
		!!(val & (1 << i)) ? TM163X_DATA_SetHigh : TM163X_DATA_SetLow;
		TM163X_CLOCK_SetHigh;
		delayMicroSecRDL(_TMCommDelay);
		TM163X_CLOCK_SetLow;
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
		-# rpiDisplay_Success
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinFree
	@note call at end of program
*/
rpiDisplay_Return_Codes_e TM1638plus_common::displayClose(void)
{
	uint8_t ErrorFlag = 0; // Becomes > 0 in event of error

	int GpioStrobeErrorstatus = 0;
	int GpioClockErrorstatus = 0;
	int GpioDataErrorstatus = 0;
	int GpioCloseStatus = 0;

	GpioStrobeErrorstatus = TM163X_GPIO_FREE_STROBE;
	GpioClockErrorstatus =  TM163X_GPIO_FREE_CLOCK;
	GpioDataErrorstatus =   TM163X_GPIO_FREE_DATA;

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

	GpioCloseStatus = TM163X_CLOSE_GPIO_HANDLE; // close gpiochip
	if ( GpioCloseStatus < 0)
	{
		fprintf(stderr,"Error :: Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		ErrorFlag = 3;
	}

	// 4 Check error flag (we don't want to return early for any failure)
	switch (ErrorFlag)
	{
		case 0:return rpiDisplay_Success;break;
		case 2:return rpiDisplay_GpioPinFree;break;
		case 3:return rpiDisplay_GpioChipDevice;;break;
		default:printf("Warning::Unknown error flag value in displayClose"); break;
	}
	return rpiDisplay_Success;
}
