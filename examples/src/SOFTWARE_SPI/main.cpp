
// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, showing use SOFTWARE SPI
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
// *****************************
// NOTES :
// (1) Speed test results measured frame rate 93 fps, 1:49s to count 10000
// (2) This is SOFTWARE SPI
// ******************************
// 

#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "ER_OLEDM1_CH1115.hpp"


// ==== Globals ====

const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

// GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
const uint8_t DIN = 5; // GPIO pin number pick any you want
const uint8_t SCLK = 6; // GPIO pin number pick any you want
const uint8_t CS = 8; // GPIO pin number pick any you want


// software SPI
ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC, CS, SCLK,DIN) ; 

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
void setup(void);
void myLoop(void);
void display(long, int );
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
	myLoop();

	myOLED.OLEDPowerDown();
	bcm2835_close(); // Close library,deallocating any allocated mem
	printf("OLED End\r\n");
	return 0;
}
// ======================= End of main  ===================


// ************* SETUP ***************
void setup()
{
	bcm2835_delay(50);
	printf("OLED Begin\r\n");
	myOLED.OLEDbegin(OLEDcontrast); // initialize the OLED
	myOLED.OLEDFillScreen(0x01);
	bcm2835_delay(2000);
}

// *********** myLoop ******************
void myLoop() {

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

// *********** EOF ***********
