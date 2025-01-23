/*!
	@file examples/max7219/CLOCK_DEMO/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
			-# Clock Demo Shows sexample with two cascades displays
			-# Display one shows time 
			-# Display two shows Date
			-# Hardware SPI Project Name: MAX7219_7SEG_RPI
	
	@note not fully tested as only one display available
	@test
		-# Test 403 Clock demonstration 
*/

// Libraries

#include <cstdio> //printf
#include <ctime>
#include <csignal> //catch user Ctrl+C
#include <string> // for string type
#include <algorithm> // for replace

#include <lgpio.h>
#include <MAX7219_7SEG_RDL.hpp>

// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

// Constructor object 
MAX7219_SS_RPI myMAX(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS);

// Function Prototypes
std::string UTC_string(void);
void signal_callback_handler(int signum);
void endTest(void);

// Main loop
int main()
{
	signal(SIGINT, signal_callback_handler);
	printf("Test Begin :: MAX7219_7SEG\r\n");
	printf("lgpio library Version Number :: %i\r\n", lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum()); 
	printf("Press Ctrl+c to QUIT\r\n");
	// -Init Display one
	myMAX.SetCurrentDisplayNumber(1);
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone) != rpiDisplay_Success)
	{
		printf("Error 1202 :: Cannot open SPI\n");
		return -1;
	}
	myMAX.ClearDisplay();
	// -Init Display Two
	myMAX.SetCurrentDisplayNumber(2);
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone);
	myMAX.ClearDisplay();


	while(1)
	{
		std::string TimeString = UTC_string(); //2023-05-25 17:27:15 UTC
		std::string DateInfo = TimeString.substr(0, 10); // 2023-05-25 
		std::string TimeInfo = TimeString.substr(11, 8); // 17:27:15
		// Write time to Display one
		std::replace(TimeInfo.begin(), TimeInfo.end(), ':', '-'); // replace all ':' to '-'
		const char *cstrTimeInfo = TimeInfo.c_str(); //convert timeinfo to c string
		myMAX.SetCurrentDisplayNumber(1);
		myMAX.DisplayText((char*)cstrTimeInfo, myMAX.AlignRight);
		// Write date to Display two
		std::replace(DateInfo.begin(), DateInfo.end(), '-', '.'); // replace all '-' to '.'
		const char *cstrDateInfo = DateInfo.c_str(); //convert dateinfo to c string
		myMAX.SetCurrentDisplayNumber(2);
		myMAX.DisplayText((char*)cstrDateInfo, myMAX.AlignRight);

		delayMilliSecRDL(1000);
	}

	endTest();
	return 0;
}
// End of main


void endTest(void)
{
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
	printf("Test End\r\n");

}

//Return UTC time as a std:.string with format "yyyy-mm-dd hh:mm:ss".
std::string UTC_string()
{
	std::time_t time = std::time({});
	char timeString[std::size("yyyy-mm-dd hh:mm:ss UTC")];
	std::strftime(std::data(timeString), std::size(timeString), "%F %T UTC", std::gmtime(&time));
	return timeString;
}

// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	endTest();
	exit(signum);
}

// EOF
