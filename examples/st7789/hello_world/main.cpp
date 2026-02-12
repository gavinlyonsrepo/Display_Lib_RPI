/*! 
	@file    examples/st7789/hello_world/main.cpp
	@brief   Hello World. hardware or software SPI test.
	@author  Gavin Lyons.
	@details See USER OPTIONS 1-3 in SETUP function
			 to Change between hardware and software SPI set 'bHardwareSPI'
	@test 
		-# 101 Hello World Hardware SPI
		-# 102 Hello World Software SPI
*/

// Section ::  libraries
#include <iostream>

#include "ST7789_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
bool bHardwareSPI = true;

ST7789_TFT myTFT;
uint16_t TFT_WIDTH = 240;  // Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

//  Section ::  Function Headers 
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
uint8_t SetupSWSPI(void); // setup + user options for software SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop
int main() 
{
	if (bHardwareSPI){
		if(SetupHWSPI() != 0) return -1; // Hardware SPI
	}else{
		if(SetupSWSPI() != 0) return -1; // Software SPI
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
	std::cout << "TFT Start Test 101 HWSPI" << std::endl;
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
// ** USER OPTION 1 GPIO  **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*******************************************
// ** USER OPTION 2 Screen Setup**
	myTFT.TFTInitScreenSize(0, 0 , TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI settings **
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

// Software SPI setup
uint8_t SetupSWSPI(void)
{
	std::cout << "TFT Start Test 102 SWSPI" << std::endl;
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int8_t SCLK_TFT = 20;
	int8_t SDIN_TFT = 16;
	int8_t CS_TFT   = 21;
	int  GPIO_CHIP_DEVICE = 0; 
	uint16_t SWSPI_CommDelay = 0; //uS GPIO SW SPI delay
// ** USER OPTION 1 GPIO/SPI TYPE SW **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//*********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(0, 0, TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI **
	if(myTFT.TFTInitSPI(SWSPI_CommDelay, GPIO_CHIP_DEVICE) != rdlib::Success)
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
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setCursor(30,60);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.setFont(font_arialBold);
	myTFT.print("Hello world");
	delayMilliSecRDL(7000);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	delayMilliSecRDL(1000);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************

/// @endcond
