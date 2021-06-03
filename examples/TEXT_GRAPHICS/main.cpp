// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, showing use of TEXT mode & graphics.
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
// *****************************

// A series of tests to display the text mode
// Test 1 Font size 3 float
// Test 2 font size 2 integer
// Test 3 font size 1 string inverted
// Test 4 draw a single character font size 4
// Test 5 print ASCII  font 0-127
// Test 6 print ASCII font 128-255,
// Test 7 fill page + fill screen functions
// Test 8 font 1-4
// Test 9 font 5 "bignumers"
// Test 10 simple graphic display 

#include <bcm2835.h>
#include <stdio.h>
#include "ER_OLEDM1_CH1115.h"

// Just for tests
#define DisplayDelay1 5000
#define DisplayDelay2 0
#define PI 3.14

#define OLEDcontrast 0x80 //00 to FF , 0x80 is default. user adjust
#define myOLEDwidth  128
#define myOLEDheight 64
// GPIO
#define RES 25 // GPIO pin number pick any you want
#define DC 24 // GPIO pin number pick any you want

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC) ; // instantiate  an object

// =============== Function prototype ================
void setup(void);
void myTests(void);
void EndTest(void);
void DisplayText(MultiBuffer* );
void DisplayGraphics(MultiBuffer* );

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!bcm2835_init())
	{
		return -1;
	}
	setup();
	myTests();
	EndTest();
	return 0;
}
// ======================= End of main  ===================


// ===================== Function Space =====================

void EndTest()
{
	myOLED.OLEDPowerDown();
	bcm2835_close(); // Close the library, 
	printf("OLED End\r\n");
}

void setup() {
	bcm2835_delay(50);
	printf("OLED Begin\r\n");
	myOLED.OLEDbegin(OLEDcontrast); // initialize the OLED
	myOLED.OLEDFillScreen(0x8F); //splash screen bars
	myOLED.SetFontNum(1);
	bcm2835_delay(2000);
}


void myTests()
{

	// Define a full screen buffer
	uint8_t  textBuffer[(myOLEDwidth * (myOLEDheight / 8)) + 1];
	MultiBuffer window;
	window.screenbitmap = (uint8_t*) &textBuffer;
	window.width = myOLEDwidth;
	window.height = myOLEDheight;
	window.xoffset = 0;
	window.yoffset = 0;
	
	DisplayText(&window); // text tests
	DisplayGraphics(&window); //graphic tests
}

void DisplayText(MultiBuffer* targetBuffer)
{

	myOLED.setTextWrap(true);
	myOLED.ActiveBuffer =  targetBuffer;
	myOLED.OLEDclearBuffer(); // Clear the buffer
	// Test 1
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(3);
	myOLED.setCursor(0, 0);
	myOLED.print(PI);

	// Test 2
	myOLED.setTextSize(2);
	myOLED.setCursor(0, 30);
	int count = 123;
	myOLED.print(count);

	// Test 3
	myOLED.setTextSize(1);
	myOLED.setTextColor(BACKGROUND, FOREGROUND);
	myOLED.setCursor(0, 50);
	myOLED.print("Hello World");

	// Test 4
	myOLED.drawChar(95, 15 , 'H', FOREGROUND, BACKGROUND, 6);

	myOLED.OLEDupdate();  // Write to the buffer
	bcm2835_delay(DisplayDelay1);
	myOLED.OLEDclearBuffer();

	// Test 5
	myOLED.setCursor(0, 0);
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(1);
	myOLED.print("ASCII font ");
	myOLED.setCursor(0, 15);
	char offset = 0;

	// Print first 127 chars of font
	for (char i = offset; i < 126; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myOLED.print(i);
		bcm2835_delay(DisplayDelay2);
	}
	myOLED.OLEDupdate();  // Write to the buffer
	bcm2835_delay(DisplayDelay1);
	myOLED.OLEDclearBuffer();

	myOLED.setCursor(0, 0);
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(1);
	
	// Test 6
	myOLED.print("ASCII font 128-255");

	uint8_t x = 0;
	uint8_t y = 15;
	myOLED.setCursor(x, y);

	for (uint8_t i = 128; i < 255; i++)
	{
		if (x > 180)
		{
			x = 0;
			y += 9;
		}
		myOLED.drawChar(x, y , i, FOREGROUND, BACKGROUND, 1);
		x += 7;
		bcm2835_delay(DisplayDelay2);
	}

	myOLED.OLEDupdate();  // Write to the buffer
	bcm2835_delay(DisplayDelay1);
	myOLED.OLEDclearBuffer();
	

	// Test 7 clear page function
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.OLEDFillPage(0, 0x01); // Write pattern (0000 0001) to a page 0
	myOLED.OLEDFillPage(6, 0x7E); // Write pattern (0111 1110) to a page 6
	bcm2835_delay(2000);
	myOLED.OLEDFillScreen(0x00); // Clear the screen

	// Test 8 Fonts 1-4
	myOLED.setCursor(0, 0);
	myOLED.print("Default font");
	myOLED.SetFontNum(2);
	myOLED.setCursor(0, 9);
	myOLED.print("THICK FONT");
	myOLED.SetFontNum(3);
	myOLED.setCursor(0, 18);
	myOLED.print("Seven seg font");
	myOLED.SetFontNum(4);
	myOLED.setCursor(0, 30);
	myOLED.print("WIDE FONT");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.OLEDclearBuffer();
	
	// Test 9 Font 5
	char mytest[] = "1234567812::5678";
	myOLED.SetFontNum(5);
	//myOLED.drawCharBigNum(0, 0, '1', FOREGROUND, BACKGROUND); // single character
	myOLED.drawTextBigNum(0, 0, mytest, FOREGROUND, BACKGROUND);
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
} // end of test texts

// Function to display Graphics.
void  DisplayGraphics(MultiBuffer* targetBuffer)
{
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	//
	bool colour = 1;
	uint8_t count = 0;
	myOLED.ActiveBuffer =  targetBuffer;   // Set the buffer struct object
	myOLED.OLEDclearBuffer(); // Clear the buffer
	while (count < 15)
	{
		colour = !colour;

		// Draw the X
		myOLED.drawLine(64,  0, 64, 64, FOREGROUND);
		myOLED.drawFastVLine(62, 0, 64, FOREGROUND);
		myOLED.drawFastHLine(0, 32, 128, FOREGROUND);

		// Q1
		myOLED.fillRect(0, 10, 20, 20, colour);
		myOLED.fillCircle(40, 20, 10, FOREGROUND);

		// Q2
		myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
		myOLED.drawRect(105, 10, 15, 15, FOREGROUND);
		// Q3
		myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
		// Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, FOREGROUND);
			myOLED.OLEDupdate();
			bcm2835_delay(50);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
}
