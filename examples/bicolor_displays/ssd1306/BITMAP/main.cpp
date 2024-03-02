/*!
	@file examples/bicolor_displays/ssd1306/BITMAP/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, showing how to draw bitmaps, 128X64 screen
		Project Name: Display_Lib_RPI

	@test
		-# Test 301 bigImage 64x64 pixels
		-# Test 302 Small icons 16x8 pixels
		-# Test 303 full screen image 128x64 pixels
		-# test 304 Small icon 16x8 (vertical addressed data)
*/

#include <bcm2835.h>
#include <cstdio>
#include "Bitmap_test_data.hpp" // Bitmap test data file
#include "SSD1306_OLED_RDL.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8))
uint8_t screenBuffer[FULLSCREEN];
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object

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
	const uint16_t I2C_Speed = BCM2835_I2C_CLOCK_DIVIDER_626; //  bcm2835I2CClockDivider enum , see readme.
	const uint8_t I2C_Address = 0x3C;
	bool I2C_debug = false;
	printf("OLED Test Begin\r\n");

	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}

	// Turn on I2C bus (optionally it may already be on)
	if(myOLED.OLED_I2C_ON() != rpiDisplay_Success)
	{
		printf("Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?\n");
		bcm2835_close(); // Close the library
		return false;
	}

	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	delayMilliSecRDL(500);
	myOLED.OLEDbegin(I2C_Speed, I2C_Address, I2C_debug); // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars, optional just for effect
	delayMilliSecRDL(1000);
	return true;
}


void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	bcm2835_close(); // Close the bcm2835 library
	printf("OLED Test End\r\n");
}

void myTests()
{
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
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
	myOLED.drawBitmap(0, 0, backupicon128x64, myOLEDwidth, myOLEDheight, RDL_WHITE, RDL_BLACK, sizeof(backupicon128x64));
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
