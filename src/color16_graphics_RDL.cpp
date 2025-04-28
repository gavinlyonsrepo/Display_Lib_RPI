/*!
	@file     color16_graphics_RDL.cpp
	@author   Gavin Lyons
	@brief    Library source file for 16 bit (565) color graphics library.
				This file handles the graphic methods
*/

#include "../include/color16_graphics_RDL.hpp"


/*!
	@brief Construct a new graphics class object
*/
color16_graphics::color16_graphics(){}

/*!
	@brief Draw a pixel to screen
	@param x  Column co-ord
	@param y  row co-ord
	@param color 565 16-bit
*/
void color16_graphics::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	if ((x >= _width) || (y >= _height))
		return;
	setAddrWindow(x, y, x + 1, y + 1);
	uint8_t TransmitBuffer[2] {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
	spiWriteDataBuffer(TransmitBuffer, 2);
}


/*!
	@brief fills a rectangle starting from coordinates (x,y) with width of w and height of h.
	@param x x coordinate
	@param y y coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 565 16-bit
	@note  uses spiWriteDataBuffer method
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::ShapeScreenBounds Error
		-# rdlib::MemoryAError Error
*/
rdlib::Return_Codes_e color16_graphics::fillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) 
{
	uint8_t hi, lo;

	// Check bounds
	if ((x >= _width) || (y >= _height)) {
		fprintf(stderr, "Error: fillRectangle 2: Out of screen bounds\n");
		return rdlib::ShapeScreenBounds;
	}
	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;

	// Colour to bytes
	hi = color >> 8;
	lo = color;

	// Create bitmap buffer using std::vector
	std::vector<uint8_t> buffer;
	try
	{
		// Allocate buffer
		buffer = std::vector<uint8_t>(w * h * sizeof(uint16_t));
	} catch (const std::bad_alloc&) {
		fprintf(stderr, "Error: fillRectangle 3: Memory allocation failed\n");
		rdlib_log::logData< int> error("Memory allocation failed", static_cast<int>(w * h * sizeof(uint16_t)));
		return rdlib::MemoryAError;
	}
	
	for (size_t i = 0; i < buffer.size(); ) 
	{
		buffer[i++] = hi;
		buffer[i++] = lo;
	}
	

	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer.data(), buffer.size());

	return rdlib::Success;
}

/*!
	@brief Fills the whole screen with a given color.
	@param  color 565 16-bit
*/
void color16_graphics::fillScreen(uint16_t color) {
	fillRectangle(0, 0, _width, _height, color);
}

/*!
	@brief Draws a vertical line starting at (x,y) with height h.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param h The height of the line
	@param color The color of the line 565 16 Bit color
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::ShapeScreenBounds Error
*/
rdlib::Return_Codes_e color16_graphics::drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color) {
	uint8_t hi, lo;
	if ((x >= _width) || (y >= _height))
		return rdlib::ShapeScreenBounds;
	if ((y + h - 1) >= _height)
		h = _height - y;
	hi = color >> 8;
	lo = color;
	setAddrWindow(x, y, x, y + h - 1);
	Display_DC_SetHigh;

	if (_hardwareSPI == false){Display_CS_SetLow;}
	while (h--) {
		spiWrite(hi);
		spiWrite(lo);
	}
	if (_hardwareSPI == false){Display_CS_SetHigh;}
	return rdlib::Success;
}

/*!
	@brief Draws a horizontal line starting at (x,y) with width w.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param w The width of the line
	@param color The color of the line 565 16 Bit color
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::ShapeScreenBounds Error
*/
rdlib::Return_Codes_e color16_graphics::drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color) {
	uint8_t hi, lo;
	if ((x >= _width) || (y >= _height))
		return rdlib::ShapeScreenBounds;
	if ((x + w - 1) >= _width)
		w = _width - x;
	hi = color >> 8;
	lo = color;
	setAddrWindow(x, y, x + w - 1, y);
	Display_DC_SetHigh;
	if (_hardwareSPI == false){Display_CS_SetLow;}
	while (w--) {
		spiWrite(hi);
		spiWrite(lo);
	}
	if (_hardwareSPI == false){Display_CS_SetHigh;}
	return rdlib::Success;
}

/*!
	@brief draws a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color The color of the circle , 565 16 Bit color
*/
void color16_graphics::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

/// @cond

/*!
	@brief Used internally by drawRoundRect
*/
void color16_graphics::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

/*!
	@brief Used internally by fill circle fillRoundRect and fillcircle
*/
void color16_graphics::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}

/// @endcond

/*!
	@brief fills a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color color of the circle , 565 16 Bit color
*/
void color16_graphics ::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

/*!
	@brief draws rectangle at (x,y) where h is height and w is width of the rectangle.
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 565 16-bit
*/
void color16_graphics::drawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y + h - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w - 1, y, h, color);
}

/*!
	@brief draws a line from (x0,y0) to (x1,y1).
	@param x0 x start coordinate
	@param y0 y start coordinate
	@param x1 x end coordinate
	@param y1 y end coordinate
	@param color color to fill  rectangle 565 16-bit
*/
void color16_graphics::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
	int16_t steep, dx, dy, err, ystep;
	steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swapint16t(x0, y0);
		swapint16t(x1, y1);
	}
	if (x0 > x1) {
		swapint16t(x0, x1);
		swapint16t(y0, y1);
	}
	dx = x1 - x0;
	dy = abs(y1 - y0);

	err = dx / 2;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

/*!
	@brief fills a rectangle at (x,y) where h is height and w is width of the rectangle.
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 565 16-bit
*/
void color16_graphics ::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	int16_t i;
	for (i = x; i < x + w; i++) {
		drawFastVLine(i, y, h, color);
	}
}

/*!
	@brief draws a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r r: radius of the rounded edges
	@param color color to fill  rectangle 565 16-bit
*/
void color16_graphics::drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color) {
	drawFastHLine(x + r, y, w - 2 * r, color);
	drawFastHLine(x + r, y + h - 1, w - 2 * r, color);
	drawFastVLine(x, y + r, h - 2 * r, color);
	drawFastVLine(x + w - 1, y + r, h - 2 * r, color);
	drawCircleHelper(x + r, y + r, r, 1, color);
	drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/*!
	@brief Fills a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r r: radius of the rounded edges
	@param color color to fill  rectangle 565 16-bit
*/
void color16_graphics::fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color) {
	fillRect(x + r, y, w - 2 * r, h, color);
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}


/*!
	@brief draws a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).
	@param x0 x start coordinate point 1
	@param y0 y start coordinate point 1
	@param x1 x start coordinate point 2
	@param y1 y start coordinate point 2
	@param x2 x start coordinate point 3
	@param y2 y start coordinate point 3
	@param color color to draw triangle 565 16-bit
*/
void color16_graphics ::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

/*!
	@brief Fills a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).
	@param x0 x start coordinate point 1
	@param y0 y start coordinate point 1
	@param x1 x start coordinate point 2
	@param y1 y start coordinate point 2
	@param x2 x start coordinate point 3
	@param y2 y start coordinate point 3
	@param color color to fill , 565 16-bit
*/
void color16_graphics::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12;
	int32_t sa = 0, sb = 0;
	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		swapint16t(y0, y1);
		swapint16t(x0, x1);
	}
	if (y1 > y2) {
		swapint16t(y2, y1);
		swapint16t(x2, x1);
	}
	if (y0 > y1) {
		swapint16t(y0, y1);
		swapint16t(x0, x1);
	}
	if (y0 == y2) {
		a = b = x0;
		if (x1 < a) a = x1;
		else if (x1 > b) b = x1;
		if (x2 < a) a = x2;
		else if (x2 > b) b = x2;
		drawFastHLine(a, y0, b - a + 1, color);
		return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	sa = 0;
	sb = 0;
	if (y1 == y2) last = y1;
	else last = y1 - 1;
	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b) swapint16t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b) swapint16t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}
}

/*!
	@brief Draws an custom Icon of X by 8 size to screen , where X = 0 to 127
	@param x X coordinate
	@param y Y coordinate
	@param w 0-MAX_Y possible values width of icon in pixels , height is fixed at 8 pixels
	@param color icon foreground colors ,is bi-color
	@param backcolor icon background colors ,is bi-color
	@param character icon data vertically addressed.
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::BitmapScreenBounds Error
		-# rdlib::BitmapDataEmpty Error
*/
rdlib::Return_Codes_e color16_graphics::drawIcon(uint16_t x, uint16_t y, uint16_t w, uint16_t color, uint16_t backcolor, const std::span<const uint8_t> character) {
	// Out of screen bounds
	if ((x >= _width) || (y >= _height))
	{
		fprintf(stderr , "Error: drawIcon 2: Out of screen bounds\n");
		return rdlib::BitmapScreenBounds ;
	}
	// Check for null pointer
	if(character.empty())
	{
		fprintf(stderr ,"Error: drawIcon 3: Icon array is an empty object\n");
		return rdlib::BitmapDataEmpty;
	}
	uint8_t value;
	for (uint8_t byte = 0; byte < w; byte++)
	{
		for (uint8_t mybit = 0; mybit < 8; mybit++)
		{
			value = !!(character[byte]& (1 << mybit));
			if (value)
			{
				drawPixel(x + byte, y + mybit, backcolor);
			} else
			{
				drawPixel(x + byte, y + mybit, color);
			}
			value = 0;
		}
	}
	return rdlib::Success;
}

/*!
	@brief: Draws an bi-color bitmap to screen
	@param x X coordinate
	@param y Y coordinate
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@param color bitmap foreground colors ,is bi-color
	@param bgcolor bitmap background colors ,is bi-color
	@param bitmap  bitmap data, horizontally addressed.
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::BitmapScreenBounds Error
		-# rdlib::BitmapDataEmpty Error
		-# rdlib::BitmapHorizontalSize Error
		-# rdlib::MemoryAError Error
*/
rdlib::Return_Codes_e color16_graphics::drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t bgcolor, const std::span<const uint8_t> bitmap) {
	int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	uint16_t mycolor = 0;
	uint32_t ptr;

	if( bitmap.empty()) //  Check for empty bitmap
	{
		fprintf(stderr, "Error: drawBitmap 1: Bitmap span is empty\n" );
		return rdlib::BitmapDataEmpty;
	}
	if(w % 8 != 0) 	// check horizontal size
	{
		fprintf(stderr, "Error: drawBitmap 2 : Horizontal Bitmap size is incorrect: %u : Width must be divisible by 8 \n", w );
		return rdlib::BitmapHorizontalSize;
	}
	if ((x >= _width) || (y >= _height)) // Check bounds
	{
		fprintf(stderr, "Error: drawBitmap 3: Out of screen bounds, check x & y\n" );
		return rdlib::BitmapScreenBounds;
	}

	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;

	// Create bitmap buffer
	std::vector<uint8_t> buffer;
	try 
	{
		// Allocate buffer
		buffer = std::vector<uint8_t>(w * h * 2);
	} catch (const std::bad_alloc&) {
		fprintf(stderr, "Error: drawBitmap 3: Memory allocation failed \n" );
		rdlib_log::logData< int> error("Memory allocation failed", static_cast<int>(w * h * 2));
		return  rdlib::MemoryAError;
	}

	ptr = 0;
	for (uint16_t j = 0; j < h; j++)
	{
		for (uint16_t i = 0; i < w; i++)
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = (bitmap[j * byteWidth + i / 8]);
			mycolor = (byte & 0x80) ? color : bgcolor;
			buffer[ptr++] = mycolor >> 8;
			buffer[ptr++] = mycolor;
		}
	}
	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer.data(), buffer.size());

	return rdlib::Success;
}

/*!
	@brief Draws an 24 bit color bitmap to screen from a data array
	@param x X coordinate
	@param y Y coordinate
	@param bitmap A span to the data buffer containing Bitmap data
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::BitmapScreenBounds Error
		-# rdlib::BitmapDataEmpty Error
		-# rdlib::MemoryAError Error
	@note 24 bit color converted to 16 bit color
*/
rdlib::Return_Codes_e  color16_graphics::drawBitmap24(uint16_t x, uint16_t y, const std::span<const uint8_t> bitmap, uint16_t w, uint16_t h)
{
	uint16_t i, j;
	uint16_t color;
	uint32_t rgb, ptr;
	// 1. Check for null pointer
	if( bitmap.empty())
	{
		fprintf(stderr, "Error: drawBitmap24 1: Bitmap array is an empty object\n");
		return rdlib::BitmapDataEmpty;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		fprintf(stderr, "Error: drawBitmap24 2: Out of screen bounds\n");
		return rdlib::BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;
	// Create bitmap buffer
	std::vector<uint8_t> buffer;
	try
	{
		// Allocate buffer
		buffer = std::vector<uint8_t>(w * h * 2);
	} catch (const std::bad_alloc&) 
	{
		fprintf(stderr, "Error:  drawBitmap24  3: Memory allocation failed\n");
		rdlib_log::logData< int> error("Memory allocation failed", static_cast<int>(w * h * 2));
		return rdlib::MemoryAError;
	}

	ptr = 0;
	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w ; i ++)
		{
			// Translate RBG24 to RGB565 bitmap
			rgb = *reinterpret_cast<const uint32_t*>(&bitmap[i * 3 + (h - 1 - j) * 3 * w]);
			color = Color565(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF), (rgb & 0xFF));
			buffer[ptr++] = color >> 8;
			buffer[ptr++] = color;
		}
	}

	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer.data(), buffer.size());

	return rdlib::Success;
}


/*!
	@brief: Draws an 16 bit color bitmap to screen from a data array
	@param x X coordinate
	@param y Y coordinate
	@param bitmap databuffer containing Bitmap data
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::BitmapScreenBounds Error
		-# rdlib::BitmapDataEmpty Error
		-# rdlib::MemoryAError Error
*/
rdlib::Return_Codes_e  color16_graphics::drawBitmap16(uint16_t x, uint16_t y, const std::span<const uint8_t> bitmap, uint16_t w, uint16_t h) {
	uint16_t i, j;
	uint16_t color;
	uint32_t ptr;
	// 1. Check for null pointer
	if( bitmap.empty())
	{
		fprintf(stderr, "Error: drawBitmap16 1: Bitmap array is an empty object\n" );
		return rdlib::BitmapDataEmpty;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		fprintf(stderr, "Error: drawBitmap16 2: Out of screen bounds\n" );
		return rdlib::BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;

	// Create bitmap buffer
	std::vector<uint8_t> buffer;
	try 
	{
		// Allocate buffer
		buffer = std::vector<uint8_t>(w * h * 2);
	} catch (const std::bad_alloc&) 
	{
		fprintf(stderr ,"Error:  drawBitmap16  3: Memory allocation failed\n");
		rdlib_log::logData< int> error("Memory allocation failed", static_cast<int>(w * h * 2));
		return rdlib::MemoryAError;
	}
	ptr = 0;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i ++)
		{
			color = *reinterpret_cast<const uint16_t*>(&bitmap[i * 2 + (h - 1 - j) * 2 * w]);
			buffer[ptr++] = color >> 8;
			buffer[ptr++] = color;
		}
	}

	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer.data(), buffer.size());

	return rdlib::Success;
}

/*!
	@brief: Convert: 24-bit color to 565 16-bit color
	@param r color red
	@param g color green
	@param b color blue
	@return 16 bit color value
	@note 3 byte to 2 byte,  Red Green Blue.
			RRRR RRRR GGGG GGGG BBBB BBBB => 565 color mode => RRRRR GGGGGG BBBBB
*/
int16_t color16_graphics::Color565(int16_t r, int16_t g, int16_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/*!
 * @brief pushColor
 * @param color
 */
void color16_graphics::pushColor(uint16_t color) {
	uint8_t hi, lo;
	hi = color >> 8;
	lo = color;
	Display_DC_SetHigh;
	if (_hardwareSPI == false){Display_CS_SetLow;}
	spiWrite(hi);
	spiWrite(lo);
	if (_hardwareSPI == false){Display_CS_SetHigh;}
}

/*!
	@brief : Write an SPI command to 
	@param spicmdbyte command byte to send
*/
void color16_graphics::writeCommand(uint8_t spicmdbyte) {
	Display_DC_SetLow;
	if (_hardwareSPI == false){Display_CS_SetLow;}
	spiWrite(spicmdbyte);
	if (_hardwareSPI == false){Display_CS_SetHigh;}
}

/*!
	@brief Write an SPI data byte to device
	@param spidatabyte byte to send
*/
void color16_graphics::writeData(uint8_t spidatabyte) {
	Display_DC_SetHigh;
	if (_hardwareSPI == false){Display_CS_SetLow;}
	spiWrite(spidatabyte);
	if (_hardwareSPI == false){Display_CS_SetHigh;}
}

/*!
	@brief  Write a buffer to SPI, both Software and hardware SPI supported
	@param spidata to send
	@param len length of buffer
	@note The maximum size of an SPI transaction by default in lgpio library
	is 65536 or _Display_SPI_BLK_SIZE. SO a buffer larger than this
	must be sent in blocks of 65536 bytes. 
	The largest current expected size is for a full screen write to a 240x320 display currently
	which is 240x320x2 = 153600 bytes. spidev.bufsiz must also be set to 65536 or higher see 
	Readme for display for more details.
*/
void color16_graphics::spiWriteDataBuffer(uint8_t* spidata, int len) {
	Display_DC_SetHigh;
	if (_hardwareSPI == false) 
	{
		Display_CS_SetLow;
		for(int i=0; i<len; i++) {spiWrite(spidata[i]);}
		Display_CS_SetHigh;
	} else 
	{
		int spiErrorStatus = 0;
		if (len >= 1) 
		{ // Remove the upper limit constraint
			int remainingLen = len;
			const uint8_t* currentData = spidata;

			while (remainingLen > 0) 
			{
				// Determine how much to write in this iteration
				int writeSize = (remainingLen > _Display_SPI_BLK_SIZE) ? _Display_SPI_BLK_SIZE : remainingLen;

				// Perform SPI write
				spiErrorStatus = Display_SPI_WRITE(_spiHandle, reinterpret_cast<const char*>(currentData), writeSize);
				if (spiErrorStatus < 0) {
					fprintf(stderr, "Error: spiWriteDataBuffer: Failure to Write SPI :(%s)\n", lguErrorText(spiErrorStatus));
					fprintf(stderr, "The problem maybe: The spidev buf size setting must be set at 65536 bytes or higher.\n") ;
					fprintf(stderr, "See readme, note section, of relevant display at https://github.com/gavinlyonsrepo/Display_Lib_RPI for more details\n");
					fprintf(stderr, "spidev buf defines the number of bytes that the SPI driver will use as a buffer for data transfers.\n");
					break; // Exit loop on error
				}

				// Update pointers and counters
				currentData += writeSize;
				remainingLen -= writeSize;
			}
		} else 
		{
			fprintf(stderr, "Buffer wrong size to draw = %i. Allowed size = 1 or greater.\n", len);
		}
	}
}

/*!
	@brief  Write byte to SPI
	@param spidata byte to write
	@note uses _HIGHFREQ_DELAY to slowdown software SPI if CPU frequency too fast
*/
void color16_graphics::spiWrite(uint8_t spidata) {
	if (_hardwareSPI == false)
	{
		uint8_t i;
		for (i = 0; i < 8; i++)
		{
			Display_SDATA_SetLow;
			if (spidata & 0x80)
				Display_SDATA_SetHigh; // b1000000 Mask with 0 & all zeros out.
			Display_SCLK_SetHigh;
			delayMicroSecRDL(_HighFreqDelay);
			spidata <<= 1;
			Display_SCLK_SetLow;
			delayMicroSecRDL(_HighFreqDelay);
		}
	} else {
		int spiErrorStatus = 0;
		char TransmitBuffer[1];
		TransmitBuffer[0] =  spidata;
		spiErrorStatus = Display_SPI_WRITE( _spiHandle, static_cast<const char*>(TransmitBuffer), 1);
		if (spiErrorStatus <0) 
		{
			fprintf(stderr, "Error: spiWrite :Failure to Write  SPI :(%s)\n", lguErrorText(spiErrorStatus));
		}
	}
}



/*!
	@brief Set the Cursor Position on screen
	@param x the x co-ord of the cursor position
	@param y the y co-ord of the cursor position
*/
void color16_graphics::setCursor(int16_t x, int16_t y) {
	_cursorX = x;
	_cursorY = y;
}

/*!
	@brief Write 1 character on Display
	@param  x character starting position on x-axis. Valid values
	@param  y character starting position on x-axis. Valid values
	@param  value Character to be written.
	@note uses spiWriteDataBuffer method to write each character as a buffer for speed.
			Much faster than pixel by pixel spi byte writes
	@return Will return rdlib::Return_Codes_e enum
		-# rdlib::Success  success
		-# rdlib::CharScreenBounds co-ords out of bounds check x and y
		-# rdlib::CharFontASCIIRange Character out of ASCII Font bounds, check Font range
		-# rdlib::MemoryAError Could not assign memory for character buffer
 */
rdlib::Return_Codes_e color16_graphics::writeChar(int16_t x, int16_t y, char value) {

	// 1. Check for screen out of  bounds
	if((x >= _width)            || // Clip right
	(y >= _height)           || // Clip bottom
	((x + _Font_X_Size+1) < 0) || // Clip left
	((y + _Font_Y_Size) < 0))   // Clip top
	{
		fprintf(stderr, "Error 1: writeChar16 : Co-ordinates out of bounds\n");
		return rdlib::CharScreenBounds;
	}
	// 2. Check for character out of font range bounds
	if ( value < _FontOffset || value >= (_FontOffset + _FontNumChars+1))
	{
		fprintf(stderr, "Error 2: writeChar16 : Character out of Font bounds %c : %u  <--> %u \n",value, _FontOffset, (unsigned int)(_FontOffset + _FontNumChars));
		return rdlib::CharFontASCIIRange;
	}

	// Create bitmap buffer
	std::vector<uint8_t> buffer;
	try
	{
		// Allocate buffer
		buffer = std::vector<uint8_t>(_Font_X_Size * _Font_Y_Size * 2);
	} catch (const std::bad_alloc&) 
	{
		fprintf(stderr, "Error: writeChar16 3: Memory allocation failed\n");
		rdlib_log::logData< int> error("Memory allocation failed", static_cast<int>(_Font_X_Size * _Font_Y_Size * 2));
		return rdlib::MemoryAError;
	}

	uint16_t ltextcolor = 0; 
	uint16_t ltextbgcolor = 0; 
	if (getInvertFont()== true)
	{
		ltextbgcolor = _textcolor;
		ltextcolor = _textbgcolor;
	}else
	{
		ltextbgcolor = _textbgcolor;
		ltextcolor = _textcolor;
	}
	uint16_t fontIndex = 0;
	uint32_t bufferIndex = 0; // Index into the display buffer
	int16_t colByte, cx, cy;
	int16_t colbit;
	fontIndex = ((value - _FontOffset)*((_Font_X_Size * _Font_Y_Size) / 8)) + 4;
	colByte = _FontSelect[fontIndex];
	colbit = 7;
	for (cx = 0; cx < _Font_X_Size; cx++)
	{
		for (cy = 0; cy < _Font_Y_Size; cy++)
		{
			if ((colByte & (1 << colbit)) != 0) {
				buffer[bufferIndex++] = (ltextcolor >> 8) & 0xFF; // High byte
				buffer[bufferIndex++] = ltextcolor & 0xFF;    // Low byte
			} else {
				buffer[bufferIndex++] = (ltextbgcolor >> 8) & 0xFF; // High byte
				buffer[bufferIndex++] = ltextbgcolor & 0xFF;    // Low byte
			}
			colbit--;
			if (colbit < 0) {
				colbit = 7;
				fontIndex++;
				colByte = _FontSelect[fontIndex];
			}
		}
	}

	// Set window and write buffer
	setAddrWindow(x, y, x + _Font_X_Size - 1, y +_Font_Y_Size - 1);;
	spiWriteDataBuffer(buffer.data(), buffer.size());

	return rdlib::Success ;
}


/*!
	@brief Write Text character array on display
	@param  x character starting position on x-axis.
	@param  y character starting position on y-axis.
	@param  pText Pointer to the array of the text to be written.
	@return Will return
		-# rdlib::Success Success
		-# rdlib::CharArrayNullptr  String pText Array invalid pointer object
		-# Failure code from  writeChar method upstream
 */
rdlib::Return_Codes_e  color16_graphics::writeCharString(int16_t x, int16_t y, char * pText) {
	uint8_t count=0;
	uint8_t MaxLength=0;
	// Check for null pointer
	if(pText == nullptr)
	{
		fprintf(stderr ,"Error: writeCharString16 1 :String array is not valid pointer" );
		return rdlib::CharArrayNullptr;
	}
	rdlib::Return_Codes_e DrawCharReturnCode;
	while(*pText != '\0')
	{
		// check if text has reached end of screen
		if ((x + (count * _Font_X_Size)) > _width - _Font_X_Size)
		{
			y = y + _Font_Y_Size;
			x = 0;
			count = 0;
		}
		DrawCharReturnCode = writeChar(x + (count * (_Font_X_Size)), y, *pText++);
		if(DrawCharReturnCode  != rdlib::Success) return DrawCharReturnCode;
		count++;
		MaxLength++;
		if (MaxLength >= 250) break; // 2nd way out of loop, safety check
	}
	return rdlib::Success;
}

/*!
	@brief write method used in the print class when user calls print
	@param character the character to print
	@return Will return
		-# 1. success
		-# rdlib::Return_Codes_e enum error code,  An error in the writeChar method.upstream
*/
size_t color16_graphics::write(uint8_t character)
{
	rdlib::Return_Codes_e DrawCharReturnCode = rdlib::Success;;
	switch (character)
	{
		case '\n':
			_cursorY += _Font_Y_Size;
			_cursorX  = 0;
		break;
		case '\r': break;
		default:
			DrawCharReturnCode = writeChar(_cursorX, _cursorY, character);
			if (DrawCharReturnCode != rdlib::Success) 
			{
				// Set the write error based on the result of the drawing operation
				setWriteError(DrawCharReturnCode); // Set error flag to non-zero value}
				break;
			}
			_cursorX += (_Font_X_Size);
			if (_textwrap && (_cursorX  > (_width - (_Font_X_Size))))
			{
				_cursorY += _Font_Y_Size;
				_cursorX = 0;
			}
		break;
	} // end of switch

return 1;
}

/*!
	@brief Set text color foreground and background
	@param c text foreground color , Color definitions 16-Bit Color Values R5G6B5
	@param b text background color , Color definitions 16-Bit Color Values R5G6B5
*/
void color16_graphics::setTextColor(uint16_t c, uint16_t b) {
	_textcolor   = c;
	_textbgcolor = b;
}

/*!
	@brief Set text color foreground
	@param c text foreground color , Color definitions 16-Bit Color Values R5G6B5
*/
void color16_graphics::setTextColor(uint16_t c) {
	_textcolor   = c;
}

/*!
	@brief turn on or off screen wrap of the text (fonts 1-6)
	@param w TRUE on
*/
void color16_graphics::setTextWrap(bool w) {
	_textwrap = w;
}

/*!
	@brief: Draws an 16 bit color sprite bitmap to screen with transparent background
	@param x X coordinate
	@param y Y coordinate
	@param sprite span to sprite data
	@param w width of the sprite in pixels
	@param h height of the sprite in pixels
	@param backgroundColor the background color of sprite (16 bit 565) this will be made transparent
	@note  Does not use buffer, just draw pixel
	@return
		-# Success=success
		-# BitmapDataEmpty=invalid pointer object
		-# BitmapScreenBounds=Co-ordinates out of bounds
		-# BitmapSize=Sprite data out of bounds
*/
rdlib::Return_Codes_e  color16_graphics::drawSprite(uint16_t x, uint16_t y, const std::span<const uint8_t> sprite, uint16_t w, uint16_t h, uint16_t backgroundColor)
{
	uint8_t i, j;
	uint16_t colour;
	// 1. Check for null pointer
	if (sprite.empty())
	{
		fprintf(stderr, "Error: drawSprite 1: Sprite array is empty object\n");
		return rdlib::BitmapDataEmpty;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		fprintf(stderr, "Error: drawSprite 2: Sprite out of screen bounds\r\n");
		return rdlib::BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;

	size_t index = 0;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			// Ensure index does not exceed span size
			if (index + 1 >= sprite.size()) 
			{
				fprintf(stderr, "Error: drawSprite 3: Sprite data out of bounds (index: %zu, size: %zu)\n", index, sprite.size());
				return rdlib::BitmapSize;
			}
			// Read 16-bit color value from sprite
			colour = (sprite[index] << 8) | sprite[index + 1];
			index += 2;
			if (colour != backgroundColor)
			{
				drawPixel(x + i - 1, y + j - 1, colour);
			}
		}
	}
	return rdlib::Success;
}

/*!
	@brief Gets the maximum SPI transaction block size.
	@return The current block size of the SPI transaction.
*/
int color16_graphics::getDisplaySPIBlockSize() const {
	return _Display_SPI_BLK_SIZE;
}

/*!
	@brief Sets the maximum SPI transaction block size.
	@param size The new block size to set.
	@note Maximum block size supported by lg library 
			is 65536 see LG_MAX_SPI_DEVICE_COUNT in lg library. 
			Also your Computer may have limit on spi transaction size 
 */
void color16_graphics::setDisplaySPIBlockSize(int size) {
	_Display_SPI_BLK_SIZE = size;
}

/*!
	@brief Get the current maximum angle of the arc.
	@return The current maximum angle in degrees.
*/
float color16_graphics::getArcAngleMax() const {
	return _arcAngleMax;
}
/*!
	@brief Set a new maximum angle for the arc.
	@param arcAngleMax The new maximum angle in degrees (should be positive).
*/
void color16_graphics::setArcAngleMax(float arcAngleMax) {
	if (arcAngleMax > 0) {  // Ensure the max angle is positive
		_arcAngleMax = arcAngleMax;
	}
}
/*!
	@brief Get the current angle offset.
	@return The current angle offset in degrees.
*/
int color16_graphics::getArcAngleOffset() const {
	return _arcAngleOffset;
}
/*!
	@brief Set a new angle offset.
	@param arcAngleOffset The new angle offset in degrees.
*/
void color16_graphics::setArcAngleOffset(int arcAngleOffset) {
	_arcAngleOffset = arcAngleOffset;
}

/*!
	@brief Draws a polygon with a specified number of sides, diameter, rotation, and color.
	This function draws a regular polygon by connecting points equally spaced around a circle, 
	with each point having a distance defined by the diameter. The polygon is rotated by the given 
	angle (in degrees) before being drawn. The number of sides is enforced to be at least 3.
	@param x The x-coordinate of the center of the polygon.
	@param y The y-coordinate of the center of the polygon.
	@param sides The number of sides the polygon will have. Must be at least 3.
	@param diameter The diameter of the circle inscribed by the polygon.
	@param rotation The angle (in degrees) by which to rotate the polygon.
	@param fill if false draw ,if true fill
	@param color The color of the polygon edges.
	@returns error code  GenericError , if user inputs incorrect sides value 
 */
rdlib::Return_Codes_e color16_graphics::drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rotation, bool fill , uint16_t color) 
{
	if ((sides < 3 ) || (sides > 12)) {
		fprintf(stderr, "Error : drawPolygon : sides incorrect value 3-12\r\n");
		return rdlib::GenericError;
	}
	// Convert degrees to radians
	const float degreesToRadians = std::numbers::pi / 180.0;
	const float angleBetweenPoints = 360.0 / sides;
	// Dynamic arrays for polygon vertices and intersections
	std::vector<int16_t> vx(sides), vy(sides); // Dynamic size based on sides
	std::vector<int16_t> intersectX(sides);    // Maximum sides intersections
	// Calculate polygon vertex positions
	for (uint8_t i = 0; i < sides; i++) {
		vx[i] = x + (sin((i * angleBetweenPoints + rotation) * degreesToRadians) * diameter);
		vy[i] = y + (cos((i * angleBetweenPoints + rotation) * degreesToRadians) * diameter);
	}
	// If not filling, just draw the polygon outline
	if (!fill) {
		for (uint8_t i = 0; i < sides; i++) {
			uint8_t j = (i + 1) % sides; // Next vertex
			drawLine(vx[i], vy[i], vx[j], vy[j], color); // Draw edge between consecutive vertices
		}
	}
	// If filling, use scanline algorithm to fill the polygon
	else {
		// Scanline fill algorithm
		int16_t minY = vy[0], maxY = vy[0];
		for (uint8_t i = 1; i < sides; i++) {
			if (vy[i] < minY) minY = vy[i];
			if (vy[i] > maxY) maxY = vy[i];
		}
		// Loop through scanlines
		for (int16_t scanY = minY; scanY <= maxY; scanY++) {
			uint8_t intersections = 0;
			// Find intersections with polygon edges
			for (uint8_t i = 0; i < sides; i++) {
				uint8_t j = (i + 1) % sides;
				if ((vy[i] <= scanY && vy[j] > scanY) || (vy[j] <= scanY && vy[i] > scanY)) {
					// Compute intersection using linear interpolation
					float t = (float)(scanY - vy[i]) / (vy[j] - vy[i]);
					intersectX[intersections++] = vx[i] + t * (vx[j] - vx[i]);
				}
			}

			// Sort intersection points (bubble sort for simplicity)
			for (uint8_t i = 0; i < intersections - 1; i++) {
				for (uint8_t j = i + 1; j < intersections; j++) {
					if (intersectX[i] > intersectX[j]) {
						std::swap(intersectX[i], intersectX[j]);
					}
				}
			}

			// Draw horizontal lines between pairs of intersections
			for (uint8_t i = 0; i < intersections; i += 2) {
				if (i + 1 < intersections) {
					drawFastHLine(intersectX[i], scanY, intersectX[i + 1] - intersectX[i] + 1, color);
				}
			}
		}
	}
	return rdlib::Success;
}

/*!
	@brief Draws a grid of dots on the screen starting from the given coordinates.
		This function draws a grid of pixels with a specified gap between them. 
		It checks that the provided coordinates and dimensions are within the screen bounds 
		and adjusts them if necessary. It also validates The grid of dots gap value and defaults it to 2 if invalid.
	@param x The x-coordinate of the top-left corner where The grid of dots will start.
	@param y The y-coordinate of the top-left corner where The grid of dots will start.
	@param w The width of the area to draw The grid of dots, from the starting x-coordinate.
	@param h The height of the area to draw The grid of dots, from the starting y-coordinate.
	@param DotGridGap The gap between each dot gap line, controlling the spacing between drawn pixels.
	@param color The color of the pixels to draw in The grid of dots.
	@return rdlib::Return_Codes_e Returns a status code indicating success or failure. Possible return values:
	- rdlib::Success: The grid of dots was successfully drawn.
	- rdlib::ShapeScreenBounds: The provided coordinates or dimensions are out of screen bounds.
 */
rdlib::Return_Codes_e color16_graphics::drawDotGrid(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t DotGridGap, uint16_t color)
{
	//User input handling
	if ((x >= _width) || (y >= _height)){
		fprintf(stderr, "Error: drawDotGrid: Out of screen bounds\n");
		return rdlib::ShapeScreenBounds;
	}
	if (DotGridGap < 2 || DotGridGap > 20) {
		printf("Warning : drawDotGrid: Invalid Dot Gap value(2-20) , setting to 2\n");
		DotGridGap = 2;
	}
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;
	//Swap coordinates if the width or height are smaller than the starting point
	int16_t dotGapWidth, dotGapHeight;
	if (w < x) { dotGapWidth = w; w = x; x = dotGapWidth; }
	if (h < y) { dotGapHeight = h; h = y; y = dotGapHeight; }

	// Draw the grid of pixels
	for (int16_t row = y; row <= h; row += DotGridGap) 
	{
		for (int16_t col = x; col <= w; col += DotGridGap) 
		{
			drawPixel(col, row, color);
		}
	}
	return rdlib::Success;
}


/*!
	@brief Draws a line using an angle and length as parameters.
	This function draws a line starting from `(x, y)`, extending in the direction 
	specified by `angle`, with a given `length`. The function also allows applying 
	an `offset` to the angle before computing the line’s end coordinates.
	@param x The starting x-coordinate (horizontal position).
	@param y The starting y-coordinate (vertical position).
	@param angle The angle (in degrees) at which the line is drawn.
	@param start calculates a new starting position by moving start units along the given angle. 
	@param length The length of the line.
	@param offset An additional offset applied to the angle before calculating the endpoint.
	@param color The color of the line (RGB565 format).
 */
void color16_graphics::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, int offset , uint16_t color)
{
	if (start == 0){
		if (length < 2) {
			drawPixel(x, y, color);
		} else {
				drawLine(x, y, 
				x + length * rdlib_maths::cosineFromDegrees(angle + offset), 
				y + length * rdlib_maths::sineFromDegrees(angle + offset), color);
		}
	} else{
		if (start - length < 2) {
			drawPixel(x,y,color);
		} else {
			drawLine(
			x + start * rdlib_maths::cosineFromDegrees(angle + offset),
			y + start * rdlib_maths::sineFromDegrees(angle + offset),
			x + (start + length) * rdlib_maths::cosineFromDegrees(angle + offset),
			y + (start + length) * rdlib_maths::sineFromDegrees(angle + offset), 
			color);
		}
	}
}

/*!
	@brief Draws a quadrilateral (four-sided polygon) by connecting four points with lines.
	This function draws a quadrilateral by drawing four lines between the given vertices. 
	The lines are drawn in the order: (x0, y0) to (x1, y1), (x1, y1) to (x2, y2), 
	(x2, y2) to (x3, y3), and finally (x3, y3) back to (x0, y0).
	@param x0 The x-coordinate of the first vertex.
	@param y0 The y-coordinate of the first vertex.
	@param x1 The x-coordinate of the second vertex.
	@param y1 The y-coordinate of the second vertex.
	@param x2 The x-coordinate of the third vertex.
	@param y2 The y-coordinate of the third vertex.
	@param x3 The x-coordinate of the fourth vertex.
	@param y3 The y-coordinate of the fourth vertex.
	@param color The 565 color used to draw the lines of the quadrilateral.
 */
void color16_graphics::drawQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color) 
{
	drawLine(x0, y0, x1, y1, color);//low 1
	drawLine(x1, y1, x2, y2, color);//high 1
	drawLine(x2, y2, x3, y3, color);//high 2
	drawLine(x3, y3, x0, y0, color);//low 2
}

/*!
	@brief Fills a quadrilateral with the specified color using triangles.
	This function fills a quadrilateral by dividing it into triangles and filling them individually. 
	The quadrilateral is filled by calling the `fillTriangle` function three times with appropriate 
	vertex coordinates. The `useTriangleSplit` parameter determines whether the quadrilateral is divided into
	two triangles or if only one triangle is used.
	@param x0 The x-coordinate of the first vertex.
	@param y0 The y-coordinate of the first vertex.
	@param x1 The x-coordinate of the second vertex.
	@param y1 The y-coordinate of the second vertex.
	@param x2 The x-coordinate of the third vertex.
	@param y2 The y-coordinate of the third vertex.
	@param x3 The x-coordinate of the fourth vertex.
	@param y3 The y-coordinate of the fourth vertex.
	@param color The color used to fill the quadrilateral.
	@param useTriangleSplit A boolean flag that determines whether the quadrilateral should be divided into
		   two or three triangles.
 */
void color16_graphics::fillQuadrilateral(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color, bool useTriangleSplit)
{
	fillTriangle(x0,y0,x1,y1,x2,y2,color);
	if (useTriangleSplit){
		fillTriangle(x2, y2, x3, y3, x0, y0, color);
	}
	fillTriangle(x1,y1,x2,y2,x3,y3,color);
}

/*!
	@brief Draw an ellipse on the display.
	This function uses the midpoint ellipse algorithm to efficiently draw an 
	ellipse centered at (cx, cy) with the given semi-major (horizontal) and 
	semi-minor (vertical) axes.
	@param cx X-coordinate of the ellipse center.
	@param cy Y-coordinate of the ellipse center.
	@param semiMajorAxis Length of the semi-major axis (horizontal radius).
	@param semiMinorAxis Length of the semi-minor axis (vertical radius).
	@param fill if false draw Ellipse , if true fill it!
	@param color 16-bit color value for the ellipse.
 */
void color16_graphics::drawEllipse(int16_t cx, int16_t cy, int16_t semiMajorAxis, int16_t semiMinorAxis,  bool fill, uint16_t color)
{
	int16_t x, y; 
	// int16_t twiceMajorAxisSquared, twiceMinorAxisSquared; //stores 2 * (a*a) &  2 * (b*b)
	int32_t stopXThreshold, stopYThreshold; // ndicates when iteration stops in X & Y direction.
	int32_t deltaX, deltaY; // Represents the X & Y direction error change.
	int32_t decisionParam; 
	// Precompute squared values for efficiency
	const int32_t twiceMajorAxisSquared = 2 * (semiMajorAxis * semiMajorAxis);
	const int32_t twiceMinorAxisSquared = 2 * (semiMinorAxis * semiMinorAxis);
	// Region 1: Upper half
	x = semiMajorAxis;
	y = 0;
	deltaX = (semiMinorAxis * semiMinorAxis) * (1 - (2 * semiMajorAxis));  // Initial error term for X
	deltaY = (semiMajorAxis * semiMajorAxis);                               // Initial error term for Y
	decisionParam = 0;
	stopXThreshold = (twiceMinorAxisSquared * semiMajorAxis);  // Stopping condition for region 1
	stopYThreshold = 0;
	// First region: X decreases faster than Y increases
	while (stopXThreshold >= stopYThreshold) {
		if (fill){
				drawFastHLine(cx - x, cy + y, 2 * x + 1, color); // Fill horizontal line
				if (y != 0) drawFastHLine(cx - x, cy - y, 2 * x + 1, color); // Mirror bottom half
		}else{
			ellipseHelper(cx, cy, x, y, color);}
		y++;
		stopYThreshold += twiceMajorAxisSquared;
		decisionParam += deltaY;
		deltaY += twiceMajorAxisSquared;
		// Check for boundary error and adjust X
		if ((2 * decisionParam) + deltaX > 0) {
			x--;
			stopXThreshold -= twiceMinorAxisSquared;
			decisionParam += deltaX;
			deltaX += twiceMinorAxisSquared;
		}
	}
	// Region 2: Lower half
	x = 0;
	y = semiMinorAxis;
	deltaX = (semiMinorAxis * semiMinorAxis);  // Reset X error term
	deltaY = (semiMajorAxis * semiMajorAxis) * (1 - (2 * semiMinorAxis));  // Initial error term for Y
	decisionParam = 0;
	stopXThreshold = 0;
	stopYThreshold = (twiceMajorAxisSquared * semiMinorAxis);  // Stopping condition for region 2
	// Second region: Y decreases faster than X increases
	while (stopXThreshold <= stopYThreshold) {
		if (fill){
			drawFastHLine(cx - x, cy + y, 2 * x + 1, color); // Fill horizontal line
			if (y != 0) drawFastHLine(cx - x, cy - y, 2 * x + 1, color); // Mirror bottom half
		}else{
			ellipseHelper(cx, cy, x, y, color);}
		x++;
		stopXThreshold += twiceMinorAxisSquared;
		decisionParam += deltaX;
		deltaX += twiceMinorAxisSquared;
		// Check for boundary error and adjust Y
		if (((2 * decisionParam) + deltaY) > 0) {
			y--;
			stopYThreshold -= twiceMajorAxisSquared;
			decisionParam += deltaY;
			deltaY += twiceMajorAxisSquared;
		}
	}
}

/// @cond

/*!
	@brief Plots four symmetric points of an ellipse.
	This function takes advantage of the symmetry of ellipses, plotting the 
	four points in each quadrant to minimize calculations.
	@param cx X-coordinate of the ellipse center.
	@param cy Y-coordinate of the ellipse center.
	@param x Current X offset.
	@param y Current Y offset.
	@param color 16-bit color value.
 */
void color16_graphics::ellipseHelper(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color)
{
	drawPixel(cx + x, cy + y, color);
	if (x != 0) drawPixel(cx - x, cy + y, color);
	if (y != 0) drawPixel(cx + x, cy - y, color);
	if (x != 0 && y != 0) drawPixel(cx - x, cy - y, color);
}



/*!
	@brief helps drawArc draw an Arc on screen 
	@param centerX X-coordinate of the center of the arc
	@param centerY Y-coordinate of the center of the arc
	@param radius The radius of the arc
	@param thickness the thickness of the arc
	@param start Starting angle of arc
	@param end End angle of arc
	@param color The color of the arc.
*/
void color16_graphics::drawArcHelper(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color) 
{
	// Define bounding box variables
	int16_t minX = 65535;
	int16_t maxX = -32767;
	int16_t minY = 32767;
	int16_t maxY = -32767;
	// Trigonometric values
	float cosStart, sinStart, cosEnd, sinEnd;
	float outerRadius, tempValue;
	float startAngle, endAngle;
	// Squared radius values for comparison
	int16_t innerRadiusSquared, outerRadiusSquared;
	// Loop variables
	int16_t x, y, xSquared, ySquared;
	int16_t y1Start, y2End, y2Start;
	// Slope calculations
	float startSlope, endSlope;
	// Boolean flags for arc filling logic
	bool y1StartFound, y2StartFound, y1EndFound, y2EndSearching;
	// Convert arc angles to degrees from the normalized input range
	startAngle = (start / _arcAngleMax) * 360;
	endAngle = (end / _arcAngleMax) * 360;
	// Normalize angles to stay within the 0-360 range
	while (startAngle < 0) startAngle += 360;
	while (endAngle < 0) endAngle += 360;
	while (startAngle > 360) startAngle -= 360;
	while (endAngle > 360) endAngle -= 360;
	// Handle cases where the arc wraps around 0 degrees
	if (startAngle > endAngle) {
		drawArcHelper(centerX, centerY, radius, thickness, ((startAngle / 360.0) * _arcAngleMax), _arcAngleMax, color);
		drawArcHelper(centerX, centerY, radius, thickness, 0, ((endAngle / 360.0) * _arcAngleMax), color);
	} else {// Compute trigonometric values for start and end angles
		cosStart = rdlib_maths::cosineFromDegrees(startAngle);
		sinStart = rdlib_maths::sineFromDegrees(startAngle);
		cosEnd = rdlib_maths::cosineFromDegrees(endAngle);
		sinEnd = rdlib_maths::sineFromDegrees(endAngle);
		// Determine the bounding box of the arc
		outerRadius = radius;
		tempValue = outerRadius * cosStart;
		if (tempValue < minX) minX = tempValue;
		if (tempValue > maxX) maxX = tempValue;
		tempValue = outerRadius * sinStart;
		if (tempValue < minY) minY = tempValue;
		if (tempValue > maxY) maxY = tempValue;

		tempValue = outerRadius * cosEnd;
		if (tempValue < minX) minX = tempValue;
		if (tempValue > maxX) maxX = tempValue;
		tempValue = outerRadius * sinEnd;
		if (tempValue < minY) minY = tempValue;
		if (tempValue > maxY) maxY = tempValue;
		// Adjust bounding box for inner arc
		outerRadius = radius - thickness;
		tempValue = outerRadius * cosStart;
		if (tempValue < minX) minX = tempValue;
		if (tempValue > maxX) maxX = tempValue;
		tempValue  = outerRadius * sinStart;
		if (tempValue < minY) minY = tempValue;
		if (tempValue > maxY) maxY = tempValue;

		tempValue = outerRadius * cosEnd;
		if (tempValue < minX) minX = tempValue;
		if (tempValue > maxX) maxX = tempValue;
		tempValue = outerRadius * sinEnd;
		if (tempValue < minY) minY = tempValue;
		if (tempValue > maxY) maxY = tempValue;
		// Special cases for quarter-circle boundary adjustments
		if ((startAngle < 90) && (endAngle > 90)) maxY = radius;
		if ((startAngle < 180) && (endAngle > 180)) minX = -radius;
		if ((startAngle < 270) && (endAngle > 270)) minY = -radius;
		// Calculate slopes for boundary conditions
		startSlope = (float)cosStart / (float)sinStart;
		endSlope = (float)cosEnd / (float)sinEnd;
		if (endAngle == 360) endSlope = -1000000; // Force slope to an extreme value
		innerRadiusSquared = (radius - thickness) * (radius - thickness);
		outerRadiusSquared = radius * radius;
		// Scan through bounding box to determine which pixels to fill
		for (x = minX; x <= maxX; x++) {
			y1StartFound 	= false;
			y2StartFound 	= false;
			y1EndFound 		= false;
			y2EndSearching 	= false;
			y1Start = 0;
			y2End = 0;
			y2Start = 0;
			for (y = minY; y <= maxY; y++) {
				xSquared = x * x;
				ySquared = y * y;
				// Check if pixel is within the arc boundaries
				if (
					(xSquared + ySquared < outerRadiusSquared && xSquared + ySquared >= innerRadiusSquared) && (
					(y > 0 && startAngle < 180 && x <= y * startSlope) ||
					(y < 0 && startAngle > 180 && x >= y * startSlope) ||
					(y < 0 && startAngle <= 180) ||
					(y == 0 && startAngle <= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)
					) && (
					(y > 0 && endAngle < 180 && x >= y * endSlope) ||
					(y < 0 && endAngle > 180 && x <= y * endSlope) ||
					(y > 0 && endAngle >= 180) ||
					(y == 0 && endAngle >= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)))
				{ // Find start and end points for vertical line drawing
					if (!y1StartFound) {
						y1StartFound = true;
						y1Start = y;
					} else if (y1EndFound && !y2StartFound) {
						y2StartFound = true;
						y2Start = y;
						y += y2End - y1Start - 1;
						if (y > maxY - 1) {
							y = y2Start;
							y2EndSearching = true;
						}
					} else if (y2StartFound && !y2EndSearching) {
						y2EndSearching = true;
					}
				} else {
					if (y1StartFound && !y1EndFound) {
						y1EndFound = true;
						y2End = y - 1;
						drawFastVLine(centerX + x, centerY + y1Start, y - y1Start, color);
						if (y < 0) {
							y = abs(y);
						} else {
							break;
						}
					} else if (y2StartFound) {
						if (y2EndSearching) {
							drawFastVLine(centerX + x, centerY + y2Start, y - y2Start, color);
							y2EndSearching = false;
							break;
						} else {
							y = y2Start;
							y2EndSearching = true;
						}
					}
				}
			}
			if (y1StartFound && !y1EndFound){
				y2End = maxY;
				drawFastVLine(centerX + x, centerY + y1Start, y2End - y1Start + 1, color);
			} else if (y2StartFound && y2EndSearching){
				drawFastVLine(centerX + x, centerY + y2Start, maxY - y2Start + 1, color);
			}
		}
	}
}

/// @endcond

/*!
	@brief Draw an arc on the TFT display.
	This function draws an arc between two angles (start and end) on a circle with a given radius.
	@param cx X-coordinate of the center of the circle.
	@param cy Y-coordinate of the center of the circle.
	@param radius The radius of the circle.
	@param thickness the thickness of the arc
	@param startAngle The starting angle of the arc (in degrees).
	@param endAngle The ending angle of the arc (in degrees).
	@param color The color of the arc.
	@details variables _arcAngleMax and  _arcAngleOffset which can be set by user with setters
		to define behaviour of arc. 
		_arcAngleOffset is zero default, means 0 degree is positive X axis , Arc is drawn counterclockwise
		XXX 270 XXX
		180 XXX 000
		XXX 090 XXX
		This function uses a modified midpoint circle algorithm combined with scanline filling
		to efficiently draw an arc with a specified thickness. It calculates pixel positions using 
		trigonometric boundary checks and slopes for accurate rendering. 
		For more information, see: https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
void color16_graphics::drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float startAngle, float endAngle, uint16_t color) {
	if(radius == 0 ) radius = 1;
	if(thickness == 0) thickness = 1;
	// Check if the arc should be a full circle (0 to 360 degrees)
	if (startAngle == 0 && endAngle == _arcAngleMax) {
		drawArcHelper(cx, cy, radius, thickness, 0, _arcAngleMax, color);
	} else {
		// Apply the offset and adjust the start/end angles
		drawArcHelper(
			cx, cy, radius, thickness,
			startAngle + (_arcAngleOffset / (float)360) * _arcAngleMax, // Adjust start angle with the offset
			endAngle + (_arcAngleOffset / (float)360) * _arcAngleMax,   // Adjust end angle with the offset
			color
		);
	}
}

/*!
	@brief Draw a simple arc of one pixel on the display( no offsets , thickness or maximum arc calculations)
	This function draws an arc between two angles (start and end) on a circle with a given radius.
	@param cx X-coordinate of the center of the circle.
	@param cy Y-coordinate of the center of the circle.
	@param radius The radius of the circle.
	@param startAngle The starting angle of the arc (in degrees).
	@param endAngle The ending angle of the arc (in degrees).
	@param color The color of the arc.
	@details 0 degree is positive X axis , arc is drawn counterclockwise
 */
void color16_graphics::drawSimpleArc(int16_t cx, int16_t cy, int16_t radius, float startAngle, float endAngle, uint16_t color)
{
	const float degreesToRadians = std::numbers::pi / 180.0;
	// Ensure that the start and end angles are in the correct order (start < end)
	if (startAngle > endAngle) {
		std::swap(startAngle, endAngle);
	}
	// Loop through the angle range, in small steps
	float step = 1.0f; // This controls the smoothness of the arc
	for (float angle = startAngle; angle <= endAngle; angle += step) {
		float rad = angle * degreesToRadians;
		int16_t x = cx + radius * cos(rad);
		int16_t y = cy + radius * sin(rad);
		drawPixel(x, y, color);
	}
}
// **************** EOF *****************
