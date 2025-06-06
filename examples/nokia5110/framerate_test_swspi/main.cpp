/*!
	@file examples/nokia5110/framerate_test_swspi/main.cpp
	@brief Description library test file,carries out FPS test  , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 602 SWSPI FPS test (frame rate per second)
*/


// Libraries

#include <iostream> // for std::cout
#include <ctime>
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

/// @cond

// Test Setup
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
uint16_t count = 0;
bool colour = 1;
uint64_t  previousCounter = 0;
uint16_t countLimit = 2500;

// GPIO
const uint8_t RST_LCD = 25;
const uint8_t DC_LCD = 24;
const uint8_t DIN_LCD = 5;
const uint8_t CLK_LCD = 6;
const uint8_t CE_LCD = 21;
int  GPIO_CHIP_DEV = 0; // GPIO chip device number usually 0

//  LCD
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
#define FULLSCREEN (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
#define LCD_INV  false // set to true to invert display pixel color
#define LCD_CST  0xB2 // contrast default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define LCD_BIAS 0x13 // LCD LCD_BIAS mode 1:48: Try 0x12 or 0x13 or 0x14

NOKIA_5110_RPI myLCD(MY_LCD_WIDTH, MY_LCD_HEIGHT , RST_LCD ,DC_LCD, CE_LCD, DIN_LCD, CLK_LCD);

// =============== Function prototype ================
bool Setup(void);
void myLoop(void);
void display_buffer(long , int );
void EndTests(void);
static uint64_t counter( void );

// ======================= Main ===================
int main()
{

	if (!Setup()){return -1;}
	myLoop();
	EndTests();
	return 0;
}
// ======================= End of main  ===================

bool Setup(void)
{
	std::cout << "LCD Start"  << std::endl;
	std::cout<< "lgpio library version : " << lguVersion() << std::endl;
	std::cout<< "Nokia 5110 library version : " << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		std::cout<< "Error 1202: Setup :Cannot start spi" << std::endl;
		return false;
	}
	printf("Frequency Software SPI delay = %u uS\r\n", myLCD.LCDHighFreqDelayGet());
	if (myLCD.LCDSetBufferPtr(screenBuffer) != rdlib::Success)
	{
		std::cout<< "Error 1202: Setup :Cannot Assign Buffer" << std::endl;
		return false;
	}
	delayMilliSecRDL(250);
	myLCD.LCDfillScreen();
	return true;
}

void EndTests(void)
{
	myLCD.LCDfillScreen();
	myLCD.LCDclearBuffer();
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	std::cout << "LCD End" << std::endl;
}

void myLoop() {

	std::cout<< "Count Limit  = " << countLimit << std::endl;
	while (count < countLimit)
	{
		static long framerate = 0;
		display_buffer(framerate, count);
		framerate++;
		count++;
	}

}

void display_buffer(long currentFramerate, int count)
{
	// Values to count frame rate per second
	static long lastFramerate = 0;
	static uint16_t fps;
	uint64_t currentCounter = counter();
	if(currentCounter - previousCounter >= 1000000000)
	{
		//(Rolls over every 1 second)
		fps = currentFramerate - lastFramerate;
		lastFramerate = currentFramerate ;
		previousCounter = currentCounter;
		colour = !colour;
	}
	
	// *****
	// Code to speed test here
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);
	myLCD.print("Nokia");

	myLCD.setCursor(0, 10);
	myLCD.print("G Lyons");

	myLCD.setCursor(0, 20);
	myLCD.print(count);

	myLCD.setCursor(0, 30);
	myLCD.print(fps);
	myLCD.print(" fps");
	myLCD.setCursor(0, 40);
	myLCD.print(rdlib::LibraryVersion());

	myLCD.fillRect(60, 1, 20, 20, colour);
	myLCD.fillCircle(60, 35, 10, !colour);
	// *****
	myLCD.LCDupdate();
}

// This func returns nano-seconds as a 64-bit unsigned number, 
// monotonically increasing, probably since system boot.
// The actual resolution looks like microseconds. returns nanoseconds
static uint64_t counter( void )
{
  struct timespec now;
  clock_gettime( CLOCK_MONOTONIC, &now );
  return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec; 
}

/// @endcond
