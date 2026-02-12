/*! 
	@file examples/st7735/hello_world/main.cpp
	@brief Hello World. hardware or software SPI test
	@author Gavin Lyons.
	@details
		See USER OPTIONS 1-3 in SetupXWSPI function.
		Hardware or software SPI selected by bHardwareSPI.
		Display type selected in USER SCREEN TYPE SECTION.
	@test 
		-# 101 Hello World Hardware SPI
		-# 102 Hello World Software SPI
*/

// Section ::  libraries 
#include <iostream>

#include "ST7735_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals 

ST7735_TFT myTFT;
bool bHardwareSPI= true;

//=====================
// USER SCREEN TYPE SECTION
// Only two types defined here, there are others, see readme. 
// Comment in one and ONE only
#define REDTAB_SETUP // Setup for 128x128 ST7735R Red Tab display.
//#define BLACKTAB_SETUP //  Setup for 120x160 ST7735S Black Tab display.

#if defined(REDTAB_SETUP) == defined(BLACKTAB_SETUP)
#error "Define exactly ONE of REDTAB_SETUP  or BLACKTAB_SETUP"
#endif

#ifdef REDTAB_SETUP
	uint8_t OFFSET_COL = 0;    // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;    // 3, with manufacture tolerance/defects at edge of display
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 128; // Screen height in pixels
	ST7735_TFT::TFT_PCBtype_e DISPLAY_TYPE = ST7735_TFT::TFT_ST7735R_Red;
#elif defined(BLACKTAB_SETUP)
	uint8_t OFFSET_COL = 0;    // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;    // 3, with manufacture tolerance/defects at edge of display
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	ST7735_TFT::TFT_PCBtype_e  DISPLAY_TYPE = ST7735_TFT::TFT_ST7735S_Black;
#endif
//=====================

//  Section ::  Function Headers 

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
uint8_t SetupSWSPI(void); // setup + user options for software SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	if (bHardwareSPI){
		if (SetupHWSPI() != 0)
			return -1; //Hardware SPI
	}else {
		if (SetupSWSPI() != 0)
			return -1; //Hardware SPI
	}
	std::cout << "Display_Lib_RPI library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
	HelloWorld();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	int8_t RST_TFT  = 25; // Reset GPIO
	int8_t DC_TFT   = 24; // DC GPIO
	int  GPIO_CHIP_DEV = 0; // GPIO chip device number
	int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
	int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int SPI_SPEED =  8000000; // The speed of serial communication in bits per second.
	int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	std::cout << "TFT Start Test 101 HWSPI" << std::endl;

// ** USER OPTION 1 GPIO **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup**
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 PCB_TYPE + SPI settings**
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType( DISPLAY_TYPE, SPI_DEV, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	return 0;
}

// Software SPI setup
uint8_t SetupSWSPI(void)
{
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int8_t SCLK_TFT = 20;
	int8_t SDIN_TFT = 16;
	int8_t CS_TFT   = 21;
	int  GPIO_CHIP_DEV = 0; // Gpio chip device number

	uint16_t SWSPI_CommDelay = 0; //uS GPIO SW SPI delay
	std::cout << "TFT Start Test 102 SW SPI" << std::endl;
// ** USER OPTION 1 GPIO/SPI TYPE SW **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 PCB_TYPE **
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(DISPLAY_TYPE, SWSPI_CommDelay, GPIO_CHIP_DEV) != rdlib::Success)
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
	myTFT.setCursor(5,5);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_RED);
	myTFT.setFont(font_default);
	myTFT.print("Hello World!");
	delayMilliSecRDL(10000);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************

/// @endcond
