 /*!
	@file examples/src/TEXT_FONTS/main.cpp
	@brief Example file for ER_OLEDM1_CH1115_RPI library, showing testing of fonts and test modes. HW SPI.
	@note
		-URL : https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI

	@test
		-# Test 701 Font size 3 float
		-# Test 702 font size 2 integer
		-# Test 703 font size 1 string inverted
		-# Test 704 draw a single character font size 4
		-# Test 705 print ASCII  font 0-127
		-# Test 706 print ASCII font 128-255
		-# Test 707 font 1-6
		-# Test 708 font 7-10
		-# Test 709 print + drawtext function

*/

#include <bcm2835.h>
#include <cstdio>
#include "ER_OLEDM1_CH1115.hpp"

// ===== Test data Defines ====
#define MYTESTDELAY 5000
#define MYTESTDELAY2 0
#define PI 3.14

// ==== Globals ====
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // 1024 bytes = 128 * 64/8

const uint32_t SPICLK_FREQ = 64; // Spi clock divid bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC) ; // instantiate  an object

// =============== Function prototype ================
bool Setup(void);
void myTests(void);
void EndTest(void);
void screenReset(void);

void TestFonts1_6(void);
void TestFonts7_10(void);
void TestPrint(void);


// ======================= Main ===================
int main(int argc, char **argv){
	if(!Setup())return -1;
	myTests();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================

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
	bcm2835_delay(50);
	if(!myOLED.OLEDbegin(OLEDcontrast, SPICLK_FREQ , SPI_CE_PIN)) // initialize the OLED
	{
		printf("Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?\r\n");
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return false;
	}
	printf("CH1115 OLED library version : %u\r\n", myOLED.OLEDLibVerNumGet());
	bcm2835_delay(50);
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	bcm2835_delay(1000);
	return true;
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	bcm2835_close(); //Close lib & /dev/mem, deallocating mem
	printf("OLED End\r\n");
}


void myTests()
{
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize];
	
	if (!myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)/sizeof(uint8_t))) 
		return;

	myOLED.OLEDclearBuffer(); // Clear buffer
	TestFonts1_6();
	TestFonts7_10();
	TestPrint();
}


void screenReset()
{
	myOLED.OLEDupdate();
	bcm2835_delay(MYTESTDELAY);
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.OLEDclearBuffer();
}

void TestFonts1_6()
{
	myOLED.setTextWrap(true);
	myOLED.OLEDclearBuffer(); // Clear the buffer
	// Test 701
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(3);
	myOLED.setCursor(0, 0);
	myOLED.print(PI);

	// Test 702
	myOLED.setTextSize(2);
	myOLED.setCursor(0, 30);
	int count = 123;
	myOLED.print(count);

	// Test 703
	myOLED.setTextSize(1);
	myOLED.setTextColor(BACKGROUND, FOREGROUND);
	myOLED.setCursor(0, 50);
	myOLED.print("Hello World");

	// Test 704
	myOLED.drawChar(95, 15 , 'H', FOREGROUND, BACKGROUND, 6);

	screenReset();

	// Test 705
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
		bcm2835_delay(MYTESTDELAY2);
	}
	screenReset();

	myOLED.setCursor(0, 0);
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(1);

	// Test 706
	myOLED.print("ASCII font 128-254");

	uint8_t x = 0;
	uint8_t y = 9;
	myOLED.setCursor(x, y);

	for (uint8_t i = 128; i < 254; i++)
	{
		if (x > 120)
		{
			x = 0;
			y += 8;
		}
		myOLED.drawChar(x, y , i, FOREGROUND, BACKGROUND, 1);
		x += 7;
		bcm2835_delay(MYTESTDELAY2);
	}

	screenReset();


	// Test 707 Fonts 1-6
	myOLED.setCursor(0, 0);
	myOLED.print("Default font");
	myOLED.setFontNum(OLEDFontType_Thick);
	myOLED.setCursor(0, 9);
	myOLED.print("THICK FONT");
	myOLED.setFontNum(OLEDFontType_SevenSeg);
	myOLED.setCursor(0, 18);
	myOLED.print("Seven seg font");
	myOLED.setFontNum(OLEDFontType_Wide);
	myOLED.setCursor(0, 30);
	myOLED.print("WIDE FONT");
	myOLED.setFontNum(OLEDFontType_Tiny);
	myOLED.setCursor(0, 41);
	myOLED.print("tiny FONT");
	myOLED.setFontNum(OLEDFontType_Homespun);
	myOLED.setCursor(0, 52);
	myOLED.print("Homespun FONT");

	screenReset();

}

void TestFonts7_10()
{

	char myTestStr[] = "11.23.2012:56-7";
	char myTestStr2[] = "ABC123!";

	// Test 708-1 Font 7
	myOLED.setFontNum(OLEDFontType_Bignum);
	myOLED.drawCharBigFont(0, 0, '7', FOREGROUND, BACKGROUND); // single character
	myOLED.drawTextBigFont(16, 0, myTestStr, FOREGROUND, BACKGROUND);
	screenReset();

	//Test 708-2 Font 8
	myOLED.setFontNum(OLEDFontType_Mednum);
	myOLED.drawCharBigFont(0, 40, '8', FOREGROUND, BACKGROUND); // single character
	myOLED.drawTextBigFont(0, 0, myTestStr, BACKGROUND, FOREGROUND);
	screenReset();

	//Test 708-3 Font 9
	myOLED.setFontNum(OLEDFontType_ArialRound);
	myOLED.drawCharBigFont(0, 0, '9', FOREGROUND, BACKGROUND); // single character
	myOLED.drawTextBigFont(0, 32,  myTestStr2, BACKGROUND, FOREGROUND);
	screenReset();

	//Test 708-4 Font 10
	myOLED.setFontNum(OLEDFontType_ArialBold);
	myOLED.drawCharBigFont(0, 0, '0', FOREGROUND, BACKGROUND); // single character
	myOLED.drawTextBigFont(0, 16, myTestStr2, BACKGROUND, FOREGROUND);
	screenReset();

	// Test 708-5 test print font 7 font 8
	myOLED.setTextColor(FOREGROUND, BACKGROUND);
	myOLED.setFontNum(OLEDFontType_Bignum);
	myOLED.setCursor(0, 0);
	myOLED.print(87.39);


	myOLED.setFontNum(OLEDFontType_Mednum);
	myOLED.setCursor(0, 40);
	myOLED.print(-3.947, 2); // prints -3.95
	screenReset();

	// Test 708-6 test print font 9 font 10
	myOLED.setFontNum(OLEDFontType_ArialRound);
	myOLED.setCursor(0, 0);
	myOLED.print(99.31);

	myOLED.setFontNum(OLEDFontType_ArialBold);
	myOLED.setCursor(0, 33);
	myOLED.print(-3.947, 2); // prints -3.95

	screenReset();
}

void TestPrint()
{
	// Test 709-1 base numbers with print function
	myOLED.setTextColor(FOREGROUND, BACKGROUND);
	myOLED.setFontNum(OLEDFontType_Default);
	myOLED.setCursor(0, 0);
	myOLED.print(47 , DEC);
	myOLED.setCursor(0, 16);
	myOLED.print(47 , HEX);
	myOLED.setCursor(0, 32);
	myOLED.print(47, BIN);
	myOLED.setCursor(0, 48);
	myOLED.print(47 , OCT);
	screenReset();

	// Test 709-2 string object and println
	std::string timeInfo = "12:45";
	myOLED.setCursor(5, 0);
	myOLED.println(timeInfo);
	myOLED.print(timeInfo);
	screenReset();

	// Test 709-3 OLED Test print overlap  + invert
	myOLED.setFontNum(OLEDFontType_Homespun);
	myOLED.setCursor(0, 0);
	myOLED.print("1234567890abcdefghij");
	myOLED.setCursor(0, 32);
	myOLED.setTextColor(BACKGROUND, FOREGROUND); // invert
	myOLED.print("inverted");
	screenReset();

	// Test 709-4 OLED Testdrawtext   overlap  + invert
	char myString[9] = {'1','9',':','2','2',':','3','8'};
	char myTest[] = "1234567812::5678";
	myOLED.setTextColor(FOREGROUND, BACKGROUND); // normal
	myOLED.setFontNum(OLEDFontType_Tiny);
	myOLED.drawText(0,0, myString, FOREGROUND, BACKGROUND,1);
	myOLED.setFontNum(OLEDFontType_Wide);
	myOLED.drawText(0,16, myTest, FOREGROUND, BACKGROUND,2);
	screenReset();

}


