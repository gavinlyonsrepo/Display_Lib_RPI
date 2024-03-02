/*!
	@file examples/color16_displays/ili9341/Text_Graphics_Functions/main.cpp
	@brief Library test file, tests Text,graphics & functions.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 500 RGB color OK?
	-# Test 502 Rotate
	-# Test 503 change modes test -> Invert, display on/off and Sleep.
	-# Test 705 print method all fonts
	-# Test 706 Misc print class tests (string object, println invert, wrap, base nums etc)
	-# Test 902 rectangles
	-# Test 903 Circle
	-# Test 904 Triangles
*/

// Section ::  libraries
#include <bcm2835.h> // for SPI GPIO and delays.
#include "ILI9341_TFT_LCD_RDL.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

// Section :: Globals
ILI9341_TFT myTFT;

//  Section ::  Function Headers

uint8_t Setup(void);
void EndTests(void);
void DisplayReset(void);

void Test500(void); // Color RGB
void Test501(void); 
void Test502(void); // Rotate
void Test503(void); // change modes test -> Invert, display on/off.

void Test701(void);
void Test704(void);
void Test705(void);
void Test706(void);

void Test902(void);  // rectangles
void Test903(void);  // Circle
void Test904(void);  // Triangles

//  Section ::  MAIN loop

int main(void)
{
	if(Setup() != 0)return -1;

	Test500();
	Test501();
	Test502();
	Test503();
	
	Test705();
	Test706();
	Test902();
	Test903();
	Test904();

	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	std::cout << "TFT Start" << std::endl;
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}

// ** USER OPTION 1 GPIO/SPI TYPE HW SPI **
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ =  8000000 ; //  freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
	if(myTFT.InitSPI(SCLK_FREQ, SPI_CE_PIN) != rpiDisplay_Success)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************
	std::cout <<"ILI9341 library version : " <<GetRDLibVersionNum()<< std::endl;
	std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	delayMilliSecRDL(50);
	return 0;
}

void Test500(void)
{
	std::cout << "Test 500: Color Test:: Red,green,blue,yellow,white, black background" << std::endl;
	myTFT.setFont(font_mega);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_GREEN,RDLC_BLACK);
	myTFT.fillRoundRect(8, 10, 24, 60, 8, RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, RDLC_WHITE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test501(void)
{
	std::cout << "Test 501: Scroll" << std::endl;
	myTFT.setFont(font_mega);
	myTFT.setCursor(20,120);
	myTFT.print("Scroll Test");
	myTFT.setScrollMargins(0, 0);
	for(uint16_t i =0 ; i < 100 ; i ++)
	{
		delayMilliSecRDL(50);
		myTFT.scrollTo(i);
	}
	delayMilliSecRDL(TEST_DELAY);
	myTFT.NormalMode();
}

void Test502()
{
	std::cout << "Test 502: Rotate" << std::endl;
	myTFT.setFont(font_retro);
	char teststr0[] = "Rotate 0"; //normal
	char teststr1[] = "Rotate 90"; // 90
	char teststr2[] = "Rotate 180"; // 180
	char teststr3[] = "Rotate 270"; // 270

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_0);
	myTFT.writeCharString(15, 15, teststr0);
	myTFT.writeCharString(15, 300, teststr0);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_90);
	myTFT.writeCharString(15, 15, teststr1);
	myTFT.writeCharString(15, 220, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_180);
	myTFT.writeCharString(15, 15, teststr2);
	myTFT.writeCharString(15, 300, teststr2);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_270);
	myTFT.writeCharString(15, 15, teststr3);
	myTFT.writeCharString(15, 220, teststr3);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.setRotation(myTFT.TFT_Degrees_0);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test503()
{
	std::cout << "Test 503: Mode Tests" << std::endl;
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(8, 10, 24, 60, 8, RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, RDLC_WHITE);
	myTFT.writeCharString(10, 80, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	// Invert on and off
	myTFT.invertDisplay(true);
	std::cout << "Test 503-1: Invert on " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.invertDisplay(false);
	std::cout << "Test 503-2: Invert off " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	
	// Display on and off
	myTFT.EnableDisplay(false);
	std::cout << "Test 503-3: Display off" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.EnableDisplay(true);
	std::cout << "Test 503-4: Turn Display back on" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	
}

void Test705(void)
{
	std::cout << "Test 705: Print text class methods" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_GREEN, RDLC_BLACK);
	
	// Test Fonts default + wide + pico+ sinclair + retro
	// Test font  mega Arial bold and Hallf.
	myTFT.setCursor(25,5);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);
	myTFT.setCursor(25,15);
	myTFT.setFont(font_wide);
	myTFT.print("WIDE ");
	myTFT.print(123.284,1); // print 123.3
	myTFT.setCursor(25,25);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);
	myTFT.setCursor(25,35);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);
	myTFT.setCursor(25,45);
	myTFT.setFont(font_retro);
	myTFT.print("retro  ");
	myTFT.print(-3.14);
	myTFT.setCursor(25,65);
	myTFT.setFont(font_mega);
	myTFT.println("mega ");
	myTFT.print(61);
	myTFT.setCursor(25,105);
	myTFT.setFont(font_arialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);
	myTFT.setCursor(25,135);
	myTFT.setFont(font_hallfetica);
	myTFT.println("hall f");
	myTFT.print(1.48);

	DisplayReset();

	// Test font grotesk + sixteen segment
	myTFT.setCursor(15,15);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);
	myTFT.setCursor(15,100);
	myTFT.setFont(font_sixteenSeg);
	myTFT.print(124.78);

	DisplayReset();
	
	// Test font thick , seven segment + mint
	// Test Font orla + arial round
	myTFT.setFont(font_orla);
	myTFT.setCursor(25,165);
	myTFT.println(-7.16);
	myTFT.print("Orla");
	myTFT.setCursor(25,105);
	myTFT.setFont(font_arialRound);
	myTFT.println(-8.16);
	myTFT.print("a rnd");
	myTFT.setCursor(25,25);
	myTFT.setFont(font_sevenSeg);
	myTFT.println(1234);
	myTFT.setFont(font_thick);
	myTFT.println("THICK");
	myTFT.setFont(font_mint);
	myTFT.print("MINT");

	DisplayReset();

}

void Test706(void)
{
	std::cout << "Test 706: Misc print class(string object, println invert, wrap, base nums etc)" << std::endl;
	//Inverted print fonts 1-6
	myTFT.setTextColor(RDLC_RED, RDLC_YELLOW);
	myTFT.setFont(font_mega);
	myTFT.setCursor(25,15);
	myTFT.print(-49);
	myTFT.setCursor(25,55);
	myTFT.print(112.09);
	myTFT.setCursor(25,75);
	myTFT.print("ABCD");
	myTFT.setCursor(25,95);
	myTFT.print("ER");

	DisplayReset();

	// Inverted print fonts 7-12
	myTFT.setTextColor(RDLC_YELLOW, RDLC_RED);

	myTFT.setFont(font_arialBold);
	myTFT.setCursor(15,5);
	myTFT.print("INVERT");
	myTFT.setCursor(15,25);
	myTFT.print(-94.982, 2);

	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(15,65);
	myTFT.print("INVERT");
	myTFT.setCursor(15,85);
	myTFT.print(123456);
	DisplayReset();
	// Test print a string object with print
	myTFT.setFont(font_groTesk);
	std::string timeInfo = "12:45";
	std::string newLine = "new line";
	myTFT.setCursor(15, 5);
	myTFT.print(timeInfo);
	// print a new line with println
	myTFT.setCursor(15,40);
	myTFT.println(newLine); // print a new line feed with println
	myTFT.print(newLine);

	//text wrap with print
	myTFT.setCursor(15,110);
	myTFT.print("12345678901234567890ABCDEFGHIJ");
	DisplayReset();
}


void Test902(void) {
	std::cout << "Test 902: rectangles " << std::endl;
	rpiDisplay_Return_Codes_e returnValue;
	myTFT.drawRectWH(25, 25, 20, 20, RDLC_RED);
	returnValue = myTFT.fillRectangle(85, 25, 20, 20, RDLC_YELLOW);
	if (returnValue != rpiDisplay_Success)
	{
		std::cout << "Warning : Test TFTfillRectangle, An error occurred returnValue =" << +returnValue << std::endl;
	}
	myTFT.fillRect(115, 25, 20, 20, RDLC_GREEN);
	myTFT.drawRoundRect(15, 60, 50, 50, 5, RDLC_CYAN);
	myTFT.fillRoundRect(70, 60, 50, 50, 10, RDLC_WHITE);

	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test903(void) {
	std::cout << "Test 903 & 904 : Triangles and circles" << std::endl;
	myTFT.drawCircle(40, 20, 15, RDLC_GREEN);
	myTFT.fillCircle(80, 20, 15, RDLC_YELLOW);
}

void Test904(void) {
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80,RDLC_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, RDLC_RED);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void DisplayReset(void)
{
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.setFont(font_mega);
	myTFT.writeCharString(25, 50, teststr1);
	DisplayReset();

	myTFT.PowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
