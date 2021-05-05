
// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, showing use of mulitple buffers.  In this case: two, divided vertically.
// The user can also divide horizontally and create as many buffers as they want.
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
// *****************************
// NOTES :
// (1) In the <ER_OLEDM1_CH1115.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// (2) measured frame rate 279 fps, 37 seconds. 64 clock divider
// (3) measured frame rate 450 fps, 21 seconds.  4 clock divider
// ******************************
// 

#include <bcm2835.h>
#include "ER_OLEDM1_CH1115.h"
#include <time.h>
#include <stdio.h>

#define OLEDcontrast 0x80 //Constrast 00 to FF , 0x80 is default. user adjust
#define myOLEDwidth  128
#define myOLEDheight 64

// GPIO
#define RES 25 // GPIO pin number pick any you want
#define DC 24 // GPIO pin number pick any you want

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC) ; // instantiate  an object

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
void setup(void);
void myLoop(void);
void display_Left(MultiBuffer* , long , int );
void display_Right(MultiBuffer* );
static uint64_t counter( void );

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!bcm2835_init())
	{
		return -1;
	}
	bcm2835_delay(500);
	printf("OLED begin\r\n");

	setup();
	myLoop();

	myOLED.OLEDSPIoff();
	myOLED.OLEDPowerDown();

	bcm2835_close(); // Close the library, deallocating any allocated memory and closing /dev/mem
	printf("OLED End\r\n");
	return 0;
}
// ======================= End of main  ===================


// ************* SETUP ***************
void setup()
{
	myOLED.OLEDbegin(OLEDcontrast); // initialize the OLED
	myOLED.OLEDFillScreen(0x01, 0);
	bcm2835_delay(2400);
}

// *********** MAIN LOOP ******************
void myLoop() {
	

	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(1);
	uint8_t  screenBuffer[(myOLEDwidth * (myOLEDheight / 8)) / 2]; //(128 * 8)/2 = 512 bytes

	MultiBuffer left_side;
	left_side.screenbitmap = (uint8_t*) &screenBuffer;
	left_side.width = (myOLEDwidth / 2) ;
	left_side.height = myOLEDheight;
	left_side.xoffset = 0;
	left_side.yoffset = 0;


	MultiBuffer right_side;
	right_side.screenbitmap = (uint8_t*) &screenBuffer;
	right_side.width = (myOLEDwidth / 2);
	right_side.height = myOLEDheight;
	right_side.xoffset = (myOLEDwidth / 2);
	right_side.yoffset = 0;

	while (count < 10000)
	{
		static long framerate = 0;
		display_Left(&left_side, framerate, count);

		display_Right(&right_side);

		framerate++;
		count++;
		bcm2835_delay(1);
	}

}
// *********** END OF MAIN ***********

// Function to display left hand side buffer
void display_Left(MultiBuffer* targetbuffer, long currentFramerate, int count)
{
	myOLED.ActiveBuffer = targetbuffer; // set target buffer object
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("LHS Buffer");

	myOLED.setCursor(0, 10);
	myOLED.print("512 bytes");

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
	myOLED.print("V 1.0.0");
	myOLED.drawFastVLine(92, 0, 63, FOREGROUND);
	myOLED.OLEDupdate();
}

// Function to display right hand side buffer
void display_Right(MultiBuffer* targetbuffer)
{
	myOLED.ActiveBuffer = targetbuffer; // set target buffer object
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("RHS buffer");

	myOLED.fillRect(0, 10, 20, 20, colour);
	myOLED.fillCircle(40, 20, 10, !colour);
	myOLED.drawRoundRect(10, 40, 40, 20, 10, FOREGROUND);

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
