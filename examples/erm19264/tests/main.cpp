/*!
	@file  examples/erm19264/tests/main.cpp
	@brief Example file for ERM19264_LCD display , tests library
			User selects hardware or software SPI based on MARCO hardwareSPI in USER OPTION below
			Project Name: Display_Lib_RPI
	@test
		-# Test 301 LCD bitmap method
		-# Test 303 LCD update init method
		-# Test 304 Drawbitmap to buffer method, vertical addressing
		-# Test 305 Drawbitmap to buffer method, horizontal addressing
		-# Test 501 All Pixels on
		-# Test 502 Invert screen
		-# Test 503 Flip 180 with LCD rotate command screen
		-# Test 504 Scroll Screen
		-# Test 505 Enable and disable Screen
		-# Test 506 Rotate software buffer
		-# Test 601 FPS frame rate per second HW SPI
		-# Test 701 Default font
		-# Test 702 gll font
		-# Test 703 Pico font
		-# Test 704 Sinclair font
		-# Test 705 Orla Font
		-# Test 706 Retro Font
		-# Test 707 Mega font
		-# Test 708 Arial bold font
		-# Test 709 hallFetica font
		-# Test 710 Arial round font
		-# Test 711 GroTesk font
		-# Test 712 Sixteen Segment font
		-# Test 713 Display ASCII font 32-127 default font
		-# Test 714 Base number systems using print method
		-# Test 715 println + print & textwrap
		-# Test 716 print method String object
		-# Test 717 print method numbers
		-# Test 718 Fonts grotesk big , inconsola, mint and seven segment
		-# Test 901 Graphics
		-# Test 905 Polygons
		-# Test 906 Dot Grid
		-# Test 907 Quadrilateral
		-# Test 908 Ellipse
		-# Test 909 Draw Arc and draw simple arc
*/


#include <cstdio>
#include "Bitmap_test_data.hpp" // Bitmap test data file
#include "ERM19264_UC1609_LCD_RDL.hpp"

/// @cond

// *** USER OPTION ***
//comment out for software SPI, leave in for hardware SPI 
#define hardwareSPI

// Screen
const uint8_t MY_LCD_WIDTH  = 192;
const uint8_t MY_LCD_HEIGHT = 64;
#define  MY_SCREEN_SIZE (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8)) 
const uint8_t LCDcontrast = 0x49; // Contrast 00 to FF , 0x49 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

// SPi
#ifdef hardwareSPI
	// Hardware SPI setup
	//GPIO
	const uint8_t RST = 25; // GPIO pin number pick any you want
	const uint8_t CD = 24; // GPIO pin number pick any you want
	// instantiate  an object for HW SPI
	ERM19264_UC1609 myLCD(MY_LCD_WIDTH ,MY_LCD_HEIGHT , RST, CD) ; 
#else
	// Software SPI setup
	// GPIO pin number pick any you want
	const uint8_t RST = 25;
	const uint8_t CD = 24;
	const uint8_t DIN = 5;
	const uint8_t SCLK = 6;
	const uint8_t CS = 21;
	// software SPI constructor
	ERM19264_UC1609 myLCD(MY_LCD_WIDTH ,MY_LCD_HEIGHT , RST, CD, CS, SCLK,DIN);
#endif

// Vars for tests
#define DisplayDelay1 9000
#define DisplayDelay2 5
uint8_t graphicsCountLimit = 25;
// vars for the test framerate
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;
uint16_t countLimit = 1000;

// 192x64px bitmap data,'UC1609' SW used to make https://github.com/gavinlyonsrepo/Guardian_LTSM/ vertical addressing
// The buffer will be assigned this data at init
// Note this is the screen buffer filled with a Bitmap , can only be used once startup.
// This is not a bitmap but bitmap data pre-loaded into the screen buffer array.
uint8_t screenBuffer192x64[1536] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0xc0, 0xe0, 0x60, 0x30, 0x30, 0x38, 0x18, 0x18, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x8c, 0x8c, 0x8c, 0x8c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x18, 0x18, 0x38, 0x30, 0x30, 0x60, 0xe0, 0xc0,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf8, 0x3e, 0x0f,
	0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x07, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
	0xff, 0xff, 0x01, 0x01, 0x01, 0x00, 0x00, 0xe0, 0xf0, 0xf8, 0x7c, 0x1c, 0x0e, 0x06, 0x03, 0x03,
	0x03, 0x01, 0x01, 0x03, 0x03, 0x02, 0x06, 0x0e, 0x3e, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x04, 0x06, 0xfe, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0xe0, 0xf0, 0xf8, 0x1c, 0x0e, 0x06, 0x83, 0x81, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0xf0, 0xfc, 0x3e, 0x06, 0x03, 0x01, 0x01, 0x01, 0x03, 0x06, 0x3e, 0xfc, 0xf0, 0x80, 0x00,
	0x00, 0xe0, 0xf8, 0xfc, 0x0e, 0x02, 0x01, 0x01, 0x01, 0x01, 0x03, 0x0e, 0xfc, 0xf8, 0xf0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x03, 0x07, 0x1e, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
	0xff, 0xff, 0x07, 0x03, 0x01, 0x01, 0x01, 0x01, 0x03, 0x07, 0x1f, 0xfe, 0xfc, 0xf0, 0x00, 0x00,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
	0x00, 0x03, 0x0f, 0x1f, 0x1e, 0x38, 0x30, 0x30, 0x30, 0x30, 0x10, 0xd0, 0xff, 0xff, 0x7f, 0x07,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x07, 0x1f, 0x3f, 0x3c, 0x70, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x20, 0x30, 0x18,
	0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0f, 0x1f, 0x3c, 0x38, 0x30, 0x60, 0x60,
	0x60, 0x60, 0x60, 0x60, 0x60, 0x20, 0x20, 0x10, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x40, 0x40, 0x7f, 0x7f, 0x7f, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x07, 0x1f, 0x3f, 0x38, 0x60, 0x40, 0x40, 0x40, 0x40, 0x20, 0x38, 0x1f, 0x0f, 0x03, 0x00, 0x00,
	0x01, 0x07, 0x1f, 0x3e, 0x30, 0x60, 0x40, 0x40, 0x40, 0x60, 0x30, 0x3e, 0x1f, 0x07, 0x01, 0x00,
	0x00, 0x00, 0x40, 0x40, 0x40, 0x60, 0x20, 0x30, 0x18, 0x1c, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x1e,
	0x38, 0x70, 0xe0, 0xc0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xc0, 0xe0, 0x70,
	0x38, 0x1c, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// =============== Function prototype ================

void Test301(void);
void Test303(void);
void Test304(void);
void Test305(void);
void Test500(void);
void Test601(void);
void displayData(long , int );
static uint64_t counter( void );
void Test701(void);
void Test702(void);
void Test703(void);
void Test704(void);
void Test705(void);
void Test706(void);
void Test707(void);
void Test708(void);
void Test709(void);
void Test710(void);
void Test711(void);
void Test712(void);
void Test713(void);
void Test714(void);
void Test715(void);
void Test716(void);
void Test717(void);
void Test718(void);
void Test901(void);
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);
bool setupHWSPI(void);
bool setupSWSPI(void);
void myLoop(void);
void EndTest(void);
void TestReset(void);
void displayMenu(void);

// ======================= Main ===================
int main()
{
#ifdef hardwareSPI
	if(!setupHWSPI()) {return -1;}
#else
	if(!setupSWSPI()) {return -1;}
#endif
	myLoop();
	EndTest();

	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
bool setupHWSPI() {
	int GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0
	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	printf("TestBegin: HW SPI\r\n");
	printf("lgpio library Version: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version: %u\r\n", rdlib::LibraryVersion()); 
	delayMilliSecRDL(100);
	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		printf("Error 1202: Cannot start spi\n");
		return false;
	}
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	delayMilliSecRDL(1000);
	return true;
}

bool setupSWSPI() {
	int GPIO_CHIP_DEVICE = 0;
	printf("TestBegi: SW SPI\r\n");
	printf("lgpio library Version: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version: %u\r\n", rdlib::LibraryVersion()); 
	delayMilliSecRDL(100);
	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return false;
	}
	printf("Frequency Software SPI delay = %u uS\r\n", myLCD.LCD_HighFreqDelayGet()); 
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	delayMilliSecRDL(1000);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	printf("LCD End\r\n");
}

void myLoop()
{
	if (myLCD.LCDSetBufferPtr(MY_LCD_WIDTH, MY_LCD_HEIGHT, screenBuffer192x64) != rdlib::Success)
	{
		exit(-1);
	};
	int choice;
	do {
		displayMenu();
		if (scanf("%d", &choice) != 1) {
			// Clear the invalid input from the buffer
			int ch;
			while ((ch = getchar()) != '\n' && ch != EOF); 
			
			printf("Invalid input. Please enter a number from list.\n\n");
			continue;
		}
		switch (choice) {
			case 1:
				Test301();
				Test303();
				Test304();
				Test305();
			break;
			case 2:
				Test500();
			break;
			case 3:
				Test601();
			 break;
			case 4:
				Test701();
				Test702();
				Test703();
				Test704();
				Test705();
				Test706();
				Test707();
				Test708();
				Test709();
				Test710();
				Test711();
				Test712();
				Test713();
				Test714();
				Test715();
				Test716();
				Test717();
				Test718();
			break;
			case 5:
				Test901();
				Test905();
				Test906();
				Test907();
				Test908();
				Test909();
			break;
			case 6:
				printf("Exiting menu\n");
			break;
			default:
				printf("Invalid choice. Please try again.\n");
			break;
		}
		printf("\n");
	} while (choice != 6);

}

void Test301(void)
{
	// Method (1) LCD bitmap method , writes data directly to screen
	printf("Test301: LCDBitmap method, full screen\n");
	myLCD.LCDBitmap(0, 0 , MY_LCD_WIDTH, MY_LCD_HEIGHT, screenBuffer192x64);
	delayMilliSecRDL(5000);
	myLCD.LCDFillScreen(0x00);
	delayMilliSecRDL(1500);
}

void Test303(void)
{
	printf("Test303: LCDupdate method, full screen\n");
	 // Screen buffer array has been preloaded with data , just update!
	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
	myLCD.LCDFillScreen(0x00);
	delayMilliSecRDL(1500);
}

void Test304(void)
{
	printf("Test304: setDrawBitmapAddr method, 3 icons Data Vertical addressed \n");
	myLCD.LCDclearBuffer();   // Clear buffer
	myLCD.setDrawBitmapAddr(true); // for Bitmap Data Vertical addressed

	myLCD.drawBitmap(0,   5, SignalIconVa, 16, 8, myLCD.BLACK, myLCD.WHITE);
	myLCD.drawBitmap(100, 15, SignalIconVa, 16, 8, myLCD.WHITE, myLCD.BLACK);
	myLCD.drawBitmap(150, 5, BatIconVa, 16, 8 , myLCD.BLACK, myLCD.WHITE);

	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
}

void Test305(void)
{
	printf("Test305: setDrawBitmapAddr method, 3 icons Data Horizontal addressed \n");
	myLCD.LCDclearBuffer();   // Clear active buffer
	myLCD.setDrawBitmapAddr(false); // for Bitmap Data Horizontal addressed

	myLCD.drawBitmap(10, 25, SignalIconHa, 16, 8, myLCD.BLACK, myLCD.WHITE);
	myLCD.drawBitmap(100, 20, SignalIconHa, 16, 8, myLCD.WHITE, myLCD.BLACK);
	myLCD.drawBitmap(60, 20, SignalIconHa, 16, 8, myLCD.WHITE, myLCD.BLACK);

	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
}

void Test701(void)
{
	myLCD.LCDclearBuffer();
	printf("Test701 Default font\r\n");
	myLCD.setFont(font_default);
	char TestStr[] = "Default font";
	char InvertedStr[] = "Invertedfont";
	myLCD.writeChar(0,0 ,'a');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-33.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'5');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test702(void)
{
	printf("Test702 gll font, NO LOWERCASE LETTERS\r\n");
	myLCD.setFont(font_gll);
	char TestStr[] = "GLL FONT";
	char InvertedStr[] = "INVERT FONT.";
	myLCD.writeChar(0,0 ,'7');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'0');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("PRINT INV 2!");
	TestReset();
}

void Test703(void){
	printf("Test703 PICO font\r\n");
	myLCD.setFont(font_pico);
	myLCD.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Inverted font";
	myLCD.writeChar(0,0 ,'p');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-3.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'6');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test704(void){
	printf("Test704 Sinclair font\r\n");
	myLCD.setFont(font_sinclairS);
	myLCD.setInvertFont(false);
	char TestStr[] = "Sinclair font";
	char InvertedStr[] = "Inverted font";
	myLCD.writeChar(0,0 ,'u');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-2.19);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'5');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test705(void)
{
	printf("Test705 Orla font\r\n");
	myLCD.setFont(font_orla);
	myLCD.setInvertFont(false);
	char TestStr[] = "Orla F";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1,32);
	myLCD.print("invert");
	TestReset();
}

void Test706(void)
{
	printf("Test706 retro font\r\n");
	myLCD.setFont(font_retro);
	myLCD.setInvertFont(false);
	char TestStr[] = "Retro";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(0,16);
	myLCD.print(14.19);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, TestStr);
	myLCD.setCursor(0,48);
	myLCD.print("invert");
	TestReset();
}

void Test707(void){
	printf("Test707 Mega font\r\n");
	myLCD.setFont(font_mega);
	myLCD.setInvertFont(false);
	char TestStr[] = "Mega f.";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-8.14);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	myLCD.setCursor(5,48);
	myLCD.print("invert");
	TestReset();
}

void Test708(void){
	printf("Test708 Arial bold font\r\n");
	myLCD.setFont(font_arialBold);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial b";
	char InvertedStr[] = "Invert 1";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.1415, 2); // print-3.14
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	myLCD.setCursor(5,48);
	myLCD.print("invert");
	TestReset();
}

void Test709(void)
{
	printf("Test709 hallFetica font\r\n");
	myLCD.setFont(font_hallfetica);
	myLCD.setInvertFont(false);
	char TestStr[] = "Hall f.";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.17);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	myLCD.setCursor(5,48);
	myLCD.print("invert");
	TestReset();
}

void Test710(void){
	printf("Test710 Arial Round font\r\n");
	myLCD.setFont(font_arialRound);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial R";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1,32);
	myLCD.print("invert");
	TestReset();
}

void Test711(void)
{
	printf("Test711 GroTesk font\r\n");
	myLCD.setFont(font_groTesk);
	myLCD.setInvertFont(false);
	char TestStr[] = "GroTesk1";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(0,32);
	myLCD.print("inv 12!");
	TestReset();
}

void Test712(void)
{
	printf("Test712 Sixteen segment font\r\n");
	myLCD.setFont(font_sixteenSeg);
	myLCD.setInvertFont(false);

	myLCD.setCursor(0,0);
	myLCD.print(-3.141, 3);
	TestReset();
	myLCD.setInvertFont(true);
	char TestStr1[] = "12:30";
	myLCD.writeCharString(0,0, TestStr1);
	TestReset();
}

void Test713()
{
	printf("Test713 print ASCII font 32-126 \r\n");
	myLCD.setFont(font_default);
	myLCD.setInvertFont(false);
	myLCD.setCursor(0, 0);

	myLCD.print("ASCII font ");
	myLCD.setCursor(0, 15);

	// Print first 127 chars of font
	for (char i = 32; i < 126; i++)
	{
		myLCD.print(i);
	}
	TestReset();

}

void Test714(void)
{
	printf("Test714 Base number systems using print \r\n");
	myLCD.setFont(font_default);
	myLCD.setCursor(0, 0);
	myLCD.print(47 , myLCD.RDL_DEC);
	myLCD.setCursor(0, 16);
	myLCD.print(47 , myLCD.RDL_HEX);
	myLCD.setCursor(0, 32);
	myLCD.print(47, myLCD.RDL_BIN);
	myLCD.setCursor(0, 48);
	myLCD.print(47 , myLCD.RDL_OCT);
	TestReset();
}

void Test715(void)
{
	printf("Test715-a println + textwrap \r\n");
	myLCD.setCursor(0, 0);
	myLCD.println("first line");
	myLCD.print("2nd Line");
	myLCD.setCursor(60, 32);
	myLCD.setFont(font_gll);
	myLCD.print("123456789ABCDEFGHIJ");
	TestReset();
	printf("Test715-b writeText new line \r\n");
	char TestStr1[] = "123456789ABCDEFGHIJ";
	myLCD.writeCharString(60,0, TestStr1);
	TestReset();
}

void Test716(void)
{
	printf("Test716-a print method + string objects \r\n");
	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myLCD.setFont(font_default);
	myLCD.setCursor(5,5);
	myLCD.print(timeInfo);
	// print a new line with println
	myLCD.setCursor(5,40);
	myLCD.println(newLine); // print a new line feed with println
	myLCD.print(newLine);
	TestReset();
	
	printf("Test716-b  print vectors\r\n");
	// For a vector of integers
	myLCD.setCursor(10, 20);
	std::vector<int> intVec = {1, 2, 3, 4};
	myLCD.print(intVec[1]); // Output: "2 "
	
	// For a vector of strings
	myLCD.setCursor(10, 40);
	std::vector<std::string> stringVec = {"HELLO", "VECTOR"};
	myLCD.print(stringVec); // Output: "HELLO VECTOR"
	TestReset();
}

void Test717(void)
{
	printf("Test717 print method numbers\r\n");
	myLCD.setCursor(0, 0);
	myLCD.println(3.986,2 ); //print 3.99
	myLCD.println(4001);
	myLCD.println(-87.39);
	myLCD.println(-19);
	myLCD.println("hello");
	myLCD.setInvertFont(true);
	myLCD.print('P');
	TestReset();
}

void Test718(void){
	printf("Test718 mint, inconsola, 7 seg and grotesk big font  \r\n");
	myLCD.setFont(font_mint);
	myLCD.setCursor(5,5);
	myLCD.print("21.35`C"); // Note ` gives degree symbol
	TestReset();

	myLCD.setFont(font_inconsola);
	myLCD.setCursor(5,5);
	myLCD.print("11.49`C"); // Note ` gives degree symbol
	TestReset();

	myLCD.setFont(font_sevenSeg);
	myLCD.setCursor(5,5);
	myLCD.print(-9.123);
	TestReset();

	myLCD.setFont(font_groTeskBig);
	myLCD.setCursor(5,5);
	myLCD.print("BIG");
	TestReset();
}

// Function to display Graphics test 901
void  Test901()
{
	myLCD.LCDclearBuffer();
	printf("Test901 Basic graphics  \r\n");
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	//
	bool colourShape = 1;
	uint8_t testCount = 0;
	myLCD.setFont(font_default);
	myLCD.LCDclearBuffer(); // Clear the buffer
	while (testCount < graphicsCountLimit)
	{
		colourShape = !colourShape;

		//print count
		myLCD.setCursor(0, 0);
		myLCD.print(testCount);
		myLCD.setCursor(45, 0);
		myLCD.print(colourShape);
		// Draw the X
		myLCD.drawLine(96,  0, 96, 64, myLCD.BLACK);
		myLCD.drawFastVLine(94, 0, 64, myLCD.BLACK);
		myLCD.drawFastHLine(0, 32, 192, myLCD.BLACK);

		//Q1
		myLCD.fillRect(0, 10, 20, 20, colourShape);
		myLCD.fillCircle(40, 20, 10, myLCD.BLACK);
		myLCD.fillTriangle(60, 30, 70, 10, 80, 30, !colourShape);
		//Q2
		myLCD.drawRect(100, 10, 20, 20, myLCD.BLACK);
		myLCD.drawCircle(140, 20, 10, colourShape);
		myLCD.drawTriangle(160, 30, 170, 10, 180, 30, myLCD.BLACK);
		//Q3
		myLCD.fillRoundRect(0, 40, 40, 20, 10, !colourShape);
		myLCD.fillRoundRect(45, 40, 40, 20, 10, colourShape);
		//Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myLCD.drawRect(100 + i, 40 + i, 80 - i * 2, 20 - i * 2, myLCD.BLACK);
			myLCD.LCDupdate();
			delayMilliSecRDL(50);
		}
	myLCD.LCDclearBuffer();
	testCount++;
	}

} // end Display graphics

void Test500()
{
	myLCD.LCDclearBuffer();
	printf("Test500: Functions\r\n");
	// Test 501 LCD all pixels on
	myLCD.setCursor(20, 30);
	myLCD.print("All Pixels on");
	myLCD.LCDupdate();
	delayMilliSecRDL(4000);
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	myLCD.LCDallpixelsOn(1);
	delayMilliSecRDL(2000);
	myLCD.LCDallpixelsOn(0);
	delayMilliSecRDL(2000);

	// Test 502 inverse
	myLCD.setCursor(20, 30);
	myLCD.print("inverse test  ");
	myLCD.LCDupdate();
	myLCD.LCDinvert(0); // Normal
	delayMilliSecRDL(2000);
	myLCD.LCDinvert(1); // Inverted
	delayMilliSecRDL(4000);
	myLCD.LCDinvert(0);

	// Test 503 Flip 180 with LCD rotate command screen
	myLCD.LCDclearBuffer();
	myLCD.setCursor(20, 30);
	myLCD.print("flip 180  test");
	myLCD.LCDupdate();
	delayMilliSecRDL(2000);
	myLCD.LCDrotate(myLCD.UC1609_ROTATION_FLIP_ONE);
	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
	myLCD.LCDrotate(myLCD.UC1609_ROTATION_NORMAL);
	myLCD.LCDupdate();
	delayMilliSecRDL(2000);

	// Test 504 LCD scroll
	myLCD.LCDclearBuffer();
	//myLCD.LCDupdate();
	myLCD.setCursor(0, 40);
	myLCD.print("scroll test");
	for (uint8_t i = 0 ; i < 62 ; i ++)
	{
		myLCD.LCDscroll(i);
		myLCD.LCDupdate();
		delayMilliSecRDL(100);
	}
	myLCD.LCDscroll(0);

	//Test 505 LCD enable and disable
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 30);
	myLCD.print("LCD Disable test");
	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
	myLCD.LCDEnable(0);
	delayMilliSecRDL(5000);
	myLCD.LCDEnable(1);
	myLCD.LCDclearBuffer();

	// ** Test 506 Rotate test **
	myLCD.LCDclearBuffer();
	myLCD.setRotation(myLCD.BC_Degrees_90);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 90");
	myLCD.setCursor(5,180);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.setRotation(myLCD.BC_Degrees_180);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 180");
	myLCD.setCursor(5,56);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.setRotation(myLCD.BC_Degrees_270);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate   270");
	myLCD.setCursor(5,180);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.setRotation(myLCD.BC_Degrees_0); //default normal
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 0");
	myLCD.setCursor(5,56);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	delayMilliSecRDL(500);
}


void Test905(void)
{
	printf("Test905 : polygons\n");
	//drawPolygon( x,  y, sides,  diameter, rotation,fill, color);
	// hexagon 6 sides
	myLCD.drawPolygon(60, 30, 6, 25, 0, false, myLCD.BLACK);
	myLCD.drawPolygon(60, 30, 6, 25, 0, true, myLCD.BLACK);
	delayMilliSecRDL(200);
	TestReset();
}

void Test906(void) {
	printf("Test906 : dot grid\n");
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myLCD.drawDotGrid(0, 0, MY_LCD_WIDTH, MY_LCD_HEIGHT, 5, myLCD.BLACK);
	TestReset();
}

void Test907() {
	printf("Test907 : quadrilateral\n");
	// Trapezoid 1 
	int16_t x8 = 30, y8 = 30;   // First vertex
	int16_t x9 = 35, y9 = 50;
	int16_t x10 = 60, y10 = 50;
	int16_t x11 = 75, y11 = 30;
	myLCD.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myLCD.BLACK);
	TestReset();
}

void Test908(void) {
	printf("Test908 : Ellipse\n");
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myLCD.drawEllipse(60, 30, 25, 20, false, myLCD.BLACK);  // Top-left
	TestReset();
	myLCD.drawEllipse(60, 30, 25, 20, true, myLCD.BLACK);
	TestReset();
}

void Test909(void) {
	printf("Test909 : Drawing Arc: drawArc and drawSimpleArc\n");
	int16_t centerX = 50;  // X-coordinate of the circle center
	int16_t centerY = 50;  // Y-coordinate of the circle center
	int16_t radius = 45;    // Radius of the circle
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full upper half (180° to 360°)
	myLCD.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myLCD.BLACK);
	TestReset();
	delayMilliSecRDL(2000);
	// Draw the third quarter (180° to 270°)
	myLCD.drawArc(centerX, centerY, radius, 3, 180.0f, 270.0f, myLCD.BLACK);
	TestReset();
	// drawSimpleArc :: Draw an arc with a radius of 50, from 0 to 300 degrees
	myLCD.drawSimpleArc(30, 30, 25, 0.0f, 300.0f, myLCD.BLACK);
	TestReset();
}

void TestReset()
{
	myLCD.LCDupdate();  // Write to the buffer
	delayMilliSecRDL(DisplayDelay1);
	myLCD.LCDclearBuffer();
}

void Test601() {

	printf("Test601 FPS: Stops at: %u \n ", countLimit);
	while (count < countLimit)
	{
		static long framerate = 0;
		displayData(framerate, count);
		framerate++;
		count++;
		delayMilliSecRDL(1);
	}

}

// Function to display data
void displayData(long currentFramerate, int count)
{
	myLCD.LCDclearBuffer();

	myLCD.setCursor(0, 20);
	myLCD.print("G Lyons");

	myLCD.setCursor(0, 30);
	myLCD.print(count);
	// Values to count frame rate per second
	// cppcheck-suppress variableScope
	static long lastFramerate = 0;
	static uint16_t fps;
	uint64_t currentCounter = counter();
	if(currentCounter - previousCounter >= 1000000000)
	{
		//(Rolls over every 1 second)
		fps = currentFramerate - lastFramerate;
		lastFramerate = currentFramerate ;
		previousCounter = currentCounter;
		colour = !colour;
	}

	myLCD.setCursor(0, 40);
	myLCD.print(fps);
	myLCD.print(" fps");
	myLCD.setCursor(0, 50);
	myLCD.print(rdlib::LibraryVersion());
	myLCD.drawFastVLine(92, 0, 63, myLCD.BLACK);
	

	myLCD.fillRect(97, 10, 20, 20, colour);
	myLCD.fillCircle(137, 20, 10, myLCD.BLACK);
	myLCD.fillTriangle(157, 30, 167, 10, 177, 30, !colour);
	myLCD.drawRoundRect(107, 40, 60, 20, 10, myLCD.BLACK);
	
	myLCD.LCDupdate();
}


//This returns nano-seconds as a 64-bit unsigned number, monotonically increasing,
//probably since system boot.
//The actual resolution looks like microseconds. 
// Returns nanoseconds
static uint64_t counter( void )
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec;
}

void displayMenu() {
	std::cout << "Tests Menu:\n";
	std::cout << "1. Bitmap Tests\n";
	std::cout << "2. Function Tests\n";
	std::cout << "3. FrameRate Tests\n";
	std::cout << "4. Text Tests\n";
	std::cout << "5. Graphic Tests\n";
	std::cout << "6. Quit\n";
	std::cout << "Enter your choice: ";
}

/// @endcond
