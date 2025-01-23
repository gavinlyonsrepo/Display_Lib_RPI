/*! 
	@file examples/st7789/Hello_world_SWSPI/main.cpp
	@brief Hello World software SPI test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 102 Hello World Software SPI
*/

// Section ::  libraries
#include <iostream>
#include <lgpio.h>
#include "ST7789_TFT_LCD_RDL.hpp"

// Section :: Globals 
ST7789_TFT myTFT;

int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int8_t SCLK_TFT = 6;
int8_t SDIN_TFT = 5;
int8_t CS_TFT   = 21;
int  GPIO_CHIP_DEVICE = 0; 

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

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
	std::cout << "TFT Start Test 102 SWSPI" << std::endl;
	std::cout << "Display_Lib_RPI library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO/SPI TYPE SW **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.TFTInitSPI(SWSPI_CommDelay, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		return 3;
	}
//*****************************
	std::cout << "SWSPI Comm GPIO Delay set to : " << myTFT.HighFreqDelayGet()<< "uS" << std::endl;
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
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
