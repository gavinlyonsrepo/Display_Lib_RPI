/*!
	@file    examples/hd44780/test_20x04/main.cpp
	@author   Gavin Lyons
	@brief
		 This file contains the "main" function for  project, a set of test sequence
 		 to test the Display_Lib_RPI::HD44780_LCD library
	@test
		-# Test 1 :: Hello world
		-# Test 2 :: Move the cursor test
		-# Test 3 :: Scroll the display test
		-# Test 4 :: Test GOTO method
		-# Test 5 :: Test clear a line method
		-# Test 6 :: Cursor type (4 off) and screen reset test, Cursor mode is changed with a reset.
		-# Test 7 :: Text entry mode (4 off) if screen is reset the entry mode will be reset to default
		-# Test 8 :: Print numerical data using print() method
		-# Test 9 :: Custom character's from the CGRAM test
		-# Test 10 :: Backlight test.
*/

// Section: Included library
#include <iostream>

#include "HD44780_LCD_RDL.hpp"

/// @cond

// Section: Defines
#define DISPLAY_DELAY_1 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY 5000
#define LCD_NO_ROWS 4
#define LCD_NO_COLS 20
#define LCD_I2C_ADDRESS 0x27
#define LCD_I2C_DEVICE 1
#define LCD_I2C_FLAGS 0

// Section: Globals
HD44780PCF8574LCD myLCD(LCD_NO_ROWS, LCD_NO_COLS,  LCD_I2C_DEVICE, LCD_I2C_ADDRESS,LCD_I2C_FLAGS);

// Section: Function Prototypes
bool setup(void);
void helloWorld(void);
void cursorMoveTest(void);
void scrollTest(void);
void gotoTest(void);
void clearLineTest(void);
void cursorTest(void);
void entryModeTest(void);
void writeNumTest(void);
void customChar(void);
void backLightTest(void);
void endTest(void);

// Section: Main Loop

int main()
{
	if (!setup()) return -1;

	helloWorld();
	cursorMoveTest();
	scrollTest();
	gotoTest();
	clearLineTest();
	cursorTest();
	entryModeTest();
	writeNumTest();
	customChar();
	backLightTest();

	endTest();

	return 0;
} // END of main

// Section :  Functions

bool setup(void) {
	std::cout <<  "LCD Test Begin" << std::endl;
	// print out library versions ( Note optional)
	std::cout << "lgpio library Version Number :" << lguVersion() << std::endl;
	std::cout << "HD44780_LCD_RPI lib Version Num :"  << rdlib::LibraryVersion() << std::endl;

	if (myLCD.LCD_I2C_ON() != rdlib::Success)
	{
		return false;
	}
	delayMilliSecRDL(250);

	myLCD.LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.LCDClearScreen();
	myLCD.LCDBackLightSet(true);

	// print out  flag status( Note optional)
	std::cout <<  "Backlight status is :" << (myLCD.LCDBackLightGet() ? "On" : "Off") << std::endl ;
	
	return true;
}

void helloWorld(void) {
	char teststr1[] = "Hello";
	char teststr2[] = "World";
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr1);
	myLCD.LCDGOTO(myLCD.LCDLineNumberThree , 0);
	myLCD.LCDSendString(teststr2); // Display a string
	myLCD.LCDSendChar('!'); // Display a single character
	delayMilliSecRDL(DISPLAY_DELAY_1);
}

void cursorMoveTest(void) {
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDMoveCursor(myLCD.LCDMoveLeft, 2);
}

void scrollTest(void) {
	for (uint8_t i = 0; i < 10; i++) {
		myLCD.LCDScroll(myLCD.LCDMoveRight, 1);
		delayMilliSecRDL(DISPLAY_DELAY_2);
	}
	myLCD.LCDScroll(myLCD.LCDMoveLeft, 10);
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void gotoTest(void) {
	myLCD.LCDClearScreen();

	char teststr1[] = "Line 1";
	char teststr2[] = "Line 2";
	char teststr3[] = "Line 3";
	char teststr4[] = "Line 4";
	char testchar = '!';
	int columnPos = 0;

	// Print a string to each line

	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.LCDSendString(teststr1);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 0);
	myLCD.LCDSendString(teststr2);
	myLCD.LCDGOTO(myLCD.LCDLineNumberThree , 0);
	myLCD.LCDSendString(teststr3);
	myLCD.LCDGOTO(myLCD.LCDLineNumberFour , 0);
	myLCD.LCDSendString(teststr4);
	delayMilliSecRDL(DISPLAY_DELAY_2);

	myLCD.LCDClearScreen();

	// Print out ASCII table one character at a time in every position
	// with a unique goto command
	for (columnPos = 0 ;  columnPos <20 ; columnPos++)
	{
		myLCD.LCDGOTO(myLCD.LCDLineNumberOne, columnPos);
		myLCD.LCDSendChar(testchar++);
	} //Line 1
	delayMilliSecRDL(DISPLAY_DELAY_2);

	for (columnPos = 0 ;  columnPos <20 ; columnPos++)
	{
		myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, columnPos);
		myLCD.LCDSendChar(testchar++);
	} //Line 2
	delayMilliSecRDL(DISPLAY_DELAY_2);

	for (columnPos = 0 ;  columnPos <20 ; columnPos++)
	{
		myLCD.LCDGOTO(myLCD.LCDLineNumberThree, columnPos);
		myLCD.LCDSendChar(testchar++);
	} //Line 3
	delayMilliSecRDL(DISPLAY_DELAY_2);

	for (columnPos = 0 ;  columnPos <20 ; columnPos++)
	{
		myLCD.LCDGOTO(myLCD.LCDLineNumberFour, columnPos);
		myLCD.LCDSendChar(testchar++);
	} // Line 4
	delayMilliSecRDL(DISPLAY_DELAY_2);

}

void clearLineTest(void)
{
	myLCD.LCDClearLine(myLCD.LCDLineNumberOne);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);
	delayMilliSecRDL(DISPLAY_DELAY_2);
		myLCD.LCDClearLine(myLCD.LCDLineNumberThree);
	delayMilliSecRDL(DISPLAY_DELAY_2);
		myLCD.LCDClearLine(myLCD.LCDLineNumberFour);
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void cursorTest(void) {
	char teststr1[] = "Cursor no 4";
	char teststr2[] = "Cursor no 1";
	char teststr3[] = "Cursor no 2";
	char teststr4[] = "Cursor no 3";

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeOnBlink); //type 4 cursor
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr1);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeOff); //type 1 cursor
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr2);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeBlink); //type 2 cursor
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr3);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeOn); // Back to initial state , type 3
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr4);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);
}

void writeNumTest()
{
	int numPos = 193;
	int numNeg = -8582;
	double myPI = 3.1456;

	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(numPos);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 0);
	myLCD.print(numNeg);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2);
	myLCD.print(myPI,3);

	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreen();

	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0); // 11
	myLCD.print(11, myLCD.RDL_DEC);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2);  // 13
	myLCD.print(11, myLCD.RDL_OCT);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 0); // B
	myLCD.print(11, myLCD.RDL_HEX);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2); // 1011
	myLCD.print(11, myLCD.RDL_BIN);

	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void customChar(void) {
	uint8_t index = 0; //  Character generator RAM location ,0-7 ,64 bytes
	
	// custom characters data to test custom character function
	uint8_t symbolData[8][8] = {
		{0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00}, // bell
		{0x02, 0x03, 0x02, 0x0E, 0x1E, 0x0C, 0x00, 0x00},  // Note
		{0x00, 0x0E, 0x15, 0x17, 0x11, 0x0E, 0x00, 0x00},  // clock
		{0x00, 0x0C, 0x1D, 0x0F, 0x0F, 0x06, 0x00, 0x00},  // duck
		{0x00, 0x01, 0x03, 0x16, 0x1C, 0x08, 0x00, 0x00},  // check
		{0x00, 0x1B, 0x0E, 0x04, 0x0E, 0x1B, 0x00, 0x00},  // cross
		{0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00},  // heart
		{0x01, 0x01, 0x05, 0x09, 0x1F, 0x08, 0x04, 0x00}   // return arrow
	};

	myLCD.LCDClearScreen();

	// Load the CGRAM with the data , custom characters
	// location argument must be 0 to 7  , load the data into LCD memory
 	 for (uint8_t index  = 0; index  < 8; index++) {
	 	 myLCD.LCDCreateCustomChar(index , symbolData[index]);
	}
 	 myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);

	// Print out custom characters from 
	// CGRAM locations 0-7 , location argument must be 0 to 7 
	for (index = 0; index < 8; index ++) {
		myLCD.LCDPrintCustomChar(index);
		myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 1);
		delayMilliSecRDL(500);
	}

	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreen();
}

void backLightTest(void)
{
	char teststr4[] = "Back Light";
	// Needs another command/data before it changes Light
	myLCD.LCDBackLightSet(false);

	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 1);
	myLCD.LCDSendString(teststr4);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDBackLightSet(true);
	myLCD.LCDClearScreen();
}

void entryModeTest(void) {

	char teststr8[] = "1234";

	myLCD.LCDChangeEntryMode(myLCD.LCDEntryModeOne);
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 8);
	myLCD.LCDSendString(teststr8); // <-C4321
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreenCmd();

	myLCD.LCDChangeEntryMode(myLCD.LCDEntryModeTwo);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 8);
	myLCD.LCDSendString(teststr8); // C4321->
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreenCmd();

	myLCD.LCDChangeEntryMode(myLCD.LCDEntryModeFour);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 8);
	myLCD.LCDSendString(teststr8); // <-1234C
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreenCmd();

	myLCD.LCDChangeEntryMode(myLCD.LCDEntryModeThree); // Set back to default entry mode
	myLCD.LCDClearScreenCmd();
	delayMilliSecRDL(DISPLAY_DELAY_1);
}

void endTest()
{
	myLCD.LCDDisplayON(false); //Switch off display
	myLCD.LCD_I2C_OFF();  // Switch off I2C , optional.
	std::cout << "LCD End" << std::endl ;
}

// *** EOF ***


/// @endcond
