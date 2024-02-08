/*!
	@file examples/color16_displays/st7735/Graphic_Tests/main.cpp
	@brief Library test file, graphics test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 901  pixels and lines
	-# Test 902  rectangles
	-# Test 903  Circle
	-# Test 904  Triangles
	-# Test 905  More shapes, media buttons graphic.
*/

// Section ::  libraries
#include <bcm2835.h> // for SPI GPIO and delays.
#include "ST7735_TFT_LCD_RDL.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY 5000

// Section :: Globals
ST7735_TFT myTFT;

//  Section ::  Function Headers

uint8_t Setup(void); // setup + user options

void Test901(void);  // pixels and lines
void Test902(void);  // rectangles
void Test903(void);  // Circle
void Test904(void);  // Triangles
void Test905(void);  // More shapes, media buttons graphic.

void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{
	if(Setup() != 0)return -1;
	myTFT.setFont(font_retro);
	Test901();
	Test902();
	Test903();
	Test904();
	Test905();

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
	}else{
		std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	}

// ** USER OPTION 1 GPIO/SPI TYPE HW SPI **
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;// Screen width in pixels
	uint16_t TFT_HEIGHT = 128; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 PCB_TYPE + SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ =  8000000 ; //  freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SCLK_FREQ, SPI_CE_PIN) != rpiDisplay_Success)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************
	std::cout << "ST7735 library version : " << GetRDLibVersionNum()<< std::endl;
	TFT_MILLISEC_DELAY(50);
	return 0;
}

void Test901(void)
{
	std::cout << "Test 901: pixels and lines" << std::endl;
	myTFT.drawPixel(85, 5, RDLC_RED);
	myTFT.drawPixel(87, 7, RDLC_RED);
	myTFT.drawPixel(89, 9, RDLC_GREEN);
	myTFT.drawPixel(91, 11, RDLC_GREEN);
	myTFT.drawPixel(93, 13, RDLC_BLUE);
	myTFT.drawPixel(95, 15, RDLC_BLUE);
	myTFT.drawLine(10, 10, 30, 30, RDLC_RED);
	myTFT.drawFastVLine(40, 40, 40, RDLC_GREEN);
	myTFT.drawFastHLine(60, 60, 40, RDLC_YELLOW);

	TFT_MILLISEC_DELAY(TEST_DELAY);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test902(void) {
	std::cout << "Test 902: rectangles " << std::endl;
	rpiDisplay_Return_Codes_e returnValue;
	myTFT.drawRectWH(5, 5, 20, 20, RDLC_RED);
	returnValue = myTFT.fillRectangle(45, 5, 20, 20, RDLC_YELLOW);
	if (returnValue != rpiDisplay_Success)
	{
		std::cout << "Warning : Test TFTfillRectangle, An error occurred returnValue =" << +returnValue << std::endl;
	}
	myTFT.fillRect(85, 5, 20, 20, RDLC_GREEN);
	myTFT.drawRoundRect(15, 60, 50, 50, 5, RDLC_CYAN);
	myTFT.fillRoundRect(70, 60, 50, 50, 10, RDLC_WHITE);

	TFT_MILLISEC_DELAY(TEST_DELAY);
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

	TFT_MILLISEC_DELAY(TEST_DELAY);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test905()
{
	std::cout << "Test 905: More shapes, media buttons graphic." << std::endl;
	char teststr1[] = "Press Play";
	//Draw play button
	myTFT.fillRoundRect(25, 10, 78, 60, 8, RDLC_WHITE);
	myTFT.fillTriangle(42, 20, 42, 60, 90, 40, RDLC_RED);
	myTFT.writeCharString(25, 80, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY);

	// change play color
	myTFT.fillTriangle(42, 20, 42, 60, 90, 40,RDLC_GREEN);
	TFT_MILLISEC_DELAY(TEST_DELAY);
	// change play color
	myTFT.fillTriangle(42, 20, 42, 60, 90, 40, RDLC_BLUE);
	TFT_MILLISEC_DELAY(TEST_DELAY);
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
