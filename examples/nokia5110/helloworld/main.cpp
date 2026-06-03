/*!
	@file examples/nokia5110/helloworld/main.cpp
	@brief Description library test file, carries out hello world test.
			PCD8544 Nokia 5110 SPI LCD display Library
			Project Name: Display_Lib_RPI
			User selects hardware or software SPI based on MARCO hardwareSPI in USER OPTION below
	@author Gavin Lyons.
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test
		-# Test 101 Hello World Hardware SPI
		-# Test 102 Hello World Software SPI
*/

// *** libraries ***
#include <iostream> // for std::cout
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

/// @cond

// *** USER OPTION ***
//comment out for software SPI, leave in for hardware SPI 
#define hardwareSPI

//  LCD
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
#define FULLSCREEN (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
#define LCD_INV  false // set to true to invert display pixel color
#define LCD_CST  0xB2 // contrast default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define LCD_BIAS 0x13 // LCD LCD_BIAS mode 1:48: Try 0x12 or 0x13 or 0x14

#ifdef hardwareSPI
	// GPIO
	const uint8_t RST_LCD = 25; // GPIO pin number pick any you want
	const uint8_t DC_LCD = 24; // GPIO pin number pick any you want
	NOKIA_5110_RPI myLCD(MY_LCD_WIDTH, MY_LCD_HEIGHT , RST_LCD, DC_LCD);
#else
	// GPIO
	const uint8_t RST_LCD = 25;
	const uint8_t DC_LCD = 24;
	const uint8_t DIN_LCD = 5;
	const uint8_t CLK_LCD = 6;
	const uint8_t CE_LCD = 21;
	NOKIA_5110_RPI myLCD(MY_LCD_WIDTH, MY_LCD_HEIGHT , RST_LCD ,DC_LCD, CE_LCD, DIN_LCD, CLK_LCD);
#endif


// ************ Function Headers ********
bool SetupHWSPI(void);
bool SetupSWSPI(void);
void Test(void);
void EndTests(void);

// ************  MAIN ***************

int main(void)
{
#ifdef hardwareSPI
	if(!SetupHWSPI()) {return -1;}
#else
	if(!SetupSWSPI()) {return -1;}
#endif
	Test();
	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device

bool SetupHWSPI(void)
{
	std::cout << "LCD Start HWSPI Test 101"  << std::endl;
	std::cout<< "lgpio library version : " << lguVersion() << std::endl;
	std::cout<< "Nokia 5110 library version : " << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(250);
	// Hardware SPI setup
	int SPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int SPI_SPEED =  1000000; // The speed of serial communication in bits per second.
	int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	int  GPIO_CHIP_DEV = 0; // GPIO chip device number usually 0
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

bool SetupSWSPI(void)
{
	std::cout << "LCD Start SWSPI Test 102"  << std::endl;
	std::cout<< "lgpio library version : " << lguVersion() << std::endl;
	std::cout<< "Nokia 5110 library version : " << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(250);
	int  GPIO_CHIP_DEV = 0; // GPIO chip device number usually 0
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		std::cout<< "Error 1202: Setup :Cannot start spi" << std::endl;
		return false;
	}
	printf("Frequency Software SPI delay = %u uS\r\n", myLCD.LCDHighFreqDelayGet());
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

void Test(void)
{
	std::cout<< "Nokia 5110, Hello World Test." << std::endl;
	char testStr[]= "Hello,    World";
	myLCD.setCursor(0, 0);
	myLCD.print(testStr);
	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
}


/// @endcond
