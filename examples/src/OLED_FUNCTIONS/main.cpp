
// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, showing use of various function
//
//  (1) Enable and disable Screen (Sleep mode , 500uA)
//  (2) Invert screen colour
//  (3) Rotate vertically screen 180 (flip )
//  (4) Contrast screen
//  (5) Scroll Screen
//  (6) Fade effect
//
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
// *****************************

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

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC ) ;

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

	setup();
	myLoop();

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
	myOLED.OLEDbegin(OLEDcontrast); // initialize the OLED
	myOLED.OLEDFillScreen(0x11); // Clears screen with pattern 0x11
	bcm2835_delay(1500);
}

void myLoop()
{
	// Define a full screen buffer and struct
	uint8_t  screenBuffer[myOLEDwidth * (myOLEDheight / 8) ];

	MultiBuffer whole_screen;
	// Intialise that struct with buffer details (&struct,  buffer, w, h, x-offset,y-offset)
	myOLED.OLEDinitBufferStruct(&whole_screen, screenBuffer, myOLEDwidth, myOLEDheight, 0, 0);
	
	myOLED.ActiveBuffer =  &whole_screen; // set buffer object
	myOLED.OLEDclearBuffer(); // clear the buffer

	// Set text parameters
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(1);


	//  ** Test1 OLED enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("OLED Disable test 1");
	myOLED.OLEDupdate();

	bcm2835_delay(3000); //display message
	myOLED.OLEDEnable(0); //turn off display (sleep mode 500uA measured)
	bcm2835_delay(3000); //wait for 3
	myOLED.OLEDEnable(1); //turn on
	bcm2835_delay(2000); //display message
	myOLED.OLEDclearBuffer();

	// ** Test 2 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("OLED Inverse test 2");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDInvert(1); // Inverted
	bcm2835_delay(4000);
	myOLED.OLEDInvert(0);
	bcm2835_delay(1000);


	// ** Test3 OLED flip **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Flip test 3");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDFlip(1);
	bcm2835_delay(3000);
	myOLED.OLEDFlip(0);
	bcm2835_delay(2000);

	// ** Test 4 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test 4");
	myOLED.OLEDupdate();
	bcm2835_delay(2500);
	myOLED.OLEDFillScreen(0x77); // fill screen

	myOLED.OLEDContrast(0x00);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0x80);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0xFF);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(OLEDcontrast);
	bcm2835_delay(1000);
	myOLED.OLEDclearBuffer();


	// ** Test5 OLED scroll **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 40);
	myOLED.print("Scroll test 5 ");
	myOLED.OLEDupdate();
	bcm2835_delay(2500);

	// See .cpp file for more info on these parmeters.
	uint8_t timeInterval = 0x00; // 6 frames 0x00 - 0x07
	uint8_t scrollDirection = 0x26; //right 0x26 or 0x27
	uint8_t scrollMode = 0x28; // contiunous 0x28-0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
	myOLED.OLEDscroll(1); //start scroll
	bcm2835_delay(15000);
	myOLED.OLEDscroll(0); // stop Scroll

	bcm2835_delay(1);

	timeInterval = 0x02; // 64 frames , 0x00 - 0x07
	scrollDirection = 0x27; // left , 0x26 or 0x27
	scrollMode = 0x29; // contiunous 0x28 one shot 0x29 , one col 0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
	myOLED.OLEDscroll(1); //start
	bcm2835_delay(15000);
	myOLED.OLEDscroll(0); // stop

	// ** TEST 6 fade ~ breath effect **
	myOLED.OLEDclearBuffer();
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Fade effect test 6");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDfadeEffect();
	myOLED.OLEDFillScreen(0xE3);
	bcm2835_delay(10000);
	myOLED.OLEDfadeEffect(0x00); // switch off fade

	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);

	myOLED.print("End of tests");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
}

// *********** EOF ***********
