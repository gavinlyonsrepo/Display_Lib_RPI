/*!
	@file examples/bicolor_displays/ssd1306/OLED_FUNCTIONS/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED dispaly, showing use of various function
		Project Name: Display_Lib_RPI
	
	@test
		-# Test 501 Enable and disable Screen (Sleep mode)
		-# Test 502 Invert screen color
		-# Test 503 Contrast screen
		-# Test 504 Scroll Screen
		-# Test 505 Rotate
*/

#include <bcm2835.h>
#include <cstdio>
#include "SSD1306_OLED_RDL.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8))
SSD1306 myOLED(myOLEDwidth ,myOLEDheight);

// =============== Function prototypes ================
bool SetupTest(void);
void Test(void);
void EndTest(void);

// ======================= Main ===================
int main(void)
{
	if (SetupTest() != true) return -1;
	Test();
	EndTest();
	return 0;
}
// ======================= End of main  =====

// =============== Functions ================
bool SetupTest() 
{
	const uint16_t I2C_Speed = BCM2835_I2C_CLOCK_DIVIDER_626;; // bcm2835I2CClockDivider enum ,see readme.
	const uint8_t I2C_Address = 0x3C;
	bool I2C_debug = false;
	printf("OLED Test Begin\r\n");
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}
	bcm2835_delay(250);
	// Turn on I2C bus (optional it may already be on)
	if(!myOLED.OLED_I2C_ON())
	{
		printf("Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?\n");
		bcm2835_close(); // Close the library
		return false;
	}
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	myOLED.OLEDbegin(I2C_Speed, I2C_Address, I2C_debug); // initialize the OLED
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

void Test()
{
	myOLED.setRotation(displayBC_Degrees_0);
	// Define a buffer to cover whole screen 
	uint8_t  screenBuffer[FULLSCREEN]; 
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.OLEDclearBuffer(); // clear the buffer

	// Set text parameters
	myOLED.setFont(font_default);


	//  ** Test 501 OLED display enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("Disable test 501");
	printf("OLED Disable test 501\r\n");
	myOLED.OLEDupdate();

	bcm2835_delay(2000); 
	myOLED.OLEDEnable(0); 
	bcm2835_delay(2000); 
	myOLED.OLEDEnable(1); 
	bcm2835_delay(2000); 
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("Inverse test 502");
	printf("OLED Inverse test 502\r\n");
	myOLED.OLEDupdate();
	bcm2835_delay(1500);
	myOLED.OLEDInvert(1); // Inverted
	bcm2835_delay(1500);
	myOLED.OLEDInvert(0);
	bcm2835_delay(1500);

	// ** Test 503 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test 503");
	printf("OLED Contrast test 503\r\n");
	myOLED.OLEDupdate();
	bcm2835_delay(1500);
	myOLED.OLEDFillScreen(0x77, 0); 
	myOLED.OLEDContrast(0x00);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0x80);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0xFF);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0x81);
	bcm2835_delay(1000);
	myOLED.OLEDclearBuffer();

	// ***** Test 504 Scroll **
	myOLED.setCursor(20,20 );
	myOLED.print("Scroll test 504");
	printf("OLED Scroll test 504\r\n");
	myOLED.OLEDupdate();
	bcm2835_delay(2500);

	myOLED.OLEDStartScrollRight(0, 0x0F);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
	
	myOLED.OLEDStartScrollLeft(0, 0x0F);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();

	myOLED.OLEDStartScrollDiagRight(0, 0x07);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();
 	
	myOLED.OLEDStartScrollDiagLeft(0, 0x07);
	bcm2835_delay(3000);
	myOLED.OLEDStopScroll();


	// ** Test 505 rotate test **

	myOLED.OLEDclearBuffer();
	printf("OLED Rotate test 505\r\n");
	myOLED.setRotation(displayBC_Degrees_90);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 90");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.setRotation(displayBC_Degrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 180");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	
	myOLED.setRotation(displayBC_Degrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate   270");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.setRotation(displayBC_Degrees_0); //default normal 
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 0");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	// ** end of tests **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("End of tests");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
}

// =============== EOF ================
