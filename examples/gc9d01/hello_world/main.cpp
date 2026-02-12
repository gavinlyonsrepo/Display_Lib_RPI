/*! 
	@file examples/gc9d01/hello_world/main.cpp
	@brief Hello World hardware or software SPI test
	@author Gavin Lyons.
	@details
			Uses advanced screen buffer mode.
			1.
				To switch between Hardware and software SPI
				set ''HardwareSPI'' in ''Globals''
			2.
				In USER SCREEN TYPE SECTION below:
				Setup for 160X160 dual gate display : DUAL_GATE_SETUP (default)
				Setup for 40X160 Single gate display : SINGLE_GATE_SETUP
			3.
				See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 101 Hello World Hardware SPI
		-# 102 Hello World Software SPI
*/

// Section ::  libraries
#include <iostream>
#include "GC9D01_TFT_LCD_RDL.hpp"

/// @cond

//=====================
// USER SCREEN TYPE SECTION
// Comment in one and ONE only
#define DUAL_GATE_SETUP // Setup for 160X160 dual gate display
//#define SINGLE_GATE_SETUP // 40X160 Single gate display

#if defined(DUAL_GATE_SETUP) == defined(SINGLE_GATE_SETUP)
#error "Define exactly ONE of DUAL_GATE_SETUP or SINGLE_GATE_SETUP"
#endif
//=====================

// Section :: Globals

#ifdef DUAL_GATE_SETUP
	uint16_t TFT_WIDTH = 160;// Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	uint16_t OFFSET_X   =   0; // Screen X offset in pixels
	uint16_t OFFSET_Y   =   0; // Screen Y offset in pixels
	GC9D01_TFT::Resolution_e DisplayType = GC9D01_TFT::Resolution_e::RGB160x160_DualGate;
#elif defined(SINGLE_GATE_SETUP)
	uint16_t TFT_WIDTH  =   40; // Screen width in pixels
	uint16_t TFT_HEIGHT =  160; // Screen height in pixels
	uint16_t OFFSET_X   =  -60; // Screen X offset in pixels
	uint16_t OFFSET_Y   =   60; // Screen Y offset in pixels
	GC9D01_TFT::Resolution_e DisplayType = GC9D01_TFT::Resolution_e::RGB40x160_SingleGate;
#endif

bool HardwareSPI = true; // Hardware SPI = true, software SPI = false

GC9D01_TFT myTFT;

//  Section ::  Function Headers 
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
uint8_t SetupSWSPI(void); // setup + user options for software SPI
uint8_t SetupBufferMode(void); //setup the buffer
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop
int main() 
{
	std::cout << "GC9D01 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout << "Lgpio library version :" << lguVersion() << std::endl;

	if (HardwareSPI == true){  // Hardware SPI
		if(SetupHWSPI() != 0){
			return -1;
		}
	}else{ // Software SPI
		if(SetupSWSPI() != 0){
			return -1;
		}
	}
	HelloWorld();

	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 
// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0
	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

	std::cout << "TFT Start Test 101 HWSPI" << std::endl;
// ** USER OPTION 1 GPIO  **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*******************************************
// ** USER OPTION 2 Screen Setup**
	myTFT.TFTInitScreenSize(TFT_WIDTH , TFT_HEIGHT, DisplayType, OFFSET_X, OFFSET_Y);
// ***********************************
// ** USER OPTION 3 SPI settings **
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************

	delayMilliSecRDL(50);
	return SetupBufferMode();
}

// Set up advanced screen buffer mode
uint8_t SetupBufferMode(void)
{
	// 1. Turn it on!
	myTFT.setAdvancedScreenBuffer_e(myTFT.AdvancedScreenBuffer_e::On);
	// 2. Check its on, optional!
	if (myTFT.getAdvancedScreenBuffer_e() == myTFT.AdvancedScreenBuffer_e::Off)
	{
		std::cout << "Error: Setup: Wrong Mode. This example is for Advanced Screen Buffer Mode" << std::endl;
		return 4;
	}
	// 3. Set up buffer
	if (myTFT.setBuffer() != rdlib::Success) 
		return 5;
	// 4. Turn on Text pixel mode(instead of local buffer mode:default)
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	return 0;
}

// Software SPI setup
uint8_t SetupSWSPI(void)
{
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int8_t SCLK_TFT = 6;
	int8_t SDIN_TFT = 5;
	int8_t CS_TFT   = 21;
	int  GPIO_CHIP_DEVICE = 0; 

	uint16_t SWSPI_CommDelay = 0; //uS GPIO SW SPI delay
	std::cout << "TFT Start Test 102 SWSPI" << std::endl;
// ** USER OPTION 1 GPIO/SPI TYPE SW **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//*********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(TFT_WIDTH , TFT_HEIGHT, DisplayType, OFFSET_X, OFFSET_Y);
// ***********************************
// ** USER OPTION 3 SPI **
	if(myTFT.TFTInitSPI(SWSPI_CommDelay, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	std::cout << "SWSPI Comm GPIO Delay set to : " << myTFT.HighFreqDelayGet()<< "uS" << std::endl;
	delayMilliSecRDL(50);
	return SetupBufferMode();
}

void HelloWorld(void) 
{
	std::cout << "Hello World" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);

	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.setFont(font_arialBold);

	myTFT.setCursor(35,10);
	myTFT.print("Hello");
	myTFT.setCursor(35,25);
	myTFT.print("World");

	myTFT.writeBuffer();
	delayMilliSecRDL(7000);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.destroyBuffer();
	delayMilliSecRDL(500);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

/// @endcond
