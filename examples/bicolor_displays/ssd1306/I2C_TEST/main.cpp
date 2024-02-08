/*!
	@file examples/bicolor_displays/ssd1306/I2C_TEST/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library, I2C test 
	Project Name: Display_Lib_RPI
	
	@test
		1. Test 50 I2C test
*/

#include <bcm2835.h>
#include <cstdio>

#include "SSD1306_OLED_RDL.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8))
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate a OLED object 

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
	const uint16_t I2C_Speed = BCM2835_I2C_CLOCK_DIVIDER_626; //  bcm2835I2CClockDivider enum , see readme.
	const uint8_t I2C_Address = 0x3C;
	bool I2C_debug = false;
	
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}
	bcm2835_delay(500);
	
	// Turn on I2C bus (optionally it may already be on)
	if(!myOLED.OLED_I2C_ON())
	{
		printf("Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?\n");
		bcm2835_close(); // Close the library
		return false;
	}
	
	myOLED.OLEDbegin(I2C_Speed, I2C_Address, I2C_debug); // initialize the OLED
	
	if (myOLED.OLEDCheckConnection() != 0)
	{
		printf("Error 1203: bcm2835_i2c_begin :OLED not on bus?\n");
		bcm2835_close(); // Close the library
		return false;
	}
	//print out library versions & flag status( Note optional)
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	printf("Debug status is : %u\r\n", myOLED.OLEDDebugGet());
	printf("I2C Debug Error : %u\r\n", myOLED.OLEDI2CErrorGet()); // Print I2C error flag
	printf("I2C Error Timeout mS : %u \r\n", myOLED.OLEDI2CErrorTimeoutGet()); // Print I2C error Timeout
	printf("I2C Error retry attempts counts : %u \r\n", myOLED.OLEDI2CErrorRetryNumGet()); // Print I2C error retry count
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars, optional just for effect
	bcm2835_delay(1000);
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
	myOLED.print("I2C TEST");
	myOLED.OLEDupdate();
	delay(5000);
}

// ============== EOF =========
