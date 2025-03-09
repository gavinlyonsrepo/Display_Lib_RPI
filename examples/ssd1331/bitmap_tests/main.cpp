/*!
	@file examples/ssd1331/bitmap_tests/main.cpp
	@brief Library test file, bitmaps
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function and in myOLED constructor
	@test
	-# Test 300 Sprites demo, small bitmap
	-# Test 302 bi-color image 128x128
	-# Test 303 24 bit color image bitmaps from the file system
	-# Test 304 16 bit color image bitmaps from the file system
*/

// Section ::  libraries
#include <ctime> // for test301
#include <iostream> // cout
#include "Bitmap_test_data.hpp" // Data for tests
#include "SSD1331_OLED_RDL.hpp"

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);
int8_t RST_OLED  = 25; // GPIO reset pin
int8_t DC_OLED   = 24; // GPIO DC pin
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0
uint16_t OLED_WIDTH = 96;// Screen width in pixels
uint16_t OLED_HEIGHT = 64; // Screen height in pixels
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers
uint8_t Setup(void); // setup + user options
void Test300(void); // Sprite 
void Test302(void); // 2 color bitmap
void Test303(void); // 24 color bitmap
void Test304(void); // 16 color bitmap
std::string UTC_string(void); // for clock demo
void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{

	if(Setup() != 0)return -1;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	Test300();
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
	std::cout << "SSD1331 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myOLED.OLEDSetupGPIO(RST_OLED, DC_OLED);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myOLED.OLEDInitScreenSize( OLED_WIDTH ,  OLED_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI**
	if(myOLED.OLEDInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
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
	// Draw as sprite, without background , 32 X 32 .background color = myOLED.RDLC_LBLUE
	// Green background screen
	myOLED.fillScreen(myOLED.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY1);
	myOLED.drawSprite(10, 10, SpriteTest16, 32, 32, myOLED.RDLC_LBLUE);
	delayMilliSecRDL(TEST_DELAY2);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color =myOLED.RDLC_LBLUE
	// Bitmap background screen
	if(myOLED.drawBitmap(0, 0, 64 , 64, myOLED.RDLC_WHITE , myOLED.RDLC_RED, bigImage64x64 ) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY1);

	myOLED.drawSprite(10, 10, SpriteTest16, 32, 32, myOLED.RDLC_LBLUE);
	myOLED.drawSprite(30, 30, SpriteTest16, 32, 32, myOLED.RDLC_LBLUE);
	delayMilliSecRDL(TEST_DELAY2);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}


// bitmap 2 colour , 128x128 bi color bitmap
void Test302(void)
{
	std::cout << "Test 302: Bi-color bitmap 64X64 pixels" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.setTextColor(myOLED.RDLC_WHITE, myOLED.RDLC_BLACK);
	myOLED.setFont(font_default);
	char teststr1[] = "Bitmap 2";
	myOLED.writeCharString(0, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	if(myOLED.drawBitmap(0, 0, 64 , 64, myOLED.RDLC_WHITE , myOLED.RDLC_GREEN, bigImage64x64 ) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY2);
}

// bitmap 24 colour , All file format = Windows BITMAPINFOHEADER offset 54
// bitmaps are 96X64
void Test303(void)
{
	std::cout << "Test 303: 24 bit color image bitmaps from the file system" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myOLED.setTextColor(myOLED.RDLC_WHITE, myOLED.RDLC_BLACK);
	myOLED.writeCharString(0, 0, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

	size_t pixelSize = 3; // 24-bit color = 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t NumberOfFiles = 2;
	uint8_t bitmapWidth = 96;
	uint8_t bitmapHeight = 64;

	// Use std::vector for safe memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try 
	{
		// Attempt to allocate memory for the vector
		bmpBuffer.resize(bitmapWidth * bitmapHeight * pixelSize);
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
			case 0:pFile = fopen("bitmap/bitmap24images/24pic8_96X64.bmp", "r"); break;
			case 1:pFile = fopen("bitmap/bitmap24images/24pic9_96X64.bmp", "r"); break;
		}

		if (pFile == nullptr) // Check if file exists
		{
			std::cout << "Error Test 303: File does not exist" << std::endl;
			return;
		}

		// Read bitmap data into buffer
		fseek(pFile, FileHeaderOffset, 0);
		fread(bmpBuffer.data(), pixelSize, bitmapWidth * bitmapHeight, pFile);
		fclose(pFile);

		// Draw bitmap
		if (myOLED.drawBitmap24(0, 0, bmpBuffer, bitmapWidth, bitmapHeight) != rdlib::Success)
		{
			std::cout << "Warning: An error occurred in drawBitmap24" << std::endl;
			return;
		}

		delayMilliSecRDL(TEST_DELAY5);
	}
}

// bitmap 16 colour
// bitmaps are 96X64
// BITMAPV5HEADERoffset 138(124+14) Made in The GIMP
void Test304(void)
{
	std::cout << "Test 304: 16 bit color image bitmaps from the file system" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	char teststr1[] = "Bitmap 16";
	myOLED.writeCharString(0, 0, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	FILE *pFile ;
	size_t pixelSize = 2; // 16 bit 2 bytes per pixel
	uint8_t NumberOfFiles  = 2;
	uint8_t offsetBMPHeader = 138;
	uint8_t bitmapWidth = 96;
	uint8_t bitmapHeight = 64;

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
			case 0:pFile = fopen("bitmap/bitmap16images/16pic4_96x64.bmp", "r");break;
			case 1:pFile = fopen("bitmap/bitmap16images/16pic5_96x64.bmp", "r");break;
		}
		if (pFile == nullptr)
		{
			std::cout << "Error Test 404 : File does not exist" << std::endl;
			return;
		}
		fseek(pFile, offsetBMPHeader, 0);
		fread(bmpBuffer1.data(), pixelSize, bitmapWidth * bitmapHeight, pFile);
		fclose(pFile);

		if (myOLED.drawBitmap16(0, 0, bmpBuffer1, bitmapWidth, bitmapHeight) != rdlib::Success)
		{
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	myOLED.fillScreen(myOLED.RDLC_BLACK);
} // end of test 


//Return UTC time as a std:.string with format "yyyy-mm-dd hh:mm:ss".
std::string UTC_string() 
{
	std::time_t time = std::time({});
	char timeString[std::size("yyyy-mm-dd hh:mm:ss UTC")];
	std::strftime(std::data(timeString), std::size(timeString), "%F %T UTC", std::gmtime(&time));
	return timeString;
}


void EndTests(void)
{
	char teststr1[] = "Tests over";
	myOLED.setFont(font_default);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.writeCharString(5, 5, teststr1);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.OLEDPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}


// *************** EOF ****************

/// @endcond
