/*!
	@file examples/bicolor_displays/nokia5110/HELLOWORLD_SWSPI/main.cpp
	@brief Description library test file, carries out hello world test , SW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 102 Hello World Software SPI
*/


// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

// **************** GPIO ***************
// Software SPI, pick any GPIO # you want.
#define RST_LCD 25
#define DC_LCD 24
#define SCLK_LCD 22
#define SDIN_LCD 27
#define CS_LCD 8

// **************** LCD ***************
#define myLCD_WIDTH 84
#define myLCD_HEIGHT 48
#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

NOKIA_5110_RPI myLCD(myLCD_WIDTH, myLCD_HEIGHT ,RST_LCD, DC_LCD, CS_LCD, SDIN_LCD, SCLK_LCD);

// ************ Function Headers ********
bool Setup(void);
void Test(void);
void EndTests(void);

// ************  MAIN ***************

int main(void)
{
	if (!Setup()) return -1;
	Test();
	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device

bool Setup(void)
{
	std::cout << "LCD Start"  << std::endl;
	if(!bcm2835_init())
	{
		std::cout<< "Error 1201 : Problem with init bcm2835 library" << std::endl;
		return false;
	}else{
		std::cout<< "bcm2835 library version : " << bcm2835_version() << std::endl;
	}
	bcm2835_delay(250);
	myLCD.LCDBegin(inverse, contrast, bias);
	std::cout<< "Nokia 5110 library version : " << GetRDLibVersionNum() << std::endl;
	std::cout<< "Nokia 5110 Software SPI GPIO delay is set to " << myLCD.LCDHighFreqDelayGet() << "uS" << std::endl;
	bcm2835_delay(250);
	myLCD.LCDdisplayClear();
	return true;
}


void EndTests(void)
{
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End" << std::endl;
}

void Test(void)
{
	std::cout<< "Nokia 5110 Software SPI, Hello World Test " << std::endl;
	char testStr[]= "Hello World    SWSPI";
	myLCD.setCursor(0, 0);
	myLCD.print(testStr);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(5000);
}
