/*!
	@file examples/nokia5110/SPEEDTEST/main.cpp
	@brief Description library test file,carries out FPS test  , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 601 FPS test (frame rate per second)
*/

// Libraries
#include <lgpio.h>
#include <iostream> // for std::cout
#include <ctime>
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

// Test Setup
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
uint16_t count = 0;
bool colour = 1;
uint64_t  previousCounter = 0;
uint16_t countLimit = 1500;

// GPIO
const uint8_t RST_LCD = 25; // GPIO pin number pick any you want
const uint8_t DC_LCD = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEV = 4; // GPIO chip device RPI 5 = 4 , other RPIs = 0

//  LCD
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
#define LCD_INV  false // set to true to invert display pixel color
#define LCD_CST  0xB2 // contrast default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define LCD_BIAS 0x13 // LCD LCD_BIAS mode 1:48: Try 0x12 or 0x13 or 0x14

// Hardware SPI setup
int SPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

NOKIA_5110_RPI myLCD(MY_LCD_WIDTH, MY_LCD_HEIGHT , RST_LCD, DC_LCD);

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
	std::cout<< "Nokia 5110 library version : " << GetRDLibVersionNum() << std::endl;
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, SPI_DEVICE, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rpiDisplay_Success)
	{
		std::cout<< "Error 1202: Setup :Cannot start spi" << std::endl;
		return false;
	}
	delayMilliSecRDL(250);
	myLCD.LCDdisplayClear();
	return true;
}

void EndTests(void)
{
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
	myLCD.LCDdisplayClear();
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
	myLCD.print(GetRDLibVersionNum());

	myLCD.fillRect(60, 1, 20, 20, colour);
	myLCD.fillCircle(60, 35, 10, !colour);
	// *****
	myLCD.LCDdisplayUpdate();
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
