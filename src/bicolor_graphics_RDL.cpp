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
	@param  y character starting position on y-axis.
	@param  value Character to be written.
	@note Horizontal font addressing 
	@return Will return rdlib::Return_Codes_e enum
		-# rdlib::Success  success
		-# rdlib::CharScreenBounds co-ords out of bounds check x and y
		-# rdlib::CharFontASCIIRange Character out of ASCII Font bounds, check Font range
 */
rdlib::Return_Codes_e bicolor_graphics::writeChar(int16_t x, int16_t y, char value) {

	// 1. Check for screen out of bounds
	if ((x >= _width) || // Clip right
		(y >= _height) || // Clip bottom
		((x + _Font_X_Size + 1) < 0) || // Clip left
		((y + _Font_Y_Size) < 0)) { // Clip top
		fprintf(stderr, "Error 1 : writeChar : Co-ordinates out of bounds \r\n");
		return rdlib::CharScreenBounds;
	}

	// 2. Check for character out of font range bounds
	if (value < _FontOffset || value >= (_FontOffset + _FontNumChars + 1)) {
		fprintf(stderr, "Error 2 : writeChar : Character out of Font bounds  %c : %u<->%u \r\n", value, _FontOffset,(unsigned int)(_FontOffset + _FontNumChars));
		return rdlib::CharFontASCIIRange;
	}

	uint16_t fontIndex = 0;
	int16_t colByte, cx, cy;
	int16_t colbit;
	fontIndex = ((value - _FontOffset)*((_Font_X_Size * _Font_Y_Size) / 8)) + 4;
	colByte = _FontSelect[fontIndex];
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
				colByte = _FontSelect[fontIndex];
			}
		}
	}
	return rdlib::Success;
}


/*!
	@brief Write Text character array on Display.
	@param  x character starting position on x-axis.
	@param  y character starting position on y-axis.
	@param  pText Pointer to the array of the text to be written.
	@return Will return
		-# rdlib::Success Success
		-# rdlib::CharArrayNullptr  String pText Array invalid pointer object
		-# 3 Failure in writeChar method upstream
 */
rdlib::Return_Codes_e  bicolor_graphics::writeCharString(int16_t x, int16_t y, char * pText) {
	uint8_t count=0;
	uint8_t MaxLength=0;
	// Check for null pointer
	if(pText == nullptr)
	{
		fprintf(stderr, "Error 1 :writeCharString :  String array is not valid pointer\n");
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
		if (MaxLength >= 150) break; // 2nd way out of loop, safety check
	}
	return rdlib::Success;
}

/*!
	@brief write method used in the print class when user calls print
	@param character the character to print
	@return Will return
		-# 1. success
		-# rdlib::Return_Codes_e enum error code An error in the writeChar method.
*/
size_t bicolor_graphics::write(uint8_t character)
{
	rdlib::Return_Codes_e DrawCharReturnCode;
	switch (character)
	{
		case '\n':
			_cursor_y += _Font_Y_Size;
			_cursor_x  = 0;
		break;
		case '\r': break;
		default:
			DrawCharReturnCode = writeChar(_cursor_x, _cursor_y, character);
			if (DrawCharReturnCode != rdlib::Success) 
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
	@param centerX The x-coordinate of the circle's center.
	@param centerY The y-coordinate of the circle's center.
	@param radius The radius of the circle.
	@param color The color of the circle
*/
void bicolor_graphics::drawCircle(int16_t centerX, int16_t centerY, int16_t radius,
	uint8_t color) {
	// Initial decision parameter for the circle drawing algorithm
	int16_t decisionParam = 1 - radius;
	// Differences for circle drawing in the x and y directions
	int16_t deltaX = 1;
	int16_t deltaY = -2 * radius;
	// Starting coordinates
	int16_t x = 0;
	int16_t y = radius;
	// Draw the initial points on the circle (4 points)
	drawPixel(centerX, centerY + radius, color);
	drawPixel(centerX, centerY - radius, color);
	drawPixel(centerX + radius, centerY, color);
	drawPixel(centerX - radius, centerY, color);
	// Apply the circle drawing algorithm to plot points around the circle
	while (x < y)
	{
		// If the decision parameter is positive or zero, adjust y and deltaY
		if (decisionParam >= 0)
		{
			y--;
			deltaY += 2;
			decisionParam += deltaY;
		}
		// Always adjust x and deltaX
		x++;
		deltaX += 2;
		decisionParam += deltaX;
		// Draw the 8 symmetrical points of the circle for each iteration
		drawPixel(centerX + x, centerY + y, color);
		drawPixel(centerX - x, centerY + y, color);
		drawPixel(centerX + x, centerY - y, color);
		drawPixel(centerX - x, centerY - y, color);
		drawPixel(centerX + y, centerY + x, color);
		drawPixel(centerX - y, centerY + x, color);
		drawPixel(centerX + y, centerY - x, color);
		drawPixel(centerX - y, centerY - x, color);
	}
}

/// @cond
/*!
	@brief Used internally by drawRoundRect
	@param centerX The x-coordinate of the circle's center.
	@param centerY The y-coordinate of the circle's center.
	@param radius The radius of the circle.
	@param cornerFlags A bitmask indicating which corners of the circle to draw.
	@param color The color of the circle.
*/
void bicolor_graphics::drawCircleHelper( int16_t centerX, int16_t centerY,
				 int16_t radius, uint8_t cornerFlags, uint8_t color) {
	// Initial decision parameter for the circle drawing algorithm
	int16_t decisionParam = 1 - radius;
	// Differences for circle drawing in the x and y directions
	int16_t deltaX = 1;
	int16_t deltaY = -2 * radius;
	// Starting coordinates
	int16_t x = 0;
	int16_t y = radius;
	// Apply the circle drawing algorithm to plot points in the specified corners
	while (x < y)
	{
		// If the decision parameter is positive or zero, adjust y and deltaY
		if (decisionParam >= 0)
		{
			y--;
			deltaY += 2;
			decisionParam += deltaY;
		}
		// Always adjust x and deltaX
		x++;
		deltaX += 2;
		decisionParam += deltaX;
		// Draw the points for each corner based on the cornerFlags
		if (cornerFlags & 0x4) // Top-right corner
		{
			drawPixel(centerX + x, centerY + y, color);
			drawPixel(centerX + y, centerY + x, color);
		}
		if (cornerFlags & 0x2) // Bottom-right corner
		{
			drawPixel(centerX + x, centerY - y, color);
			drawPixel(centerX + y, centerY - x, color);
		}
		if (cornerFlags & 0x8) // Top-left corner
		{
			drawPixel(centerX - y, centerY + x, color);
			drawPixel(centerX - x, centerY + y, color);
		}
		if (cornerFlags & 0x1) // Bottom-left corner
		{
			drawPixel(centerX - y, centerY - x, color);
			drawPixel(centerX - x, centerY - y, color);
		}
	}
}

/*!
	@brief Internal helper function used by fillCircle and fillRoundRect to fill parts of a circle.
	@param centerX The x-coordinate of the circle's center.
	@param centerY The y-coordinate of the circle's center.
	@param radius The radius of the circle.
	@param cornerFlags A bitmask indicating which parts of the circle to fill.
	@param verticalOffset An additional vertical offset to adjust the line length.
	@param color The color to fill the circle with.
*/
void bicolor_graphics::fillCircleHelper(int16_t centerX, int16_t centerY, int16_t radius,
										   uint8_t cornerFlags, int16_t verticalOffset, uint8_t color){
	// Initial decision parameter for the circle filling algorithm
	int16_t decisionParam = 1 - radius;
	// Differences for circle drawing in the x and y directions
	int16_t deltaX = 1;
	int16_t deltaY = -2 * radius;
	// Starting coordinates
	int16_t x = 0;
	int16_t y = radius;
	// Apply the circle filling algorithm to plot vertical lines at the specified points
	while (x < y)
	{
		// If the decision parameter is positive or zero, adjust y and deltaY
		if (decisionParam >= 0)
		{
			y--;
			deltaY += 2;
			decisionParam += deltaY;
		}
		// Always adjust x and deltaX
		x++;
		deltaX += 2;
		decisionParam += deltaX;
		// Draw the vertical lines for each part of the circle based on the cornerFlags
		if (cornerFlags & 0x1) // Bottom-right corner
		{
			drawFastVLine(centerX + x, centerY - y, 2 * y + 1 + verticalOffset, color);
			drawFastVLine(centerX + y, centerY - x, 2 * x + 1 + verticalOffset, color);
		}
		if (cornerFlags & 0x2) // Bottom-left corner
		{
			drawFastVLine(centerX - x, centerY - y, 2 * y + 1 + verticalOffset, color);
			drawFastVLine(centerX - y, centerY - x, 2 * x + 1 + verticalOffset, color);
		}
	}
}

/// @endcond

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
	swapint16t(x0, y0);
	swapint16t(x1, y1);
	}

	if (x0 > x1) {
	swapint16t(x0, x1);
	swapint16t(y0, y1);
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
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::ShapeScreenBounds Error
*/
rdlib::Return_Codes_e bicolor_graphics::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint8_t color) {
	if ((x >= _width) || (y >= _height))
		return rdlib::ShapeScreenBounds;
	drawLine(x, y, x, y+h-1, color);
	return rdlib::Success;
}

/*!
	@brief Draws a horizontal line starting at (x,y) with width w.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param w The width of the line
	@param color The color of the line
	@return enum rdlib::Return_Codes_e 
		-# rdlib::Success for success
		-# rdlib::ShapeScreenBounds Error
*/
rdlib::Return_Codes_e  bicolor_graphics::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint8_t color) {
	if ((x >= _width) || (y >= _height))
		return rdlib::ShapeScreenBounds;
	drawLine(x, y, x+w-1, y, color);
	return rdlib::Success;
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
	@brief Fills a triangle defined by the coordinates (x0, y0), (x1, y1), and (x2, y2).
	@param startX0 The x-coordinate of the first vertex.
	@param startY0 The y-coordinate of the first vertex.
	@param startX1 The x-coordinate of the second vertex.
	@param startY1 The y-coordinate of the second vertex.
	@param startX2 The x-coordinate of the third vertex.
	@param startY2 The y-coordinate of the third vertex.
	@param color The color to fill the triangle.
*/
void bicolor_graphics::fillTriangle (int16_t startX0, int16_t startY0,
									int16_t startX1, int16_t startY1,
									int16_t startX2, int16_t startY2, uint8_t color) {
	// Temporary variables for line drawing
	int16_t leftX, rightX, y, lastY;
	// Sort vertices by their Y-coordinates to ensure the top-to-bottom orientation
	if (startY0 > startY1)
	{
		swapint16t(startY0, startY1);
		swapint16t(startX0, startX1);
	}
	if (startY1 > startY2)
	{
		swapint16t(startY2, startY1);
		swapint16t(startX2, startX1);
	}
	if (startY0 > startY1)
	{
		swapint16t(startY0, startY1);
		swapint16t(startX0, startX1);
	}
	// If the triangle is flat (top and bottom vertices are the same y-coordinate)
	if (startY0 == startY2)
	{
		leftX = rightX = startX0;
		if (startX1 < leftX)
			leftX = startX1;
		else if (startX1 > rightX)
			rightX = startX1;
		if (startX2 < leftX)
			leftX = startX2;
		else if (startX2 > rightX)
			rightX = startX2;
		drawFastHLine(leftX, startY0, rightX - leftX + 1, color);
		return;
	}
	// Differences in x and y for the edges of the triangle
	int16_t dx01 = startX1 - startX0,
			dy01 = startY1 - startY0,
			dx02 = startX2 - startX0,
			dy02 = startY2 - startY0,
			dx12 = startX2 - startX1,
			dy12 = startY2 - startY1;
	// Accumulated error terms for drawing the triangle
	int32_t sa = 0, sb = 0;
	// Determine the last y-coordinate for the top part of the triangle
	if (startY1 == startY2)
		lastY = startY1;
	else
		lastY = startY1 - 1;
	// Draw the upper part of the triangle
	for (y = startY0; y <= lastY; y++)
	{
		leftX = startX0 + sa / dy01;
		rightX = startX0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		// Ensure leftX is always less than rightX
		if (leftX > rightX)
			swapint16t(leftX, rightX);

		// Draw the horizontal line for this y-coordinate
		drawFastHLine(leftX, y, rightX - leftX + 1, color);
	}
	// Reset error terms for the lower part of the triangle
	sa = dx12 * (y - startY1);
	sb = dx02 * (y - startY0);
	// Draw the lower part of the triangle
	for (; y <= startY2; y++)
	{
		leftX = startX1 + sa / dy12;
		rightX = startX0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		// Ensure leftX is always less than rightX
		if (leftX > rightX)
			swapint16t(leftX, rightX);
		// Draw the horizontal line for this y-coordinate
		drawFastHLine(leftX, y, rightX - leftX + 1, color);
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
	@return _rotation value enum
*/
bicolor_graphics::displayBC_rotate_e  bicolor_graphics::getRotation(void){return _display_rotate;}

 /*!
	@brief Sets the _rotation of the display
	@param CurrentRotation value enum
*/
void bicolor_graphics::setRotation(displayBC_rotate_e  CurrentRotation) {

	_display_rotate = CurrentRotation;
	switch(CurrentRotation)
	{
		case BC_Degrees_0:
		case BC_Degrees_180:
			_width  = WIDTH;
			_height = HEIGHT;
			break;
		case BC_Degrees_90 :
		case BC_Degrees_270:
			_width  = HEIGHT;
			_height = WIDTH;
			break;
	}
}

/*!
	@brief Draw a 1-bit color bitmap
	@param x x co-ord position
	@param y y co-ord posiiton a
	@param bitmap span to bitmap data 
	@param w width of the bitmap
	@param h height of the bitmap
	@param color foreground colour
	@param bg background colour.
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
rdlib::Return_Codes_e bicolor_graphics::drawBitmap(int16_t x, int16_t y,
				const std::span<const uint8_t> bitmap, int16_t w, int16_t h,
				uint8_t color, uint8_t bg)
{
	// 1. User error check : Completely out of bounds?
	if (x > _width || y > _height)
	{
		fprintf(stderr, "Error 1 : drawBitmap : Bitmap co-ord out of bounds, check x and y\n");
		return rdlib::BitmapScreenBounds ;
	}
	// 2. User error check  bitmap weight and height
	if (w > _width || h > _height)
	{
		fprintf(stderr, "Error 2 : drawBitmap : Bitmap is larger than screen, check w and h\n");
		return rdlib::BitmapLargerThanScreen ;
	}
	// 3. User error check  bitmap is empty
	if(bitmap.empty())
	{
		fprintf(stderr, "Error 3 : drawBitmap : Bitmap is not valid object\n");
		return rdlib::BitmapDataEmpty;
	}

if (_drawBitmapAddr== true)
{
	if(h % 8 != 0 )
	{
		fprintf(stderr, "Error drawBitmap 4A-1: Check is bitmap height divisible evenly by eight\n");
		return rdlib::BitmapVerticalSize;
	}
	if (bitmap.size() != static_cast<size_t>(w * (h / 8))) // 4A-1 check  bitmap size
	{
		fprintf(stderr, "Error drawBitmap 4A-2 : Vertical Bitmap size is incorrect: (w * (h / 8)\n");
		return rdlib::BitmapSize;
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
} else {
	if(w % 8 != 0 )
	{
		fprintf(stderr, "Error drawBitmap 4B-1 : Check is bitmap width divisible evenly by eight\n");
		return rdlib::BitmapHorizontalSize;
	}
	if (bitmap.size() != static_cast<size_t>((w / 8) * h))
	{
		fprintf(stderr, "Error drawBitmap 4B-2: Horizontal Bitmap size is incorrect:  Check Size =  (w/8 * h)\n");
		return rdlib::BitmapSize;
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
return rdlib::Success;
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

/*!
	@brief Get the current maximum angle of the arc.
	@return The current maximum angle in degrees.
*/
float bicolor_graphics::getArcAngleMax() const {
	return _arcAngleMax;
}
/*!
	@brief Set a new maximum angle for the arc.
	@param arcAngleMax The new maximum angle in degrees (should be positive).
*/
void bicolor_graphics::setArcAngleMax(float arcAngleMax) {
	if (arcAngleMax > 0) {  // Ensure the max angle is positive
		_arcAngleMax = arcAngleMax;
	}
}
/*!
	@brief Get the current angle offset.
	@return The current angle offset in degrees.
*/
int bicolor_graphics::getArcAngleOffset() const {
	return _arcAngleOffset;
}
/*!
	@brief Set a new angle offset.
	@param arcAngleOffset The new angle offset in degrees.
*/
void bicolor_graphics::setArcAngleOffset(int arcAngleOffset) {
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
rdlib::Return_Codes_e bicolor_graphics::drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rotation, bool fill , uint8_t color) 
{
	if ((sides < 3 ) || (sides > 10)) {
		fprintf(stderr, "Error : drawPolygon : sides incorrect value 3-10\r\n");
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
rdlib::Return_Codes_e bicolor_graphics::drawDotGrid(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t DotGridGap, uint8_t color)
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
	@param color The color of the line
 */
void bicolor_graphics::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, int offset , uint8_t color)
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
void bicolor_graphics::drawQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint8_t color) 
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
 */
void bicolor_graphics::fillQuadrilateral(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color)
{
	// Compute the cross product of vectors (x1-x0, y1-y0) and (x2-x0, y2-y0)
	// to determine convexity
	int32_t crossProduct = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
	if (crossProduct >= 0) {
		// Convex case (or degenerate, treat as convex)
		fillTriangle(x0, y0, x1, y1, x2, y2, color);
		fillTriangle(x2, y2, x3, y3, x0, y0, color);
	} else {
		// Concave case: Choose the alternative diagonal
		fillTriangle(x1, y1, x2, y2, x3, y3, color);
		fillTriangle(x3, y3, x0, y0, x1, y1, color);
	}
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
	@param color color value for the ellipse.
 */
void bicolor_graphics::drawEllipse(int16_t cx, int16_t cy, int16_t semiMajorAxis, int16_t semiMinorAxis,  bool fill, uint8_t color)
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
	@param color  color value.
 */
void bicolor_graphics::ellipseHelper(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint8_t color)
{
	drawPixel(cx + x, cy + y, color);
	if (x != 0) drawPixel(cx - x, cy + y, color);
	if (y != 0) drawPixel(cx + x, cy - y, color);
	if (x != 0 && y != 0) drawPixel(cx - x, cy - y, color);
}

/*!
	@brief helps drawARC draw an Arc on screen 
	@param centerX X-coordinate of the center of the arc
	@param centerY Y-coordinate of the center of the arc
	@param radius The radius of the arc
	@param thickness the thickness of the arc
	@param start Starting angle of arc
	@param end End angle of arc
	@param color The color of the arc.
	@details @details This function uses a modified midpoint circle algorithm combined with scanline filling
		to efficiently draw an arc with a specified thickness. It calculates pixel positions using 
		trigonometric boundary checks and slopes for accurate rendering. 
		For more information, see: https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
*/
void bicolor_graphics::drawArcHelper(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t thickness, float start, float end, uint8_t color) 
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
	@note variables _arcAngleMax and  _arcAngleOffset which can be set by user with setters
		define behaviour of arc. 
		_arcAngleOffset is zero default means 0 is positive X axis , Arc is drawn counterclockwise
		XXX 270 XXX
		180 XXX 000
		XXX 090 XXX
 */
void bicolor_graphics::drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float startAngle, float endAngle, uint8_t color) {
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
void bicolor_graphics::drawSimpleArc(int16_t cx, int16_t cy, int16_t radius, float startAngle, float endAngle, uint8_t color)
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
