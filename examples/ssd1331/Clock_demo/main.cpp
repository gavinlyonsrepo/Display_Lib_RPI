/*! 
	@file examples/ssd1331/Clock_demo/main.cpp
	@brief Clock demo
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function and in myOLED constructor
	@test 
		-# Test 408 Analog Clock Demo
*/

// Section ::  libraries
#include <iostream>
#include <ctime>
#include <csignal> //catch user Ctrl+C
#include <lgpio.h>
#include "SSD1331_OLED_RDL.hpp"

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);
int8_t RST_OLED  = 25; // GPIO reset pin
int8_t DC_OLED   = 24; // GPIO DC pin
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0
uint16_t OLED_WIDTH = 96;// Screen width in pixels
uint16_t OLED_HEIGHT = 64; // Screen height in pixels
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers 

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
void ClockDemo(void);
std::string UTC_string(void);
std::string UTC_string(void);
void signal_callback_handler(int signum);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	if(SetupHWSPI() != 0) return -1; //Hardware SPI
	signal(SIGINT, signal_callback_handler); //Ctrl+ c
	ClockDemo();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	std::cout << "Start Clock Demo, Ctrl +c to quit" << std::endl;
	std::cout << "SSD1331 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout << "Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO  **
	myOLED.OLEDSetupGPIO(RST_OLED, DC_OLED);
//*******************************************
// ** USER OPTION 2 Screen Setup**
	myOLED.OLEDInitScreenSize(OLED_WIDTH , OLED_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI settings **
	if(myOLED.OLEDInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

void ClockDemo(void) 
{
	// Determine the dimensions of the clock
	int radius = (OLED_HEIGHT - 10) / 2;
	int centerX = OLED_WIDTH / 2;
	int centerY = (OLED_HEIGHT / 2);
	// Draw Clock
	myOLED.fillScreen(myOLED.RDLC_DGREY);
	myOLED.fillCircle(centerX, centerY, radius, myOLED.RDLC_BLACK);
	// Start the clock
	uint16_t xSecond = centerX, ySecond = centerY;
	uint16_t xMinute = centerX, yMinute = centerY;
	uint16_t xHour = centerX, yHour = centerY;
	float angle;
	int lastSecond = -1; // Initialize to a value that is not valid for comparison

	while(true) 
	{
		// Get UTC time as string
		std::string utcTime = UTC_string();  // UTC_string() returns a string like "YYYY-MM-DD HH:MM:SS"
		// Parse the time string to extract hour, minute, and second
		int hour = std::stoi(utcTime.substr(11, 2));  // Extract hour
		int minute = std::stoi(utcTime.substr(14, 2)); // Extract minute
		int second = std::stoi(utcTime.substr(17, 2)); // Extract second

		// Check if a second has passed (i.e., the second has changed)
		if (second != lastSecond) 
		{
			// Draw the hour hand
			myOLED.drawLine(centerX, centerY, xHour, yHour, myOLED.RDLC_BLACK);
			angle = 2 * M_PI / 12 * hour;
			xHour = (uint16_t) (centerX + (radius / 2) * sinf(angle));
			yHour = (uint16_t) (centerY - (radius / 2) * cosf(angle));
			myOLED.drawLine(centerX, centerY, xHour, yHour, myOLED.RDLC_RED);
			// Draw the minute hand
			myOLED.drawLine(centerX, centerY, xMinute, yMinute, myOLED.RDLC_BLACK);
			angle = 2 * M_PI / 60 * minute;
			xMinute = (uint16_t) (centerX + (2 * radius / 3) * sinf(angle));
			yMinute = (uint16_t) (centerY - (2 * radius / 3) * cosf(angle));
			myOLED.drawLine(centerX, centerY, xMinute, yMinute, myOLED.RDLC_GREEN);
			// Draw the second hand
			myOLED.drawLine(centerX, centerY, xSecond, ySecond, myOLED.RDLC_BLACK);
			angle = 2 * M_PI / 60 * second;
			xSecond = (uint16_t) (centerX + (radius - 2) * sinf(angle));
			ySecond = (uint16_t) (centerY - (radius - 2) * cosf(angle));
			myOLED.drawLine(centerX, centerY, xSecond, ySecond, myOLED.RDLC_YELLOW);
			lastSecond = second;
		}
		delayMilliSecRDL(10);
		std::cout<< utcTime << "\r" << std::flush;
	}

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
	EndTests();
	exit(signum);
}

void EndTests(void)
{
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	delayMilliSecRDL(1000);
	myOLED.OLEDPowerDown();
	std::cout << "End Test" << std::endl;
}

// *************** EOF ****************
