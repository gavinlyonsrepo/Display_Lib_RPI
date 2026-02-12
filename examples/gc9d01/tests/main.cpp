/*!
	@file examples/gc9d01/tests/main.cpp
	@brief  Example cpp file for gc9d01 display Library test file, tests :
			Bitmaps, text,graphics & functions.
	@details Uses advanced screen buffer mode. Hardware SPI
			Setup for 160X160 dual gate display
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# Test 300 Sprites demo, small bitmap
		-# Test 301 icons, small bitmap
		-# Test 302 bi-color image 128x128
		-# Test 303 24 bit color image bitmaps from the file system
		-# Test 304 16 bit color image bitmaps from the file system
		-# Test 305 16 bit color image bitmap from data
		-# Test 500 RGB color OK?
		-# Test 501 Scroll test
		-# Test 502 Rotate
		-# Test 503 change modes test -> Invert, display on/off and Sleep.
		-# Test 701 Print out some fonts with writeCharString
		-# Test 702 Print out some fonts with print
		-# Test 703 Misc print class tests(string object, println invert, wrap, base nums etc)
		-# Test 902 rectangles
		-# Test 903 Circles
		-# Test 904 Triangles
		-# Test 905 Polygons
		-# Test 906 Dot Gap
		-# Test 907 Quadrilateral
		-# Test 908 Ellipse
		-# Test 909 Draw Arc
*/

// Section ::  libraries
#include <iostream> // cout
#include <atomic>  // Ctrl+C exit
#include <csignal> // Ctrl+C exit
#include <thread>  // Ctrl+C exit

#include "Bitmap_test_data.hpp" // Data for test 30X

#include "GC9D01_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
std::atomic<bool> stopRequested{false};

GC9D01_TFT myTFT;

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY1 1000

//  Section ::  Function Headers
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
uint8_t SetupBufferMode(void); // setup the buffer
void EndTests(void);
void DisplayReset(void);
void displayMenu(void);
// // for Ctrl+C
void handleSignal(int){
	stopRequested = true;
}

// Bitmap test
void Test300(void);
void Test301(void);
void Test302(void);
void Test303(void);
void Test304(void);
void Test305(void);
// Function tests
void ColorBlock(void);
void Test500(void);
void Test501(void);
void Test502(void);
void Test503(void);
// Text tests
void Test701(void);
void Test702(void);
void Test703(void);
// Graphics tests
void Test902_904(void);
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);


// Section :: MAIN loop
int main()
{
	//rdlib_config::loadConfig(); ,optional
	if(SetupHWSPI() != 0) return -1; //Hardware SPI
	std::signal(SIGINT, handleSignal); // for Ctrl+C
	std::signal(SIGTERM, handleSignal);// for kill command
	int choice;
	do {
		if (stopRequested) break;
		myTFT.fillScreen(myTFT.RDLC_BLACK);
		displayMenu();
		std::cin >> choice;
		// Check if input is valid
		if (std::cin.fail()) {
			std::cin.clear(); // Clear error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Please enter a number from list.\n\n";
			continue;
		}
		switch (choice) {
			case 1:
				Test300();
				Test301();
				Test302();
				Test303();
				Test304();
				Test305();
			break;
			case 2:
				Test500();
				Test501();
				Test502();
				Test503();
			break;
			case 3:
				Test701();
				Test702();
				Test703();
			 break;
			case 4:
				Test902_904();
				Test905();
				Test906();
				Test907();
				Test908();
				Test909();
			break;
			case 5:
				std::cout << "Exiting menu\n";
			break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
			break;
		}
		std::cout << std::endl;
	} while (choice != 5);

	if (stopRequested)
		std::cout << "Stop Signal Received." << std::endl;
	EndTests();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space

void displayMenu() {
	std::cout << "Tests Menu:\n";
	std::cout << "1. Bitmap Tests\n";
	std::cout << "2. Function Tests\n";
	std::cout << "3. Text Tests\n";
	std::cout << "4. Graphic Tests\n";
	std::cout << "5. Quit\n";
	std::cout << "Enter your choice: ";
}


// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	// GPIO
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int  GPIO_CHIP_DEVICE = 0;
	// SPI
	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	// Screen
	uint16_t TFT_WIDTH = 160;// Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	uint16_t OFFSET_X   =   0; // Screen X offset in pixels
	uint16_t OFFSET_Y   =   0; // Screen Y offset in pixels
	GC9D01_TFT::Resolution_e DisplayType = GC9D01_TFT::Resolution_e::RGB160x160_DualGate;
	
	std::cout << "TFT Start Tests: Ctrl + C quits" << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.TFTInitScreenSize(TFT_WIDTH , TFT_HEIGHT, DisplayType, OFFSET_X, OFFSET_Y);
// ***********************************
// ** USER OPTION 3 SPI **
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	return SetupBufferMode();
}

// Set up advanced screen buffer mode
uint8_t SetupBufferMode(void)
{
	// 1. Turn it on!
	myTFT.setAdvancedScreenBuffer_e(myTFT.AdvancedScreenBuffer_e::On);
	// 2. Check its on, optional!
	if (myTFT.getAdvancedScreenBuffer_e() == myTFT.AdvancedScreenBuffer_e::Off)
	{
		std::cout << "Error: Setup: Wrong Mode. This example is for Advanced Screen Buffer Mode" << std::endl;
		return 4;
	}
	// 3. Set up buffer
	if (myTFT.setBuffer() != rdlib::Success) 
		return 5;
	// 4. Turn on Text pixel mode(instead of local buffer mode:default)
	myTFT.setTextCharPixelOrBuffer(true);
	// 5. clear buffer and screen
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.writeBuffer();
	return 0;
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "End Tests" << std::endl;
}

void DisplayReset(void)
{
	if (stopRequested) {
		return;
	}
	myTFT.writeBuffer();
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
}
// Bitmap
void Test300(void)
{
	if (stopRequested) return; 
	std::cout << "Test 300: Sprites demo" << std::endl;
	// Test 300-A test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color = myTFT.RDLC_LBLUE
	// Green background screen
	myTFT.fillScreen(myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.drawSprite(55, 55, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	myTFT.drawSprite(90, 90, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE, true);
	DisplayReset();
	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color =myTFT.RDLC_LBLUE
	// Bitmap background screen
	if(myTFT.drawBitmap(20, 20, 128 , 128, myTFT.RDLC_WHITE , myTFT.RDLC_RED, BackupMenuBitmap) != rdlib::Success)
		return;
	myTFT.drawSprite(65, 10, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	myTFT.drawSprite(70, 60, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	DisplayReset();
}

void Test301(void)
{
	if (stopRequested) return; 
	std::cout << "Test 301: icons, small bitmap" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.drawIcon(42, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, SignalIconVa);
	myTFT.drawIcon(62, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, MsgIconVa);
	myTFT.drawIcon(80, 40, 8, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, AlarmIconVa);
	myTFT.drawIcon(100, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, BatIconVa);
	myTFT.drawIcon(25, 60, 12, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK, PowerIcon);
	myTFT.drawIcon(45, 60, 12, myTFT.RDLC_RED, myTFT.RDLC_YELLOW, SpeedIcon);
	DisplayReset();

	if (myTFT.drawBitmap(80, 60, 40 , 16, myTFT.RDLC_RED , myTFT.RDLC_YELLOW, SunText) != rdlib::Success)
	{
		std::cout << "Warning an Error occurred in drawBitmap." << std::endl;
		return ;
	}
	DisplayReset();
}

// bitmap 2 colour , 128x128 bi color bitmap
void Test302(void)
{
	std::cout << "Test 302: Bi-color bitmap 128X128 pixels" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 2";
	myTFT.writeCharString(45, 10, teststr1);
	if(myTFT.drawBitmap(25, 25, 128 , 128, myTFT.RDLC_WHITE , myTFT.RDLC_GREEN, BackupMenuBitmap) != rdlib::Success)
		return;
	DisplayReset();
}

// bitmap 24 colour , All file format = Windows BITMAPINFOHEADER offset 54
// bitmap are 220 by 240 
void Test303(void)
{
	if (stopRequested) return; 
	std::cout << "Test 303: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(45, 10, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

	size_t pixelSize = 3; // 24-bit color = 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t NumberOfFiles = 1;

	// Use std::vector for safe memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try 
	{
		// Attempt to allocate memory for the vector
		bmpBuffer.resize(128 * 128 * pixelSize);
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
				pFile = fopen("bitmap/bitmap24images/24pic2.bmp", "r");
				break;
		}

		if (pFile == nullptr) // Check if file exists
		{
			std::cout << "Error Test 303: File does not exist" << std::endl;
			return;
		}

		// Read bitmap data into buffer
		fseek(pFile, FileHeaderOffset, SEEK_SET);
		fread(bmpBuffer.data(), pixelSize, 128 * 128, pFile);
		fclose(pFile);

		// Draw bitmap
		if (myTFT.drawBitmap24(25, 25, bmpBuffer, 128, 128) != rdlib::Success)
		{
			std::cout << "Warning: An error occurred in drawBitmap24" << std::endl;
			return;
		}

		DisplayReset();
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
	if (stopRequested) return; 
	std::cout << "Test 304: 16 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 16";
	myTFT.writeCharString(45, 10, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

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

		if (myTFT.drawBitmap16(25, 25, bmpBuffer1, bitmapWidth, bitmapHeight) != rdlib::Success)
		{
		// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		DisplayReset();
	} // end of for loop

	myTFT.fillScreen(myTFT.RDLC_BLACK);
} // end of test 


// bitmap 16 colour , Data from array as opposed to file system, When creating data flip image upside down
void Test305(void)
{
	if (stopRequested) return; 
	std::cout << "Test 305: 16-color bitmap 32x32 pixels from raw data as opposed to file-system(default), " << std::endl;
	std::cout << "Reusing the sprite data from test300, note it will be displayed upside down" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 16 data";
	myTFT.writeCharString(45, 10, teststr1);
	delayMilliSecRDL(TEST_DELAY1);
	
	if(myTFT.drawBitmap16(60, 60, SpriteTest16, 32 , 32) != rdlib::Success)
		return;
	DisplayReset();
}

// Functions
void Test500(void)
{
	std::cout << "Test 500: Color Test:: Red,green,blue,yellow,white, black background" << std::endl;
	myTFT.setFont(font_mega);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_GREEN,myTFT.RDLC_BLACK);
	ColorBlock();
	DisplayReset();
}

void ColorBlock(void){
	myTFT.fillRoundRect(8, 50, 24, 48, 8, myTFT.RDLC_RED);
	myTFT.fillRoundRect(32, 50, 24, 48, 8, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(56, 50, 24, 48, 8, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(80, 50, 24, 48, 8, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(104, 50, 24, 48, 8, myTFT.RDLC_WHITE);
}

void Test501(void)
{
	if (stopRequested) return; 
	std::cout << "Test 501: Scroll" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setFont(font_retro);
	char teststr0[] = "Scroll Test";
	myTFT.writeCharString(25, 80, teststr0);
	myTFT.writeBuffer(); // update screen from buffer

	const uint16_t screenH = 160;
	const uint16_t scrollArea = screenH - 0; // no fixed areas
	myTFT.TFTsetScrollArea(0, scrollArea, 0);

	for (uint16_t y = 0; y < screenH; y++) {
		myTFT.TFTsetScrollStart(y);
		delayMilliSecRDL(20);
	}
	myTFT.TFTsetScrollStart(0); // reset
	myTFT.TFTScrollModeLeave(); // leave scroll mode
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
}

void Test502()
{
	if (stopRequested) return; 
	std::cout << "Test 502: Rotate" << std::endl;
	myTFT.setFont(font_retro);
	char teststr0[] = "Rotate 0"; //normal
	char teststr1[] = "Rotate 90"; // 90
	char teststr2[] = "Rotate 180"; // 180
	char teststr3[] = "Rotate 270"; // 270

	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.writeCharString(15, 80, teststr0);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.writeCharString(15, 80, teststr1);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_180);
	myTFT.writeCharString(15, 80, teststr2);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_270);
	myTFT.writeCharString(15, 80, teststr3);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
}

void Test503()
{
	if (stopRequested) return; 
	std::cout << "Test 503: Mode Tests" << std::endl;
	char teststr1[] = "Invert";
	char teststr2[] = "Display on/off";
	char teststr3[] = "Sleep   on/off";
	ColorBlock();
	myTFT.writeCharString(25, 100, teststr1);
	myTFT.writeBuffer();
	delayMilliSecRDL(TEST_DELAY2);

	// Invert on and off
	myTFT.TFTchangeInvertMode(false);
	std::cout << "Test 503-1: Invert on " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTchangeInvertMode(true);
	std::cout << "Test 503-2: Invert off " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);

	// Display on and off
	myTFT.TFTenableDisplay(false);
	std::cout << "Test 503-3: Display off" << std::endl;
	myTFT.writeCharString(10, 100, teststr2);
	myTFT.writeBuffer();
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTenableDisplay(true);
	std::cout << "Test 503-4: Turn Display back on" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	// modes
	myTFT.writeCharString(10, 100, teststr3);
	myTFT.writeBuffer();
	delayMilliSecRDL(TEST_DELAY5); 
	// 1. sleep mode
	myTFT.TFTsetPowerMode(GC9D01_TFT::PowerState_e::SleepNormalIdleOff);
	std::cout << "Test 503-7: Power mode Sleep ON Idle Off" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	delayMilliSecRDL(TEST_DELAY5);
	//back to sleep off - normal on - idle off
	myTFT.TFTsetPowerMode(GC9D01_TFT::PowerState_e::NormalIdleOff);	
	delayMilliSecRDL(TEST_DELAY5);
}

// Text
void Test701(void) {
	if (stopRequested) return; 
	std::cout << "Test 701: Print out some fonts with writeCharString" << std::endl;
	char teststr1[] = "Default ";
	char teststr2[] = "GLL ";
	char teststr3[] = "Pico ";
	char teststr4[] = "Sinclair ";
	char teststr5[] = "Orla ";
	char teststr6[] = "Retro ";
	char teststr7[] = "Mega";


	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	// Start at ~20px Y offset and use ~20px spacing
	int startY = 20;
	int spacing = 20;
	int x = 30;
	int y = startY;

	myTFT.setFont(font_default);
	myTFT.writeCharString(x, y, teststr1);
	y += spacing;
	myTFT.setFont(font_gll);
	myTFT.writeCharString(x, y, teststr2);
	y += spacing;
	myTFT.setFont(font_pico);
	myTFT.writeCharString(x, y, teststr3);
	y += spacing;
	myTFT.setFont(font_sinclairS);
	myTFT.writeCharString(x, y, teststr4);
	y += spacing;
	myTFT.setFont(font_orla);
	myTFT.writeCharString(x, y, teststr5);
	y += spacing;
	myTFT.setFont(font_retro);
	myTFT.writeCharString(x, y, teststr6);
	y += spacing;
	myTFT.setFont(font_mega);
	myTFT.writeCharString(x, y, teststr7);
	y += spacing;

	DisplayReset();
}

void Test702(void)
{
	if (stopRequested) return; 
	std::cout << "Test 702: Print class methods" << std::endl;

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);

	int x = 25;
	int y = 20;
	int spacing = 25;

	// --- First Block ---
	myTFT.setCursor(x, y);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_gll);
	myTFT.print("GLL ");
	myTFT.print(123.284, 1); // print 123.3
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_retro);
	myTFT.print("retro ");
	myTFT.print(-3.14);

	DisplayReset();

	// --- Second Block ---
	y = 20;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_mega);
	myTFT.print("mega");
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_arialBold);
	myTFT.print("A bold");
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_hallfetica);
	myTFT.print(1.48);

	DisplayReset();

	// --- Third Block ---
	y = 20;
	myTFT.setFont(font_orla);
	myTFT.setCursor(x, y);
	myTFT.print("Orla");
	y += spacing;
	myTFT.setCursor(x, y + spacing);
	myTFT.setFont(font_arialRound);
	myTFT.print(-8.16);

	DisplayReset();

	// --- Final Block ---
	y = 20;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);
	DisplayReset();
}

void Test703(void)
{
	if (stopRequested) return; 
	std::cout << "Test 703: Misc print class(string object, println invert, wrap, base nums etc)" << std::endl;
	// --- Inverted print fonts 1–6 ---
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(30, 30);
	myTFT.print(-49);
	myTFT.setFont(font_mega);
	myTFT.setCursor(10, 60);
	myTFT.print(112.09);
	myTFT.setCursor(10, 90);
	myTFT.print("ABCD");

	DisplayReset();

	// --- Inverted print ---
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_RED);
	myTFT.setFont(font_arialBold);
	myTFT.setCursor(40, 40);
	myTFT.print("INVERT");
	myTFT.setCursor(40, 80);
	myTFT.print(-94.982, 2);
	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(40, 100);
	myTFT.print("INVERT");
	myTFT.setCursor(40, 140);
	myTFT.print(123456);

	DisplayReset();

	// --- Print in different number bases ---
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(font_mega);
	myTFT.setCursor(40, 50);
	myTFT.print(numPos, myTFT.RDL_DEC); // 47
	myTFT.setCursor(40, 70);
	myTFT.print(numPos, myTFT.RDL_BIN); // 101111
	myTFT.setCursor(40, 90);
	myTFT.print(numPos, myTFT.RDL_OCT); // 57
	myTFT.setCursor(40, 110);
	myTFT.print(numPos, myTFT.RDL_HEX); // 2F

	DisplayReset();

	// --- Test std::string print and wrapping ---
	myTFT.setFont(font_mega);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(60, 60);
	myTFT.print(timeInfo);
	myTFT.setCursor(60, 60);
	myTFT.println(newLine); // newline
	myTFT.print(newLine);
	myTFT.setCursor(10, 100);
	myTFT.print("1234567890ABCDEFGHIJ"); // wrap long text

	DisplayReset();
}

// Graphics
void Test902_904(void) {
	if (stopRequested) return; 
	std::cout << "Test 902: rectangles " << std::endl;
	rdlib::Return_Codes_e returnValue;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Note .fillRectangle() function does not write to global buffer, 
	// it writes directly to the screen, 
	// with local buffer. so no need to call writeBuffer() after it.
	// Note also that .fillRectangle  is wrapped by fillscreen() function.
	returnValue = myTFT.fillRectangle(85, 25, 20, 20, myTFT.RDLC_YELLOW);
	if (returnValue != rdlib::Return_Codes_e::Success)
	{
		std::cout << "Warning : Test TFTfillRectangle, An error occurred returnValue " +returnValue << std::endl;
	}
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.drawRectWH(45, 25, 20, 20, myTFT.RDLC_RED);
	myTFT.fillRect(110, 25, 20, 20, myTFT.RDLC_GREEN);
	myTFT.drawRoundRect(15, 80, 70, 60, 20, myTFT.RDLC_CYAN);
	myTFT.fillRoundRect(110, 80, 40, 50, 10, myTFT.RDLC_WHITE);

	DisplayReset();

	std::cout << "Test 903 & 904 : Triangles and circles" << std::endl;
	myTFT.drawCircle(40, 40, 25, myTFT.RDLC_GREEN);
	myTFT.fillCircle(80, 80, 25, myTFT.RDLC_YELLOW);
	DisplayReset();

	myTFT.drawTriangle(5, 80, 50, 40, 95, 80, myTFT.RDLC_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.RDLC_RED);
	DisplayReset();
}


void Test905(void)
{
	if (stopRequested) return; 
	std::cout << "Test 905 : polygons" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// octagon
	myTFT.drawPolygon(80, 80, 8, 50, 0, false, myTFT.RDLC_GREEN);
	DisplayReset();
	// octagon
	myTFT.drawPolygon(80, 80, 8, 50, 0, true, myTFT.RDLC_GREEN);
	DisplayReset();
}

void Test906(void) {
	if (stopRequested) return; 
	std::cout << "Test 906 : mesh" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(50, 50, 100,100, 5, myTFT.RDLC_CYAN);
	DisplayReset();
}

void Test907() {
	if (stopRequested) return; 
	std::cout << "Test 907 : quadrilateral" << std::endl;
	// Define the four quadrilateral vertices for different shapes
	// First shape: Quadrilateral
	int16_t x0 = 30, y0 = 30;
	int16_t x1 = 80, y1 = 30;
	int16_t x2 = 80, y2 = 80;
	int16_t x3 = 30, y3 = 80;

	// Second shape: Parallelogram 
	int16_t x4 = 90,  y4 = 100;
	int16_t x5 = 130, y5 = 110;
	int16_t x6 = 120, y6 = 140;
	int16_t x7 = 80,  y7 = 130;

	// Third shape: Trapezoid 1
	int16_t x8  = 95,  y8  = 110;
	int16_t x9  = 100, y9  = 130;
	int16_t x10 = 140, y10 = 130;
	int16_t x11 = 150, y11 = 110;

	// Fourth shape: Trapezoid 2
	int16_t x12 = 40,  y12 = 120;
	int16_t x13 = 55,  y13 = 155;
	int16_t x14 = 100, y14 = 155;
	int16_t x15 = 70,  y15 = 120;
	myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.RDLC_RED);
	myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.RDLC_GREEN);
	DisplayReset();

	myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.RDLC_BLUE);
	myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.RDLC_YELLOW);
	DisplayReset();
}

void Test908(void) {
	if (stopRequested) return; 
	std::cout << "Test 908 : Ellipse" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	// Draw unfilled ellipses
	myTFT.drawEllipse(60, 60, 18, 30, false, myTFT.RDLC_WHITE);
	myTFT.drawEllipse(120, 60, 18, 30, false, myTFT.RDLC_RED);
	DisplayReset();

	// Draw filled ellipses in same positions
	myTFT.drawEllipse(60, 60, 18, 30, true, myTFT.RDLC_WHITE);
	myTFT.drawEllipse(120, 60, 18, 30, true, myTFT.RDLC_RED);
	DisplayReset();
}

void Test909(void) {
	if (stopRequested) return; 
	std::cout << "Test 909 : Drawing Arc: drawArc" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	int16_t centerX = 80;  // X-coordinate of the circle center
	int16_t centerY = 80;  // Y-coordinate of the circle center
	int16_t radius = 50;    // Radius of the circle
	std::cout << " Angle offset default : " << myTFT.getArcAngleOffset() << std::endl;
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myTFT.RDLC_GREEN);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.RDLC_BLUE);
	DisplayReset();
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myTFT.RDLC_RED);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.RDLC_YELLOW);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.RDLC_CYAN);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.RDLC_MAGENTA);
	DisplayReset();
}


/// @endcond
