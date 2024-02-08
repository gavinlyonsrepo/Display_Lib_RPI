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
#include <bcm2835.h>
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

// GPIO
#define LCD_DC_SetHigh  bcm2835_gpio_write(_LCD_DC, HIGH)
#define LCD_DC_SetLow  bcm2835_gpio_write(_LCD_DC, LOW)
#define LCD_RST_SetHigh  bcm2835_gpio_write(_LCD_RST, HIGH)
#define LCD_RST_SetLow  bcm2835_gpio_write(_LCD_RST, LOW)
#define LCD_CE_SetHigh bcm2835_gpio_write(_LCD_CE, HIGH) // SW SPI last 6 lines
#define LCD_CE_SetLow bcm2835_gpio_write(_LCD_CE, LOW)
#define LCD_CLK_SetHigh bcm2835_gpio_write(_LCD_CLK, HIGH)
#define LCD_CLK_SetLow  bcm2835_gpio_write(_LCD_CLK, LOW)
#define LCD_DIN_SetHigh bcm2835_gpio_write(_LCD_DIN, HIGH)
#define LCD_DIN_SetLow  bcm2835_gpio_write(_LCD_DIN,LOW)

#define LCD_DC_SetDigitalOutput bcm2835_gpio_fsel(_LCD_DC, BCM2835_GPIO_FSEL_OUTP)
#define LCD_RST_SetDigitalOutput bcm2835_gpio_fsel(_LCD_RST, BCM2835_GPIO_FSEL_OUTP)
#define LCD_CLK_SetDigitalOutput bcm2835_gpio_fsel(_LCD_CLK, BCM2835_GPIO_FSEL_OUTP) // SW SPI last 3 lines
#define LCD_DIN_SetDigitalOutput bcm2835_gpio_fsel(_LCD_DIN, BCM2835_GPIO_FSEL_OUTP)
#define LCD_CE_SetDigitalOutput bcm2835_gpio_fsel(_LCD_CE, BCM2835_GPIO_FSEL_OUTP)

#define LCD_Rotate_swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

// Section :: Classes

/*!
	@brief Class Controls SPI comms and LCD functionality
*/
class NOKIA_5110_RPI : public bicolor_graphics 
{

public:

	NOKIA_5110_RPI(int16_t lcdwidth, int16_t lcdheight, uint8_t LCD_RST, uint8_t LCD_DC, uint8_t LCD_CE, int8_t LCD_DIN, int8_t LCD_CLK);
	NOKIA_5110_RPI(int16_t lcdwidth, int16_t lcdheight, uint8_t LCD_RST, uint8_t LCD_DC, uint32_t LCD_spi_divider, uint8_t SPICE_Pin);
	~NOKIA_5110_RPI(){};

	rpiDisplay_Return_Codes_e LCDBegin(bool Inverse = false, uint8_t Contrast = LCD_CONTRAST,uint8_t Bias = LCD_BIAS);
	void LCDenableSleep(void);
	void LCDdisableSleep(void);
	bool LCDIsSleeping(void);

	void LCDSPIHWSettings(void);
	void LCDSPIoff(void);
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

private:

	void LCDWriteData(uint8_t data);
	void LCDWriteCommand(uint8_t command);
	bool isHardwareSPI(void);
	
	bool _LCDHardwareSPI = true;      /**< Hardware SPI true on , false off*/
	uint32_t _LCD_SPICLK_DIVIDER = 0 ; /**< SPI clock divider , bcm2835SPIClockDivider , HW SPI only */
	uint8_t _LCD_SPICE_PIN = 0;      /**< which SPI_CE pin to use , 0 or 1 , HW SPI only */
	uint16_t _LCDHighFreqDelay = 0;  /**< uS GPIO Communications delay ,used in SW SPI ONLY */
	
	int8_t _LCD_DC;  /**< Data or command GPIO */
	int8_t _LCD_RST; /**< Reset GPIO */
	int8_t _LCD_CE;  /**< Chip enable,  Software SPI only */
	int8_t _LCD_CLK; /**< Clock GPIO, Software SPI only*/
	int8_t _LCD_DIN; /**< Data GPIO ,Software SPI only */

	uint8_t  _contrast;     /**< LCD contrast */
	uint8_t  _bias;         /**< LCD bias*/
	bool _inverse = false;  /**< LCD inverted , false for off*/
	bool _sleep;            /**< LCD sleep mode*/
	
	int16_t _LCD_HEIGHT = 84;
	int16_t _LCD_WIDTH =  48;

	const uint16_t _LCD_Display_size = 504;// 504 = LCDWIDTH*LCDHEIGHT / 8
	uint8_t LCDDisplayBuffer[504]; /**< pointer to buffer which holds screen data */
}; //end of class


