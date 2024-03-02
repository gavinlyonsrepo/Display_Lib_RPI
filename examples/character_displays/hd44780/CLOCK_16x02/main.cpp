/*!
	@file examples/character_displays/hd44780/CLOCK_16x02/main.cpp
	@author   Gavin Lyons
	@brief  This file contains the "main" function for a clock demo (16x02 LCD)
		to test the Display_Lib_RPI::HD44780_LCD
	@note -# press ctrl + c to quit
 */

// Section: Included library 
#include <iostream>
#include <ctime>
#include <bcm2835.h>
#include <csignal> //catch user Ctrl+C
#include "HD44780_LCD_RDL.hpp"

// Section: Defines
#define DISPLAY_DELAY_1 1000

// Section: Globals
// myLCD(rows , cols , PCF8574 I2C address, I2C speed)
HD44780PCF8574LCD myLCD( 2, 16, 0x27, BCM2835_I2C_CLOCK_DIVIDER_626); // instantiate an object

// Section: Function Prototypes
bool setup(void);
void DisplayInfo(void);
void endTest(void);
std::string UTC_string(void);
void signal_callback_handler(int signum);

// Section: Main Loop

int main()
{
	signal(SIGINT, signal_callback_handler);
	
	if (!setup()) return -1;
	while(1)
	{
		DisplayInfo();
		delayMilliSecRDL(DISPLAY_DELAY_1);
	} 
	endTest();
	
	return 0;
} // End of main

// Section :  Functions

bool setup(void) {
	std::cout <<  "LCD Clock Demo Begin" << std::endl;
	std::cout <<  "Press 'ctrl + c' to quit" << std::endl;
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		std::cout << "Error 1201: init bcm2835 library , Is it installed ?"  << std::endl;
		return false;
	}

	delayMilliSecRDL(250);
	
	// Turn on I2C bus (optionally it may already be on)
	if (myLCD.LCD_I2C_ON() != rpiDisplay_Success)
	{
		std::cout << "Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?" << std::endl;
		bcm2835_close(); // Close the library
		return false;
	}
	
	myLCD.LCDDebugSet(false); // Set to true to turn on debug mode
	myLCD.LCD_I2C_SetSpeed();
	myLCD.LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.LCDClearScreen();
	myLCD.LCDBackLightSet(true);
	
	// print out library versions & flag status( Note optional)
	std::cout << "bcm2835 library Version Number :" << bcm2835_version() << std::endl;
	std::cout << "HD44780_LCD_RPI :"  << GetRDLibVersionNum()  << std::endl;
	std::cout << "Debug status is : " << (myLCD.LCDDebugGet() ? "On" : "Off") << std::endl ;
	std::cout <<  "Backlight status is : " << (myLCD.LCDBackLightGet() ? "On" : "Off")<< std::endl ;
	
	return true;
}

void DisplayInfo(void) 
{
	std::string TimeString = UTC_string();
	std::cout<< TimeString << "\r" << std::flush;
	auto DateInfo = TimeString.substr(0, 10);
	auto TimeInfo = TimeString.substr(11);
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(TimeInfo);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.print(DateInfo);
}


void endTest()
{
	myLCD.LCDDisplayON(false); //Switch off display
	myLCD.LCD_I2C_OFF();
	bcm2835_close(); // Close the library
	std::cout << std::endl;
	std::cout << "LCD Clock Demo End" << std::endl ;
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
// *** EOF ***

