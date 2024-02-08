/*!
	@file: examples/bicolor_displays/nokia5110/HelloWorld/main.cpp
	@brief Description library test file, carries out hello world test , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
			Project Name: Display_Lib_RPI
	@author Gavin Lyons.
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test
		-# Test 101 Hello World Hardware SPI
*/

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

// **************** GPIO ***************
#define RST_LCD 25
#define DC_LCD 24

// **************** LCD ***************
#define myLCD_WIDTH 84
#define myLCD_HEIGHT 48
#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

// ********************* HW SPI *****************
const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1

NOKIA_5110_RPI myLCD(myLCD_WIDTH, myLCD_HEIGHT , RST_LCD, DC_LCD, SPICLK_FREQ, SPI_CE_PIN);

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
		std::cout<< "Error 1201 : Setup : Problem with init bcm2835 library" << std::endl;
		return false;
	}else{
		std::cout<< "bcm2835 library version : " << bcm2835_version() << std::endl;
	}
	bcm2835_delay(250);
	if(myLCD.LCDBegin(inverse, contrast, bias) != rpiDisplay_Success)
	{
		std::cout<< "Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?" << std::endl;
		bcm2835_close(); // Close the bcm2835 library
		return false;
	}
	std::cout<< "Nokia 5110 library version : " << GetRDLibVersionNum() << std::endl;
	bcm2835_delay(250);
	myLCD.LCDdisplayClear();
	return true;
}

void EndTests(void)
{
	myLCD.LCDSPIoff(); //Stop the hardware SPI
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End" << std::endl;
}

void Test(void)
{
	std::cout<< "Nokia 5110 Hardware SPI, Hello World Test." << std::endl;
	char testStr[]= "Hello World";
	myLCD.setCursor(0, 0);
	myLCD.print(testStr);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(5000);
}

