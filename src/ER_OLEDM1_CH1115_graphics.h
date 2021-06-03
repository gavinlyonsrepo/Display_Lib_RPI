/*
* Project Name: ER_OLEDM1_CH1115
* File: ER_OLEDM1_CH1115_graphics.h
* Description: ER_OLEDM1 OLED driven by CH1115controller header file for the custom graphics functions based on Adafruit graphics library
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
*/

#ifndef _ER_OLEDM1_CH1115_GRAPHICS_H
#define _ER_OLEDM1_CH1115_GRAPHICS_H

#include "ER_OLEDM1_CH1115_Print.h"
#include <stdio.h>
#include <stdint.h>
#define swap(a, b) { int16_t t = a; a = b; b = t; }

class ERMCH1115_graphics : public Print{

 public:

	ERMCH1115_graphics(int16_t w, int16_t h); // Constructor

	// This defined by the subclass:
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;

	virtual void
	drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
	drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
	drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
	drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
	fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
	fillScreen(uint16_t color);

	void
	drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
	drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint16_t color),
	fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
	fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		int16_t delta, uint16_t color),
	drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color),
	fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color),
	drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint16_t color),
	fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint16_t color),
	drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
		uint16_t bg, uint8_t size),
	setCursor(int16_t x, int16_t y),
	setTextColor(uint16_t c),
	setTextColor(uint16_t c, uint16_t bg),
	setTextSize(uint8_t s),
	setTextWrap(bool w);

	virtual size_t write(uint8_t);

	void SetFontNum(uint8_t FontNumber);
	void drawCharBigNum(uint8_t x, uint8_t y, uint8_t c, uint8_t color , uint8_t bg);
	void drawTextBigNum(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg);
	int16_t height(void) const;
	int16_t width(void) const;

 protected:
	const int16_t
	WIDTH, HEIGHT;
	int16_t
	_width, _height,
	cursor_x, cursor_y;
	uint16_t
	textcolor, textbgcolor;
	uint8_t textsize;

	bool wrap; // If set, 'wrap' text at right edge of display
	uint8_t _FontNumber = 1;
	uint8_t _CurrentFontWidth = 5;
	uint8_t _CurrentFontoffset = 0;
	uint8_t _CurrentFontheight = 8;
};

#endif
