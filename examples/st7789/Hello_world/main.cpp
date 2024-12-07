/*! 
	@file examples/st7789/Hello_world/main.cpp
	@brief Hello World hardware SPI test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 101 Hello World Hardware SPI
*/

// Section ::  libraries
#include <iostream>
#include <lgpio.h>
#include "ST7789_TFT_LCD_RDL.hpp"

// Section :: Globals 
ST7789_TFT myTFT;

int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

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
	std::cout << "TFT Start Test 101 HWSPI" << std::endl;
	std::cout << "ST7789 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout << "Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO  **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*******************************************
// ** USER OPTION 2 Screen Setup**
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI settings **
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
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
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setCursor(30,60);
	myTFT.setTextColor(RDLC_GREEN, RDLC_BLACK);
	myTFT.setFont(font_arialBold);
	myTFT.print("Hello world");
	delayMilliSecRDL(7000);
}

void EndTests(void)
{
	myTFT.fillScreen(RDLC_BLACK);
	delayMilliSecRDL(1000);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
