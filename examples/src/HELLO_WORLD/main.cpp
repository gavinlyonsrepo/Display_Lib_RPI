
// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, showing use of hello world
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
// *****************************

#include <bcm2835.h>
#include "ER_OLEDM1_CH1115.h"
#include <stdio.h>

#define OLEDcontrast 0x80 //Constrast 00 to FF , 0x80 is default.
#define myOLEDwidth  128
#define myOLEDheight 64
#define mode 2

// GPIO 
#define RES 25 // GPIO pin number pick any you want
#define DC 24 // GPIO pin number pick any you want 

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight, RES, DC ); // instantiate an object 

// =============== Function prototype ================
void setup(void);
void myTest(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv) 
{
	if(!bcm2835_init())
	{
		return -1;
	}
	setup();
	myTest();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void setup() {
	bcm2835_delay(50);
	printf("OLED Begin\r\n");
	myOLED.OLEDbegin(OLEDcontrast); // initialize the OLED
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	bcm2835_delay(3000);
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	bcm2835_close(); //Close lib & /dev/mem, deallocating mem 
	printf("OLED End\r\n");
}

void myTest() {

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
	myOLED.setCursor(20, 20);
	myOLED.print("Hello world");
	myOLED.OLEDupdate();  //write to active buffer
	delay(7000);
}

// ============== EOF =========
