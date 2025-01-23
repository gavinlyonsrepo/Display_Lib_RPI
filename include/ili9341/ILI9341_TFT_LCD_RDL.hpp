/*!
	@file     ILI9341_TFT_LCD_RDL.hpp
	@author   Gavin Lyons
	@brief    Library header file for ILI9341_TFT_LCD_RDL library.
				to manage hardware interface with ILI9341
*/

#pragma once

#include <lgpio.h>
#include "color16_graphics_RDL.hpp"

#define ILI9341_RDMODE     0x0A /**< Read the power mode */
#define ILI9341_RDMADCTL   0x0B /**< Read MADCTL */
#define ILI9341_RDPIXFMT   0x0C /**< Read pixel Format */
#define ILI9341_RDIMGFMT   0x0D /**< Read image format */
#define ILI9341_RDSELFDIAG 0x0F /**< Read self-diagnostic result */


/*!
@brief Class to interface with ILI9341 LCD display
*/
class ILI9341_TFT : public color16_graphics
{
public:
	ILI9341_TFT();
	~ILI9341_TFT(){};

	/*! TFT rotate modes in degrees*/
	enum  TFT_rotate_e : uint8_t
	{
		TFT_Degrees_0 = 0, /**< No rotation 0 degrees*/
		TFT_Degrees_90,    /**< Rotation 90 degrees*/
		TFT_Degrees_180,   /**< Rotation 180 degrees*/
		TFT_Degrees_270    /**< Rotation 270 degrees*/
	};

	//Functions
	virtual void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;

	//Init Setup related
	void SetupGPIO(int8_t RST, int8_t DC, int8_t CS, int8_t SCLK, int8_t MOSI, int8_t MISO); //SW SPI
	void SetupGPIO(int8_t RST, int8_t DC); // HW SPI
	void InitScreenSize(uint16_t w, uint16_t h);
	rpiDisplay_Return_Codes_e InitSPI(uint16_t CommDelay, int gpioDev); // SW SPI
	rpiDisplay_Return_Codes_e InitSPI(int device, int channel, int speed, int flags, int gpioDev); // HW SPI

	// SPI related
	uint16_t HighFreqDelayGet(void);
	void HighFreqDelaySet(uint16_t);
	rpiDisplay_Return_Codes_e PowerDown(void);
	void ResetPIN(void);
	void EnableDisplay(bool enableDisplay);

	// screen related
	void setRotation(TFT_rotate_e rotation);
	void invertDisplay(bool i);
	void scrollTo(uint16_t y);
	void setScrollMargins(uint16_t top, uint16_t bottom);
	void NormalMode(void);
	rpiDisplay_Return_Codes_e TFTResetPin(void);
	void PrintDiagnostic(void);

protected:

private:

	rpiDisplay_Return_Codes_e TFTDataCommandPin(void);
	rpiDisplay_Return_Codes_e TFTClock_Data_ChipSelect_Pins(void);
	rpiDisplay_Return_Codes_e ILI9341Initialize(void);
	void TFTSetupResetPin(int8_t rst);
	void cmdInit(void);

	// SPI
	bool _resetPinOn = true; /**< reset pin? true:hw rst pin, false:sw rt*/
	
	// SPI related 
	int _spiDev = 0; /**< A SPI device, >= 0. */
	int _spiChan = 0; /**< A SPI channel, >= 0. */
	int _spiBaud = 50000;   /**< The speed of serial communication in bits per second. */
	int _spiFlags = 0; /**<The flags 2 LSB defines SPI mode */ 
	
	// Screen 
	uint16_t _widthStartTFT = 240;  /**< never change after first init */
	uint16_t _heightStartTFT = 320; /**< never change after first init */
};

