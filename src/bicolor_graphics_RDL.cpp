/*!
	@file bicolor_graphics_RDL.cpp
	@brief Graphics based functions for bicolor display
	@details Project Name: Display_Lib_RPI
	@author  Gavin Lyons
*/


#include "../include/bicolor_graphics_RDL.hpp"

/*!
	@brief init the Display  Graphics class object
	@param w width defined  in sub-class
	@param h height defined in sub-class
 */
bicolor_graphics::bicolor_graphics(int16_t w, int16_t h):
	WIDTH(w), HEIGHT(h)
{
	_width     = WIDTH;
	_height    = HEIGHT;
	_cursor_y  = 0;
	_cursor_x  = 0;
	_textwrap  = true;
}


/*!
	@brief Write 1 character on display
	@param  x character starting position on x-axis.
	@param  y character starting position on x-axis.
	@param  value Character to be written.
	@note Horizontal font addressing 
	@return Will return rpiDisplay_Return_Codes_e enum
		-# rpiDisplay_Success  success
		-# rpiDisplay_CharScreenBounds co-ords out of bounds check x and y
		-# rpiDisplay_CharFontASCIIRange Character out of ASCII Font bounds, check Font range
 */
rpiDisplay_Return_Codes_e bicolor_graphics::writeChar(int16_t x, int16_t y, char value) {

	// 1. Check for screen out of bounds
	if ((x >= _width) || // Clip right
		(y >= _height) || // Clip bottom
		((x + _Font_X_Size + 1) < 0) || // Clip left
		((y + _Font_Y_Size) < 0)) { // Clip top
		printf("writeChar Error 1: Co-ordinates out of bounds \r\n");
		return rpiDisplay_CharScreenBounds;
	}

	// 2. Check for character out of font range bounds
	if (value < _FontOffset || value >= (_FontOffset + _FontNumChars + 1)) {
		printf("writeChar Error 2: Character out of Font bounds  %c : %u<->%u \r\n", value, _FontOffset,(unsigned int)(_FontOffset + _FontNumChars));
		return rpiDisplay_CharFontASCIIRange;
	}

	uint16_t fontIndex = 0;
	int16_t colByte, cx, cy;
	int16_t colbit;
	fontIndex = ((value - _FontOffset)*((_Font_X_Size * _Font_Y_Size) / 8)) + 4;
	colByte = *(_FontSelect + fontIndex);
	colbit = 7;
	// Loop through each row (cy) and column (cx) of the character bitmap
	for (cy = 0; cy < _Font_Y_Size; cy++)
	{
		for (cx = 0; cx < _Font_X_Size; cx++)
		{
			 // Check if the current bit is set
			if ((colByte & (1 << colbit)) != 0) {
				drawPixel(x + cx, y + cy, !getInvertFont());
			} else {
				drawPixel(x + cx, y + cy, getInvertFont());
			}
			colbit--;
			if (colbit < 0) {
				colbit = 7;
				fontIndex++;
				colByte = *(_FontSelect + fontIndex);
			}
		}
	}
	return rpiDisplay_Success;
}


/*!
	@brief Write Text character array on Display.
	@param  x character starting position on x-axis.
	@param  y character starting position on y-axis.
	@param  pText Pointer to the array of the text to be written.
	@return Will return
		-# rpiDisplay_Success Success
		-# rpiDisplay_CharArrayNullptr  String pText Array invalid pointer object
		-# 3 Failure in writeChar method upstream
 */
rpiDisplay_Return_Codes_e  bicolor_graphics::writeCharString(int16_t x, int16_t y, char * pText) {
	uint8_t count=0;
	uint8_t MaxLength=0;
	// Check for null pointer
	if(pText == nullptr)
	{
		printf("writeCharString Error 1 :String array is not valid pointer\n");
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
size_t bicolor_graphics::write(uint8_t character)
{
	rpiDisplay_Return_Codes_e DrawCharReturnCode;
	switch (character)
	{
		case '\n':
			_cursor_y += _Font_Y_Size;
			_cursor_x  = 0;
		break;
		case '\r': break;
		default:
			DrawCharReturnCode = writeChar(_cursor_x, _cursor_y, character);
			if (DrawCharReturnCode != rpiDisplay_Success) 
			{
				// Set the write error based on the result of the drawing operation
				setWriteError(DrawCharReturnCode); // Set error flag to non-zero value}
				break;
			}
			_cursor_x += (_Font_X_Size);
			if (_textwrap && (_cursor_x  > (_width - (_Font_X_Size))))
			{
				_cursor_y += _Font_Y_Size;
				_cursor_x = 0;
			}
		break;
	} // end of switch

	return 1;
}

/*!
	@brief draws a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color The color of the circle
*/
void bicolor_graphics::drawCircle(int16_t x0, int16_t y0, int16_t r,
	uint8_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0  , y0+r, color);
	drawPixel(x0  , y0-r, color);
	drawPixel(x0+r, y0  , color);
	drawPixel(x0-r, y0  , color);

	while (x<y) {
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
void bicolor_graphics::drawCircleHelper( int16_t x0, int16_t y0,
				 int16_t r, uint8_t cornername, uint8_t color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
	if (f >= 0) {
		y--;
		ddF_y += 2;
		f     += ddF_y;
	}
	x++;
	ddF_x += 2;
	f     += ddF_x;
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
	@brief fills a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color color of the filled circle
*/
void bicolor_graphics::fillCircle(int16_t x0, int16_t y0, int16_t r,
					uint8_t color) {
	drawFastVLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

/*!
	@brief Used internally by fill circle fillRoundRect and fillcircle
*/
void bicolor_graphics::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
	uint8_t cornername, int16_t delta, uint8_t color) {

	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
	if (f >= 0) {
		y--;
		ddF_y += 2;
		f     += ddF_y;
	}
	x++;
	ddF_x += 2;
	f     += ddF_x;

	if (cornername & 0x1) {
		drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
		drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
	}
	if (cornername & 0x2) {
		drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
		drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
	}
	}
}

/*!
	@brief draws a line from (x0,y0) to (x1,y1).
	@param x0 x start coordinate
	@param y0 y start coordinate
	@param x1 x end coordinate
	@param y1 y end coordinate
	@param color color to draw line
*/
void bicolor_graphics::drawLine(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				uint8_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
	swapDisplayRPI(x0, y0);
	swapDisplayRPI(x1, y1);
	}

	if (x0 > x1) {
	swapDisplayRPI(x0, x1);
	swapDisplayRPI(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
	ystep = 1;
	} else {
	ystep = -1;
	}

	for (; x0<=x1; x0++) {
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
	@brief draws rectangle at (x,y) where h is height and w is width of the rectangle.
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to draw  rect
*/
void bicolor_graphics::drawRect(int16_t x, int16_t y,
				int16_t w, int16_t h,
				uint8_t color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y+h-1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x+w-1, y, h, color);
}


/*!
	@brief Draws a vertical line starting at (x,y) with height h.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param h The height of the line
	@param color The color of the line
*/
void bicolor_graphics::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint8_t color) {
	drawLine(x, y, x, y+h-1, color);
}

/*!
	@brief Draws a horizontal line starting at (x,y) with width w.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param w The width of the line
	@param color The color of the line
*/
void bicolor_graphics::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint8_t color) {
		drawLine(x, y, x+w-1, y, color);
}

/*!
	@brief fills a rectangle starting from coordinates (x,y) with width of w and height of h.
	@param x x coordinate
	@param y y coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle
*/
void bicolor_graphics::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
				uint8_t color) {
	for (int16_t i=x; i<x+w; i++) {
	drawFastVLine(i, y, h, color);
	}
}

/*!
	@brief Fills the whole screen with a given color.
	@param  color color to fill screen
*/
void bicolor_graphics::fillScreen(uint8_t color) {
	fillRect(0, 0, _width, _height, color);
}

/*!
	@brief draws a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r radius of the rounded edges
	@param color color to draw rounded rectangle
*/
void bicolor_graphics::drawRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r, uint8_t color) {
	drawFastHLine(x+r  , y    , w-2*r, color); // Top
	drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
	drawFastVLine(x    , y+r  , h-2*r, color); // Left
	drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
	// draw four corners
	drawCircleHelper(x+r    , y+r    , r, 1, color);
	drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
	drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

/*!
	@brief Fills a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r  radius of the rounded edges
	@param color color to fill round  rectangle
*/
void bicolor_graphics::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint8_t color) {
	// smarter version
	fillRect(x+r, y, w-2*r, h, color);

	// draw four corners
	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

/*!
	@brief draws a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).
	@param x0 x start coordinate point 1
	@param y0 y start coordinate point 1
	@param x1 x start coordinate point 2
	@param y1 y start coordinate point 2
	@param x2 x start coordinate point 3
	@param y2 y start coordinate point 3
	@param color color to draw triangle
*/
void bicolor_graphics::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint8_t color) {
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
	@param color color to fill  triangle
*/
void bicolor_graphics::fillTriangle ( int16_t x0, int16_t y0,
					int16_t x1, int16_t y1,
					int16_t x2, int16_t y2, uint8_t color) {

	int16_t a, b, y, last;

	if (y0 > y1) {
	swapDisplayRPI(y0, y1); swapDisplayRPI(x0, x1);
	}
	if (y1 > y2) {
	swapDisplayRPI(y2, y1); swapDisplayRPI(x2, x1);
	}
	if (y0 > y1) {
	swapDisplayRPI(y0, y1); swapDisplayRPI(x0, x1);
	}

	if(y0 == y2) {
	a = b = x0;
	if(x1 < a)      a = x1;
	else if(x1 > b) b = x1;
	if(x2 < a)      a = x2;
	else if(x2 > b) b = x2;
	drawFastHLine(a, y0, b-a+1, color);
	return;
	}

	int16_t
	dx01 = x1 - x0,
	dy01 = y1 - y0,
	dx02 = x2 - x0,
	dy02 = y2 - y0,
	dx12 = x2 - x1,
	dy12 = y2 - y1;
	int32_t
	sa   = 0,
	sb   = 0;

	if(y1 == y2) last = y1;
	else         last = y1-1;

	for(y=y0; y<=last; y++) {
	a   = x0 + sa / dy01;
	b   = x0 + sb / dy02;
	sa += dx01;
	sb += dx02;

	if(a > b) swapDisplayRPI(a,b);
	drawFastHLine(a, y, b-a+1, color);
	}


	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
	a   = x1 + sa / dy12;
	b   = x0 + sb / dy02;
	sa += dx12;
	sb += dx02;
	if(a > b) swapDisplayRPI(a,b);
	drawFastHLine(a, y, b-a+1, color);
	}
}

/*!
	@brief set the cursor position
	@param x X co-ord position
	@param y Y co-ord position
*/
void bicolor_graphics::setCursor(int16_t x, int16_t y) {
	_cursor_x = x;
	_cursor_y = y;
}

/*!
	@brief Gets the width of the display (per current _rotation)
	@return width member of display in pixels
*/
int16_t bicolor_graphics::width(void) const {return _width;}

/*!
	@brief Gets the height of the display (per current _rotation)
	@return height member of display in pixels
*/
int16_t bicolor_graphics::height(void) const {return _height;}

 /*!
	@brief Gets the _rotation of the display
	@return _rotation value 0-3
*/
displayBC_rotate_e  bicolor_graphics::getRotation(void){return _display_rotate;}

 /*!
	@brief Sets the _rotation of the display
	@param CurrentRotation value 0-3
*/
void bicolor_graphics::setRotation(displayBC_rotate_e  CurrentRotation) {

	_display_rotate = CurrentRotation;
	switch(CurrentRotation)
	{
		case 0:
		case 2:
			_width  = WIDTH;
			_height = HEIGHT;
			break;
		case 1:
		case 3:
			_width  = HEIGHT;
			_height = WIDTH;
			break;
	}
}

/*!
	@brief Draw a 1-bit color bitmap
	@param x x co-ord position
	@param y y co-ord posiiton a
	@param bitmap pointer to bitmap data 
	@param w width of the bitmap
	@param h height of the bitmap
	@param color foreground colour
	@param bg background colour.
	@param sizeOfBitmap size of the bitmap
	@return Will return true for success, false for failure
		Failure could be  out of bounds , wrong size , invalid pointer object.
	@note Variable drawBitmapAddr controls data addressing
		-# drawBitmapAddr  = true Vertical  data addressing
		-# drawBitmapAddr  = false Horizontal data addressing
		-# A vertical  Bitmap's h must be divisible by 8.for a  bitmap with wh=128 & h=64.
		-# Bitmap excepted size = 128 * (64/8) = 1024 bytes.
		-# A horizontal Bitmap's w must be divisible by 8. For a bitmap with w=88 & h=48.
		-# Bitmap excepted size = (88/8) * 48 = 528 bytes.
*/
rpiDisplay_Return_Codes_e bicolor_graphics::drawBitmap(int16_t x, int16_t y,
				const uint8_t *bitmap, int16_t w, int16_t h,
				uint8_t color, uint8_t bg, uint16_t sizeOfBitmap)
{
	// 1. User error check : Completely out of bounds?
	if (x > _width || y > _height)
	{
		printf("Error 1 : drawBitmap : Bitmap co-ord out of bounds, check x and y\n");
		return rpiDisplay_BitmapScreenBounds ;
	}
	// 2. User error check  bitmap weight and height
	if (w > _width || h > _height)
	{
		printf("Error 2 : drawBitmap : Bitmap is larger than screen, check w and h\n");
		return rpiDisplay_BitmapLargerThanScreen ;
	}
	// 3. User error check  bitmap is null
	if(bitmap == nullptr)
	{
		printf("Error 3 : drawBitmap : Bitmap is is not valid pointer\n");
		return rpiDisplay_BitmapNullptr ;
	}

if (_drawBitmapAddr== true)
{
	if((sizeOfBitmap != (w * (h/8))) || (h % 8 != 0) ) 	// 4A.check vertical bitmap size
	{
		printf("Error 4A : drawBitmap : vertical Bitmap size is incorrect:   %u  %i  %i \n", sizeOfBitmap , w , h);
		printf("Check size =  (w*(h/8) or Is bitmap height  divisible evenly by eight or is all bitmap data there or too much \n");
		return rpiDisplay_BitmapSize;
	}
	// Vertical byte bitmaps mode
	uint8_t vline;
	int16_t i, j, r = 0, yin = y;

	for (i=0; i<(w+1); i++ ) {
		if (r == (h+7)/8 * w) break;
		vline = bitmap [ r] ;
		r++;
		if (i == w) {
			y = y+8;
			i = 0;
		}

		for (j=0; j<8; j++ ) {
			if (y+j-yin == h) break;
			if (vline & 0x1) {
				drawPixel(x+i, y+j, color);
			}
			else {
				drawPixel(x+i, y+j, bg);
			}
			vline >>= 1;
		}
	}
} else if (_drawBitmapAddr == false) {
	if((sizeOfBitmap != ((w/8) * h)) || (w % 8 != 0)) // 4B.check Horizontal bitmap size
	{
		printf("Error 4B: drawBitmap : Horizontal Bitmap size is incorrect:  Check Size =  (w/8 * h): %u  %i  %i \n", sizeOfBitmap , w , h);
		printf("Check size = (w/8 *h) or Is bitmap width divisible evenly by eight or is all bitmap data there or too much \n");
		return rpiDisplay_BitmapSize;
	}
	// Horizontal byte bitmaps mode
	int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	for (int16_t j = 0; j < h; j++, y++)
	{
		for (int16_t i = 0; i < w; i++)
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = bitmap[j * byteWidth + i / 8];
			drawPixel(x+i, y, (byte & 0x80) ? color : bg);
		}
	}
} // end of elseif
return rpiDisplay_Success;
} // end of function


/*!
	@brief sets the data addressing mode in drawBitmap function.
	@param  mode boolean mode  , true default
		-# True =  bitmap data vertically addressed
		-# False = bitmap data horizontally addressed
*/
void bicolor_graphics::setDrawBitmapAddr(bool mode) {
	_drawBitmapAddr = mode;
}

/*!
	@brief turn on or off screen wrap of the text
	@param w TRUE on
*/
void bicolor_graphics::setTextWrap(bool w) {
	_textwrap = w;
}
