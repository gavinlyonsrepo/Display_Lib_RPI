/*!
	@file     ST7789_TFT_LCD_RDL.hpp
	@author   Gavin Lyons
	@brief    Library header file for ST7789_TFT_RPI library.
*/

#pragma once

// Section Libraries
#include <cstdint>
#include <cstdbool>
#include <cstring>
#include <bcm2835.h> // Dependency
#include "color16_graphics_RDL.hpp"

/*!
	@brief Class to control ST7789 TFT basic functionality.
*/
class ST7789_TFT : public color16_graphics
{

public:

	ST7789_TFT();
	~ST7789_TFT(){};

	//  Enum

	/*! TFT rotate modes in degrees*/
	enum  TFT_rotate_e : uint8_t
	{
		TFT_Degrees_0 = 0, /**< No rotation 0 degrees*/
		TFT_Degrees_90,    /**< Rotation 90 degrees*/
		TFT_Degrees_180,   /**< Rotation 180 degrees*/
		TFT_Degrees_270    /**< Rotation 270 degrees*/
	};

	TFT_rotate_e TFT_rotate = TFT_Degrees_0; /**< Enum to hold rotation */

	// Functions
	// From graphics virtual
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;
	//Setup related
	void TFTSetupGPIO(int8_t, int8_t, int8_t, int8_t, int8_t); //SW SPI
	void TFTSetupGPIO(int8_t, int8_t); // HW SPI
	void TFTInitScreenSize(uint8_t xOffset, uint8_t yOffset, uint16_t w, uint16_t h);
	void TFTInitSPI(uint16_t CommDelay); // SW SPI
	rpiDisplay_Return_Codes_e TFTInitSPI(uint32_t hertz = 0, uint8_t SPICE_Pin = 0 ); // HW SPI

	// SPI related
	uint16_t HighFreqDelayGet(void);
	void HighFreqDelaySet(uint16_t);
	void TFTSPIHWSettings(void);
	void TFTPowerDown(void);
	// Screen related
	void TFTsetRotation(TFT_rotate_e r);
	void TFTchangeInvertMode(bool m);
	void TFTpartialDisplay(bool m);
	void TFTenableDisplay(bool m);
	void TFTidleDisplay(bool m);
	void TFTsleepDisplay(bool m);
	void TFTNormalMode(void);
	void TFTsetScrollDefinition(uint16_t th, uint16_t tb, bool sd);
	void TFTVerticalScroll(uint16_t vsp);

private:

	void TFTResetPIN(void);
	rpiDisplay_Return_Codes_e TFTST7789Initialize(void);
	void cmd89(void);
	void AdjustWidthHeight(void);
	
	// Screen
	uint8_t _colstart = 0;          /**< Used to offset column in the event of defect at edge of screen */
	uint8_t _rowstart = 0;          /**< Used to offset row in the event of defect at edge of screen */
	uint8_t _colstart2 = 0;         /**< Offset from the right for ST7789*/
	uint8_t _rowstart2 = 0;         /**< Offset from the bottom for ST7789*/
	uint16_t _widthStartTFT = 240;  /**<  never change after first init */
	uint16_t _heightStartTFT = 280; /**< never change after first init */

	// SPI
	uint32_t _hertz = 8000000; /**< Spi freq in Hertz , MAX 125 Mhz MIN 30Khz */
	uint8_t _SPICEX_pin = 0;    /**< value = X , which SPI_CE pin to use */

}; //end of ST7789_TFT  class

// ********************** EOF *********************
