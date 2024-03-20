/*!
	@file examples/bicolor_displays/sh1106/HELLO_WORLD/main.cpp
	@author Gavin Lyons
	@brief Test file for Sh1106 OLED display, showing  "hello world" basic use case
	Project Name: Display_Lib_RPI

	@test
		1. Test 100 Hello World 128x64 screen
*/

#include <bcm2835.h>
#include <cstdio>
#include "SH110X_OLED_RDL.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8))
SH110X_RDL myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate a OLED object

// =============== Function prototype ================
bool SetupTest(void);
void TestLoop(void);
void EndTest(void);

// ======================= Main ===================
int main(void)
{
	if(!SetupTest()) return -1;
	TestLoop();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
bool SetupTest()
{
	const uint16_t I2C_Speed = BCM2835_I2C_CLOCK_DIVIDER_626; //  bcm2835I2CClockDivider enum , see readme.
	const uint8_t I2C_Address = 0x3C;
	bool I2C_debug = true;
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

	printf("SH1106  library Version Number :: %u\r\n",GetRDLibVersionNum());
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	delayMilliSecRDL(500);
	myOLED.OLEDbegin(myOLED.SH1106_IC, I2C_Speed, I2C_Address, I2C_debug, -1); // initialize the OLED
	return true;
}

void EndTest()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	bcm2835_close(); // Close the library
	printf("OLED Test End\r\n");
}

void TestLoop()
{

	// Define a buffer to cover whole screen
	uint8_t  screenBuffer[FULLSCREEN];
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.OLEDclearBuffer();
	myOLED.setFont(font_default);
	myOLED.setCursor(10, 10);
	myOLED.print("Hello World.");
	myOLED.OLEDupdate();
	delay(5000);

}

// ============== EOF =========
