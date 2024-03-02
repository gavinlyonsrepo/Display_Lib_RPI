/*! 
	@file examples/color16_displays/ili9341/Hello_world/main.cpp
	@brief Hello World hardware and software SPI test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function, run as sudo
		Tests both Hardware and software SPI to decide which test to run adjust 
		Global variable boolean 'TestHardwareSPI'
	@test 
		-# 101 Hello World Hardware SPI
		-# 102 Hello World Software SPI
*/

// Section ::  libraries 
#include <bcm2835.h> 
#include "ILI9341_TFT_LCD_RDL.hpp"

// Section :: Globals 
ILI9341_TFT myTFT;
bool TestHardwareSPI = true; // True for hardware SPI, false for Software SPI

//  Section ::  Function Headers 

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
uint8_t SetupSWSPI(void); // setup + user options for software SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	if (TestHardwareSPI == true)
	{
		if(SetupHWSPI() != 0) return -1; //Hardware SPI
	}else{
		if(SetupSWSPI() != 0) return -1; //Software SPI
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
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}
// ** USER OPTION 1 GPIO HW SPI **
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ =  8000000 ; //  freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
	if(myTFT.InitSPI(SCLK_FREQ, SPI_CE_PIN) != rpiDisplay_Success)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************
	std::cout <<"ILI9341 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	delayMilliSecRDL(100);
	return 0;
}

// Software SPI setup
uint8_t SetupSWSPI(void)
{
	std::cout << "TFT Start Test 102 SWSPI" << std::endl;
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}

// ** USER OPTION 1 GPIO/SPI TYPE SW **
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int8_t SCLK_TFT = 5;
	int8_t MOSI_TFT = 6;
	int8_t CS_TFT   = 8;
	int8_t MISO_TFT = 19;
	myTFT.SetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, MOSI_TFT , MISO_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	uint16_t SWSPI_CommDelay = 0; //uS GPIO SW SPI delay
	myTFT.InitSPI(SWSPI_CommDelay);
//*****************************
	std::cout <<"ILI9341 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	std::cout <<"SWSPI Comm GPIO Delay set to : " << myTFT.HighFreqDelayGet()<< "uS" << std::endl;
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
	myTFT.drawRoundRect(10,10, 210, 225,15, RDLC_RED);
	delayMilliSecRDL(7000);
}

void EndTests(void)
{
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.PowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
