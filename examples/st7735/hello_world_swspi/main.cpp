/*! 
	@file examples/st7735/hello_world_swspi/main.cpp
	@brief Hello World software SPI test, 128x128 ST7735R Red Tab.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 102 Hello World Software SPI
*/

// Section ::  libraries 
#include <iostream>

#include "ST7735_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals 
ST7735_TFT myTFT;

// GPIO
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int8_t SCLK_TFT = 6;
int8_t SDIN_TFT = 5;
int8_t CS_TFT   = 21;
int  GPIO_CHIP_DEV = 0; // Gpio chip device number

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 128;// Screen width in pixels
uint16_t TFT_HEIGHT = 128; // Screen height in pixels

uint16_t SWSPI_CommDelay = 0; //uS GPIO SW SPI delay

//  Section ::  Function Headers 

uint8_t SetupSWSPI(void); // setup + user options for software SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	if(SetupSWSPI() != 0) return -1; //Software SPI
	HelloWorld();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 


// Software SPI setup
uint8_t SetupSWSPI(void)
{
	std::cout << "TFT Start Test 102 SW SPI" << std::endl;
	std::cout << "Display_Lib_RPI library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
// ** USER OPTION 1 GPIO/SPI TYPE SW **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 PCB_TYPE **
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SWSPI_CommDelay, GPIO_CHIP_DEV) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	std::cout << "SWSPI Comm GPIO Delay set to : " << myTFT.HighFreqDelayGet()<< "uS" << std::endl;
	delayMilliSecRDL(50);
	return 0;
}

void HelloWorld(void) 
{
	std::cout << "Hello World" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setCursor(10,10);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.print("Hello World");
	delayMilliSecRDL(5000);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************

/// @endcond
