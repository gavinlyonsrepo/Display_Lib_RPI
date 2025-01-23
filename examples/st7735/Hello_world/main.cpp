/*! 
	@file examples/st7735/Hello_world/main.cpp
	@brief Hello World hardware  SPI test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 101 Hello World Hardware SPI
*/

// Section ::  libraries 
#include <iostream>
#include <lgpio.h>
#include "ST7735_TFT_LCD_RDL.hpp"

// Section :: Globals 
ST7735_TFT myTFT;
int8_t RST_TFT  = 25; // Reset GPIO
int8_t DC_TFT   = 24; // DC GPIO
int  GPIO_CHIP_DEV = 0; // GPIO chip device number

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 128;// Screen width in pixels
uint16_t TFT_HEIGHT = 128; // Screen height in pixels

int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers 

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	if(SetupHWSPI() != 0)  return -1; //Hardware SPI
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
	std::cout << "Display_Lib_RPI library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup**
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 PCB_TYPE + SPI settings**
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SPI_DEV, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rpiDisplay_Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	return 0;
}

void HelloWorld(void) 
{
	std::cout << "Hello World" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setCursor(5,5);
	myTFT.setTextColor(RDLC_YELLOW, RDLC_RED);
	myTFT.setFont(font_default);
	myTFT.print("Hello World!");
	delayMilliSecRDL(10000);
}

void EndTests(void)
{
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
