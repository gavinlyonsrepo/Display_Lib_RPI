 /*!
	@file  NOKIA_5110_LCD_RDL.hpp
	@brief Header file for NOKIA 5110 library to communicate with LCD
			Controls SPI and basic functionality
			Project Name: Display_Lib_RPI
			URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@author  Gavin Lyons
*/

#pragma once

// ************ libraries ************
#include <cstdint>
#include <cstdbool>
#include <stdarg.h>
#include <cstdio>
#include <lgpio.h>
#include "bicolor_graphics_RDL.hpp"

//*********** Definitions **************

#define LCD_FUNCTIONSET          0x20 /** LCD function set*/
#define LCD_POWERDOWN            0x04 /**< LCD power off */
#define LCD_ENTRYMODE            0x02 /**< LCD entry mode */
#define LCD_EXTENDEDINSTRUCTION  0x01 /**< LCD get into the EXTENDED mode when combined with Function set*/

#define LCD_DISPLAYCONTROL       0x08 /**< Set display control */
#define LCD_DISPLAYBLANK         0x00 /**< Blank display */
#define LCD_DISPLAYNORMAL        0x04 /**< normal mode display */
#define LCD_DISPLAYALLON         0x01 /**< all pixels on */
#define LCD_DISPLAYINVERTED      0x05 /**< display inverted */

#define LCD_SETYADDR             0x40
#define LCD_SETXADDR             0x80

#define LCD_SETTEMP  0x04  /**< set temperature coefficient */ 
#define LCD_CONTRAST 0xB0  /**< default value set LCD VOP contrast range 0xB1-BF */
#define LCD_BIAS 0x13      /**< LCD Bias mode 1:48 0x12 to 0x14 */

// lgpio bstraction 
// GPIO
#define LCD_DC_SetHigh  lgGpioWrite(_GpioHandle, _LCD_DC, 1)
#define LCD_DC_SetLow  lgGpioWrite(_GpioHandle ,_LCD_DC, 0)
#define LCD_RST_SetHigh  lgGpioWrite(_GpioHandle, _LCD_RST, 1)
#define LCD_RST_SetLow  lgGpioWrite(_GpioHandle, _LCD_RST, 0)
#define LCD_CE_SetHigh lgGpioWrite(_GpioHandle ,_LCD_CE, 1) // SW SPI only last 6 lines
#define LCD_CE_SetLow lgGpioWrite(_GpioHandle, _LCD_CE, 0)
#define LCD_CLK_SetHigh lgGpioWrite(_GpioHandle, _LCD_CLK, 1)
#define LCD_CLK_SetLow  lgGpioWrite(_GpioHandle, _LCD_CLK, 0)
#define LCD_DIN_SetHigh lgGpioWrite(_GpioHandle, _LCD_DIN, 1)
#define LCD_DIN_SetLow  lgGpioWrite(_GpioHandle, _LCD_DIN,0)

#define LCD_RST_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _LCD_RST,  0);
#define LCD_DC_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _LCD_DC,  0);
#define LCD_CE_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _LCD_CE,  0); // SW SPI only last 3 lines
#define LCD_CLK_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _LCD_CLK,  0);
#define LCD_DIN_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0, _LCD_DIN,  0);

// GPIO open and close
#define LCD_OPEN_GPIO_CHIP lgGpiochipOpen(_DeviceNumGpioChip)
#define LCD_CLOSE_GPIO_HANDLE lgGpiochipClose(_GpioHandle)
// GPIO free modes
#define LCD_GPIO_FREE_DC lgGpioFree(_GpioHandle , _LCD_DC)
#define LCD_GPIO_FREE_RST lgGpioFree(_GpioHandle , _LCD_RST)
#define LCD_GPIO_FREE_CE lgGpioFree(_GpioHandle , _LCD_CE) // SW SPI only last 3 lines
#define LCD_GPIO_FREE_CLK lgGpioFree(_GpioHandle , _LCD_CLK)
#define LCD_GPIO_FREE_DIN lgGpioFree(_GpioHandle , _LCD_DIN)

//SPI 
#define LCD_OPEN_SPI lgSpiOpen(_spiDev, _spiChan, _spiBaud, _spiFlags)
#define LCD_CLOSE_SPI lgSpiClose(_spiHandle)
#define LCD_WRITE_SPI lgSpiWrite( _spiHandle, (const char*)TransmitBuffer, sizeof(TransmitBuffer))

#define LCD_Rotate_swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

// Section :: Classes

/*!
	@brief Class Controls SPI comms and LCD functionality
*/
class NOKIA_5110_RPI : public bicolor_graphics 
{

public:

	// SW SPI 
	NOKIA_5110_RPI(int16_t lcdwidth, int16_t lcdheight, uint8_t LCD_RST, uint8_t LCD_DC, uint8_t LCD_CE, int8_t LCD_DIN, int8_t LCD_CLK);
	//HW SPI 
	NOKIA_5110_RPI(int16_t lcdwidth, int16_t lcdheight, uint8_t LCD_RST, uint8_t LCD_DC);
	~NOKIA_5110_RPI(){};

	// SW SPI 
	rpiDisplay_Return_Codes_e LCDBegin(bool Inverse, uint8_t Contrast,uint8_t Bias, int gpioDev);
	// HW SPI
	rpiDisplay_Return_Codes_e LCDBegin(bool Inverse, uint8_t Contrast,uint8_t Bias, int device, int channel, int speed, int flags, int gpioDev);
	void LCDenableSleep(void);
	void LCDdisableSleep(void);
	bool LCDIsSleeping(void);

	rpiDisplay_Return_Codes_e LCDSPIoff(void);
	void LCDPowerDown(void);

	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) override;

	void LCDSetContrast(uint8_t con);
	void LCDdisplayUpdate(void);
	void LCDdisplayClear(void);
	void LCDfillScreen(void);
	void LCDfillScreenPattern(uint8_t);
	void LCDinvertDisplay(bool inv);

	uint16_t LCDHighFreqDelayGet(void);
	void LCDHighFreqDelaySet(uint16_t);
	bool isHardwareSPI(void);

private:

	void LCDWriteData(uint8_t data);
	void LCDWriteCommand(uint8_t command);
	void LCDInit(void);

	// SPI
	bool _LCDHardwareSPI = true;      /**< Hardware SPI true on , false off*/
	uint16_t _LCDHighFreqDelay = 0;  /**< uS GPIO Communications delay ,used in SW SPI ONLY */
	int _spiHandle = 0; /**< Hold a handle for the SPI device on the channel.*/
	int _spiDev = 0; /**< A SPI device, >= 0. */
	int _spiChan = 0; /**< A SPI channel, >= 0. */
	int _spiBaud = 50000;   /**< The speed of serial communication in bits per second. */
	int _spiFlags = 0; /**<The flags 2 LSB defines SPI mode */ 

	// GPIO
	int8_t _LCD_DC;  /**< Data or command GPIO */
	int8_t _LCD_RST; /**< Reset GPIO */
	int8_t _LCD_CE;  /**< Chip enable,  Software SPI only */
	int8_t _LCD_CLK; /**< Clock GPIO, Software SPI only*/
	int8_t _LCD_DIN; /**< Data GPIO ,Software SPI only */
	int _DeviceNumGpioChip = 0; /**< The device number of a gpiochip 4=rpi5 0=rpi4,3 /dev/gpio */
	int _GpioHandle = 0; /**< This holds a handle to a gpiochip device opened by lgGpiochipOpen  */

	// Display
	uint8_t  _contrast;     /**< LCD contrast */
	uint8_t  _bias;         /**< LCD bias*/
	bool _inverse = false;  /**< LCD inverted , false for off*/
	bool _sleep;            /**< LCD sleep mode*/
	int16_t _LCD_HEIGHT=48; /**< height of LCD in pixels*/
	int16_t _LCD_WIDTH=84; /**< width of LCD in pixels*/
	uint16_t _LCD_Display_size = _LCD_WIDTH * (_LCD_HEIGHT/8);/**< size of display in pixels 504 (LCDWIDTH*(LCDHEIGHT / 8)*/
	uint8_t LCDDisplayBuffer[504]; /**< buffer which holds screen data */
}; //end of class


