/*!
	@file     color16_graphics_RDL.hpp
	@author   Gavin Lyons
	@brief    Library header file for 16 bit (565) color graphics library.
				This file handles the graphic methods
*/

#pragma once

// Includes
#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include <bcm2835.h> // Dependency
#include "print_data_RDL.hpp"
#include "font_data_RDL.hpp"
#include "common_data_RDL.hpp"

// defines 
#define _swap_int16_t_RDL(a, b) { int16_t t; t = a; a = b; b = t;}

// GPIO abstraction 
#define Display_DC_SetHigh  bcm2835_gpio_write(_Display_DC, HIGH)
#define Display_DC_SetLow  bcm2835_gpio_write(_Display_DC, LOW)
#define Display_RST_SetHigh  bcm2835_gpio_write(_Display_RST, HIGH)
#define Display_RST_SetLow  bcm2835_gpio_write(_Display_RST, LOW)
#define Display_CS_SetHigh bcm2835_gpio_write(_Display_CS, HIGH)
#define Display_CS_SetLow bcm2835_gpio_write(_Display_CS, LOW)
#define Display_SCLK_SetHigh bcm2835_gpio_write(_Display_SCLK, HIGH)
#define Display_SCLK_SetLow  bcm2835_gpio_write(_Display_SCLK, LOW)
#define Display_SDATA_SetHigh bcm2835_gpio_write(_Display_SDATA, HIGH)
#define Display_SDATA_SetLow  bcm2835_gpio_write(_Display_SDATA,LOW)

#define Display_DC_SetDigitalOutput bcm2835_gpio_fsel(_Display_DC, BCM2835_GPIO_FSEL_OUTP)
#define Display_RST_SetDigitalOutput bcm2835_gpio_fsel(_Display_RST, BCM2835_GPIO_FSEL_OUTP)
#define Display_SCLK_SetDigitalOutput bcm2835_gpio_fsel(_Display_SCLK, BCM2835_GPIO_FSEL_OUTP)
#define Display_SDATA_SetDigitalOutput bcm2835_gpio_fsel(_Display_SDATA, BCM2835_GPIO_FSEL_OUTP)
#define Display_CS_SetDigitalOutput bcm2835_gpio_fsel(_Display_CS, BCM2835_GPIO_FSEL_OUTP)

// Color definitions 16-Bit Color Values R5G6B5
#define   RDLC_BLACK   0x0000
#define   RDLC_BLUE    0x001F
#define   RDLC_RED     0xF800
#define   RDLC_GREEN   0x07E0
#define   RDLC_CYAN    0x07FF
#define   RDLC_MAGENTA 0xF81F
#define   RDLC_YELLOW  0xFFE0
#define   RDLC_WHITE   0xFFFF
#define   RDLC_TAN     0xED01
#define   RDLC_GREY    0x9CD1
#define   RDLC_BROWN   0x6201
#define   RDLC_DGREEN  0x01c0
#define   RDLC_ORANGE  0xFC00

/*!
	@brief Class to handle fonts and graphics of color 16 bit display
*/
class color16_graphics:public display_Fonts, public Print  {

 public:

	color16_graphics(); // Constructor
	~color16_graphics(){};
	
	// Screen related
	// Defined in the display sub class
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) = 0;
	void fillScreen(uint16_t color);
	void setCursor(int16_t x, int16_t y);
	
	// Shapes and lines
	void drawPixel(uint16_t, uint16_t, uint16_t);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	rpiDisplay_Return_Codes_e drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
	rpiDisplay_Return_Codes_e drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

	void drawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	rpiDisplay_Return_Codes_e fillRectangle(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
	void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

	void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
	void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
	
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

	// Text related functions 
	virtual size_t write(uint8_t) override;
	rpiDisplay_Return_Codes_e writeChar( int16_t x, int16_t y, char value );
	rpiDisplay_Return_Codes_e writeCharString( int16_t x, int16_t y, char *text);
	void setTextWrap(bool w);
	void setTextColor(uint16_t c, uint16_t bg);
	void setTextColor(uint16_t c);
	
	// Bitmap & Icon
	rpiDisplay_Return_Codes_e drawIcon(uint16_t x, uint16_t y, uint16_t w, uint16_t color, uint16_t bgcolor, const unsigned char character[]);
	rpiDisplay_Return_Codes_e drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t bgcolor, const uint8_t *pBmp);
	rpiDisplay_Return_Codes_e drawBitmap24(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h);
	rpiDisplay_Return_Codes_e drawBitmap16(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h);

protected:

	void pushColor(uint16_t color);
	int16_t Color565(int16_t ,int16_t , int16_t );
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
	
	void writeCommand(uint8_t);
	void writeData(uint8_t);
	void spiWrite(uint8_t);
	void spiWriteSoftware(uint8_t spidata);
	void spiWriteDataBuffer(uint8_t* spidata, uint32_t len);

	bool _hardwareSPI=true; /**< True for Hardware SPI on , false fpr Software SPI on*/
	
	bool _textwrap = true;              /**< wrap text around the screen on overflow*/
	uint16_t _textcolor = 0xFFFF ;   /**< 16 bit ForeGround color for text*/
	uint16_t _textbgcolor =0x0000 ; /**< 16 bit BackGround color for text*/

	int16_t _cursorX = 0; /**< Current X co-ord cursor position */
	int16_t _cursorY = 0; /**< Current Y co-ord cursor position */
	uint16_t _width;   /**< Display w as modified by current rotation*/
	uint16_t _height; /**< Display h as modified by current rotation*/
	uint8_t _XStart;     /**< Used to store _colstart changed by current rotation */
	uint8_t _YStart;     /**< Used to store _rowstart changed by current rotation */

	int8_t _Display_DC;    /**< GPIO for data or command line */
	int8_t _Display_RST;   /**< GPIO for reset line */
	int8_t _Display_CS;    /**< GPIO for chip select line,  Software SPI only */
	int8_t _Display_SCLK;  /**< GPIO for Clock line,  Software SPI only */
	int8_t _Display_SDATA; /**< GPIO for data in line,  Software SPI only */

	uint16_t _HighFreqDelay = 0; /**< uS GPIO Communications delay, SW SPI ONLY */

private:

};
// ********************** EOF *********************
