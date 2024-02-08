/*!
	@file  examples/bicolor_displays/erm19264/FRAMERATE_PER_SEC/main.cpp
	@brief Example file for ERM19264_LCD display, showing FPS, HW SPI
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@details 
			Measured:: frame rate 214 fps at 64 clock divider on Rpi 3 v101
			Measured:: frame rate 350 fps at 16 clock divider on Rpi 3 v101
	@test
		-# 601 FPS frame rate per second HW SPI
*/

#include <bcm2835.h>
#include <ctime>
#include <cstdio>
#include "ERM19264_UC1609_LCD_RDL.hpp"

// LCD Setup
const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want 
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;
#define myScreenSize (myLCDwidth * (myLCDheight/8))

const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t LCDcontrast = 0x49; //Constrast 00 to FF , 0x80 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

 // instantiate  an object
ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD) ;

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
bool setup(void);
void myTest(void);
void EndTest(void);
void displayData(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main()
{

	if(!setup()) {return -1;}
	myTest();
	EndTest();

	return 0;
}
// =============== End of main  ===============


// ============== Function Space ============

bool setup() {
	printf("LCD Test Begin\r\n");
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}
	bcm2835_delay(100);
	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, SPICLK_FREQ , SPI_CE_PIN) != rpiDisplay_Success)
	{
		printf("Error 1202: bcm2835_spi_begin :Cannot start spi, Running as root?\n");
		bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
		return false;
	}
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("ERM19264 Library version number :: %u \r\n", GetRDLibVersionNum());
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	bcm2835_delay(1000);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
	printf("LCD End\r\n");
}

void myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize];
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success)
	{
		bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
		exit(-1);
	};
	myLCD.LCDclearBuffer();  // Clear buffer
	while (count < 10000)
	{
		static long framerate = 0;
		displayData(framerate, count);
		framerate++;
		count++;
		bcm2835_delay(1);
	}

}

// Function to display data
void displayData(long currentFramerate, int count)
{
	myLCD.LCDclearBuffer();

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
	myLCD.print(" fps");
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
//The actual resolution looks like microseconds. 
// Returns nanoseconds
static uint64_t counter( void )
{
  struct timespec now;
  clock_gettime( CLOCK_MONOTONIC, &now );
  return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec;
}

// ******************** EOF *********************8
