/*! 
	@file examples/ssd1331/Hello_world/main.cpp
	@brief Hello World hardware SPI test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function and in myOLED constructor
	@test 
		-# 101 Hello World Hardware SPI
*/

// Section ::  libraries
#include <iostream>
#include <lgpio.h>
#include "SSD1331_OLED_RDL.hpp"

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);
int8_t RST_OLED  = 25; // GPIO reset pin
int8_t DC_OLED   = 24; // GPIO DC pin
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0
uint16_t OLED_WIDTH = 96;// Screen width in pixels
uint16_t OLED_HEIGHT = 64; // Screen height in pixels
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers 

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	if(SetupHWSPI() != 0) return -1; //Hardware SPI
	HelloWorld();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	std::cout << "Start Test 101 HWSPI" << std::endl;
	std::cout << "SSD1331 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout << "Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO  **
	myOLED.OLEDSetupGPIO(RST_OLED, DC_OLED);
//*******************************************
// ** USER OPTION 2 Screen Setup**
	myOLED.OLEDInitScreenSize(OLED_WIDTH , OLED_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI settings **
	if(myOLED.OLEDInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

void HelloWorld(void) 
{
	std::cout << "Hello World" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.setCursor(0,0);
	myOLED.setTextColor(myOLED.RDLC_GREEN, myOLED.RDLC_BLACK);
	myOLED.setFont(font_default);
	myOLED.print("Hello world");
	delayMilliSecRDL(7000);
}

void EndTests(void)
{
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	delayMilliSecRDL(1000);
	myOLED.OLEDPowerDown(); // Power down device
	std::cout << "End Test" << std::endl;
}

// *************** EOF ****************
