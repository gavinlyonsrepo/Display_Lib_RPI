/*!
	@file examples/nokia5110/BITMAP/main.cpp
	@brief Description library test file, carries out series of Bitmap tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 301 Bitmap full screen
		-# Test 302 Small bitmaps 
		-# Test 303 horizontal addressed data bitmaps

*/

// libraries
#include <lgpio.h>
#include <iostream> // for std::cout
#include "Bitmap_test_data.hpp" // Bitmap test data file
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver


// Test Delays
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// GPIO
const uint8_t RST_LCD = 25; // GPIO pin number pick any you want
const uint8_t DC_LCD = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEV = 4; // GPIO chip device RPI 5 = 4 , other RPIs = 0

//  LCD
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
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
	
	myLCD.LCDdisplayClear();
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
	std::cout<< "Nokia 5110 library version : " << GetRDLibVersionNum() << std::endl;
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, SPI_DEVICE, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rpiDisplay_Success)
	{
		std::cout<< "Error 1202: Setup :Cannot start spi" << std::endl;
		return false;
	}
	
	delayMilliSecRDL(250);
	myLCD.LCDdisplayClear();
	return true;
}

void EndTests(void)
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	std::cout << "LCD End" << std::endl;
}

void testBitMap(void)
{
	std::cout <<"Test 301 Bitmap full screen" << std::endl;
	myLCD.drawBitmap(0,0, backUpIcon84x48, MY_LCD_WIDTH, MY_LCD_HEIGHT, RDL_BLACK,RDL_WHITE, sizeof(backUpIcon84x48)); // Splash screen
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}


void testSmallBitmap(void)
{
	std::cout <<"Test 302 Small bitmaps " << std::endl;

	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, SignalIconVa, 16, 8, RDL_BLACK,RDL_WHITE, sizeof(SignalIconVa));
	myLCD.drawBitmap(60, 5, TemperatureImageVA, 16, 16, RDL_BLACK, RDL_WHITE, sizeof(TemperatureImageVA));
	screenReset();
}

void testSmallBitmapHa(void)
{
	std::cout <<"Test 303 Horizontally addressed data bitmaps " << std::endl;
	myLCD.setDrawBitmapAddr(false);
	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, BatIconHa, 16, 8, RDL_BLACK,RDL_WHITE,sizeof(BatIconHa));
	myLCD.drawBitmap(25, 5, BatIconHa, 16, 8, RDL_WHITE,RDL_BLACK,sizeof(BatIconHa));
	screenReset();
}

// *************** EOF ****************