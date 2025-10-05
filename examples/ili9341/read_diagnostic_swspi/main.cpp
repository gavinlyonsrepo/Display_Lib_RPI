/*! 
	@file   examples/ili9341/read_diagnostic_swspi/main.cpp
	@brief  Example file for ili9341 Library, read diagnostics from Display
			Useful for diagnostics, debug and testing. Outputs results to console.
			Software SPI.
	@author Gavin Lyons.
	@note   See USER OPTIONS 1-3 in SETUP function. Software SPI only , hardware SPI will NOT
			work with readDiagByte() the way library is currently configured,
			complete software control of chip select line required. 
	@details ILI9341 Read Diagnostics, Results from author's display :
		  -# Display Power Mode: 0x94
		  -# MADCTL Mode: 0x48
		  -# Pixel Format: 0x05
		  -# Image Format: 0x80
		  -# Self Diagnostic: 0xC0
	@test 
		-# Test 597 Read Diagnostics from display
*/

// Section ::  libraries 
#include <iostream> // cout
#include "ILI9341_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals 
ILI9341_TFT myTFT;

//  Section ::  Function Headers 
uint8_t SetupSWSPI(void); // setup + user options for software SPI
void ReadDiags(void);
void EndTests(void);

//  Section ::  MAIN loop
int main() 
{
	if(SetupSWSPI() != 0) return -1; //Hardware SPI
	ReadDiags();
	EndTests();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space 

// Software SPI setup
uint8_t SetupSWSPI(void)
{
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int8_t SCLK_TFT = 6;
	int8_t SDIN_TFT = 5;
	int8_t CS_TFT   = 21;
	int8_t MISO_TFT = 13;
	uint16_t SWSPI_CommDelay = 0; // uS GPIO SW SPI delay
	int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	
	std::cout << "TFT Read Diags SW SPI" << std::endl;
	std::cout << "Display_Lib_RPI library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
// ** USER OPTION 1 GPIO/SPI TYPE SW **
	myTFT.SetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT, MISO_TFT);
//*********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI **
	if(myTFT.InitSPI(SWSPI_CommDelay, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	std::cout << "SWSPI Comm GPIO Delay set to : " << myTFT.HighFreqDelayGet()<< "uS" << std::endl;
	delayMilliSecRDL(100);
	return 0;
}

void ReadDiags(void) 
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.setFont(font_groTesk);
	myTFT.setCursor(0,15);
	myTFT.println(" Read");
	myTFT.print(" Diags");
	delayMilliSecRDL(1000);
	printf("= Read Diags =\n");
	uint8_t returnByte = 0xFF;
	// Read and print Display Power Mode
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDMODE, 0);
	printf("Display Power Mode: 0x%02X\n", returnByte);
	delayMilliSecRDL(50);
	// Read and print MADCTL Mode
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDMADCTL, 0);
	printf("MADCTL Mode: 0x%02X\n", returnByte);
	delayMilliSecRDL(50);
	// Read and print Pixel Format
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDPIXFMT, 0);
	printf("Pixel Format: 0x%02X\n", returnByte);
	delayMilliSecRDL(50);
	// Read and print Image Format
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDIMGFMT, 0);
	printf("Image Format: 0x%02X\n", returnByte);
	delayMilliSecRDL(50);
	// Read and print Self Diagnostic
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDSELFDIAG, 0);
	printf("Self Diagnostic: 0x%02X\n", returnByte);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.PowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

/// @endcond
