/*!
	@file examples/max7219/BCDMODE/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
	Carries out tests on BCD decode mode 3. BCD mode uses the built in font Code B
	this font has BCD code B (0-9, E, H, L, P, and -) 
	Hardware SPI , Project Name: Display_Lib_RPI

	@test
		-# Test 1000 BCD character
		-# Test 1001 BCD string
*/

// Libraries 
#include <lgpio.h>
#include <cstdio>
#include <MAX7219_7SEG_RDL.hpp> 

// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

// Constructor object 
MAX7219_SS_RPI myMAX(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS);

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
	printf("lgpio library Version Number :: %i\r\n", lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum()); 
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeBCDThree) != rpiDisplay_Success)
	{
		printf("Error 1202 :: Cannot open SPI\n");
		return false;
	}
	myMAX.ClearDisplay();
	return true;
}

// Clean up before exit
void EndTest(void)
{
	myMAX.DisplayEndOperations();
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
	delayMilliSecRDL(5000);
	myMAX.ClearDisplay();
	
	// Test BCD string 
	char teststring1[] = "PH  3-14";
	myMAX.DisplayBCDText(teststring1);
	delayMilliSecRDL(5000);
	myMAX.ClearDisplay();
}

// EOF
