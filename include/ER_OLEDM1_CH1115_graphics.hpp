 /*!
	@file ER_OLEDM1_CH1115_graphics.hpp
	@brief ER_OLEDM1 OLED driven by CH1115 controller header file
	for the graphics  based functions. Project Name: ER_OLEDM1_CH1115_RPI
	 URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
	@author  Gavin Lyons
*/

#pragma once

#include "ER_OLEDM1_CH1115_Print.hpp"
#include "ER_OLEDM1_CH1115_font.hpp"
#include <cstdio>
#include <cstdint>

#define Ch1115_OLEDSwap(a, b) { int16_t t = a; a = b; b = t; }

/*! @brief Graphics class to hold graphic related functions */
class ERMCH1115_graphics : public Print{

 public:

	ERMCH1115_graphics(int16_t w, int16_t h); // Constructor

	// This defined by the subclass:
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;

	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillScreen(uint8_t color);

	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint8_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		int16_t delta, uint8_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color);
	void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color);
	void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color);
	void setCursor(int16_t x, int16_t y);


	void setDrawBitmapAddr(bool mode);
	bool drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
			int16_t w, int16_t h, uint8_t color, uint8_t bg, uint16_t sizeOfBitmap);

	virtual size_t write(uint8_t);
	void setTextColor(uint8_t c);
	void setTextColor(uint8_t c, uint8_t bg);
	void setTextSize(uint8_t s);
	void setTextWrap(bool w);
	void setFontNum(OLEDFontType_e FontNumber);
	bool drawCharBigFont(uint8_t x, uint8_t y, uint8_t c, uint8_t color , uint8_t bg);
	bool drawTextBigFont(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg);
	bool drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);
	bool drawText(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg, uint8_t size);

	int16_t height(void) const;
	int16_t width(void) const;

 protected:
	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT; /**< This is the 'raw' display h - never changes*/
	int16_t  _width;      /**< Display w as modified by current rotation*/
	int16_t  _height;     /**< Display h as modified by current rotation*/
	int16_t _cursorX;     /**< Current X co-ord cursor position */
	int16_t _cursorY;     /**< Current Y co-ord cursor position */

	uint8_t _textColor;   /**< Text foreground color */
	uint8_t _textBgColor; /**< Text background color */
	uint8_t _textSize;    /**< Size of text ,fonts 1-6 */

	bool _textWrap;       /**< If set, 'wrap' text at right edge of display*/
	bool _drawBitmapAddr; /**< data addressing mode for method drawBitmap, True-vertical , false-horizontal */

	uint8_t _FontNumber = OLEDFontType_Default;         /**< Holds current font number */
	uint8_t _CurrentFontWidth = OLEDFontWidth_5;        /**<Holds current font width in bits */
	uint8_t _CurrentFontoffset = OLEDFontOffset_Extend; /**<Holds current font ASCII table offset */
	uint8_t _CurrentFontheight = OLEDFontHeight_8;      /**<Holds current font height in bits */
	uint8_t _CurrentFontLength = OLEDFontLenAll;        /**<Holds current font number of characters */
};


