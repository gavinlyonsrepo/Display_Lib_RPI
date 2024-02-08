/*! 
	@file examples/color16_displays/Function_Tests/main.cpp
	@brief Library test file, function tests
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 500 RGB color OK?
	-# Test 501 scroll
	-# Test 502 Rotate
	-# Test 503 change modes test -> Invert, display on/off and Sleep.
*/

// Section ::  libraries 
#include <bcm2835.h> // for SPI GPIO and delays.
#include "ST7735_TFT_LCD_RDL.hpp"

// Section :: Defines   
//  Test timing related defines 
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// Section :: Globals 
ST7735_TFT myTFT;

//  Section ::  Function Headers 

uint8_t Setup(void); // setup + user options
void Test500(void);  // Color RGB
void Test501(void);  // scroll
void Test502(void);  // Rotate
void Test503(void); // change modes test -> Invert, display on/off and Sleep.
void EndTests(void);

//  Section ::  MAIN loop

int main(void) 
{
	if(Setup() != 0)return -1;
	myTFT.setFont(font_default);
	myTFT.setTextColor(RDLC_GREEN,RDLC_BLACK);
	Test500();
	Test501();
	Test502();
	Test503();
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
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SCLK_FREQ, SPI_CE_PIN) != 0)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************
	std::cout << "ST7735 library version : " << GetRDLibVersionNum()<< std::endl;
	TFT_MILLISEC_DELAY(50);
	myTFT.fillScreen(RDLC_BLACK);
	return 0;
}


void Test500(void)
{
	std::cout << "Test 500: Color Test:: Red,green,blue,yellow,white, black background" << std::endl;
	myTFT.fillRoundRect(8, 10, 24, 60, 8, RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, RDLC_WHITE);
	std::cout << "If colors are wrong user may have selected wrong PCB type in setup option 3" << std::endl;
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test501(void) 
{
	std::cout << "Test 501: Scroll" << std::endl;
	const uint8_t LINES = 10, LINE_SIZE = 10, LINE_OFFSET = 3, TOP_FIXED = 0, BOTTOM_FIXED = 0;
	char teststr1[] = "Scroll test";
	
	for (uint8_t i = 0; i < LINES; i++)
	{
	myTFT.writeCharString(5, LINE_OFFSET+i*LINE_SIZE,teststr1);
	}
	myTFT.TFTsetScrollDefinition(TOP_FIXED,BOTTOM_FIXED,1);  // bottom-to-top
	uint8_t pos = LINE_OFFSET;
	for (uint8_t i = 0; i < LINES; i++) 
	{
		for (uint8_t j = 0; j < LINE_SIZE; j++) 
		{
			myTFT.TFTVerticalScroll(pos + TOP_FIXED);
			pos++;
			// check pos if necessary: must be < tftTFT_HEIGHT - TOP_FIXED - BOTTOM_FIXED 
		}
	TFT_MILLISEC_DELAY(TEST_DELAY1);
	}
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode); 
	myTFT.fillScreen(RDLC_BLACK);
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
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_0);
	myTFT.writeCharString(5, 5, teststr0);
	myTFT.writeCharString(5, 108, teststr0);
	TFT_MILLISEC_DELAY(TEST_DELAY2);
	
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_90);
	myTFT.writeCharString(5, 5, teststr1);
	myTFT.writeCharString(5, 108, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY2);
	
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_180);
	myTFT.writeCharString(5, 5, teststr2);
	myTFT.writeCharString(5, 108, teststr2);
	TFT_MILLISEC_DELAY(TEST_DELAY2);
	
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_270);
	myTFT.writeCharString(5, 5, teststr3);
	myTFT.writeCharString(5, 108, teststr3);
	TFT_MILLISEC_DELAY(TEST_DELAY2);
	
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_0);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test503()
{
	std::cout << "Test 503-1: Mode Tests" << std::endl;
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(8, 10, 24, 60, 8, RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, RDLC_WHITE);
	myTFT.writeCharString(10, 80, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Invert_mode);
	std::cout << "Test 503-2: Mode Invert" << std::endl;
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	std::cout << "Test 503-3: Mode Normal" << std::endl;
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Display_off_mode);
	std::cout << "Test 503-4: Mode Display off" << std::endl;
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Display_on_mode);
	std::cout << "Test 503-5: Mode Display on" << std::endl;
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Sleep_mode);
	std::cout << "Test 503-6: Mode Sleep on" << std::endl;
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	std::cout << "Test 503-7 Mode Normal" << std::endl;
	TFT_MILLISEC_DELAY(TEST_DELAY2);
}


void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
