/*!
	@file     TM1638plus_common_RDL.hpp
	@author   Gavin Lyons
	@brief    RPI library Tm1638plus, Header  file for common data and functions between model classes. 
	@note  See URL for full details. https://github.com/gavinlyonsrepo/Display_Lib_RPI
*/

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include <cstring>

#include <lgpio.h>
#include "common_data_RDL.hpp"
#include "tm163X_font_data_RDL.hpp"


/*!
	@brief  The base Class , used to store common data & functions for all models types.
*/
class TM1638plus_common{

public:

	/*! Alignment of text on display */
	enum AlignTextType_e : uint8_t 
	{
		TMAlignTextRight = 1, /**< Align text to the right on  display */
		TMAlignTextLeft = 2, /**< Align text to the left on  display */
		TMAlignTextZeros = 3 /**< Add leading zeros  to the text */
	};

	/*! Display Offsets and Masks*/
	enum TMOffsetsMasks : uint8_t
	{
		TM_ASCII_OFFSET = 0x20, /**< 32 Ascii table offset to jump over first missing 32 chars */
		TM_HEX_OFFSET   = 0x10, /**< 16 Ascii table offset to reach number position */
		TM_DOT_MASK_DEC = 0x80, /**< 128  Mask to  switch on decimal point in seven seg. */
		TM_BRIGHT_MASK = 0x07 /**< Brightness mask */
	};

	/*! Tm1638 register commands*/
	enum TMCommands : uint8_t
	{
		TM_ACTIVATE = 0x8F, /**< Start up */
		TM_BUTTONS_MODE = 0x42, /**< Buttons mode */
		TM_WRITE_LOC = 0x44, /**< Write to a location */
		TM_WRITE_INC = 0x40, /**< Incremental write */
		TM_SEG_ADR = 0xC0,  /**< leftmost segment Address C0 C2 C4 C6 C8 CA CC CE */
		TM_LEDS_ADR = 0xC1,  /**< Leftmost LED address C1 C3 C5 C7 C9 CB CD CF */
		TM_BRIGHT_ADR = 0x88 /**< Brightness address */
	};

	// Constructor
	TM1638plus_common(uint8_t strobe, uint8_t clock, uint8_t data, int gpioDev);

	void reset(void);
	rpiDisplay_Return_Codes_e displayBegin(void);
	void brightness(uint8_t brightness);
	uint16_t TMCommDelayGet(void);
	void TMCommDelayset(uint16_t);
	rpiDisplay_Return_Codes_e displayClose(void);

protected:
	void sendCommand(uint8_t value);
	void sendData(uint8_t  data);
	uint8_t  HighFreqshiftin(void);
	void HighFreqshiftOut(uint8_t val);
	
	uint8_t _STROBE_IO; /**<  GPIO connected to STB on Tm1638  */
	uint8_t _DATA_IO; /**<  GPIO connected to DIO on Tm1638  */
	uint8_t _CLOCK_IO; /**<  GPIO connected to CLk on Tm1638  */
	const uint8_t _TMDisplaySize = 8; /**< size of display in digts */
	int _DeviceNumGpioChip = 0; /**< The device number of a gpiochip 4=rpi5 0=rpi4,3 /dev/gpio */
	int _GpioHandle = 0; /**< This returns a handle to a gpiochip device. */

private:
	uint16_t _TMCommDelay = 1; /**<  uS delay used in communications , User adjust */
	const uint8_t _TMDefaultBrightness = 0x02; /**<  can be 0x00 to 0x07 , User adjust */


};


