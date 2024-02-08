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
	writeData(color >> 8);
	writeData(color & 0xFF);
}


/*!
	@brief fills a rectangle starting from coordinates (x,y) with width of w and height of h.
	@param x x coordinate
	@param y y coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 565 16-bit
	@note  uses spiWriteDataBuffer method
	@return 0 for success , 2= out of screen bounds ,3 = Malloc failure
*/
rpiDisplay_Return_Codes_e color16_graphics::fillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	uint8_t hi, lo;

	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		std::cout << "Error fillRectangle 2: Out of screen bounds" << std::endl;
		return rpiDisplay_ShapeScreenBounds;
	}
	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;

	// Colour to bytes
	hi = color >> 8;
	lo = color;

	// Create bitmap buffer
	uint8_t* buffer = (uint8_t*)malloc(w*h*sizeof(uint16_t));
	if (buffer == nullptr) // check malloc
	{
		std::cout << "Error fillRectangle 3: MALLOC could not assign memory " << std::endl;
		return rpiDisplay_MallocError;
	}
	for(uint32_t i = 0; i<w*h*sizeof(uint16_t);) {
		buffer[i++] = hi;
		buffer[i++] = lo;
	}

	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer, h*w*sizeof(uint16_t));
	free(buffer);
	return rpiDisplay_Success;
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
*/
rpiDisplay_Return_Codes_e color16_graphics::drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color) {
	uint8_t hi, lo;
	if ((x >= _width) || (y >= _height))
		return rpiDisplay_ShapeScreenBounds;
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
	return rpiDisplay_Success;
}

/*!
	@brief Draws a horizontal line starting at (x,y) with width w.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param w The width of the line
	@param color The color of the line 565 16 Bit color
*/
rpiDisplay_Return_Codes_e color16_graphics::drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color) {
	uint8_t hi, lo;
	if ((x >= _width) || (y >= _height))
		return rpiDisplay_ShapeScreenBounds;
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
	return rpiDisplay_Success;
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
		_swap_int16_t_RDL(x0, y0);
		_swap_int16_t_RDL(x1, y1);
	}
	if (x0 > x1) {
		_swap_int16_t_RDL(x0, x1);
		_swap_int16_t_RDL(y0, y1);
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
	int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12, sa, sb;
	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		_swap_int16_t_RDL(y0, y1);
		_swap_int16_t_RDL(x0, x1);
	}
	if (y1 > y2) {
		_swap_int16_t_RDL(y2, y1);
		_swap_int16_t_RDL(x2, x1);
	}
	if (y0 > y1) {
		_swap_int16_t_RDL(y0, y1);
		_swap_int16_t_RDL(x0, x1);
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
		if (a > b) _swap_int16_t_RDL(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b) _swap_int16_t_RDL(a, b);
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
	@param character  An array of unsigned chars containing icon data vertically addressed.
	@return 0 for success ,2=Co-ordinates out of bounds,
		3=invalid pointer object
*/
rpiDisplay_Return_Codes_e color16_graphics::drawIcon(uint16_t x, uint16_t y, uint16_t w, uint16_t color, uint16_t backcolor, const unsigned char character[]) {
	// Out of screen bounds
	if ((x >= _width) || (y >= _height))
	{
		std::cout << "Error drawIcon 2: Out of screen bounds" << std::endl;
		return rpiDisplay_BitmapScreenBounds ;
	}
	// Check for null pointer
	if(character == nullptr)
	{
		std::cout << "Error drawIcon 3: Icon array is not valid pointer object" << std::endl;
		return rpiDisplay_BitmapNullptr;
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
	return rpiDisplay_Success;
}

/*!
	@brief: Draws an bi-color bitmap to screen
	@param x X coordinate
	@param y Y coordinate
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@param color bitmap foreground colors ,is bi-color
	@param bgcolor bitmap background colors ,is bi-color
	@param pBmp  an array of unsigned chars containing bitmap data horizontally addressed.
	@return 0 for success ,1=invalid pointer object ,2=Co-ordinates out of bounds,
		3= malloc failure
*/
rpiDisplay_Return_Codes_e color16_graphics::drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t bgcolor, const uint8_t* pBmp) {
	int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	uint16_t mycolor = 0;
	uint32_t ptr;

	//  Check for null pointer
	if( pBmp == nullptr)
	{
		std::cout << "Error drawBitmap 1: Bitmap array is nullptr" << std::endl;
		return rpiDisplay_BitmapNullptr;
	}
	
	// check horizontal size
	if(w % 8 != 0)
	{
		std::cout << ("Error drawBitmap 2 : Horizontal Bitmap size is incorrect:  Check Size = w % 8 != 0: ")<< w << std::endl;
		return rpiDisplay_BitmapHorizontalSize;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		std::cout << "Error drawBitmap 3: Out of screen bounds, check x & y" << std::endl;
		return rpiDisplay_BitmapScreenBounds;
	}

	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;

	// Create bitmap buffer
	uint8_t* buffer = (uint8_t*)malloc(w * h * 2);
	if (buffer == nullptr) // check malloc
	{
		std::cout << "Error drawBitmap 3: MALLOC could not assign memory " << std::endl;
		return rpiDisplay_MallocError;
	}
	ptr = 0;

	for (uint16_t j = 0; j < h; j++)
	{
		for (uint16_t i = 0; i < w; i++)
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = (pBmp[j * byteWidth + i / 8]);
			mycolor = (byte & 0x80) ? color : bgcolor;
			buffer[ptr++] = mycolor >> 8;
			buffer[ptr++] = mycolor;
		}
	}
	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer, h*w*sizeof(uint16_t));

	free(buffer);
	return rpiDisplay_Success;
}

/*!
	@brief Draws an 24 bit color bitmap to screen from a data array
	@param x X coordinate
	@param y Y coordinate
	@param pBmp A pointer to the databuffer containing Bitmap data
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@return 0 for success ,1=invalid pointer object ,2=Co-ordinates out of bounds,
		3= malloc failure
	@note 24 bit color converted to 16 bit color
*/
rpiDisplay_Return_Codes_e  color16_graphics::drawBitmap24(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h)
{
	uint8_t i, j;
	uint16_t color;
	uint32_t rgb, ptr;
	// 1. Check for null pointer
	if( pBmp == nullptr)
	{
		std::cout << "Error drawBitmap24 1: Bitmap array is nullptr" << std::endl;
		return rpiDisplay_BitmapNullptr;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		std::cout << "Error drawBitmap24 2: Out of screen bounds" << std::endl;
		return rpiDisplay_BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;

	// Create bitmap buffer
	uint8_t* buffer = (uint8_t*)malloc(w * h * 2);
	if (buffer == nullptr) // check malloc
	{
		std::cout << "Error drawBitmap24 3: MALLOC could not assign memory " << std::endl;
		return rpiDisplay_MallocError;
	}
	ptr = 0;
	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w ; i ++)
		{
			// Translate RBG24 to RGB565 bitmap
			rgb = *(unsigned int*)(pBmp + i * 3 + (h-1-j) * 3 * w);
			color = Color565(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF), (rgb & 0xFF));
			buffer[ptr++] = color >> 8;
			buffer[ptr++] = color;
		}
	}

	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer, h*w*sizeof(uint16_t));

	free(buffer);
	return rpiDisplay_Success;
}


/*!
	@brief: Draws an 16 bit color bitmap to screen from a data array
	@param x X coordinate
	@param y Y coordinate
	@param pBmp A pointer to the databuffer containing Bitmap data
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@return 0 for success ,1=invalid pointer object ,2=Co-ordinates out of bounds,
		3= malloc failure
*/
rpiDisplay_Return_Codes_e  color16_graphics::drawBitmap16(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h) {
	uint8_t i, j;
	uint16_t color;
	uint32_t ptr;
	// 1. Check for null pointer
	if( pBmp == nullptr)
	{
		std::cout << "Error drawBitmap24 1: Bitmap array is nullptr" << std::endl;
		return rpiDisplay_BitmapNullptr;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		std::cout << "Error drawBitmap16 2: Out of screen bounds" << std::endl;
		return rpiDisplay_BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width) w = _width - x;
	if ((y + h - 1) >= _height) h = _height - y;

	// Create bitmap buffer
	uint8_t* buffer = (uint8_t*)malloc(w * h * 2);
	if (buffer == nullptr) // check malloc
	{
		std::cout << "Error drawBitmap16 3 :MALLOC could not assign memory " << std::endl;
		return rpiDisplay_MallocError;
	}
	ptr = 0;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i ++)
		{
			color = *(unsigned int*)(pBmp + i * 2 + (h-1-j) * 2 * w);
			buffer[ptr++] = color >> 8;
			buffer[ptr++] = color;
		}
	}

	// Set window and write buffer
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	spiWriteDataBuffer(buffer, h*w*sizeof(uint16_t));

	free(buffer);
	return rpiDisplay_Success;
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
 * @note not in use currently.
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
*/
void color16_graphics::spiWriteDataBuffer(uint8_t* spidata, uint32_t len) {
	Display_DC_SetHigh;
	if (_hardwareSPI == false) {
		Display_CS_SetLow;
		for(uint32_t i=0; i<len; i++) {spiWriteSoftware(spidata[i]);}
		Display_CS_SetHigh;
	} else {
		bcm2835_spi_writenb((char*)spidata,len);
	}
}

/*!
	@brief  Write byte to SPI
	@param spidata byte to write
*/
void color16_graphics::spiWrite(uint8_t spidata) {
	if (_hardwareSPI == false)
	{
		spiWriteSoftware(spidata);
	} else {
		bcm2835_spi_transfer(spidata);
	}
}

/*!
	@brief Write a byte to SPI using software SPI
	@param spidata byte to send
	@note uses _HIGHFREQ_DELAY to slowdown software SPI if CPU frequency too fast
*/
void color16_graphics::spiWriteSoftware(uint8_t spidata) {
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		Display_SDATA_SetLow;
		if (spidata & 0x80)
			Display_SDATA_SetHigh; // b1000000 Mask with 0 & all zeros out.
		Display_SCLK_SetHigh;
		bcm2835_delayMicroseconds(_HighFreqDelay);
		spidata <<= 1;
		Display_SCLK_SetLow;
		bcm2835_delayMicroseconds(_HighFreqDelay);
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
	@brief Write 1 character on OLED.
	@param  x character starting position on x-axis. Valid values: 0..127
	@param  y character starting position on x-axis. Valid values: 0..63
	@param  value Character to be written.
	@return Will return rpiDisplay_Return_Codes_e enum
		-# rpiDisplay_Success  success
		-# rpiDisplay_CharScreenBounds co-ords out of bounds check x and y
		-# rpiDisplay_CharFontASCIIRange Character out of ASCII Font bounds, check Font range
 */
rpiDisplay_Return_Codes_e color16_graphics::writeChar(int16_t x, int16_t y, char value) {

	// 1. Check for screen out of  bounds
	if((x >= _width)            || // Clip right
	(y >= _height)           || // Clip bottom
	((x + _Font_X_Size+1) < 0) || // Clip left
	((y + _Font_Y_Size) < 0))   // Clip top
	{
		std::cout << "writeChar Error 1: Co-ordinates out of bounds" << std::endl;
		return rpiDisplay_CharScreenBounds;
	}
	// 2. Check for character out of font range bounds
	if ( value < _FontOffset || value >= (_FontOffset + _FontNumChars+1))
	{
		std::cout << "writeChar Error 2: Character out of Font bounds" << value << " :" << +_FontOffset << " <--> " << +(_FontOffset + _FontNumChars)<< std::endl;
		return rpiDisplay_CharFontASCIIRange;
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

	if (_Font_Y_Size % 8 == 0) // Is the font height divisible by 8
	{
		uint16_t rowCount = 0;
		uint16_t temp = 0;
		uint16_t count;
		uint8_t colIndex;
		fontIndex = ((value - _FontOffset)*(_Font_X_Size * (_Font_Y_Size/ 8))) + 4;
		for (rowCount = 0; rowCount < (_Font_Y_Size / 8); rowCount++)
		{
			for (count = 0; count < _Font_X_Size; count++)
			{
				temp = *(_FontSelect + fontIndex + count + (rowCount * _Font_X_Size));
				for (colIndex = 0; colIndex < 8; colIndex++)
				{
					if (temp & (1 << colIndex)) {
							drawPixel(x + count, y + (rowCount * 8) + colIndex, ltextcolor);
					} else {
							drawPixel(x + count, y + (rowCount * 8) + colIndex, ltextbgcolor);
					}
				}
			}
		}
	} else
	{
		int16_t colByte, cx, cy;
		int16_t colbit;
		fontIndex = ((value - _FontOffset)*((_Font_X_Size * _Font_Y_Size) / 8)) + 4;
		colByte = *(_FontSelect + fontIndex);
		colbit = 7;
		for (cx = 0; cx < _Font_X_Size; cx++)
		{
			for (cy = 0; cy < _Font_Y_Size; cy++)
			{
				if ((colByte & (1 << colbit)) != 0) {
					drawPixel(x + cx, y + cy, ltextcolor);
				} else {
					drawPixel(x + cx, y + cy, ltextbgcolor);
				}
				colbit--;
				if (colbit < 0) {
					colbit = 7;
					fontIndex++;
					colByte = *(_FontSelect + fontIndex);
				}
			}
		}
	}
	return rpiDisplay_Success ;
}

/*!
	@brief Write Text character array on OLED.
	@param  x character starting position on x-axis.
	@param  y character starting position on y-axis.
	@param  pText Pointer to the array of the text to be written.
	@return Will return
		-# rpiDisplay_Success Success
		-# rpiDisplay_CharArrayNullptr  String pText Array invalid pointer object
		-# 3 Failure in writeChar method upstream
 */
rpiDisplay_Return_Codes_e  color16_graphics::writeCharString(int16_t x, int16_t y, char * pText) {
	uint8_t count=0;
	uint8_t MaxLength=0;
	// Check for null pointer
	if(pText == nullptr)
	{
		std::cout << "writeCharString Error 1 :String array is not valid pointer" << std::endl;
		return rpiDisplay_CharArrayNullptr;
	}
	rpiDisplay_Return_Codes_e DrawCharReturnCode;
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
		if(DrawCharReturnCode  != rpiDisplay_Success) return DrawCharReturnCode;
		count++;
		MaxLength++;
		if (MaxLength >= 150) break; // 2nd way out of loop, safety check
	}
	return rpiDisplay_Success;
}

/*!
	@brief write method used in the print class when user calls print
	@param character the character to print
	@return Will return
		-# 1. success
		-# rpiDisplay_Return_Codes_e enum error code An error in the writeChar method.
*/
size_t color16_graphics::write(uint8_t character)
{
	rpiDisplay_Return_Codes_e DrawCharReturnCode;
	switch (character)
	{
		case '\n':
			_cursorY += _Font_Y_Size;
			_cursorX  = 0;
		break;
		case '\r': break;
		default:
			DrawCharReturnCode = writeChar(_cursorX, _cursorY, character);
			if(DrawCharReturnCode  != rpiDisplay_Success) return DrawCharReturnCode;
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
// **************** EOF *****************
