/*!
	@file examples/ht16k33/test_14_segment/main.cpp
	@brief Example file for HT16k33 display 14 segment
	@details test for 14 segment display, the display used is LDD-F5406RI
		Common Cathode, 2 Digit , LT0565GWk, wiring connections in readme.
	@test
		-# Test 1 Brightness
		-# Test 2 Blink settings 
		-# Test 3 Display raw data, no ASCII font 
		-# Test 4 Standby mode
		-# Test 5 On/Off modes
		-# Test 6 Display the available members
		-# Test 7 Display characters
		-# Test 8 Display strings
		-# Test 9 Display hexadecimal characters
		-# Test 11 Display positive integers 
		-# Test 11 Display negative integers 
		-# Test 13 Display floating point numbers 
*/

// Included library
#include <iostream>
#include <csignal> //catch user Ctrl+C
#include "HT16K33plus_Model1_RDL.hpp"

/// @cond

// Test timing
#define DISPLAY_DELAY_1 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY_5 5000

// Display
const uint8_t  numberofDigits = 2;
const uint8_t  brightness = 8;
#define I2C_ADDRESS 0x70
#define I2C_DEVICE 1
#define I2C_FLAGS 0
HT16K33plus_Model1 myHT(I2C_DEVICE, I2C_ADDRESS, I2C_FLAGS);

// Function Prototypes
bool setup(void);
void TestBright(void);
void TestBlink(void);
void TestRaw(void);
void TestStandby(void);
void TestOnOff(void);
void TestPrintFlags(void);
void TestChar(void);
void TestString(void);
void TestHex(void);
void TestIntPos(void);
void TestIntNeg(void);
void TestFloat(void);
void endTest(void);
void signal_callback_handler(int signum); // Ctrl + c

// Main Loop
int main()
{
	signal(SIGINT, signal_callback_handler);  // Ctrl + c
	if (!setup()) return -1;
	TestPrintFlags();
	TestRaw();
	TestBright();
	TestBlink();
	TestStandby();
	TestOnOff();
	TestChar();
	TestString();
	TestHex();
	TestIntPos();
	TestIntNeg();
	TestFloat();
	endTest();
	return 0;
} // END of main

// Functions

bool setup(void) {
	delayMilliSecRDL(50);
	std::cout <<  "Test Begin" << std::endl;
	if (myHT.Display_I2C_ON() != rdlib::Success)
	{
		std::cout << "Error 1201: Display_I2C_OFF failed : A Handle for I2C bus was not allocated" <<std::endl;
		return false;
	}else {
		std::cout << "Display_I2C_ON passed: A Handle for I2C bus, allocated" << std::endl;
	}
	if (myHT.DisplayCheckConnection() < 0)// check on bus  ( Note optional)
	{
		std::cout << "Error 1202: Display not on bus?" << std::endl;
		return false;
	}else {
		std::cout << "CheckConnection passed: Display detected on the I2C bus" << std::endl;
	}
	delayMilliSecRDL(50);
	myHT.DisplayInit(brightness, myHT.BLINKOFF, numberofDigits, myHT.SegType14);
	delayMilliSecRDL(50);
	myHT.ClearDigits(); //Clear display
	return true;
}

void TestPrintFlags(void)
{
	// Print out flag status
	std::cout << "Flags Status" << std::endl;
	std::cout << "Brightness               : " << +myHT.getBrightness() << std::endl;
	std::cout << "Blink intensity setting  : " << +myHT.getBlink() << std::endl;
	std::cout << "Display Type             : " << +myHT.getDisplayType() << std::endl;
	std::cout << "I2C Debug Error          : " << myHT.DisplayI2CErrorGet() << std::endl;
	std::cout << "I2C Error Timeout mS     : " << myHT.DisplayI2CErrorTimeoutGet() << std::endl;
	std::cout << "I2C Error retry attempts : " << +myHT.DisplayI2CErrorRetryNumGet() << std::endl;
	std::cout << "Debug enabled : " << (rdlib_config::isDebugEnabled() ? "On" : "Off") << std::endl ;
	std::cout << std::endl;
}

void TestRaw(void)
{
	std::cout <<  "Test Raw data on display" << std::endl;

	// Loop through each digit and segment to display each segment sequentially
	for (uint8_t digit : {0, 1}) {
		for (uint16_t segment = 0; segment <= 14; ++segment) {
			myHT.displayRawData(digit, 1 << segment);  // Set the specific segment for the digit
			delayMilliSecRDL(250);  // Wait for 1 second before moving to the next segment
		}
		myHT.displayRawData(digit, 0);  // Clear the digit after completing the segment loop
	}

	myHT.ClearDigits();
	delayMilliSecRDL(DISPLAY_DELAY_1);

	// More raw data will display  00.
	myHT.displayRawData(0, 0x0C3F); // raw data for 0
	myHT.displayRawData(1, 0x0C3F| 0x4000);  // raw data for 0 or'd with decimal point position
	delayMilliSecRDL(DISPLAY_DELAY_5);
}

void TestBright(void)
{
	std::cout <<  "Test brightness" << std::endl;
	myHT.setBrightness(0x01); // min
	delayMilliSecRDL(DISPLAY_DELAY_5);
	myHT.setBrightness(0x0F); // max
	delayMilliSecRDL(DISPLAY_DELAY_5);
	myHT.setBrightness(brightness); //back to default
	delayMilliSecRDL(DISPLAY_DELAY_1);
}

void TestBlink(void)
{
	std::cout <<  "Test blink mode" << std::endl;
	myHT.setBlink(myHT.BLINKON2HZ); // 2 Hertz , 4 options
	delayMilliSecRDL(DISPLAY_DELAY_5);
	myHT.setBlink(myHT.BLINKOFF);
	delayMilliSecRDL(DISPLAY_DELAY_1);//back to default
}

void TestStandby(void)
{
	std::cout <<  "Test standby mode" << std::endl;
	myHT.DisplaySleep();
	delayMilliSecRDL(DISPLAY_DELAY_5);
	myHT.DisplayNormal();
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void TestOnOff(void)
{
	std::cout <<  "Test on/off" << std::endl;
	myHT.DisplayOff();
	delayMilliSecRDL(DISPLAY_DELAY_5);
	myHT.DisplayOn();
	delayMilliSecRDL(DISPLAY_DELAY_2);
}

void TestChar(void)
{
	std::cout <<  "Test single character 12" << std::endl;
	myHT.ClearDigits(); //Clear display
	delayMilliSecRDL(DISPLAY_DELAY_1);

	myHT.displayChar(0,'1', myHT.DecPointOff);
	myHT.displayChar(1,'2', myHT.DecPointOff);
	delayMilliSecRDL(DISPLAY_DELAY_2);

	myHT.ClearDigits(); //Clear display
	delayMilliSecRDL(DISPLAY_DELAY_2);

	std::cout <<  "Test single character with dec point on, 4.5." << std::endl;
	myHT.displayChar(0,'4', myHT.DecPointOn);
	myHT.displayChar(1,'5', myHT.DecPointOn);
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myHT.ClearDigits(); //Clear display
	delayMilliSecRDL(DISPLAY_DELAY_1);
}


void TestString(void)
{
	std::cout <<  "Test strings" << std::endl;
	// Define test strings
	const char *testStrings[] = {
		"1", "AB", "3.4", "-.1", "..", "2.0", "0.5", "-5"
	};
	const size_t numTests = sizeof(testStrings) / sizeof(testStrings[0]);
	// Loop through each test case
	for (size_t i = 0; i < numTests; ++i) {
		// Align Right
		myHT.displayText(testStrings[i], myHT.AlignRight);
		delayMilliSecRDL(DISPLAY_DELAY_5);
		myHT.ClearDigits();
		// Align Left
		myHT.displayText(testStrings[i], myHT.AlignLeft);
		delayMilliSecRDL(DISPLAY_DELAY_5);
		myHT.ClearDigits();
	}
	myHT.ClearDigits();
	myHT.displayText(testStrings[3]); // display text can also be called without alignment
	delayMilliSecRDL(DISPLAY_DELAY_2);
	myHT.ClearDigits();
}

void TestHex(void)
{
	std::cout <<  "Test Hexadecimal" << std::endl;
	myHT.displayHexChar(0, '0');
	myHT.displayHexChar(1, '1');
	delayMilliSecRDL(DISPLAY_DELAY_2); // 012

	myHT.displayHexChar(0, 'A');
	myHT.displayHexChar(1, 'B');
	delayMilliSecRDL(DISPLAY_DELAY_2); // display ABC
	myHT.ClearDigits();
}

void TestIntPos(void)
{
	std::cout << "Test positive integers" << std::endl;
	// Array of numbers to test
	int32_t testNumbers[] = {1, 34};
	HT16K33plus_Model1::TextAlignment_e alignments[] = {myHT.AlignRight, myHT.AlignLeft, myHT.AlignRightZeros};
	// Loop through each number and alignment
	for (int32_t  num : testNumbers)
	{
		for (HT16K33plus_Model1::TextAlignment_e align : alignments)
		{
			myHT.displayIntNum(num, align);
			delayMilliSecRDL(DISPLAY_DELAY_2);
			myHT.ClearDigits();
		}
	}
	myHT.ClearDigits();
}

void TestIntNeg(void)
{
	std::cout << "Test negative integers" << std::endl;
	// Array of numbers to test
	int32_t testNumbers[] = {-2, -6};
	HT16K33plus_Model1::TextAlignment_e alignments[] = {myHT.AlignRight, myHT.AlignLeft, myHT.AlignRightZeros};
	// Loop through each number and alignment
	for (int32_t  num : testNumbers)
	{
		for (HT16K33plus_Model1::TextAlignment_e align : alignments)
		{
			myHT.displayIntNum(num, align);
			delayMilliSecRDL(DISPLAY_DELAY_2);
			myHT.ClearDigits();
		}
	}
}

void TestFloat(void)
{
	std::cout << "Test floating point numbers " << std::endl;
	// Array of numbers to test
	float testNumbers[] = {0.5, 1.0, 2.5, 1.39};
	HT16K33plus_Model1::TextAlignment_e alignments[] = {myHT.AlignRight, myHT.AlignLeft};
	// Loop through each number and alignment
	for (float  num : testNumbers)
	{
		for (HT16K33plus_Model1::TextAlignment_e align : alignments)
		{
			myHT.displayFloatNum(num, align, 1);  // One digit precision
			delayMilliSecRDL(DISPLAY_DELAY_2);
			myHT.ClearDigits();
		}
	}
	myHT.ClearDigits();
}


void endTest()
{
	myHT.ClearDigits(); //Clear display
	myHT.DisplayOff();
	myHT.Display_I2C_OFF();  // Switch off I2C , optional.
	std::cout << "Test End" << std::endl ;
}

// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	endTest();
	exit(signum);
}


/// @endcond
