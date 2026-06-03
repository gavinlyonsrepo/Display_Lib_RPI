/*!
	@file    examples/hd44780/tests/main.cpp
	@author  Gavin Lyons
	@brief
		This file contains the "main" function for project, a set of test sequences
		to test the Display_Lib_RPI::HD44780_LCD library.
		Select display type by defining one of the MACROS in Display type selection below
		-# LCD_16x02  (default if neither defined)
		-# LCD_20x04
	@test
		-# Test 1  :: Hello world
		-# Test 2  :: Move the cursor test
		-# Test 3  :: Scroll the display test
		-# Test 4  :: Test GOTO method
		-# Test 5  :: Test clear a line method
		-# Test 6  :: Cursor type (4 off) and screen reset test
		-# Test 7  :: Text entry mode (4 off)
		-# Test 8  :: Print numerical data using print() method
		-# Test 9  :: Custom character's from the CGRAM test
		-# Test 10 :: Backlight test
		-# Test 11 :: Vector test              (16x02 only)
		-# Test 12 :: std::array test          (16x02 only)
		-# Test 13 :: Clock Demo               (16x02 only)
*/

// Section: Included library
#include <iostream>
#include <atomic>  // Ctrl + C exit
#include <csignal> // Ctrl + C exit
#include <thread>  // Ctrl + C exit

#include "HD44780_LCD_RDL.hpp"

/// @cond


// Section: Display type selection, pick one and one only
#define LCD_16x02
//#define LCD_20x04

// Define LCD_20x04 for 20x04 display, otherwise defaults to 16x02
#if defined(LCD_20x04)
	#define LCD_NO_ROWS     4
	#define LCD_NO_COLS     20
#else
	// Default: 16x02
	#if !defined(LCD_16x02)
		#define LCD_16x02
	#endif
	#define LCD_NO_ROWS     2
	#define LCD_NO_COLS     16
#endif

// Section: Defines
#define DISPLAY_DELAY_1  1000  // mS
#define DISPLAY_DELAY_2  2000  // mS
#define DISPLAY_DELAY    5000  // mS
#define DISPLAY_CLOCK    30    // seconds

// LCD I2C settings
#define LCD_I2C_ADDRESS  0x27
#define LCD_I2C_DEVICE   1
#define LCD_I2C_FLAGS    0

// Section: Globals
HD44780PCF8574LCD myLCD(LCD_NO_ROWS, LCD_NO_COLS, LCD_I2C_DEVICE, LCD_I2C_ADDRESS, LCD_I2C_FLAGS);
std::atomic<bool> stopRequested{false}; // Stop signal , Ctrl + c etc

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
void handleSignal(int){
	stopRequested = true; // for CtrL +C
}

#if defined(LCD_16x02)
void vectorTest(void);
void stdarrayTest(void);
void ClockDemo(uint16_t secondsDisplay);
#endif

// Section: Main Loop

int main()
{
	std::signal(SIGINT, handleSignal); // for user press Ctrl+C
	std::signal(SIGTERM, handleSignal);// for kill command
	if (!setup()) return -1;

#if defined(LCD_16x02)
	std::cout << "LCD 16x02 Start Testing" << std::endl;
#else
	std::cout << "LCD 20x04 Start Testing" << std::endl;
#endif

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

#if defined(LCD_16x02)
	vectorTest();
	stdarrayTest();
	ClockDemo(DISPLAY_CLOCK);
#endif

	if (stopRequested)
		std::cout << "Exit Signal received" << std::endl;
	endTest();
	return 0;
} // END of main

// Section: Functions

bool setup(void) {
	std::cout << "LCD Test Begin, press ctrl + c to quit" << std::endl;
	std::cout << "lgpio library Version Number :"         << lguVersion()           << std::endl;
	std::cout << "HD44780_LCD_RPI lib Version Num :"      << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(500);

	if (myLCD.LCD_I2C_ON() != rdlib::Success)
	{
		std::cout << "Error 1201: LCD_I2C_ON failed : Handle for LCD on I2C bus was not allocated" << std::endl;
		return false;
	}
	else
	{
		std::cout << "LCD_I2C_ON passed : Handle for LCD on I2C bus allocated" << std::endl;
	}
	delayMilliSecRDL(500);

	if (myLCD.LCDCheckConnection() < 0)
	{
		std::cout << "Error 1202: LCD not on bus?" << std::endl;
		return false;
	}
	else
	{
		std::cout << "LCDCheckConnection passed : LCD detected on the I2C bus" << std::endl;
	}

	myLCD.LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.LCDClearScreen();
	myLCD.LCDBackLightSet(true);

	std::cout << "Backlight status is : "          << (myLCD.LCDBackLightGet() ? "On" : "Off") << std::endl;
	std::cout << "I2C Debug Error : "              << myLCD.LCDI2CErrorGet()          << std::endl;
	std::cout << "I2C Error Timeout mS : "         << myLCD.LCDI2CErrorTimeoutGet()   << std::endl;
	std::cout << "I2C Error retry attempts counts: "<< +myLCD.LCDI2CErrorRetryNumGet() << std::endl;
	std::cout << "Debug enabled : "                << (rdlib_config::isDebugEnabled() ? "On" : "Off") << std::endl;
	return true;
}

void helloWorld(void) {
	if (stopRequested) return;
	char teststr1[] = "Hello";
	char teststr2[] = "World";
#if defined(LCD_16x02)
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.LCDSendString(teststr1);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
#else
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr1);
	myLCD.LCDGOTO(myLCD.LCDLineNumberThree, 0);
#endif
	myLCD.LCDSendString(teststr2);
	myLCD.LCDSendChar('!');
	delayMilliSecRDL(DISPLAY_DELAY_1);
}

void cursorMoveTest(void) {
	if (stopRequested) return;
#if defined(LCD_16x02)
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 3);
	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDMoveCursor(myLCD.LCDMoveLeft, 1);
	myLCD.LCDMoveCursor(myLCD.LCDMoveLeft, 2);
	delayMilliSecRDL(DISPLAY_DELAY_2);
#else
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDMoveCursor(myLCD.LCDMoveLeft, 2);
#endif
}

void scrollTest(void) {
	if (stopRequested) return;
#if defined(LCD_16x02)
	for (uint8_t i = 0; i < 5; i++) {
		myLCD.LCDScroll(myLCD.LCDMoveRight, 1);
		delayMilliSecRDL(DISPLAY_DELAY_2);
	}
	myLCD.LCDScroll(myLCD.LCDMoveLeft, 5);
#else
	for (uint8_t i = 0; i < 10; i++) {
		myLCD.LCDScroll(myLCD.LCDMoveRight, 1);
		delayMilliSecRDL(DISPLAY_DELAY_2);
	}
	myLCD.LCDScroll(myLCD.LCDMoveLeft, 10);
#endif
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void gotoTest(void) {
	if (stopRequested) return;
	myLCD.LCDClearScreen();

#if defined(LCD_16x02)
	if (stopRequested) return;
	char teststr3[] = "Line 2";
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 10);
	myLCD.LCDSendChar('A');
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 2);
	myLCD.LCDSendString(teststr3);
	delayMilliSecRDL(DISPLAY_DELAY);
#else
	char teststr1[] = "Line 1";
	char teststr2[] = "Line 2";
	char teststr3[] = "Line 3";
	char teststr4[] = "Line 4";
	char testchar = '!';

	// Print a string to each line
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne,   0); myLCD.LCDSendString(teststr1);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo,   0); myLCD.LCDSendString(teststr2);
	myLCD.LCDGOTO(myLCD.LCDLineNumberThree, 0); myLCD.LCDSendString(teststr3);
	myLCD.LCDGOTO(myLCD.LCDLineNumberFour,  0); myLCD.LCDSendString(teststr4);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreen();

	// Print ASCII table across all four lines
	for (int columnPos = 0; columnPos < LCD_NO_COLS; columnPos++) {
		myLCD.LCDGOTO(myLCD.LCDLineNumberOne, columnPos);
		myLCD.LCDSendChar(testchar++);
	}
	delayMilliSecRDL(DISPLAY_DELAY_2);
	for (int columnPos = 0; columnPos < LCD_NO_COLS; columnPos++) {
		myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, columnPos);
		myLCD.LCDSendChar(testchar++);
	}
	delayMilliSecRDL(DISPLAY_DELAY_2);
	for (int columnPos = 0; columnPos < LCD_NO_COLS; columnPos++) {
		myLCD.LCDGOTO(myLCD.LCDLineNumberThree, columnPos);
		myLCD.LCDSendChar(testchar++);
	}
	delayMilliSecRDL(DISPLAY_DELAY_2);
	for (int columnPos = 0; columnPos < LCD_NO_COLS; columnPos++) {
		myLCD.LCDGOTO(myLCD.LCDLineNumberFour, columnPos);
		myLCD.LCDSendChar(testchar++);
	}
	delayMilliSecRDL(DISPLAY_DELAY_2);
#endif
}

void clearLineTest(void) {
	if (stopRequested) return;
	myLCD.LCDClearLine(myLCD.LCDLineNumberOne);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);
	delayMilliSecRDL(DISPLAY_DELAY_2);
#if defined(LCD_20x04)
	myLCD.LCDClearLine(myLCD.LCDLineNumberThree);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberFour);
	delayMilliSecRDL(DISPLAY_DELAY_2);
#endif
}

void cursorTest(void) {
	if (stopRequested) return;
	char teststr1[] = "Cursor no 4";
	char teststr2[] = "Cursor no 1";
	char teststr3[] = "Cursor no 2";
	char teststr4[] = "Cursor no 3";

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeOnBlink); // type 4
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr1);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeOff);     // type 1
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr2);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeBlink);   // type 2
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr3);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.LCDResetScreen(myLCD.LCDCursorTypeOn);      // type 3, back to default
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.LCDSendString(teststr4);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearLine(myLCD.LCDLineNumberTwo);
}

void entryModeTest(void) {
	if (stopRequested) return;
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

	myLCD.LCDChangeEntryMode(myLCD.LCDEntryModeThree); // back to default
	myLCD.LCDClearScreenCmd();
	delayMilliSecRDL(DISPLAY_DELAY_1);
}

void writeNumTest(void) {
	if (stopRequested) return;
	int numPos   = 193;
	int numNeg   = -8582;
	double myPI  = 3.1456;

	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(numPos);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.print(numNeg);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2);
	myLCD.print(myPI, 3);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreen();

	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0); // 11
	myLCD.print(11, myLCD.RDL_DEC);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2); // 13
	myLCD.print(11, myLCD.RDL_OCT);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0); // B
	myLCD.print(11, myLCD.RDL_HEX);
	myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 2); // 1011
	myLCD.print(11, myLCD.RDL_BIN);
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void customChar(void) {
	if (stopRequested) return;
	uint8_t index = 0;

	uint8_t symbolData[8][8] = {
		{0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00}, // bell
		{0x02, 0x03, 0x02, 0x0E, 0x1E, 0x0C, 0x00, 0x00},  // note
		{0x00, 0x0E, 0x15, 0x17, 0x11, 0x0E, 0x00, 0x00},  // clock
		{0x00, 0x0C, 0x1D, 0x0F, 0x0F, 0x06, 0x00, 0x00},  // duck
		{0x00, 0x01, 0x03, 0x16, 0x1C, 0x08, 0x00, 0x00},  // check
		{0x00, 0x1B, 0x0E, 0x04, 0x0E, 0x1B, 0x00, 0x00},  // cross
		{0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00},  // heart
		{0x01, 0x01, 0x05, 0x09, 0x1F, 0x08, 0x04, 0x00}   // return arrow
	};

	myLCD.LCDClearScreen();
	for (index = 0; index < 8; index++) {
		myLCD.LCDCreateCustomChar(index, symbolData[index]);
	}
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	for (index = 0; index < 8; index++) {
		myLCD.LCDPrintCustomChar(index);
		myLCD.LCDMoveCursor(myLCD.LCDMoveRight, 1);
		delayMilliSecRDL(500);
	}
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDClearScreen();
}

void backLightTest(void) {
	if (stopRequested) return;
	char teststr4[] = "Back Light";
	myLCD.LCDBackLightSet(false);
	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 1);
	myLCD.LCDSendString(teststr4);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myLCD.LCDBackLightSet(true);
	myLCD.LCDClearScreen();
}

#if defined(LCD_16x02)
void vectorTest(void) {
	if (stopRequested) return;
	std::vector<int> intVec = {1, 2, 3, 4};
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(intVec); // Output: "1 2 3 4"
	delayMilliSecRDL(DISPLAY_DELAY);

	myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	std::vector<std::string> stringVec = {"Hello", "Vector"};
	myLCD.print(stringVec); // Output: "Hello Vector"
	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDClearScreen();
}

void stdarrayTest(void) {
	if (stopRequested) return;
	std::array<int, 3> intArray = {47, 255, 1023};
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(intArray, myLCD.RDL_HEX); // Output: " 2F FF 3FF "
	delayMilliSecRDL(DISPLAY_DELAY);
	myLCD.LCDClearScreen();
}

void ClockDemo(uint16_t secondsDisplay) {
	if (stopRequested) return;
	std::cout << "LCD Clock Demo Begin, ends in seconds : " << DISPLAY_CLOCK << std::endl;
	while (secondsDisplay-- > 1)
	{
		std::string TimeString = rdlib_time::UTC_string();
		std::cout << TimeString << "\r" << std::flush;
		auto DateInfo = TimeString.substr(0, 10);
		auto TimeInfo = TimeString.substr(11);
		myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
		myLCD.print(TimeInfo);
		myLCD.LCDGOTO(myLCD.LCDLineNumberTwo, 0);
		myLCD.print(DateInfo);
		delayMilliSecRDL(DISPLAY_DELAY_1);
		if (stopRequested) break;
	}
}
#endif // LCD_16x02

void endTest(void) {
	myLCD.LCDDisplayON(false);
	myLCD.LCD_I2C_OFF();
	std::cout << "\nLCD Test End\n" << std::endl;
}

// *** EOF ***

/// @endcond
