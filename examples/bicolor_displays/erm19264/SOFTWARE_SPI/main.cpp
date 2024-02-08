/*!
	@file  examples/bicolor_displays/erm19264/SOFTWARE_SPI/main.cpp
	@brief Example file for ERM19264_LCD display, showing FPS, SW SPI
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@details 
			measured frame rate = 66 fps, 1:49 min to 10000, v101, rpi3
	@test
		-# 601 FPS frame rate per second with SW SPI
*/

#include <bcm2835.h>
#include <ctime>
#include <cstdio>
#include "ERM19264_UC1609_LCD_RDL.hpp"

// LCD SETUP
const uint8_t LCDcontrast = 0x50; //Constrast 00 to FF , 0x50 is default. user adjust
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;
#define myScreenSize (myLCDwidth * (myLCDheight/8))

// GPIO pin number pick any you want
const uint8_t RST = 25;
const uint8_t CD = 24;
const uint8_t DIN = 5;
const uint8_t SCLK = 6;
const uint8_t CS = 8;

// software SPI constructor
ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD, CS, SCLK,DIN);

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

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
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}
	bcm2835_delay(100);
	myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast);
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("ERM19264 Library version number :: %u \r\n", GetRDLibVersionNum());
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	bcm2835_delay(1500);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
	printf("LCD End\r\n");
}

bool myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize];
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success)
	{
		bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
		return false;
	}
	myLCD.LCDclearBuffer();  // Clear buffer
	//myLCD.LCD_HighFreqDelaySet(1); // optional set the GPIO delay for software SPI
	while (count < 5000)
	{
		static long framerate = 0;
		displayData(framerate, count);

		framerate++;
		count++;
		bcm2835_delay(1);
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
