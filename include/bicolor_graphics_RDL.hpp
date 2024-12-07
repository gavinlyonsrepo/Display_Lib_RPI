/*!
	@file bicolor_graphics_RDL.hpp
	@brief Graphics based functions for bicolor display
	@details Project Name: Display_Lib_RPI
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@author  Gavin Lyons
*/

#pragma once

#include <cstdio>
#include <cstdint>
#include <cmath> // for "abs"

#include "print_data_RDL.hpp"
#include "font_data_RDL.hpp"
#include "common_data_RDL.hpp"

#define swapDisplayRPI(a, b) { int16_t t = a; a = b; b = t; } /**< Swap two int16_t variables marco */

// Pixel color
#define RDL_WHITE   0 /**< pixel color for background */
#define RDL_BLACK   1 /**< pixel color for foreground*/
#define RDL_INVERSE 2 /**< inverted pixel color */

/*! Enum to hold current screen rotation in degrees bi color display  */
enum displayBC_rotate_e : uint8_t
{
	displayBC_Degrees_0 =   0,    /**< display screen rotated 0 degrees */
	displayBC_Degrees_90 =  1,    /**< display screen rotated 90 degrees  */
	displayBC_Degrees_180 = 2,    /**< display screen rotated 180 degrees  */
	displayBC_Degrees_270 = 3     /**< display screen rotated 270 degrees */
};

/*! @brief Graphics class to hold graphic related functions  for 1-bit displays*/
class bicolor_graphics : public display_Fonts, public Print{

 public:

	bicolor_graphics(int16_t w, int16_t h); // Constructor

	// Graphic related member functions
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

	// Screen related member functions
	void setCursor(int16_t x, int16_t y);
	void setRotation(displayBC_rotate_e  m);
	displayBC_rotate_e  getRotation();
	int16_t height(void) const;
	int16_t width(void) const;

	// Text related functions 
	virtual size_t write(uint8_t) override;
	rpiDisplay_Return_Codes_e writeChar( int16_t x, int16_t y, char value );
	rpiDisplay_Return_Codes_e writeCharString( int16_t x, int16_t y, char *text);
	void setTextWrap(bool w);
	
	// Bitmap related
	void setDrawBitmapAddr(bool mode);
	rpiDisplay_Return_Codes_e drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
			int16_t w, int16_t h, uint8_t color, uint8_t bg, uint16_t sizeOfBitmap);

protected:

	displayBC_rotate_e  _display_rotate = displayBC_Degrees_0; /**< Enum to hold rotation */

	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT; /**< This is the 'raw' display h - never changes*/
	int16_t _width;       /**< Display w as modified by current _rotation*/
	int16_t _height;      /**< Display h as modified by current _rotation*/
	int16_t _cursor_x;    /**< Current X co-ord cursor position */
	int16_t _cursor_y;    /**< Current Y co-ord cursor position */

	bool _textwrap = true;  /**< If set, text at right edge of display will wrap, print method*/

private:
	bool _drawBitmapAddr; /**< data addressing mode for method drawBitmap, True-vertical , false-horizontal */
};
