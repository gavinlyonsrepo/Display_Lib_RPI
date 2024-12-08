/*!
	@file    examples/hd44780/TEST_I2C_16x02/main.cpp
	@author   Gavin Lyons
	@brief 
		 This file contains the "main" function for project, I2C test
 		 to test the Display_Lib_RPI::HD44780_LCD library

	@test 
		-# Test 1003 :: I2c Test
*/

// Section: Included library
#include <iostream>
#include <lgpio.h>
#include "HD44780_LCD_RDL.hpp"


// Section: Defines
#define LCD_NO_ROWS 2
#define LCD_NO_COLS 16
#define LCD_I2C_ADDRESS 0x27
#define LCD_I2C_DEVICE 1
#define LCD_I2C_FLAGS 0

// Section: Globals
HD44780PCF8574LCD myLCD(LCD_NO_ROWS, LCD_NO_COLS,  LCD_I2C_DEVICE, LCD_I2C_ADDRESS,LCD_I2C_FLAGS);

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
	// print out library versions ( Note optional)
	std::cout << "lgpio library Version Number :" << lguVersion() << std::endl;
	std::cout << "HD44780_LCD_RPI lib Version Num :"  << GetRDLibVersionNum() << std::endl;

	if (myLCD.LCD_I2C_ON() != rpiDisplay_Success)
	{
		return false;
	}else{
		std::cout << "LCD_I2C_ON passed : A Handle for the LCD on I2C bus allocated" <<std::endl;
	}
	
	delayMilliSecRDL(1000);
	myLCD.LCDDebugSet(true); // Turn debug messages on 

	if (myLCD.LCDCheckConnection() < 0)
	{
		std::cout << "Error 1201: LCD not on bus?" << std::endl;
		return false;
	}else {
		std::cout << "LCDCheckConnection passed : LCD detected on the I2C bus" << std::endl;
	}

	// Print out flag status
	std::cout << "Debug status is : " << (myLCD.LCDDebugGet() ? "On" : "Off") << std::endl ;
	std::cout <<  "Backlight status is : " << (myLCD.LCDBackLightGet() ? "On" : "Off") << std::endl ;
	std::cout << "I2C Debug Error : " << myLCD.LCDI2CErrorGet() << std::endl; // Print I2C error flag
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
		myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(testString);
	delayMilliSecRDL(5000);
	myLCD.LCDClearScreen();
}


void endTest()
{
	myLCD.LCDDisplayON(false); //Switch off display
	myLCD.LCD_I2C_OFF(); // Switch off I2C
	std::cout << "LCD Test End" << std::endl ;
}

// *** EOF ***

