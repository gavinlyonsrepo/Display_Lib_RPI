/*!
	@file examples/nokia5110/bitmap/main.cpp
	@brief Description library test file, carries out series of Bitmap tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 301 Bitmap full screen
		-# Test 302 Vertically addressed data small bitmaps
		-# Test 303 horizontal addressed data small bitmaps

*/

// libraries

#include <iostream> // for std::cout
#include "Bitmap_test_data.hpp" // Bitmap test data file
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

/// @cond


// Test Delays
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// GPIO
const uint8_t RST_LCD = 25; // GPIO pin number pick any you want
const uint8_t DC_LCD = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEV = 0; // GPIO chip device number usually 0

//  LCD
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
#define FULLSCREEN (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
#define LCD_INV  false // set to true to invert display pixel color
#define LCD_CST  0xB2 // contrast default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define LCD_BIAS 0x13 // LCD LCD_BIAS mode 1:48: Try 0x12 or 0x13 or 0x14

// Hardware SPI setup
int SPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

NOKIA_5110_RPI myLCD(MY_LCD_WIDTH, MY_LCD_HEIGHT , RST_LCD, DC_LCD);

// ************ Function Headers ****************
bool Setup(void);
void screenReset(void);
void EndTests(void);

void testBitMap(void);
void testCustomChar(void);
void testSmallBitmap(void);
void testSmallBitmapHa(void);

// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;

	myLCD.LCDclearBuffer();
	myLCD.setDrawBitmapAddr(true);
	testBitMap();
	testSmallBitmap();
	testSmallBitmapHa();

	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device
bool Setup(void)
{
	std::cout << "LCD Start"  << std::endl;
	std::cout<< "lgpio library version : " << lguVersion() << std::endl;
	std::cout<< "Nokia 5110 library version : " << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, SPI_DEVICE, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		std::cout<< "Error 1201: Setup :Cannot start spi" << std::endl;
		return false;
	}
	if (myLCD.LCDSetBufferPtr(screenBuffer) != rdlib::Success)
	{
		std::cout<< "Error 1202: Setup :Cannot Assign Buffer" << std::endl;
		return false;
	}
	delayMilliSecRDL(250);
	myLCD.LCDfillScreen();
	return true;
}

void EndTests(void)
{
	myLCD.LCDfillScreen();
	myLCD.LCDclearBuffer();
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	std::cout << "LCD End" << std::endl;
}

void testBitMap(void)
{
	std::cout <<"Test 301 Bitmap full screen" << std::endl;
	myLCD.drawBitmap(0,0, backUpIcon84x48, MY_LCD_WIDTH, MY_LCD_HEIGHT, myLCD.BLACK,myLCD.WHITE); // Splash screen
	screenReset();
}

void screenReset(void) {
	myLCD.LCDupdate();
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDclearBuffer();
}


void testSmallBitmap(void)
{
	std::cout <<"Test 302 Vertically addressed data bitmaps " << std::endl;

	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, SignalIconVa, 16, 8, myLCD.BLACK,myLCD.WHITE);
	myLCD.drawBitmap(60, 5, TemperatureImageVA, 16, 16, myLCD.BLACK, myLCD.WHITE);
	screenReset();
}

void testSmallBitmapHa(void)
{
	std::cout <<"Test 303 Horizontally addressed data bitmaps " << std::endl;
	myLCD.setDrawBitmapAddr(false);
	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, BatIconHa, 16, 8, myLCD.BLACK,myLCD.WHITE);
	myLCD.drawBitmap(25, 5, BatIconHa, 16, 8, myLCD.WHITE,myLCD.BLACK);
	screenReset();
}



// *************** EOF ****************

/// @endcond
