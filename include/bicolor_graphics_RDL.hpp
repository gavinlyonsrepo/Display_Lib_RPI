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


/*! @brief Graphics class to hold graphic related functions  for 1-bit displays*/
class bicolor_graphics : public display_Fonts, public Print{

 public:

	bicolor_graphics(int16_t w, int16_t h); // Constructor
	
	/*! Enum to hold current screen rotation in degrees bi color display  */
	enum displayBC_rotate_e : uint8_t
	{
		BC_Degrees_0 =   0,    /**< display screen rotated 0 degrees */
		BC_Degrees_90 =  1,    /**< display screen rotated 90 degrees  */
		BC_Degrees_180 = 2,    /**< display screen rotated 180 degrees  */
		BC_Degrees_270 = 3     /**< display screen rotated 270 degrees */
	};

	/*! Pixel color */
	enum pixel_color_e : uint8_t 
	{
		WHITE   = 0, /**< pixel color for background */
		BLACK   = 1, /**< pixel color for foreground*/
		INVERSE = 2 /**< inverted pixel color */
	};
	
	// Graphic related member functions
	/*! @brief define in the sub class */
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;

	rdlib::Return_Codes_e drawDotGrid(int16_t x, int16_t y, int16_t w, 
		int16_t h, uint8_t DotGridGap, uint8_t color);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
	rdlib::Return_Codes_e drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
	rdlib::Return_Codes_e drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
	void drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, 
		uint8_t length, int offset, uint8_t color);
	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillScreen(uint8_t color);
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void drawEllipse(int16_t cx, int16_t cy, int16_t semiMajorAxis, 
		int16_t semiMinorAxis, bool fill, uint8_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color);
	void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color);
	void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color);
	void drawQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, 
		int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color);
	void fillQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, 
		int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color,bool useTriangleSplit=true);
	rdlib::Return_Codes_e drawPolygon(int16_t x, int16_t y, uint8_t sides, 
		int16_t diameter, float rotation, bool fill , uint8_t color);
	void drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, 
		float startAngle, float endAngle, uint8_t color);
	void drawSimpleArc(int16_t cx, int16_t cy, int16_t radius, float startAngle, 
		float endAngle, uint8_t color);
	float getArcAngleMax() const;
	void setArcAngleMax(float arcAngleMax);
	int getArcAngleOffset() const;
	void setArcAngleOffset(int arcAngleOffset);

	// Screen related member functions
	void setCursor(int16_t x, int16_t y);
	void setRotation(displayBC_rotate_e  m);
	displayBC_rotate_e  getRotation();
	int16_t height(void) const;
	int16_t width(void) const;

	// Text related functions 
	virtual size_t write(uint8_t) override;
	rdlib::Return_Codes_e writeChar( int16_t x, int16_t y, char value );
	rdlib::Return_Codes_e writeCharString( int16_t x, int16_t y, char *text);
	void setTextWrap(bool w);
	
	// Bitmap related
	void setDrawBitmapAddr(bool mode);
	rdlib::Return_Codes_e drawBitmap(int16_t x, int16_t y, const std::span<const uint8_t> bitmap,
			int16_t w, int16_t h, uint8_t color, uint8_t bg);

protected:

	displayBC_rotate_e  _display_rotate = BC_Degrees_0; /**< Enum to hold rotation */

	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT; /**< This is the 'raw' display h - never changes*/
	int16_t _width;       /**< Display w as modified by current _rotation*/
	int16_t _height;      /**< Display h as modified by current _rotation*/
	int16_t _cursor_x;    /**< Current X co-ord cursor position */
	int16_t _cursor_y;    /**< Current Y co-ord cursor position */
	float _arcAngleMax = 360.0f; /**< Maximum angle of Arc , used by drawArc*/
	int _arcAngleOffset= 0; /**< used by drawArc, offset for adjusting the starting angle of arc. default positive X-axis (0°)*/
	bool _textwrap = true;  /**< If set, text at right edge of display will wrap, print method*/

private:
	bool _drawBitmapAddr; /**< data addressing mode for method drawBitmap, True-vertical , false-horizontal */

	/// @cond
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint8_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t color);
	void ellipseHelper(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint8_t color);
	void drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint8_t color);
	inline void swapint16t(int16_t& a, int16_t& b) 
	{
		int16_t t = a;
		a = b;
		b = t;
	}
	/// @endcond
};
