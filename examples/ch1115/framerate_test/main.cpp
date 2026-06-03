/*!
	@file examples/ch1115/framerate_test/main.cpp
	@brief Example file for ER_OLEDM1_CH1115 display, measuring Frame rate per second (FPS)
			Configurable HW or SW SPI at compile time.
	@author Gavin Lyons.
	@test
		-# Test 601 hardware SPI
		-# Test 602 software SPI
*/

#include <ctime>
#include <cstdio>
#include "ERM1_CH1115_OLED_RDL.hpp"

/// @cond

// -------------------------------------------------------------------------
// CONFIGURATION: Comment out for software SPI, leave in for hardware SPI 
// -------------------------------------------------------------------------
#define hardwareSPI 

// ==== Globals & Common Settings ====
const uint8_t MY_OLED_WIDTH  = 128;
const uint8_t MY_OLED_HEIGHT = 64;
const uint8_t OLEDcontrast = 0x80; // Contrast 00 to FF, 0x80 is default.
#define myScreenSize (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))

// Common GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24;  // GPIO pin number pick any you want
int GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

// Test variables
uint16_t count  = 0;
bool colour = 1;
uint64_t previousCounter = 0;

// =========================================================================
// SPI-Specific Parameters & Object Initialization
// =========================================================================
#ifdef hardwareSPI
	uint16_t countLimit = 1500; // HW SPI limit
	// Hardware SPI setup parameters
	int SPI_DEVICE = 0;       // A SPI device, >= 0. which SPI interface to use
	int SPI_CHANNEL = 0;      // A SPI channel, >= 0. Which Chip enable pin to use
	int SPI_SPEED =  1000000; // The speed of serial communication in bits per second.
	int SPI_FLAGS = 0;        // last 2 LSB bits define SPI mode, mode 0 for this device
	// Constructor variant for Hardware SPI
	ERMCH1115 myOLED(MY_OLED_WIDTH, MY_OLED_HEIGHT, RES, DC);
#else // Software SPI Mode active
	uint16_t countLimit = 1500; // SW SPI
	// Software SPI pin mappings
	const uint8_t DIN = 5;
	const uint8_t SCLK = 6;
	const uint8_t CS = 21;
	// Constructor variant for Software SPI
	ERMCH1115 myOLED(MY_OLED_WIDTH, MY_OLED_HEIGHT, RES, DC, CS, SCLK, DIN);
#endif
// ===

// === Function prototypes ===
bool Setup(void);
void myTest(void);
void EndTest(void);
void display(long , int );
static uint64_t counter( void );

// === Main ===
int main()
{
	if(!Setup()) return -1;
	myTest();
	EndTest();
	return 0;
}
// === End of main  ===


bool Setup(void)
{
	printf("OLED Begin\r\n");
	printf("lgpio library Version Number :: %i\r\n", lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", rdlib::LibraryVersion()); 
	delayMilliSecRDL(50);

#ifdef hardwareSPI
	printf("Running in HARDWARE SPI Mode...\r\n");
	// Initialize using Hardware SPI parameters
	if(myOLED.OLEDbegin(OLEDcontrast, SPI_DEVICE, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		printf("Error 1201: Setup GPIO/HW SPI failed, \r\n");
		return false;
	}
#else
	printf("Running in SOFTWARE SPI Mode...\r\n");
	// Initialize using Software SPI parameters
	if(myOLED.OLEDbegin(OLEDcontrast, GPIO_CHIP_DEVICE) != rdlib::Success) 
	{
		printf("Error 1202: Setup GPIO/SW SPI failed, \r\n");
		return false;
	}
	delayMilliSecRDL(50);
	printf("Software SPI Freq delay is set to %u uS \n", myOLED.OLEDHighFreqDelayGet());
#endif

	delayMilliSecRDL(50);
	myOLED.OLEDFillScreen(0x0F); // splash screen bars
	delayMilliSecRDL(1500);
	return true;
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	printf("OLED End\r\n");
}

void myTest() {

#ifdef hardwareSPI
	printf("FPS HW SPI:: test ends at %u\r\n", countLimit);
#else
	printf("FPS SW SPI:: test ends at %u\r\n", countLimit);
#endif

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize]; // 1024 bytes = 128 * 64/8
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer) != rdlib::Success) return;

	myOLED.OLEDclearBuffer(); // Clear buffer

	while (count < countLimit)
	{
		static long framerate = 0;
		display(framerate, count);
		framerate++;
		count++;
		delayMilliSecRDL(1);
	}
}

// Function to display left hand side buffer
void display(long currentFramerate, int count)
{
	myOLED.OLEDclearBuffer();

	myOLED.setCursor(0, 10);
	myOLED.print("1024 bytes");
	myOLED.setCursor(0, 20);
	myOLED.print("G Lyons");
	myOLED.setCursor(0, 30);
	myOLED.print(count);

	// Values to count frame rate per second
	// cppcheck-suppress variableScope
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

	myOLED.setCursor(0, 40);
	myOLED.print(fps);
	myOLED.print(" fps");
	myOLED.setCursor(0, 50);
	myOLED.print(rdlib::LibraryVersion());

	myOLED.drawFastVLine(64, 0, 63, myOLED.BLACK);
	myOLED.fillRect(70, 10, 20, 20, colour);
	myOLED.fillCircle(110, 20, 10, !colour);
	myOLED.drawRoundRect(80, 40, 40, 20, 10, myOLED.BLACK);
	myOLED.drawPixel(65, 60, myOLED.BLACK);

	myOLED.OLEDupdate();
}

// This returns nano-seconds as a 64-bit unsigned number.
static uint64_t counter( void )
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec;
}

/// @endcond
