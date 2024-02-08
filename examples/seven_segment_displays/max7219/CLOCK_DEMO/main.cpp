/*!
	@file examples/seven_segment_displays/max7219/CLOCK_DEMO/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
			-# Clock Demo Shows sexample with two cascades displays
			-# Display one shows time 
			-# Display two shows Date
			-# Hardware SPI Project Name: MAX7219_7SEG_RPI
	
	@note not fully tested as only one display available
*/

// Libraries

#include <cstdio> //printf
#include <ctime>
#include <csignal> //catch user Ctrl+C
#include <string> // for string type
#include <algorithm> // for replace

#include <bcm2835.h>
#include <MAX7219_7SEG_RDL.hpp>

// Hardware SPI setup
uint32_t SPI_SCLK_FREQ =  5000; // HW Spi only , freq in kiloHertz , MAX 125 Mhz MIN 30Khz
uint8_t SPI_CEX_GPIO   =  0;     // HW Spi only which HW SPI chip enable pin to use,  0 or 1

// Constructor object
MAX7219_SS_RPI myMAX(SPI_SCLK_FREQ, SPI_CEX_GPIO);

// Function Prototypes
std::string UTC_string(void);
void signal_callback_handler(int signum);
void endTest(void);

// Main loop
int main()
{

	signal(SIGINT, signal_callback_handler);

	// Init the bcm2835 library
	printf("Test Begin :: MAX7219_7SEG\r\n");
	printf("Press Ctrl+c to QUIT\r\n");
	if(!bcm2835_init())
	{
		printf("Error 1201 :: bcm2835_init failed. Are you running as root??\n");
		return -1;
	}
	// Init the displays
	// -Init Display one
	myMAX.SetCurrentDisplayNumber(1);
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone) != rpiDisplay_Success)
	{
		printf("Error 1202 : bcm2835_spi_begin failed. Are you running as root??\n");
		bcm2835_close();
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

		MAX7219_MilliSecondDelay(1000);
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
	// Close the bcm2835  library
	bcm2835_close();
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
