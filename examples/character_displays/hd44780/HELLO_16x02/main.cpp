/*!
	@file    examples/character_displays/hd44780/HELLO_16x02/main.cpp
	@author   Gavin Lyons
	@brief 
		 This file contains the "main" function for project, Hello world
 		 to test the Display_Lib_RPI::HD44780_LCD library

	@note 
		-# Test 101 :: Hello world
*/

// Section: Included library
#include <iostream>
#include <bcm2835.h>
#include "HD44780_LCD_RDL.hpp"


// Section: Globals
// myLCD(rows , cols , PCF8574 I2C address, I2C speed)
HD44780PCF8574LCD myLCD(2, 16, 0x27, BCM2835_I2C_CLOCK_DIVIDER_626); // instantiate an object

// Section: Function Prototypes
bool setup(void);
void helloWorld(void);
void endTest(void);

// Section: Main Loop
int main()
{
	if (!setup()) return -1;
	helloWorld();
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

	bcm2835_delay(250);
	// Turn on I2C bus (optionally it may already be on)
	if (myLCD.LCD_I2C_ON() != rpiDisplay_Success)
	{
		std::cout << "Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?" << std::endl;
		bcm2835_close(); // Close the library
		return false;
	}
	myLCD.LCDDebugSet(false);
	myLCD.LCD_I2C_SetSpeed();
	myLCD.LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.LCDClearScreen();

	return true;
}

void helloWorld(void) 
{
	char testString[] = "Hello World";
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.LCDSendString(testString);
	bcm2835_delay(5000);
}


void endTest()
{
	myLCD.LCDDisplayON(false); //Switch off display
	myLCD.LCD_I2C_OFF(); // Switch off I2C , optional.
	bcm2835_close(); // Close the library
	std::cout << "LCD Test End" << std::endl ;
}

// *** EOF ***

