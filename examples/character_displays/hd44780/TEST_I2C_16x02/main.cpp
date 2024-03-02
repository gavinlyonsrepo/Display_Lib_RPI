/*!
	@file    examples/character_displays/hd44780/TEST_I2C_16x02/main.cpp
	@author   Gavin Lyons
	@brief 
		 This file contains the "main" function for project, I2C test
 		 to test the Display_Lib_RPI::HD44780_LCD library

	@note 
		-# Test 1001 :: I2c Test
*/

// Section: Included library
#include <iostream>
#include <bcm2835.h>
#include "HD44780_LCD_RDL.hpp"


// Section: Globals
// myLCD(rows , cols , PCF8574 I2C address, I2C speed)
HD44780PCF8574LCD myLCD(2, 16, 0x27, BCM2835_I2C_CLOCK_DIVIDER_626 ); // instantiate an object

// Section: Function Prototypes
bool setup(void);
void test(void);
void endTest(void);

// Section: Main Loop
int main()
{
	if (!setup()) return -1;
	test();
	endTest();
	return 0;
} 

// Section :  Functions

bool setup(void) {
	std::cout <<  "LCD Test Begin" << std::endl;
	
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		std::cout << "Error 1201: init bcm2835 library , Is it installed ?"  << std::endl;
		return false;
	}

	// Turn on I2C bus (optionally it may already be on)
	if (myLCD.LCD_I2C_ON() != rpiDisplay_Success)
	{
		std::cout << "Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?" << std::endl;
		bcm2835_close(); // Close the library
		return false;
	}
	
	delayMilliSecRDL(1000);
	myLCD.LCDDebugSet(false); // Turn debug messages on 
	myLCD.LCD_I2C_SetSpeed();
	if (myLCD.LCDCheckConnection() != 0)
	{
		std::cout << "Error 1203: bcm2835_i2c_begin :LCD not on bus?" << std::endl;
		bcm2835_close(); // Close the library
		return false;
	}
	
	// print out library versions & flag status( Note optional)
	std::cout << "bcm2835 library Version Number :" << bcm2835_version() << std::endl;
	std::cout << "HD44780_LCD_RPI lib Version Num :"  << GetRDLibVersionNum() << std::endl;
	std::cout << "Debug status is : " << (myLCD.LCDDebugGet() ? "On" : "Off") << std::endl ;
	std::cout <<  "Backlight status is : " << (myLCD.LCDBackLightGet() ? "On" : "Off") << std::endl ;
	std::cout << "I2C Debug Error : " << +myLCD.LCDI2CErrorGet() << std::endl; // Print I2C error flag
	std::cout << "I2C Error Timeout mS : " << myLCD.LCDI2CErrorTimeoutGet() << std::endl; // Print I2C error Timeout
	std::cout << "I2C Error retry attempts counts: " << +myLCD.LCDI2CErrorRetryNumGet() << std::endl; // Print I2C error retry count
	myLCD.LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.LCDClearScreen();
	return true;
}

void test(void) 
{
	std::cout << "Test" << std::endl;
	char testString[] = "I2C Test";
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.LCDSendString(testString);
	delayMilliSecRDL(5000);
	myLCD.LCDClearScreen();
}


void endTest()
{
	myLCD.LCDDisplayON(false); //Switch off display
	myLCD.LCD_I2C_OFF(); // Switch off I2C , optional.
	bcm2835_close(); // Close the library
	std::cout << "LCD Test End" << std::endl ;
}

// *** EOF ***

