/*!
	@file examples/max7219/cascade_demo/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
			Shows simple example with two cascades displays
			Hardware SPI Project Name: Display_Lib_RPI
	@test
		-# Test 410 Cascade demonstration 
*/

// Libraries 

#include <MAX7219_7SEG_RDL.hpp> 

/// @cond

// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
uint8_t NumberOfDisplays = 2; // Number of displays connected

// Constructor object 
MAX7219_SS_RPI myMAX(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, NumberOfDisplays);

// Main loop
int main() 
{

	printf("Test Begin :: MAX7219_7SEG_RPI\r\n");
	printf("lgpio library Version Number :: %i\r\n", lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", rdlib::LibraryVersion()); 
	// Some test data defined
	char teststr1[] = "Display1";
	char teststr2[] = "Display2";
	
	// Init the displays
	// -Init Display one 
	myMAX.SetCurrentDisplayNumber(1);
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone) != rdlib::Success)
	{
		printf("Error 1202 :: Cannot open SPI\n");
		return -1;
	}
	myMAX.ClearDisplay();
	// -Init Display Two
	myMAX.SetCurrentDisplayNumber(2);
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone);
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

	// Write to Display one again
	myMAX.SetCurrentDisplayNumber(2);
	printf("Display 2 again\r\n");
	myMAX.DisplayIntNum(222, myMAX.AlignRight);
	delayMilliSecRDL(5000);

	// Clear the displays 
	printf("Clear the display 1\r\n");
	// -Clear Display one 
	myMAX.SetCurrentDisplayNumber(1);
	myMAX.ClearDisplay();
	delayMilliSecRDL(2500);
	printf("Clear the display 2\r\n");
	// -Clear DisplayTwo 
	myMAX.SetCurrentDisplayNumber(2);
	myMAX.ClearDisplay();
	delayMilliSecRDL(1000);
	// End operations
	myMAX.DisplayEndOperations();
	printf("Test End\r\n");
	return 0;
} 
// End of main

// EOF

/// @endcond
