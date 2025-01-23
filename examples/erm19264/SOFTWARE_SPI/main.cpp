/*!
	@file  examples/erm19264/SOFTWARE_SPI/main.cpp
	@brief Example file for ERM19264_LCD display, showing FPS, SW SPI
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test
		-# 601 FPS frame rate per second with SW SPI
*/

#include <lgpio.h>
#include <ctime>
#include <cstdio>
#include "ERM19264_UC1609_LCD_RDL.hpp"

// Screen
const uint8_t MY_LCD_WIDTH  = 192;
const uint8_t MY_LCD_HEIGHT = 64;
#define  MY_SCREEN_SIZE (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8)) 
const uint8_t LCDcontrast = 0x49; // Contrast 00 to FF , 0x49 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

// GPIO pin number pick any you want
const uint8_t RST = 25;
const uint8_t CD = 24;
const uint8_t DIN = 5;
const uint8_t SCLK = 6;
const uint8_t CS = 21;
int  GPIO_CHIP_DEVICE = 0;

// software SPI constructor
ERM19264_UC1609 myLCD(MY_LCD_WIDTH ,MY_LCD_HEIGHT , RST, CD, CS, SCLK,DIN);

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;
uint16_t countLimit = 1000;

// =============== Function prototype ================
bool setup(void);
bool myTest(void);
void EndTest(void);
void displayData(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main()
{
	if(!setup()) {return -1;}
	if(!myTest()) {return -1;}
	EndTest();

	return 0;
}
// ======================= End of main  ===================

bool setup() {
	printf("LCD Test Begin\r\n");
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum()); 
	delayMilliSecRDL(100);
	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		return false;
	}
	printf("Frequency Software SPI delay = %u uS\r\n", myLCD.LCD_HighFreqDelayGet()); 
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	delayMilliSecRDL(1000);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	printf("LCD End\r\n");
}

bool myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[MY_SCREEN_SIZE];
	if (myLCD.LCDSetBufferPtr(MY_LCD_WIDTH, MY_LCD_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success)
	{
		return false;
	}
	myLCD.LCDclearBuffer();  // Clear buffer
	//myLCD.LCD_HighFreqDelaySet(1); // optional set the GPIO delay for software SPI
	printf("Stops at :: %u \n ", countLimit);
	while (count < countLimit)
	{
		static long framerate = 0;
		displayData(framerate, count);

		framerate++;
		count++;
		delayMilliSecRDL(1);
	}
	return true;
}

// Function to display data
void displayData(long currentFramerate, int count)
{
	myLCD.LCDclearBuffer();

	myLCD.setCursor(0, 10);
	myLCD.print(myLCD.LCD_HighFreqDelayGet()); //Software SPI GPIO delay
	myLCD.setCursor(0, 20);
	myLCD.print("G Lyons");

	myLCD.setCursor(0, 30);
	myLCD.print(count);
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

	myLCD.setCursor(0, 40);
	myLCD.print(fps);
	myLCD.print(" FPS SWSPI");
	myLCD.setCursor(0, 50);
	myLCD.print(GetRDLibVersionNum());
	myLCD.drawFastVLine(92, 0, 63, RDL_BLACK);


	myLCD.fillRect(97, 10, 20, 20, colour);
	myLCD.fillCircle(137, 20, 10, RDL_BLACK);
	myLCD.fillTriangle(157, 30, 167, 10, 177, 30, !colour);
	myLCD.drawRoundRect(107, 40, 60, 20, 10, RDL_BLACK);

	myLCD.LCDupdate();
}

//This returns nano-seconds as a 64-bit unsigned number, monotonically increasing,
//probably since system boot.
//The actual resolution looks like microseconds. returns nanoseconds
static uint64_t counter( void )
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec;
}
