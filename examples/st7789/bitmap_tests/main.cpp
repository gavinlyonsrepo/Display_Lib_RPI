/*!
	@file examples/st7789/bitmap_tests/main.cpp
	@brief Library test file, bitmaps
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 300 Sprites demo, small bitmap
	-# Test 301 "clock demo" , icons, small bitmap
	-# Test 302 bi-color image 128x128
	-# Test 303 24 bit color image bitmaps from the file system
	-# Test 304 16 bit color image bitmaps from the file system
	-# Test 305 16 bit color image bitmap from data

*/

// Section ::  libraries
#include <ctime> // for test301
#include <iostream> // cout
#include "Bitmap_test_data.hpp" // Data for test 301-302
#include "ST7789_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 350

// Section :: Globals
ST7789_TFT myTFT;

int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 280; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers
uint8_t Setup(void); // setup + user options
void Test300(void); // Sprite 
void Test301(void); // "clock demo" icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303(void); // 24 color bitmap
void Test304(void); // 16 color bitmap
void Test305(void); // 16 color bitmap (from data)
std::string UTC_string(void); // for clock demo
void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{

	if(Setup() != 0)return -1;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	Test300();
	Test301();
	Test302();
	Test303();
	Test304();
	Test305();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	std::cout << "TFT Start" << std::endl;
	std::cout << "ST7789 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI**
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}


void Test300(void)
{
	std::cout << "Test 300: Sprites demo" << std::endl;
	// Test 300-A test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color = myTFT.RDLC_LBLUE
	// Green background screen
	myTFT.fillScreen(myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.drawSprite(80, 100, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color =myTFT.RDLC_LBLUE
	// Bitmap background screen
	if(myTFT.drawBitmap(40, 40, 128 , 128, myTFT.RDLC_WHITE , myTFT.RDLC_RED, BackupMenuBitmap) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);

	myTFT.drawSprite(80, 30, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	myTFT.drawSprite(60, 60, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test301(void)
{
	std::cout << "Test 301: clock demo , icons, small bitmap" << std::endl;
	uint16_t count=CLOCK_DISPLAY_TIME;
	if (count > 999) count = 999;
	char teststr1[] = "G Lyons";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	
	// Draw top bar of clock
	myTFT.drawIcon(15, 20, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, SignalIconVa);
	myTFT.drawIcon(40, 20, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, MsgIconVa);
	myTFT.drawIcon(60, 20, 8, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, AlarmIconVa);
	myTFT.drawIcon(210, 20, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, BatIconVa);

	myTFT.drawIcon(5, 45, 12, myTFT.RDLC_GREEN , myTFT.RDLC_BLACK, PowerIcon);
	myTFT.drawIcon(20, 45, 12, myTFT.RDLC_RED, myTFT.RDLC_YELLOW, SpeedIcon);
	myTFT.setFont(font_arialBold);
	myTFT.setCursor(60,45);
	myTFT.print(rdlib::LibraryVersion());

	//print name
	myTFT.setCursor(10,210);
	myTFT.setTextColor(myTFT.RDLC_CYAN, myTFT.RDLC_BLACK);
	myTFT.print(teststr1);
	if (myTFT.drawBitmap(180, 45, 40 , 16, myTFT.RDLC_CYAN , myTFT.RDLC_BLACK, SunText) != rdlib::Success)
	{
		std::cout << "Warning an Error occurred in drawBitmap." << std::endl;
		return ;
	}
	// Lines
	myTFT.drawFastHLine(0,15,240,myTFT.RDLC_WHITE);
	myTFT.drawFastHLine(0,40,240,myTFT.RDLC_WHITE);
	myTFT.drawFastHLine(0,90,240,myTFT.RDLC_WHITE);
	myTFT.drawFastHLine(0,135,240,myTFT.RDLC_WHITE);
	myTFT.drawFastHLine(0,180,240,myTFT.RDLC_WHITE);
	myTFT.drawFastHLine(0,230,240,myTFT.RDLC_WHITE);
	// draw clock
	while(count > 99)
	{
		std::string TimeString = UTC_string();
		std::cout<< TimeString << "\r" << std::flush;
		auto DateInfo = TimeString.substr(0, 10);
		auto TimeInfo = TimeString.substr(11,8);
		
		myTFT.setFont(font_groTesk);
		//print time
		myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_BLACK);
		myTFT.setCursor(20,100);
		myTFT.print(TimeInfo);
		myTFT.setFont(font_groTesk);
		//print date
		myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
		myTFT.setCursor(20,140);
		myTFT.print(DateInfo);
		
		myTFT.setFont(font_retro);
		// print count
		myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
		myTFT.setCursor(160,210);
		myTFT.print(count);
		count--;
	}
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	
}

// bitmap 2 colour , 128x128 bi color bitmap
void Test302(void)
{
	std::cout << "Test 302: Bi-color bitmap 128X128 pixels" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 2";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	if(myTFT.drawBitmap(40, 40, 128 , 128, myTFT.RDLC_WHITE , myTFT.RDLC_GREEN, BackupMenuBitmap) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);
}

// bitmap 24 colour , All file format = Windows BITMAPINFOHEADER offset 54
// bitmap are 220 by 240 
void Test303(void)
{
	std::cout << "Test 303: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

	size_t pixelSize = 3; // 24-bit color = 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t NumberOfFiles = 1;

	// Use std::vector for safe memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try 
	{
		// Attempt to allocate memory for the vector
		bmpBuffer.resize(220 * 240 * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error Test 303: Memory allocation failed for bmpBuffer" << std::endl;
		return;
	}

	for (uint8_t i = 0; i < NumberOfFiles; i++)
	{
		FILE *pFile = nullptr;

		// Select file
		switch (i)
		{
			case 0:
				pFile = fopen("bitmap/bitmap24images/24pic7_220X240.bmp", "r");
				break;
		}

		if (pFile == nullptr) // Check if file exists
		{
			std::cout << "Error Test 303: File does not exist" << std::endl;
			return;
		}

		// Read bitmap data into buffer
		fseek(pFile, FileHeaderOffset, SEEK_SET);
		fread(bmpBuffer.data(), pixelSize, 220 * 240, pFile);
		fclose(pFile);

		// Draw bitmap
		if (myTFT.drawBitmap24(10, 20, bmpBuffer, 220, 240) != rdlib::Success)
		{
			std::cout << "Warning: An error occurred in drawBitmap24" << std::endl;
			return;
		}

		delayMilliSecRDL(TEST_DELAY5);
	}
}

// test function for 16 bit color bitmaps made in GIMP (RGB 565 16 bit color)
// 3 files
// File 1 16pic1.bmp BITMAPV5HEADER offset 132
// Color space information written
// File 2&3 16pic2.bmp & 16pic3.bmp
// OS/2 OS22XBITMAPHEADER (BITMAPINFOHEADER2) offset 72
// NO color space information written
// All bitmaps are 128 by 128.
void Test304(void)
{
	std::cout << "Test 304: 16 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 16";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	FILE *pFile ;
	size_t pixelSize = 2; // 16 bit 2 bytes per pixel
	uint8_t NumberOfFiles  = 3;
	uint8_t offsetBMPHeader = 0;
	uint8_t offsetNoColSpace = 72;
	uint8_t offsetColSpace = 132;
	uint8_t bitmapWidth = 128;
	uint8_t bitmapHeight = 128;

	// Use std::vector for safe memory management of buffer
	std::vector<uint8_t> bmpBuffer1;
	try 
	{
		// Attempt to allocate memory for the vector
		bmpBuffer1.resize((bitmapWidth * bitmapHeight) * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error Test 303: Memory allocation failed for bmpBuffer1" << std::endl;
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
			return;
		}
		fseek(pFile, offsetBMPHeader, 0);
		fread(bmpBuffer1.data(), pixelSize, bitmapWidth * bitmapHeight, pFile);
		fclose(pFile);

		if (myTFT.drawBitmap16(40, 40, bmpBuffer1, bitmapWidth, bitmapHeight) != rdlib::Success)
		{
		// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	myTFT.fillScreen(myTFT.RDLC_BLACK);
} // end of test 


//Return UTC time as a std:.string with format "yyyy-mm-dd hh:mm:ss".
std::string UTC_string() 
{
	std::time_t time = std::time({});
	char timeString[std::size("yyyy-mm-dd hh:mm:ss UTC")];
	std::strftime(std::data(timeString), std::size(timeString), "%F %T UTC", std::gmtime(&time));
	return timeString;
}

// bitmap 16 colour , Data from array as opposed to file system, When creating data flip image upside down
void Test305(void)
{
	std::cout << "Test 305: 16-color bitmap 32x32 pixels from raw data as opposed to file-system(default), " << std::endl;
	std::cout << "Reusing the sprite data from test300, note it will be displayed upside down" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 16 data";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	if(myTFT.drawBitmap16(60, 60, SpriteTest16, 32 , 32) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}


void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_mega);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.writeCharString(15, 50, teststr1);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}


// *************** EOF ****************

/// @endcond
