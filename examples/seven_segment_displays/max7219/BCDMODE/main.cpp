/*!
	@file examples/seven_segment_displays/max7219/BCDMODE/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
	Carries out tests on BCD decode mode 3. BCD mode uses the built in font Code B
	this font has BCD code B (0-9, E, H, L, P, and -) 
	Hardware SPI , Project Name: Display_Lib_RPI
	

	@test
		-# Test 30 BCD character
		-# Test 31 BCD string
*/

// Libraries 
#include <bcm2835.h>
#include <cstdio>
#include <MAX7219_7SEG_RDL.hpp> 

// Hardware SPI setup
uint32_t SPI_SCLK_FREQ =  5000; // HW Spi only , freq in kiloHertz , MAX 125 Mhz MIN 30Khz
uint8_t SPI_CEX_GPIO   =  0;     // HW Spi only which HW SPI chip enable pin to use,  0 or 1

// Constructor object 
MAX7219_SS_RPI myMAX(SPI_SCLK_FREQ, SPI_CEX_GPIO);

// Function Prototypes
bool Setup(void);
void myTest(void);
void EndTest(void);

// Main loop
int main() 
{
	if (!Setup()) return -1;
	myTest();
	EndTest();
	return 0;
}
// End of main

// Function Space

// Setup test
bool Setup(void)
{
	printf("Test Begin :: MAX7219_7SEG_RPI\r\n");
	if(!bcm2835_init())  // Init the bcm2835 library
	{
		printf("Error 1201 :: bcm2835_init failed. Are you running as root??\n");
		return false;
	}
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("MAX7219_7SEG Library version number :: %u\r\n", GetRDLibVersionNum()); 
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeBCDThree) != rpiDisplay_Success)
	{
		printf("Error 1202 : bcm2835_spi_begin failed. Are you running as root??\n");
		bcm2835_close();  // Close the bcm2835 library
		return false;
	}
	myMAX.ClearDisplay();
	return true;
}

// Clean up before exit
void EndTest(void)
{
	myMAX.DisplayEndOperations();
	bcm2835_close();  // Close the bcm2835 library
	printf("Test End\r\n");
}

void myTest(void)
{
	// Test BCD Char
	myMAX.DisplayBCDChar(7, myMAX.CodeBFontH); // Digit 7 LHS of display
	myMAX.DisplayBCDChar(6, myMAX.CodeBFontE);
	myMAX.DisplayBCDChar(5, myMAX.CodeBFontL);
	myMAX.DisplayBCDChar(4, myMAX.CodeBFontP);
	myMAX.DisplayBCDChar(3, myMAX.CodeBFontSpace);
	myMAX.DisplayBCDChar(2, myMAX.CodeBFontFour);
	myMAX.DisplayBCDChar(1, myMAX.CodeBFontDash);
	myMAX.DisplayBCDChar(0, myMAX.CodeBFontOne); // Digit 0 RHS
	MAX7219_MilliSecondDelay(5000);
	myMAX.ClearDisplay();
	
	// Test BCD string 
	char teststring1[] = "PH  3-14";
	myMAX.DisplayBCDText(teststring1);
	MAX7219_MilliSecondDelay(5000);
	myMAX.ClearDisplay();
}

// EOF
