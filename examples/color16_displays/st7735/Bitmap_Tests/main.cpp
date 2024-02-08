/*!
	@file examples/color16_displays/st7735/Bitmap_Tests/main.cpp
	@brief Library test file, bitmaps
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 301 "clock demo" , icons, small bitmap, font 7
	-# Test 302 bi-color full screen image 128x128
	-# Test 303 24 bit color image bitmaps from the file system
	-# Test 304 16 bit colorimage bitmaps from the file system

*/

// Section ::  libraries
#include <bcm2835.h> // for SPI GPIO and delays.
#include <ctime> // for test301
#include "Bi_Color_Bitmap_Data.hpp" // Data for test 401-402
#include "ST7735_TFT_LCD_RDL.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 350

// Display size in pixels
#define myTFTWidth 128
#define myTFTHeight 128

// Section :: Globals
ST7735_TFT myTFT;

//  Section ::  Function Headers

uint8_t Setup(void); // setup + user options
void Test301(void); // "clock demo" icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303(void); // 24 color bitmap
void Test304(void); // 16 color bitmap
std::string UTC_string(void); // clock dmeo 
void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{

	if(Setup() != 0)return -1;
	myTFT.fillScreen(RDLC_BLACK);
	Test301();
	Test302();
	Test303();
	Test304();

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
	uint16_t TFT_WIDTH = myTFTWidth;// Screen width in pixels
	uint16_t TFT_HEIGHT = myTFTHeight; // Screen height in pixels
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


void Test301(void)
{
	std::cout << "Test 301: clock demo , icons, small bitmap" << std::endl;
	uint16_t count=CLOCK_DISPLAY_TIME;
	if (count > 999) count = 999;
	char teststr1[] = "G Lyons";
	myTFT.fillScreen(RDLC_BLACK);
	
	// Draw top bar of clock
	myTFT.drawIcon(2, 2, 16, RDLC_BLACK, RDLC_WHITE, pSignalIcon);
	myTFT.drawIcon(20, 2, 16, RDLC_BLACK, RDLC_WHITE, pMsgIcon);
	myTFT.drawIcon(40, 2, 8, RDLC_BLACK, RDLC_WHITE, pAlarmIcon);
	myTFT.drawIcon(102, 2, 16, RDLC_BLACK, RDLC_WHITE, pBatIcon);

	myTFT.drawIcon(5, 20, 12, RDLC_GREEN , RDLC_BLACK, pPowerIcon);
	myTFT.drawIcon(20, 20, 12, RDLC_RED, RDLC_YELLOW, pSpeedIcon);
	myTFT.setCursor(40,20);
	myTFT.print(GetRDLibVersionNum());
	if (myTFT.drawBitmap(80, 20, 40 , 16, RDLC_CYAN , RDLC_BLACK, pSunText) != rpiDisplay_Success)
	{
		std::cout << "Warning an Error occurred in drawBitmap." << std::endl;
		return ;
	}
	// Lines
	myTFT.drawFastHLine(0,12,128,RDLC_WHITE);
	myTFT.drawFastHLine(0,36,128,RDLC_WHITE);
	myTFT.drawFastHLine(0,72,128,RDLC_WHITE);
	myTFT.drawFastHLine(0,95,128,RDLC_WHITE);
	myTFT.drawFastHLine(0,120,128,RDLC_WHITE);
	// draw clock
	while(count > 99)
	{
		std::string TimeString = UTC_string();
		std::cout<< TimeString << "\r" << std::flush;
		auto DateInfo = TimeString.substr(0, 10);
		auto TimeInfo = TimeString.substr(11,8);
		
		myTFT.setFont(font_mega);
		//print time
		myTFT.setTextColor(RDLC_RED, RDLC_BLACK);
		myTFT.setCursor(0,50);
		myTFT.print(TimeInfo);
		myTFT.setFont(font_retro);
		//print date
		myTFT.setTextColor(RDLC_GREEN, RDLC_BLACK);
		myTFT.setCursor(20,75);
		myTFT.print(DateInfo);
		
		myTFT.setFont(font_default);
		// print count
		myTFT.setTextColor(RDLC_YELLOW, RDLC_BLACK);
		myTFT.setCursor(90,110);
		myTFT.print(count);
		count--;
		//print name
		myTFT.setCursor(10,110);
		myTFT.setTextColor(RDLC_CYAN, RDLC_BLACK);
		myTFT.print(teststr1);
	}
	TFT_MILLISEC_DELAY(TEST_DELAY2);
	myTFT.fillScreen(RDLC_BLACK);
	
}

void Test302(void)
{
	std::cout << "Test 302: Bi-color bitmap" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 2 ";
	myTFT.writeCharString(5, 5, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY2);
	
	if(myTFT.drawBitmap(0, 0, myTFTWidth , myTFTHeight, RDLC_WHITE , RDLC_GREEN, pBackupMenuBitmap) != rpiDisplay_Success)
		return;
	TFT_MILLISEC_DELAY(TEST_DELAY5);
}

// bitmap 24 colour , All files format = Windows BITMAPINFOHEADER offset 54
// All bitmaps are 128 by 128 , same size as test screen
void Test303(void)
{
	std::cout << "Test 303: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeCharString(5, 5, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY1);

	FILE *pFile ;
	size_t pixelSize = 3; // 24 bit 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t NumberOfFiles  = 5;
	uint8_t* bmpBuffer = nullptr;
	bmpBuffer = (uint8_t*)malloc((myTFTWidth * myTFTHeight) * pixelSize);
	if (bmpBuffer == nullptr)
	{
		std::cout << "Error Test 303 : MALLOC could not assign memory " << std::endl;
		return;
	}
	for (uint8_t i = 0 ; i < NumberOfFiles ; i++)
	{
		switch (i) // select file
		{
			case 0: pFile = fopen("bitmap/bitmap24images/24pic1.bmp", "r"); break;
			case 1: pFile = fopen("bitmap/bitmap24images/24pic2.bmp", "r"); break;
			case 2: pFile = fopen("bitmap/bitmap24images/24pic3.bmp", "r"); break;
			case 3: pFile = fopen("bitmap/bitmap24images/24pic4.bmp", "r"); break;
			case 4: pFile = fopen("bitmap/bitmap24images/24pic5.bmp", "r"); break;
		}

		if (pFile == nullptr)  // Check file exists
		{
			std::cout << "Error Test 303: File does not exist" << std::endl;
			free(bmpBuffer);
			return;
		}

		fseek(pFile, FileHeaderOffset, 0); // Put file in Buffer
		fread(bmpBuffer, pixelSize, myTFTWidth * myTFTHeight, pFile);
		fclose(pFile);

		if(myTFT.drawBitmap24(0, 0, bmpBuffer, myTFTWidth, myTFTHeight) != rpiDisplay_Success)
		{// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap24" << std::endl;
			free(bmpBuffer);
			return;
		}
		TFT_MILLISEC_DELAY(TEST_DELAY5);
	}
	free(bmpBuffer);  // Free Up Buffer
}

// test function for 16 bit color bitmaps made in GIMP (RGB 565 16 bit color)
// 3 files
// File 1 16pic1.bmp BITMAPV5HEADER offset 132
// Color space information written
// File 2&3 16pic2.bmp & 16pic3.bmp
// OS/2 OS22XBITMAPHEADER (BITMAPINFOHEADER2) offset 72
// NO color space information written
// All bitmaps are 128 by 128 , same size as test screen
void Test304(void)
{
	std::cout << "Test 304: 16 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	char teststr1[] = "Bitmap 16";
	myTFT.writeCharString(5, 5, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY2);

	FILE *pFile ;
	size_t pixelSize = 2; // 16 bit 2 bytes per pixel
	uint8_t NumberOfFiles  = 3;
	uint8_t offsetBMPHeader = 0;
	uint8_t offsetNoColSpace = 72;
	uint8_t offsetColSpace = 132;
	uint8_t* bmpBuffer1 = nullptr;
	bmpBuffer1 = (uint8_t*)malloc((myTFTWidth * myTFTHeight) * pixelSize);

	if (bmpBuffer1 == nullptr)
	{
		std::cout << "Error Test 304 : MALLOC could not assign memory " << std::endl;
		return;
	}

	for (uint8_t i = 0 ; i < NumberOfFiles ; i++)
	{
		switch (i) // select file
		{
			case 0:
				pFile = fopen("bitmap/bitmap16images/16pic1.bmp", "r");
				offsetBMPHeader = offsetColSpace ;
			break;
			case 1: pFile = fopen("bitmap/bitmap16images/16pic2.bmp", "r"); 
				offsetBMPHeader = offsetNoColSpace;
			break;
			case 2: pFile = fopen("bitmap/bitmap16images/16pic3.bmp", "r"); 
				offsetBMPHeader = offsetNoColSpace;
			break;
		}
		if (pFile == nullptr)
		{
			std::cout << "Error Test 404 : File does not exist" << std::endl;
			free(bmpBuffer1);
			return;
		}
		fseek(pFile, offsetBMPHeader, 0);
		fread(bmpBuffer1, pixelSize, myTFTWidth * myTFTHeight, pFile);
		fclose(pFile);

		if (myTFT.drawBitmap16(0, 0, bmpBuffer1, myTFTWidth, myTFTHeight) != rpiDisplay_Success)
		{
		// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			free(bmpBuffer1);
			return;
		}
		TFT_MILLISEC_DELAY(TEST_DELAY5);
	} // end of for loop

	free(bmpBuffer1); // Free Up Buffer
	myTFT.fillScreen(RDLC_BLACK);
} // end of test 


void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_retro);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}


//Return UTC time as a std:.string with format "yyyy-mm-dd hh:mm:ss".
std::string UTC_string() 
{
	std::time_t time = std::time({});
	char timeString[std::size("yyyy-mm-dd hh:mm:ss UTC")];
	std::strftime(std::data(timeString), std::size(timeString), "%F %T UTC", std::gmtime(&time));
	return timeString;
}
// *************** EOF ****************
