/*!
	@file     color16_graphics_RDL.hpp
	@author   Gavin Lyons
	@brief    Library header file for 16 bit (565) color graphics library.
				This file handles the graphic methods
	@details Note  USER OPTION for advanced screen buffer mode, its off by default
*/

#pragma once

// Includes
#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include <lgpio.h>
#include "print_data_RDL.hpp"
#include "font_data_RDL.hpp"
#include "common_data_RDL.hpp"

// ===== USER OPTION turns on advanced screen buffer mode if commented in =
// Default is off, commented OUT , tested in file examples/st7735/advanced_screen_buffer_mode
//#define color16_ADVANCED_SCREEN_BUFFER_ENABLE
// ================================================================

/*!
	@brief Class to handle fonts and graphics of color 16 bit display
*/
class color16_graphics:public display_Fonts, public Print  {

 public:

	color16_graphics(); // Constructor
	~color16_graphics(){};

	/*!
	 * @brief 16-bit color definitions (RGB565 format).
	 */
	enum  pixel_color565_e : uint16_t
	{
		RDLC_BLACK   = 0x0000, /**< Black */
		RDLC_BLUE    = 0x001F, /**< Blue */
		RDLC_RED     = 0xF800, /**< Red */
		RDLC_GREEN   = 0x07E0, /**< Green */
		RDLC_CYAN    = 0x07FF, /**< Cyan */
		RDLC_MAGENTA = 0xF81F, /**< Magenta */
		RDLC_YELLOW  = 0xFFE0, /**< Yellow */
		RDLC_WHITE   = 0xFFFF, /**< White */
		RDLC_TAN     = 0xED01, /**< Tan */
		RDLC_GREY    = 0x9CD1, /**< Grey */
		RDLC_BROWN   = 0x6201, /**< Brown */
		RDLC_DGREEN  = 0x01C0, /**< Dark Green */
		RDLC_ORANGE  = 0xFC00, /**< Orange */
		RDLC_NAVY    = 0x000F, /**< Navy */
		RDLC_DCYAN   = 0x03EF, /**< Dark Cyan */
		RDLC_MAROON  = 0x7800, /**< Maroon */
		RDLC_PURPLE  = 0x780F, /**< Purple */
		RDLC_OLIVE   = 0x7BE0, /**< Olive */
		RDLC_LGREY   = 0xC618, /**< Light Grey */
		RDLC_DGREY   = 0x7BEF, /**< Dark Grey */
		RDLC_GYELLOW = 0xAFE5, /**< Greenish Yellow */
		RDLC_PINK    = 0xFC18, /**< Pink */
		RDLC_LBLUE   = 0x7E5F, /**< Light Blue */
		RDLC_BEIGE   = 0xB5D2  /**< Beige */
};

	/*! TFT rotate modes in degrees*/
	enum  display_rotate_e : uint8_t
	{
		Degrees_0 = 0, /**< No rotation 0 degrees*/
		Degrees_90,    /**< Rotation 90 degrees*/
		Degrees_180,   /**< Rotation 180 degrees*/
		Degrees_270    /**< Rotation 270 degrees*/
	};

	// buffer screen mode functions
#ifdef color16_ADVANCED_SCREEN_BUFFER_ENABLE
	rdlib::Return_Codes_e setBuffer(void);
	rdlib::Return_Codes_e clearBuffer(uint16_t color =RDLC_BLACK);
	rdlib::Return_Codes_e writeBuffer(void);
	rdlib::Return_Codes_e destroyBuffer(void);
#endif

	// Screen related
	/*! @brief define in the sub class */
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) = 0;
	void fillScreen(uint16_t color);
	void setCursor(int16_t x, int16_t y);

	// Shapes and lines
	void drawPixel(uint16_t, uint16_t, uint16_t);
	rdlib::Return_Codes_e drawDotGrid(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t DotGridGap, uint16_t color);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	rdlib::Return_Codes_e drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
	rdlib::Return_Codes_e drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
	void drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, int offset, uint16_t color);
	void drawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	rdlib::Return_Codes_e fillRectangle(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
	void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
	void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void drawEllipse(int16_t cx, int16_t cy, int16_t semiMajorAxis, int16_t semiMinorAxis, bool fill, uint16_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void drawQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
	void fillQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
	rdlib::Return_Codes_e drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rotation, bool fill , uint16_t color);
	void drawSimpleArc(int16_t cx, int16_t cy, int16_t radius, float startAngle, float endAngle, uint16_t color);
	void drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float startAngle, float endAngle, uint16_t color);
	float getArcAngleMax() const;
	void setArcAngleMax(float arcAngleMax);
	int getArcAngleOffset() const;
	void setArcAngleOffset(int arcAngleOffset);
	
	// Text related functions
	virtual size_t write(uint8_t) override;
	rdlib::Return_Codes_e writeChar( int16_t x, int16_t y, char value );
	rdlib::Return_Codes_e writeCharString( int16_t x, int16_t y, char *text);
	void setTextWrap(bool w);
	void setTextColor(uint16_t c, uint16_t bg);
	void setTextColor(uint16_t c);
	void setTextCharPixelOrBuffer(bool mode);
	bool getTextCharPixelOrBuffer() const;

	// Bitmap & Icon
	rdlib::Return_Codes_e drawIcon(uint16_t x, uint16_t y, uint16_t w, uint16_t color, uint16_t bgcolor,const std::span<const uint8_t> data);
	rdlib::Return_Codes_e drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t bgcolor, const std::span<const uint8_t> data);
	rdlib::Return_Codes_e drawBitmap24(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h);
	rdlib::Return_Codes_e drawBitmap16(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h);
	rdlib::Return_Codes_e drawSprite(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h, uint16_t backgroundColor, bool printBg = false);
	// RGB to 565
	int16_t Color565(int16_t ,int16_t , int16_t );
	//SPI 
	int getDisplaySPIBlockSize() const;
	void setDisplaySPIBlockSize(int size);

protected:

	void pushColor(uint16_t color);
	void writeCommand(uint8_t);
	void writeData(uint8_t);
	uint8_t spiRead(void);
	rdlib::Return_Codes_e  spiWrite(uint8_t);
	rdlib::Return_Codes_e  spiWriteDataBuffer(uint8_t* spidata, int len);

	bool _textwrap = true;           /**< wrap text around the screen on overflow*/
	uint16_t _textcolor = 0xFFFF ;   /**< 16 bit ForeGround color for text*/
	uint16_t _textbgcolor = 0x0000 ; /**< 16 bit BackGround color for text*/
	

	int16_t _cursorX = 0; /**< Current X co-ord cursor position */
	int16_t _cursorY = 0; /**< Current Y co-ord cursor position */
	uint16_t _width;      /**< Display w as modified by current rotation*/
	uint16_t _height;     /**< Display h as modified by current rotation*/
	uint8_t _XStart= 0;   /**< Used to store _colstart changed by current rotation */
	uint8_t _YStart= 0;   /**< Used to store _rowstart changed by current rotation */
	float _arcAngleMax = 360.0f; /**< Maximum angle of Arc , used by drawArc*/
	int _arcAngleOffset= 0; /**< used by drawArc, offset for adjusting the starting angle of arc. default positive X-axis (0°)*/

	int8_t _Display_DC;    /**< GPIO for data or command line */
	int8_t _Display_RST;   /**< GPIO for reset line */
	int8_t _Display_CS;    /**< GPIO for chip select line,  Software SPI only */
	int8_t _Display_SCLK;  /**< GPIO for Clock line,  Software SPI only */
	int8_t _Display_SDATA; /**< GPIO for MOSI line,  Software SPI only */
	int8_t _Display_MISO;  /**< GPIO for  MISO line, Software SPI only*/

	int _DeviceNumGpioChip = 0; /**< The device number of a gpiochip ls /dev/gpio */
	int _GpioHandle = 0;        /**< This holds a handle to a gpiochip device opened by lgGpiochipOpen  */

	bool _hardwareSPI=true;      /**< True for Hardware SPI on , false fpr Software SPI on*/
	uint16_t _HighFreqDelay = 0; /**< uS GPIO Communications delay, SW SPI ONLY */
	int _spiHandle = 0;          /**< Hold a handle for the SPI device on the channel.*/

private:
/// @cond
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
	void ellipseHelper(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color);
	void drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color);

	inline void swapint16t(int16_t& a, int16_t& b) 
	{
		int16_t t = a;
		a = b;
		b = t;
	}
/// @endcond
	int _Display_SPI_BLK_SIZE = 65536; /**< max block size SPI Transaction, lgpio lib default(LG_MAX_SPI_DEVICE_COUNT)*/
	bool _textCharPixelOrBuffer = false;  /**< Text character is drawn by buffer(false) or pixel(true) */
#ifdef color16_ADVANCED_SCREEN_BUFFER_ENABLE
	std::vector <uint8_t> _screenBuffer; /**< Buffer for screen used by advanced screen buffer mode, off by default*/
#endif
};
// ********************** EOF *********************
