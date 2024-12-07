/*!
	@file examples/ssd1306/BITMAP/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, showing how to draw bitmaps, 128X64 screen
		Project Name: Display_Lib_RPI

	@test
		-# Test 301 bigImage 64x64 pixels
		-# Test 302 Small icons 16x8 pixels
		-# Test 303 full screen image 128x64 pixels
		-# test 304 Small icon 16x8 (vertical addressed data)
*/

#include <lgpio.h>
#include <cstdio>
#include "Bitmap_test_data.hpp" // Bitmap test data file
#include "SSD1306_OLED_RDL.hpp"

#define MY_OLED_WIDTH  128
#define MY_OLED_HEIGHT 64
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
SSD1306_RDL myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT) ; // instantiate  an object

// I2C related
bool I2C_debug = true;
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_DEVICE 1
#define OLED_I2C_FLAGS 0

// =============== Function prototype ================
bool SetupTest(void);
void EndTests(void);
void myTests(void);
void Test301(void);
void Test302(void);
void Test303(void);
void Test304(void);

void EndTests(void);

// ======================= Main ===================
int main()
{
	if (SetupTest() != true) return -1;
	myTests();
	EndTests();
	return 0;
}
// ======================= End of Main ===================


// ======================= Function space ===================
bool SetupTest()
{
	printf("OLED Test Begin\r\n");

	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	printf("lgpio library Version Number :: %i\r\n",lguVersion());

	// Open  on I2C device
	if(myOLED.OLED_I2C_ON(OLED_I2C_DEVICE, OLED_I2C_ADDRESS,OLED_I2C_FLAGS) != rpiDisplay_Success)
	{
		printf("Error 1201:Cannot open I2C device bus\n");
		return false;
	}
	// Check if OLED on Bus
	if(myOLED.OLEDCheckConnection() < 0)
	{
		printf("Error 1202 : Cannot See Device on Bus\n");
		return false;
	}

	delayMilliSecRDL(500);
	myOLED.OLEDbegin(I2C_debug); // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars, optional just for effect
	delayMilliSecRDL(1000);
	return true;
}


void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	printf("OLED Test End\r\n");
}

void myTests()
{
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.setDrawBitmapAddr(false); // horizontal addressed data bitmaps.
	Test301();
	Test302();
	Test303();
	myOLED.setDrawBitmapAddr(true); // Vertical addressed data bitmap.
	Test304();
}


void Test301(void)
{
	printf("OLED Test 301 64x64 pixel Image\r\n");
	myOLED.drawBitmap(0, 0 ,bigImage64x64, 64, 64, RDL_BLACK,RDL_WHITE, sizeof(bigImage64x64));
	myOLED.OLEDupdate();
	delayMilliSecRDL(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

void Test302(void)
{
	printf("OLED Test 302 16x8 pixel Images , one inverted \r\n");
	myOLED.drawBitmap(70, 10,SignalIconHa, 16, 8,RDL_BLACK, RDL_WHITE, sizeof(SignalIconHa));
	myOLED.drawBitmap(40, 10,SignalIconHa, 16, 8,RDL_WHITE, RDL_BLACK, sizeof(SignalIconHa));
	myOLED.OLEDupdate();

	delayMilliSecRDL(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}

void Test303(void)
{
	printf("OLED Test 303 128x64 pixel image, inverted color\r\n");
	myOLED.drawBitmap(0, 0, backupicon128x64, MY_OLED_WIDTH, MY_OLED_HEIGHT, RDL_WHITE, RDL_BLACK, sizeof(backupicon128x64));
	myOLED.OLEDupdate();

	delayMilliSecRDL(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

void Test304(void)
{
	printf("OLED Test 304 16x16 pixel image, vertical addressed data\r\n");
	myOLED.drawBitmap(0, 0, TemperatureImageVA, 16, 16, RDL_BLACK, RDL_WHITE, sizeof(TemperatureImageVA));
	myOLED.OLEDupdate();

	delayMilliSecRDL(5000);
	myOLED.OLEDFillScreen(0x00, 0);
}
