
// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, FPS TEST
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
//
// Test results values SPICLK_FREQ ::
// 256 = BCM2835_SPI_CLOCK_DIVIDER_256 =  120 FPS , ~1.6 MHz RPI3
// 64  = default(BCM2835_SPI_CLOCK_DIVIDER_64) = 270 FPS ,~6 MHZ RPI3
// 8 = BCM2835_SPI_CLOCK_DIVIDER_8 = 470 FPS , ~50 MHZ RPI3

// *****************************

#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "ER_OLEDM1_CH1115.hpp"


// ==== Globals ====
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want 
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;

const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC); // instantiate  an object

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
void setup(void);
void myTest(void);
void display(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!bcm2835_init())
	{
		printf("OLED :: ERROR failed to init bcm2835 library.\r\n");
		return -1;
	}

	setup();
	myTest();

	myOLED.OLEDPowerDown();
	bcm2835_close(); // Close the library, 
	printf("OLED End\r\n");
	return 0;
}
// ======================= End of main  ===================


void setup()
{
	bcm2835_delay(50);
	printf("OLED Begin\r\n");
	myOLED.OLEDbegin(OLEDcontrast, SPICLK_FREQ, SPI_CE_PIN); // initialize the OLED
	myOLED.OLEDFillScreen(0x03);
	bcm2835_delay(2400);
}

void myTest() {
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(1);
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[(myOLEDwidth * (myOLEDheight / 8))+1]; // 1024 bytes = 128 * 64/8
	myOLED.OLEDbuffer = (uint8_t*) &screenBuffer;  // Assign the pointer to the buffer
	myOLED.OLEDclearBuffer(); // Clear buffer

	while (count < 10000)
	{
		static long framerate = 0;
		display(framerate, count);
		framerate++;
		count++;
		bcm2835_delay(1);
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
	myOLED.print("V 1.3.2");
	
	myOLED.drawFastVLine(64, 0, 63, FOREGROUND);
	myOLED.fillRect(70, 10, 20, 20, colour);
	myOLED.fillCircle(110, 20, 10, !colour);
	myOLED.drawRoundRect(80, 40, 40, 20, 10, FOREGROUND);
	myOLED.drawPixel(65, 60, FOREGROUND);
	
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
