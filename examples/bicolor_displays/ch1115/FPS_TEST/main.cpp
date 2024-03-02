/*!
	@file examples/bicolor_displays/ch1115/FPS_TEST/main.cpp
	@brief Example file for ER_OLEDM1_CH1115 display, measuring Frame rate per second)FPS, HW SPI
		Test results values SPICLK_FREQ ::

		| test no | SPICLK_FREQ | BCM2835_SPI_CLOCK_DIVIDER | FPS result  | freq|
		| ---- |  ---- |  ---- | ----  | ---- |
		|1| 256| BCM2835_SPI_CLOCK_DIVIDER_256 |120 |  ~1.6 MHz RPI3 |
		|2| 64 | BCM2835_SPI_CLOCK_DIVIDER_64 | 270 | ~6 MHZ RPI3 |
		|3|  8 | BCM2835_SPI_CLOCK_DIVIDER_8 | 470 | ~50 MHZ RPI3 |

	@test
		-# Test 601 FPS HW SPI
*/

#include <bcm2835.h>
#include <ctime>
#include <cstdio>
#include "ERM1_CH1115_OLED_RDL.hpp"


// ==== Globals ====
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
#define myScreenSize (myOLEDwidth * (myOLEDheight/8))

const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC); // instantiate  an object

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;
uint16_t countLimit = 10000;

// =============== Function prototype ================
bool Setup(void);
void myTest(void);
void EndTest(void);
void display(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main()
{
	if(!Setup()) return -1;
	myTest();
	EndTest();
	return 0;
}
// ======================= End of main  ===================


bool Setup(void)
{
	printf("OLED Begin\r\n");
	if(!bcm2835_init())
	{
		printf("Error 1201 : Setup : Problem with init bcm2835 library\r\n");
		return false;
	}else{
		printf("bcm2835 library version : %u\r\n", bcm2835_version());
	}
	delayMilliSecRDL(50);
	if(myOLED.OLEDbegin(OLEDcontrast, SPICLK_FREQ , SPI_CE_PIN) != rpiDisplay_Success) // initialize the OLED
	{
		printf("Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?\r\n");
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return false;
	}
	printf("CH1115 OLED library version : %u\r\n", GetRDLibVersionNum());
	delayMilliSecRDL(50);
	myOLED.OLEDFillScreen(0x77); //splash screen bars
	delayMilliSecRDL(1000);
	return true;
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	bcm2835_close(); //Close lib & /dev/mem, deallocating mem
	printf("OLED End\r\n");
}

void myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize ]; // 1024 bytes = 128 * 64/8
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;

	myOLED.OLEDclearBuffer(); // Clear buffer
	printf("FPS HW SPI:: test ends at 10000\r\n");
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
	myOLED.print(GetRDLibVersionNum());

	myOLED.drawFastVLine(64, 0, 63, RDL_BLACK);
	myOLED.fillRect(70, 10, 20, 20, colour);
	myOLED.fillCircle(110, 20, 10, !colour);
	myOLED.drawRoundRect(80, 40, 40, 20, 10, RDL_BLACK);
	myOLED.drawPixel(65, 60, RDL_BLACK);

	myOLED.OLEDupdate();
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
