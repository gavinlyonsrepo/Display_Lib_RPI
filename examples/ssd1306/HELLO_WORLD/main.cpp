/*!
	@file examples/ssd1306/HELLO_WORLD/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED display, showing  "hello world" basic use case
	Project Name: Display_Lib_RPI

	@test
		1. Test 100 Hello World 128x64 screen
*/

#include <lgpio.h>
#include <cstdio>
#include "SSD1306_OLED_RDL.hpp"


// Screen Size 
#define MY_OLED_WIDTH  128
#define MY_OLED_HEIGHT 64
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))

// I2C 
bool I2C_debug = false;
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_DEVICE 1
#define OLED_I2C_FLAGS 0

// instantiate a OLED object
SSD1306_RDL myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT) ; 

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

void EndTest()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	printf("OLED Test End\r\n");
}

void TestLoop()
{
	// Define a buffer to cover whole screen
	uint8_t  screenBuffer[FULLSCREEN];
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.OLEDclearBuffer();
	myOLED.setFont(font_default);
	myOLED.setCursor(10, 10);
	myOLED.print("Hello World.");
	myOLED.OLEDupdate();
	delayMilliSecRDL(5000);
}

// ============== EOF =========
