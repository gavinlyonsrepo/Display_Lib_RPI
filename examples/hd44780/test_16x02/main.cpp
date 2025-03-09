/*!
	@file    examples/hd44780/test_16x02/main.cpp
	@author   Gavin Lyons
	@brief 
		 This file contains the "main" function for  project, a set of test sequence
 		 to test the Display_Lib_RPI::HD44780_LCD library, ends with clock demo
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
		-# Test 10 :: Test vectors using print() method
		-# Test 11 :: Backlight test.
		-# Test 12 :: Clock Demo
*/

// Section: Included library
#include <iostream>
#include <csignal> //catch user Ctrl+C
#include "HD44780_LCD_RDL.hpp"


/// @cond

// Section: Defines
// Test Timing delays
#define DISPLAY_DELAY_1 1000  //mS
#define DISPLAY_DELAY_2 2000 // mS
#define DISPLAY_DELAY 5000   // mS
#define DISPLAY_CLOCK 30     // seconds
// LCD setup
#define LCD_NO_ROWS 2
#define LCD_NO_COLS 16
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
void vectorTest(void);
void stdarrayTest(void);
void backLightTest(void);
void ClockDemo(uint16_t secondsDisplay);
void endTest(void);

std::string UTC_string(void); // clock demo
void signal_callback_handler(int signum); // Ctrl + c
// Section: Main Loop

int main()
{
	signal(SIGINT, signal_callback_handler);
	if (!setup()) return -1;

	std::cout <<  "LCD Start Testing" << std::endl;
	helloWorld();
	cursorMoveTest();
	scrollTest();
	gotoTest();
	clearLineTest();
	cursorTest();
	entryModeTest();
	writeNumTest();
	customChar();
	vectorTest();
	stdarrayTest();
	backLightTest();
	ClockDemo(DISPLAY_CLOCK);

	endTest();
	return 0;

} // END of main

// Section :  Functions

bool setup(void) {
	std::cout <<  "LCD Test Begin, press ctrl + c to quit" << std::endl;
	// print out library versions ( Note optional)
	std::cout << "lgpio library Version Number :" << lguVersion() << std::endl;
	std::cout << "HD44780_LCD_RPI lib Version Num :"  << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(500);

	if (myLCD.LCD_I2C_ON() != rdlib::Success)// check I2c handle allocated"  ( Note optional)
	{
		std::cout << "Error 1201: LCD_I2C_OFF failed : A Handle for the LCD on I2C bus was not allocated" <<std::endl;
		return false;
	}else{
		std::cout << "LCD_I2C_ON passed : A Handle for the LCD on I2C bus allocated" <<std::endl;
	}

	delayMilliSecRDL(500);

	if (myLCD.LCDCheckConnection() < 0)// check on bus  ( Note optional)
	{
		std::cout << "Error 1202: LCD not on bus?" << std::endl;
		return false;
	}else {
		std::cout << "LCDCheckConnection passed : LCD detected on the I2C bus" << std::endl;
	}

	myLCD.LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.LCDClearScreen();
	myLCD.LCDBackLightSet(true);

	// Print out flag status ( Note optional)
	std::cout << "Backlight status is : " << (myLCD.LCDBackLightGet() ? "On" : "Off") << std::endl ;
	std::cout << "I2C Debug Error : " << myLCD.LCDI2CErrorGet() << std::endl; // Print I2C error flag
	std::cout << "I2C Error Timeout mS : " << myLCD.LCDI2CErrorTimeoutGet() << std::endl; // Print I2C error Timeout
	std::cout << "I2C Error retry attempts counts: " << +myLCD.LCDI2CErrorRetryNumGet() << std::endl; // Print I2C error retry count
	std::cout << "Debug enabled : " << (rdlib_config::isDebugEnabled() ? "On" : "Off") << std::endl ;
	return true;
}

void helloWorld(void) {
	char teststr1[] = "Hello";
	char teststr2[] = "World";
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.LCDSendString(teststr1);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 0);
	myLCD.LCDSendString(teststr2); // Display a string
	myLCD.LCDSendChar('!'); // Display a single character
	delayMilliSecRDL(DISPLAY_DELAY_1);
}

void cursorMoveTest(void) {
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 3);
	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDMoveCursor(myLCD.LCDMoveLeft, 1);
	myLCD.LCDMoveCursor(myLCD.LCDMoveLeft, 2);
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void scrollTest(void) {
	for (uint8_t i = 0; i < 5; i++) {
		myLCD.LCDScroll(myLCD.LCDMoveRight, 1);
		delayMilliSecRDL(DISPLAY_DELAY_2);
	}
	myLCD.LCDScroll(myLCD.LCDMoveLeft, 5);
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void gotoTest(void) {
	char teststr3[] = "Line 2";
	myLCD.LCDClearScreen();
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 10);
	myLCD.LCDSendChar('A');
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 2);
	myLCD.LCDSendString(teststr3);
	delayMilliSecRDL(DISPLAY_DELAY);
}

void clearLineTest(void)
{
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo );
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberOne);
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

	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDClearScreen();

	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0); // 11
	myLCD.print(11, myLCD.RDL_DEC);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2);  // 13
	myLCD.print(11, myLCD.RDL_OCT);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 0); // B
	myLCD.print(11, myLCD.RDL_HEX);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2); // 1011
	myLCD.print(11, myLCD.RDL_BIN);

	delayMilliSecRDL(DISPLAY_DELAY);
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

	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDClearScreen();
}

void vectorTest(void)
{
	// For a vector of integers
	std::vector<int> intVec = {1, 2, 3, 4};
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(intVec); // Output: "1 2 3 4"
	delayMilliSecRDL(DISPLAY_DELAY);
	
	// For a vector of strings
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	std::vector<std::string> stringVec = {"Hello", "Vector"};
	myLCD.print(stringVec); // Output: "Hello Vector"
	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDClearScreen();
}

void stdarrayTest(void)
{
	// std::array (int)
	std::array<int, 3> intArray = {47, 255, 1023};
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(intArray, myLCD.RDL_HEX); // Output: " 2F FF 3FF "
	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDClearScreen();
}

void backLightTest(void)
{
	char teststr4[] = "Back Light";
	
	// Note : just changes flag Needs another command/data before it changes Light
	myLCD.LCDBackLightSet(false);

	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo , 1);
	myLCD.LCDSendString(teststr4);
	delayMilliSecRDL(DISPLAY_DELAY);
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

void ClockDemo(uint16_t secondsDisplay)
{
	std::cout <<  "LCD Clock Demo Begin ends in seconds : " << DISPLAY_CLOCK << std::endl;
	while(secondsDisplay-- > 1)
	{
		std::string TimeString = UTC_string();
		std::cout<< TimeString << "\r" << std::flush;
		auto DateInfo = TimeString.substr(0, 10);
		auto TimeInfo = TimeString.substr(11);
		myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
		myLCD.print(TimeInfo);
		myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
		myLCD.print(DateInfo);
		delayMilliSecRDL(DISPLAY_DELAY_1);
	}
}

void endTest()
{
	myLCD.LCDDisplayON(false); //Switch off display
	myLCD.LCD_I2C_OFF();  // Switch off I2C , optional.
	std::cout << "\nLCD Test End\n" << std::endl ;
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


/// @endcond
