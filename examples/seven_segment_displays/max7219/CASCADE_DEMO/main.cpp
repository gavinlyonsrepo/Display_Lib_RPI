/*!
	@file examples/seven_segment_displays/max7219/CASCADE_DEMO/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
			Shows simple example with two cascades displays
			Hardware SPI Project Name: Display_Lib_RPI
	
	@note not fully tested as only one display available
	
*/

// Libraries 
#include <bcm2835.h>
#include <cstdio>
#include <MAX7219_7SEG_RDL.hpp> 

// Hardware SPI setup
uint32_t SPI_SCLK_FREQ =  5000; // HW Spi only , freq in kiloHertz , MAX 125 Mhz MIN 30Khz
uint8_t SPI_CEX_GPIO   =  0;    // HW Spi only which HW SPI chip enable pin to use,  0 or 1

// Constructor object 
MAX7219_SS_RPI myMAX(SPI_SCLK_FREQ, SPI_CEX_GPIO);

// Main loop
int main() 
{

	// Init the bcm2835 library
	printf("Test Begin :: MAX7219_7SEG_RPI\r\n");
		if(!bcm2835_init())  // Init the bcm2835 library
	{
		printf("Error 1201 :: bcm2835_init failed. Are you running as root??\n");
		return -1;
	}
	
	// Some test data defined
	char teststr1[] = "Display1";
	char teststr2[] = "Display2";
	
	// Init the displays
	// -Init Display one 
	myMAX.SetCurrentDisplayNumber(1);
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone)!= rpiDisplay_Success)
	{
		printf("Error 1202-A :: bcm2835_spi_begin failed. Are you running as root??\n");
		bcm2835_close();  // Close the bcm2835 library
		return -1;
	}
	myMAX.ClearDisplay();
	// -Init Display Two
	myMAX.SetCurrentDisplayNumber(2);
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone) != rpiDisplay_Success)
	{
		printf("Error 1202-B :: bcm2835_spi_begin failed. Are you running as root??\n");
		bcm2835_close();  // Close the bcm2835 library
		return -1;
	}
	myMAX.ClearDisplay();

	// Write to Display one 
	myMAX.SetCurrentDisplayNumber(1);
	printf("Display 1\r\n");
	myMAX.DisplayText(teststr1, myMAX.AlignRight);
	delayMilliSecRDL(5000);
	
	// Write to Display Two 
	myMAX.SetCurrentDisplayNumber(2);
	printf("Display 2\r\n");
	myMAX.DisplayText(teststr2, myMAX.AlignRight);
	delayMilliSecRDL(5000);
	
	// Write to Display one again
	myMAX.SetCurrentDisplayNumber(1);
	printf("Display 1 again\r\n");
	myMAX.DisplayIntNum(111, myMAX.AlignRight);
	delayMilliSecRDL(5000);
	
	// Clear the displays 
	printf("Clear the displays\r\n");
	// -Clear Display one 
	myMAX.SetCurrentDisplayNumber(1);
	myMAX.ClearDisplay();
	// -Clear DisplayTwo 
	myMAX.SetCurrentDisplayNumber(2);
	myMAX.ClearDisplay();
	
	// End operations
	myMAX.DisplayEndOperations();
	// Close the bcm2835  library
	bcm2835_close(); 
	printf("Test End\r\n");
	return 0;
} 
// End of main

// EOF
