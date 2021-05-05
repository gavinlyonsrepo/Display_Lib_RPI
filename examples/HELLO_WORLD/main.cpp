
// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, showing use of hello world
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
// *****************************
// NOTES :
// (1) In the <ER_OLEDM1_CH1115.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// ******************************

#include <bcm2835.h>
#include "ER_OLEDM1_CH1115.h"
#include <time.h>
#include <stdio.h>

#define OLEDcontrast 0x80 //Constrast 00 to FF , 0x80 is default. user adjust
#define myOLEDwidth  128
#define myOLEDheight 64
#define mode 2

// GPIO 
#define RES 25 // GPIO pin number pick any you want
#define DC 24 // GPIO pin number pick any you want 

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight, RES, DC ) ; // instantiate  an object 

// =============== Function prototype ================
void setup(void);
void myLoop(void);

// ======================= Main ===================
int main(int argc, char **argv) 
{
	if(!bcm2835_init())
	{
		return -1;
	}
	bcm2835_delay(500);
	setup();
	myLoop();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void setup() {
	printf("OLED begin\r\n");
	myOLED.OLEDbegin(OLEDcontrast); // initialize the OLED
	myOLED.OLEDFillScreen(0x0F, 0); //splash screen bars
	bcm2835_delay(3000);
}

void myLoop() {

	// define a buffer to cover whole screen 
	uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight/8)]; // 1024 bytes = 128 * 64/8

	// Declare a buffer struct
	MultiBuffer mybuffer;

	// Intialise struct 
	mybuffer.screenbitmap = (uint8_t*) &screenBuffer; // point it to the buffer
	mybuffer.width = myOLEDwidth ;
	mybuffer.height = myOLEDheight;
	mybuffer.xoffset = 0;
	mybuffer.yoffset = 0;

	// Assign address of struct to be the active buffer pointer 
	myOLED.ActiveBuffer = &mybuffer;

	myOLED.OLEDclearBuffer();   // Clear active buffer 

	myOLED.setTextColor(FOREGROUND);
	myOLED.setCursor(0, 0);
	myOLED.print("Hello world");
	myOLED.OLEDupdate();  //write to active buffer
	delay(5000);
	myOLED.OLEDSPIoff();
	myOLED.OLEDPowerDown();
	bcm2835_close(); // Close the library, deallocating any allocated memory and closing /dev/mem
	printf("OLED End\r\n");
}

// ============== EOF =========
