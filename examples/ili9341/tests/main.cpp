/*!
	@file    examples/ili9341/tests/main.cpp
	@brief   Library test file, tests Text,graphics, functions, bitmaps and FPS.
	@author  Gavin Lyons.
	@note    See USER OPTIONS 1-3 in SETUP function
	@details The FPS test(601) is setup for a 128by128 pixel bitmap
			 Five files 128by128 pixels are loaded and displayed as many times as possible in 
			 10 seconds to calculate FPS. 
			 289 frames, 10 sec, 28.8396 fps  at 8Mhz SPI Baud rate, HW SPI
	@test
			-# Test 300 Sprites demo, small bitmap
			-# Test 301 "clock demo" , icons, small bitmap
			-# Test 302 bi-color image 128x128
			-# Test 303A 24 bit 240by280 color image bitmap from the file system
			-# Test 303B 24 bit 128by128 color image bitmaps from the file system
			-# Test 304A 16 bit 128by128 color image bitmaps from the file system
			-# Test 304B 16 bit 240by320 color image bitmap from the file system
			-# Test 500 RGB color OK?
			-# Test 502 Rotate
			-# Test 503 change modes test -> Invert, display on/off and Sleep.
			-# Test 601 Frame rate per second (FPS) test. 24 bit bitmaps.
			-# Test 701 print out fonts 1-12
			-# Test 702 defined 16-bit Colors, text
			-# Test 703 print entire ASCII font 32 to 127, default font
			-# Test 704 font invert + test character draw using draw functions
			-# Test 705 print method all fonts
			-# Test 706 Misc print class tests (string object, println invert, wrap, base nums etc)
			-# Test 707 Misc draw functions (Invert, wrap)s
			-# Test 708 Vectors with print class
			-# Test 902 rectangles
			-# Test 903 Circle
			-# Test 904 Triangles
			-# Test 905 Polygons
			-# Test 906 Dot Gap
			-# Test 907 Quadrilateral
			-# Test 908 Ellipse
			-# Test 909 Draw Arc and draw simple arc
			-# Test 910 Draw line at an angle
*/

// Section ::  libraries
#include <iostream> // cout
#include "Bitmap_test_data.hpp" // Bitmap Data for test 30X and 60X
#include "ILI9341_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Defines & Global
//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000
#define CLOCK_DISPLAY_TIME 300
//size in pixels of bitmap used in FPS test
#define myBMPWidth 128
#define myBMPHeight 128
uint8_t numberOfFilesFPS = 5;

ILI9341_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers

uint8_t Setup(void);
void EndTests(void);
void DisplayReset(void);
void displayMenu(void);

void Test300(void); // Sprite
void Test301(void); // "clock demo" icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303A(void); // 24 color bitmap
void Test303B(void); // 24 color bitmap
void Test304A(void); // 16 color bitmap
void Test304B(void); // 16 color bitmap
void Test601(void); // Frames per second 24 color bitmap test,
int64_t getTime(); // Utility for FPS test
std::vector<uint8_t> loadImage(const char* name); // Utility for FPS test

void Test500(void);
void Test501(void);
void Test502(void);
void Test503(void);

void Test701(void);
void Test702(void);
void Test703(void);
void Test704(void);
void Test705(void);
void Test706(void);
void Test707(void);
void Test708(void);

void Test902(void);
void Test903(void);
void Test904(void);
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);
void Test910(uint8_t revolutions);

//  Section ::  MAIN loop

int main(void)
{
	if(Setup() != 0)return -1;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	int choice;
	do {
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
				Test303A();
				Test303B();
				Test304A();
				Test304B();
			break;
			case 2:
				Test500();
				Test501();
				Test502();
				Test503();
			break;
			case 3:
				Test601();
			 break;
			case 4:
				Test701();
				Test702();
				Test703();
				Test704();
				Test705();
				Test706();
				Test707();
				Test708();
			break;
			case 5:
				Test902();
				Test903();
				Test904();
				Test905();
				Test906();
				Test907();
				Test908();
				Test909();
				Test910(2);
			break;
			case 6:
				std::cout << "Exiting menu\n";
			break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
			break;
		}
		std::cout << std::endl;
	} while (choice != 6);
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	std::cout << "TFT Start Test " << std::endl;
	std::cout << "ili9341 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.InitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

void displayMenu() {
	std::cout << "Tests Menu:\n";
	std::cout << "1. Bitmap Tests\n";
	std::cout << "2. Function Tests\n";
	std::cout << "3. FrameRate Tests\n";
	std::cout << "4. Text Tests\n";
	std::cout << "5. Graphic Tests\n";
	std::cout << "6. Quit\n";
	std::cout << "Enter your choice: ";
}

void Test300(void)
{
	std::cout << "Test 300: Sprites demo" << std::endl;
	// Test 300-A test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color = myTFT.RDLC_LBLUE
	// Green background screen
	myTFT.fillScreen(myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY);
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
		std::string TimeString = rdlib_time::UTC_string();
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
		//print name
		myTFT.setCursor(10,210);
		myTFT.setTextColor(myTFT.RDLC_CYAN, myTFT.RDLC_BLACK);
		myTFT.print(teststr1);
	}
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	
}

// bitmap 2 colour , 128x128 bi color 1-bit bitmap
void Test302(void)
{
	std::cout << "Test 302: Bi-color bitmap 128X128 pixels" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap bi-color, 128 X 128";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	if(myTFT.drawBitmap(40, 40, 128 , 128, myTFT.RDLC_WHITE , myTFT.RDLC_GREEN, BackupMenuBitmap) != rdlib::Success)
		return;
	if(myTFT.drawBitmap(40, 180, 128 , 128, myTFT.RDLC_RED , myTFT.RDLC_YELLOW, BackupMenuBitmap) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);
}

// bitmap 24 colour , File format = Windows BITMAPINFOHEADER offset 54
// bitmap is 220 by 240
void Test303A(void)
{
	std::cout << "Test 303-A: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 24,  220 X 240";
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(10,10, teststr1);
	delayMilliSecRDL(TEST_DELAY);

	FILE *pFile ;
	size_t pixelSize = 3; // 24 bit 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint16_t widthBitmap = 220;
	uint16_t heightBitmap = 240;

	// Use std::vector for safer memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try 
	{
		// Attempt to allocate memory for the vector
		bmpBuffer.resize(( widthBitmap * heightBitmap) * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error Test 303-A: Memory allocation failed for bmpBuffer" << std::endl;
		return;
	}

	pFile = fopen("bitmap/bitmap24images/24pic7_220X240.bmp", "r"); 
	if (pFile == nullptr)  // Check file exists
	{
		std::cout << "Error Test 303-A: File does not exist" << std::endl;
		return;
	}
	fseek(pFile, FileHeaderOffset, 0); // Put file in Buffer
	fread(bmpBuffer.data(), pixelSize, widthBitmap *heightBitmap, pFile);
	fclose(pFile);

	if(myTFT.drawBitmap24(10, 20, bmpBuffer, widthBitmap ,heightBitmap) != rdlib::Success)
	{// Check for success 0x00
		std::cout << "Warning an Error occurred in drawBitmap24" << std::endl;
		return;
	}

	delayMilliSecRDL(TEST_DELAY5);
}

// bitmap 24 colour , All files format = Windows BITMAPINFOHEADER offset 54
// All bitmaps are 128 by 128
void Test303B(void)
{
	std::cout << "Test 303-B: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 24, 128 X 128";
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 5, teststr1);
	delayMilliSecRDL(TEST_DELAY);

	FILE *pFile ;
	size_t pixelSize = 3; // 24 bit 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t NumberOfFiles  = 5;
	uint16_t widthBitmap = 128;
	uint16_t heightBitmap =128;
	// Use std::vector for safer memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try 
	{
		// Attempt to allocate memory for the vector
		bmpBuffer.resize(( widthBitmap * heightBitmap) * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error Test 303-B: Memory allocation failed for bmpBuffer" << std::endl;
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
			std::cout << "Error Test 303-B: File does not exist" << std::endl;
			return;
		}

		fseek(pFile, FileHeaderOffset, 0); // Put file in Buffer
		fread(bmpBuffer.data(), pixelSize,widthBitmap * heightBitmap, pFile);
		fclose(pFile);

		if(myTFT.drawBitmap24(20, 29, bmpBuffer,widthBitmap, heightBitmap) != rdlib::Success)
		{// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap24" << std::endl;
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
void Test304A(void)
{
	std::cout << "Test 304-A: 16 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 16, 128 X 128";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	FILE *pFile ;
	size_t pixelSize = 2; // 16 bit 2 bytes per pixel
	uint8_t NumberOfFiles  = 3;
	uint8_t offsetBMPHeader = 0;
	uint8_t offsetNoColSpace = 72;
	uint8_t offsetColSpace = 132;
	uint16_t bitmapWidth = 128;
	uint16_t bitmapHeight = 128;
	// Use std::vector for safer memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try 
	{
		// Attempt to allocate memory for the vector
	bmpBuffer.resize(( bitmapWidth *  bitmapHeight ) * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error Test 304-A: Memory allocation failed for bmpBuffer" << std::endl;
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
			std::cout << "Error Test 304-A : File does not exist" << std::endl;
			return;
		}
		fseek(pFile, offsetBMPHeader, 0);
		fread(bmpBuffer.data(), pixelSize, bitmapWidth * bitmapHeight, pFile);
		fclose(pFile);

		if (myTFT.drawBitmap16(40, 40, bmpBuffer, bitmapWidth, bitmapHeight) != rdlib::Success)
		{
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	myTFT.fillScreen(myTFT.RDLC_BLACK);
} // end of test 


// test function for 16 bit color bitmap made in GIMP (RGB 565 16 bit color)
// File 1 fourbyteburger240X320.bmp BITMAPV5HEADER offset 132 , 240x320 pixels
// Color space information written
void Test304B(void)
{
	std::cout << "Test 304-B: 16 bit color image bitmap from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 16, 240 X 320";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	FILE *pFile ;
	size_t pixelSize = 2; // 16 bit 2 bytes per pixel
	uint8_t NumberOfFiles  = 1;
	uint8_t offsetBMPHeader = 132;
	uint16_t bitmapWidth = 240;
	uint16_t bitmapHeight = 320;
	// Use std::vector for safer memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try 
	{
		// Attempt to allocate memory for the vector
	bmpBuffer.resize(( bitmapWidth *  bitmapHeight ) * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error Test 304-B: Memory allocation failed for bmpBuffer" << std::endl;
		return;
	}

	for (uint8_t i = 0 ; i < NumberOfFiles ; i++)
	{
		switch (i) // select file
		{
			case 0:
				pFile = fopen("bitmap/bitmap16images/fourbyteburger240X320.bmp", "r");
			break;
		}
		if (pFile == nullptr)
		{
			std::cout << "Error Test 304-B : File does not exist" << std::endl;
			return;
		}
		fseek(pFile, offsetBMPHeader, 0);
		fread(bmpBuffer.data(), pixelSize, bitmapWidth * bitmapHeight, pFile);
		fclose(pFile);

		if (myTFT.drawBitmap16(0, 0, bmpBuffer, bitmapWidth, bitmapHeight) != rdlib::Success)
		{
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	myTFT.fillScreen(myTFT.RDLC_BLACK);
} // end of test 


void Test601(void) 
{
	myTFT.fillScreen(myTFT.RDLC_RED);
	printf("\n FPS TESTING Test 601\r\n");
	// Load images into buffers
	std::vector<uint8_t>  img[5] = {
		loadImage("bitmap/bitmap24images/24pic1.bmp"),
		loadImage("bitmap/bitmap24images/24pic2.bmp"),
		loadImage("bitmap/bitmap24images/24pic3.bmp"),
		loadImage("bitmap/bitmap24images/24pic4.bmp"),
		loadImage("bitmap/bitmap24images/24pic5.bmp")
	};

	// Check if any loadImage call failed
	for (size_t i = 0; i < 5; ++i) {
		if (img[i].empty()) 
		{
			std::cout << "Error: Image " << i + 1 << " failed to load." << std::endl;
			return;
		}
	}

	int64_t start = getTime(), duration = 0;
	uint32_t frames = 0;
	double fps = 0;

	// Run for ~10 seconds
	while (duration < 10000000) {
		myTFT.drawBitmap24(20, 50, img[frames % 5], myBMPWidth, myBMPHeight);
		duration = getTime() - start;

		if ((++frames % 50) == 0) {
			fps = (double)frames / ((double)duration / 1000000);
			std::cout << fps << std::endl;
		}
	}

	// Get final stats and print
	duration = getTime() - start;
	fps = (double)frames / ((double)duration / 1000000);
	std::cout << frames << " frames, " << duration / 1000000 << " sec, " << fps << " fps" << std::endl;
}


std::vector<uint8_t> loadImage(const char* name) {
	size_t pixelSize = 3; // 24-bit color = 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;

	FILE *pFile = fopen(name, "r");
	if (pFile == nullptr) {
		std::cout << "Error: File does not exist" << std::endl;
		return {}; // Return empty vector on failure
	}

	std::vector<uint8_t> bmpBuffer;
	try {
		bmpBuffer.resize(myBMPWidth * myBMPHeight * pixelSize);
	} catch (const std::bad_alloc&) {
		std::cout << "Error: Could not allocate memory" << std::endl;
		fclose(pFile);
		return {};
	}

	fseek(pFile, FileHeaderOffset, SEEK_SET);
	fread(bmpBuffer.data(), pixelSize, myBMPWidth *myBMPHeight, pFile);
	fclose(pFile);

	return bmpBuffer; // Return by value (RVO applies)
}

// used in FPS 24 color bitmap test, 
int64_t getTime() {
	struct timespec tms;
	if (clock_gettime(CLOCK_REALTIME,&tms)) return -1;
	int64_t micros = tms.tv_sec * 1000000;
	micros += tms.tv_nsec/1000;
	if (tms.tv_nsec % 1000 >= 500) ++micros;
	return micros;
}

void Test500(void)
{
	std::cout << "Test 500: Color Test:: Red,green,blue,yellow,white, black background" << std::endl;
	myTFT.setFont(font_mega);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_GREEN,myTFT.RDLC_BLACK);
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.RDLC_WHITE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
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

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.writeCharString(15, 15, teststr0);
	myTFT.writeCharString(15, 300, teststr0);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setRotation(myTFT.Degrees_90);
	myTFT.writeCharString(15, 15, teststr1);
	myTFT.writeCharString(15, 220, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setRotation(myTFT.Degrees_180);
	myTFT.writeCharString(15, 15, teststr2);
	myTFT.writeCharString(15, 300, teststr2);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.writeCharString(15, 15, teststr3);
	myTFT.writeCharString(15, 220, teststr3);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test503()
{
	std::cout << "Test 503: Mode Tests" << std::endl;
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.RDLC_WHITE);
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


void Test701(void) {

	std::cout << "Test 701: Print out some fonts with writeCharString" << std::endl;
	char teststr1[] = "Default ";
	char teststr2[] = "GLL ";
	char teststr3[] = "Pico ";
	char teststr4[] = "Sinclair ";
	char teststr5[] = "Orla ";
	char teststr6[] = "Retro ";
	char teststr7[] = "Mega";
	char teststr8[] = "Arial b";
	char teststr9[] = "Hall ";
	char teststr10[] = "Arial R";
	char teststr11[] = "GroTesk";
	char teststr12[] = "16";
	char teststr13[] = "7";
	char teststr14[] = "inco";
	char teststr15[] = "GB";
	char teststr16[] = "Mint";

	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.setFont(font_default);
	myTFT.writeCharString(5, 5, teststr1);
	myTFT.setFont(font_gll);
	myTFT.writeCharString(5, 15, teststr2);
	myTFT.setFont(font_pico);
	myTFT.writeCharString(5, 30, teststr3);
	myTFT.setFont(font_sinclairS);
	myTFT.writeCharString(5, 45, teststr4);
	myTFT.setFont(font_orla);
	myTFT.writeCharString(5, 65, teststr5);
	myTFT.setFont(font_retro);
	myTFT.writeCharString(5, 95, teststr6);
	DisplayReset();

	myTFT.setFont(font_mega);
	myTFT.writeCharString(5, 2, teststr7);
	myTFT.setFont(font_arialBold);
	myTFT.writeCharString(5, 30, teststr8);
	myTFT.setFont(font_hallfetica);
	myTFT.writeCharString(5, 60, teststr9);
	myTFT.setFont(font_arialRound);
	myTFT.writeCharString(5, 90, teststr10);
	DisplayReset();

	myTFT.setFont(font_groTesk);
	myTFT.writeCharString(5, 5, teststr11);
	myTFT.setFont(font_sixteenSeg);
	myTFT.writeCharString(5, 40, teststr12);
	DisplayReset();

	myTFT.setFont(font_sevenSeg);
	myTFT.writeCharString(5, 5, teststr13);
	myTFT.setFont(font_inconsola);
	myTFT.writeCharString(5, 60, teststr14);
	DisplayReset();

	myTFT.setFont(font_groTeskBig);
	myTFT.writeCharString(5, 5, teststr15);
	myTFT.setFont(font_mint);
	myTFT.writeCharString(5, 80, teststr16);
	DisplayReset();
}

void Test702(void) {
	std::cout << "Test 702: Some of the Defined 16-bit Colors for text" << std::endl;
	myTFT.setFont(font_mega);
	char teststr1[] = "WHITE";
	char teststr2[] = "BLUE";
	char teststr3[] = "RED";
	char teststr4[] = "GREEN";
	char teststr5[] = "CYAN";
	char teststr6[] = "MAGENTA";
	char teststr7[] = "YELLOW";
	char teststr8[] = "GREY";
	char teststr9[] = "TAN";
	char teststr10[] = "BROWN";
	char teststr11[] = "ORANGE";
	char teststr12[] = "DGREEN";

	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 0, teststr1);
	myTFT.setTextColor(myTFT.RDLC_BLUE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 16, teststr2);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 33, teststr3);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 49, teststr4);
	myTFT.setTextColor( myTFT.RDLC_CYAN, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 66, teststr5);
	myTFT.setTextColor(myTFT.RDLC_MAGENTA, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 100, teststr6);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 130, teststr7);
	myTFT.setTextColor(myTFT.RDLC_GREY, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 170, teststr8);
	myTFT.setTextColor(myTFT.RDLC_TAN, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 200, teststr9);
	myTFT.setTextColor(myTFT.RDLC_BROWN, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 220, teststr10);
	myTFT.setTextColor( myTFT.RDLC_ORANGE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 260, teststr11);
	myTFT.setTextColor(myTFT.RDLC_DGREEN, myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 300, teststr12);
	DisplayReset();
}

void Test703(void) {
	std::cout << "Test 703: Print entire ASCII font 32 to 127" << std::endl;
	uint8_t row = 5;
	uint8_t col = 5;
	myTFT.setFont(font_mega);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	for (char i = 32; i < 127; i++) {

		myTFT.writeChar(col, row, i);
		col += 17;
		if (col > 200) {
			row += 17;
			col = 5;
		}
	}

	DisplayReset();

}



void Test704(void) {
	std::cout << "Test 704: font invert + test character draw using draw functions" << std::endl;
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
	myTFT.setFont(font_mega);
	char teststr1[] = "TEST";
	myTFT.writeCharString(15, 5, teststr1);
	myTFT.setInvertFont(true);
	myTFT.writeCharString(15, 35, teststr1); //inverted yellow on red
	myTFT.setInvertFont(false);
	myTFT.writeCharString(15, 55, teststr1);
	myTFT.writeChar(0, 85, 'G');
	myTFT.writeChar(25, 85, 'L');
	DisplayReset();
}


void Test705(void)
{
	std::cout << "Test 705: Print class methods" << std::endl;

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);

	myTFT.setCursor(5,5);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);

	myTFT.setCursor(5,15);
	myTFT.setFont(font_gll);
	myTFT.print("GLL ");
	myTFT.print(123.284,1); // print 123.3

	myTFT.setCursor(5,25);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);

	myTFT.setCursor(5,65);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);

	myTFT.setCursor(5,85);
	myTFT.setFont(font_retro);
	myTFT.print("retro  ");
	myTFT.print(-3.14);

	DisplayReset();

	// Test font  mega Arial bold and Hallf.
	myTFT.setCursor(5,5);
	myTFT.setFont(font_mega);
	myTFT.println("mega ");
	myTFT.print(61);

	myTFT.setCursor(5,40);
	myTFT.setFont(font_arialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);

	myTFT.setCursor(5,80);
	myTFT.setFont(font_hallfetica);
	myTFT.println("hall f");
	myTFT.print(1.48);

	DisplayReset();

	// Test Font orla + arial round

	myTFT.setFont(font_orla);
	myTFT.setCursor(5,5);
	myTFT.println(-7.16);
	myTFT.print("Orla");

	myTFT.setCursor(5,70);
	myTFT.setFont(font_arialRound);
	myTFT.println(-8.16);
	myTFT.print("a rnd");

	DisplayReset();

	// Test font grotesk + sixteen segment
	myTFT.setCursor(5,5);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);

	myTFT.setCursor(0,90);
	myTFT.setFont(font_sixteenSeg);
	myTFT.print(1245);

	DisplayReset();

	// Test font seven segment
	myTFT.setCursor(5,5);
	myTFT.setFont(font_sevenSeg);
	myTFT.println(12);
	DisplayReset();

	// Test font mint + gll
	myTFT.setCursor(5,5);
	myTFT.setFont(font_gll);
	myTFT.println("GLL");
	myTFT.setFont(font_mint);
	myTFT.println("MINT");;
	myTFT.print("9`C");
	DisplayReset();

}

void Test706(void)
{
	std::cout << "Test 706: Misc print class(string object, println invert, wrap, base nums etc)" << std::endl;
	//Inverted print fonts
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
	myTFT.setFont(font_default);

	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(5,15);
	myTFT.print(-49);

	myTFT.setFont(font_default);
	myTFT.setCursor(5,35);
	myTFT.print(112.09);
	myTFT.setCursor(5,55);
	myTFT.print("ABCD");

	myTFT.setFont(font_gll);
	myTFT.setCursor(5,85);
	myTFT.print("ER");

	DisplayReset();

	// Inverted print fonts
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_RED);

	myTFT.setFont(font_arialBold);
	myTFT.setCursor(5,5);
	myTFT.print("INVERT");
	myTFT.setCursor(5,25);
	myTFT.print(-94.982, 2);

	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(5,65);
	myTFT.print("INVERT");
	myTFT.setCursor(5,85);
	myTFT.print(123456);
	DisplayReset();


	// Test print with DEC BIN OCT HEX
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(font_pico);
	myTFT.setCursor(5,5);
	myTFT.print(numPos , myTFT.RDL_DEC); // 47
	myTFT.setCursor(5,25);
	myTFT.print(numPos , myTFT.RDL_BIN); // 10111
	myTFT.setCursor(5,45);
	myTFT.print(numPos , myTFT.RDL_OCT); // 57
	myTFT.setCursor(5,65);
	myTFT.print(numPos , myTFT.RDL_HEX); // 2F

	DisplayReset();

	// Test print a string object with print
	myTFT.setFont(font_default);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(5, 5);
	myTFT.print(timeInfo);
	// print a new line with println
	myTFT.setCursor(5,40);
	myTFT.println(newLine); // print a new line feed with println
	myTFT.print(newLine);

	//text wrap with print
	myTFT.setCursor(105,70);
	myTFT.print("12345678901234567890ABCDEFGHIJ");
	DisplayReset();
}


void Test707(void)
{
	std::cout << "Test 707: Misc draw functions tests wrap,  etc" << std::endl;

	//wrap
	char teststr0[] = "123456789";

	myTFT.setFont(font_default);
	myTFT.writeCharString(200, 5, teststr0);
	myTFT.setFont(font_mega);
	myTFT.writeCharString(180, 60, teststr0);
	DisplayReset();

	//invert + Numeric fonts  sprintf
	int myInt=931;
	char myStr[5];
	myTFT.setFont(font_sinclairS);
	sprintf(myStr, "%d", myInt);
	myTFT.writeCharString(5, 5, myStr);

	myTFT.setFont(font_retro);
	float  myPI = 3.171592;
	char myStr2[8];
	sprintf(myStr2, "%0.3f", myPI); // 3.172
	myTFT.writeCharString(5, 65, myStr2);

	DisplayReset();

	//invert + Numeric
	char teststr1[] = "12:81";
	char teststr2[] = "72:83";
	char teststr3[] = "9 rnd";
	char teststr4[] = "10 bold";

	myTFT.setFont(font_mega);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_BLACK);
	myTFT.writeChar(2, 2, '7');
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	myTFT.writeCharString(2, 40, teststr1);

	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeChar(2, 75, '8');
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_RED);
	myTFT.writeCharString(2, 100, teststr2);

	DisplayReset();

	myTFT.setFont(font_arialRound);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_BLACK);
	myTFT.writeChar(2, 2, '9');
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	myTFT.writeCharString(2, 26, teststr3);

	myTFT.setFont(font_arialBold);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeChar(2, 56, 'A');
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_RED);
	myTFT.writeCharString(2, 90, teststr4);

	DisplayReset();
}

void Test708(void)
{
	std::cout << "Test 708: print class : vectors" << std::endl;
	myTFT.setFont(font_mega);
	// For a vector of floats
	myTFT.setCursor(5, 40);
	std::vector<float> floatVec = {1.0, 22.004, -3.149823, 478.55434};
	myTFT.println(floatVec, 1); // Output 1 decimal places
	myTFT.println(floatVec);    // Output 2 decimal places(default)
	myTFT.print(floatVec, 3); // Output 3 decimal places
	DisplayReset();
	// For a vector of integers
	std::vector<int> intVec = {47, 11, 34};
	myTFT.setCursor(0, 40);
	myTFT.println( intVec[0]);  // print 47
	myTFT.println( intVec[0], myTFT.RDL_HEX);  // print 2F
	myTFT.println( intVec[0] ,myTFT.RDL_OCT); //print 57
	myTFT.println( intVec[0], myTFT.RDL_BIN); // print 101111
	DisplayReset();

	myTFT.setCursor(0, 60);
	myTFT.print( intVec); // 47 11 34

	//For a vector of strings
	myTFT.setCursor(0, 100);
	std::vector<std::string> stringVec = {"HELLO", "VECTOR"};
	myTFT.println(stringVec); // Output: "HELLO VECTOR"
	myTFT.println(stringVec[0]); // Output: "HELLO "
	myTFT.print(stringVec[1]); // Output: "VECTOR"
	DisplayReset();
}


void Test902(void) {
	std::cout << "Test 902: rectangles " << std::endl;
	rdlib::Return_Codes_e returnValue;
	myTFT.drawRectWH(25, 25, 20, 20, myTFT.RDLC_RED);
	returnValue = myTFT.fillRectangle(85, 25, 20, 20, myTFT.RDLC_YELLOW);
	if (returnValue != rdlib::Success)
	{
		std::cout << "Warning : Test TFTfillRectangle, An error occurred returnValue =" << +returnValue << std::endl;
	}
	myTFT.fillRect(115, 25, 20, 20, myTFT.RDLC_GREEN);
	myTFT.drawRoundRect(15, 100, 70, 70, 20, myTFT.RDLC_CYAN);
	myTFT.fillRoundRect(120, 100, 50, 50, 10, myTFT.RDLC_WHITE);

	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test903(void) {
	std::cout << "Test 903 & 904 : Triangles and circles" << std::endl;
	myTFT.drawCircle(40, 20, 15, myTFT.RDLC_GREEN);
	myTFT.fillCircle(80, 20, 15, myTFT.RDLC_YELLOW);
}

void Test904(void) {
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80,myTFT.RDLC_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.RDLC_RED);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test905(void)
{
	std::cout << "Test 905 : polygons" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// decagon
	myTFT.drawPolygon(50, 100, 10, 50, 0, false, myTFT.RDLC_RED);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, false, myTFT.RDLC_GREEN);
	// hexagon rotated
	myTFT.drawPolygon(50, 200, 6, 50, 45, false, myTFT.RDLC_WHITE);
	// pentagon
	myTFT.drawPolygon(150, 200, 5, 50, 0, false, myTFT.RDLC_TAN);
	delayMilliSecRDL(TEST_DELAY5);
	// decagon
	myTFT.drawPolygon(50, 100, 10, 50, 0, true, myTFT.RDLC_RED);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, true, myTFT.RDLC_GREEN);
	// hexagon rotated
	myTFT.drawPolygon(50, 200, 6, 50, 45, true, myTFT.RDLC_WHITE);
	// pentagon
	myTFT.drawPolygon(150, 200, 5, 50, 0, true, myTFT.RDLC_TAN);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test906(void) {
	std::cout << "Test 906 : mesh" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(0, 0, 100,100, 5, myTFT.RDLC_CYAN);
	myTFT.drawDotGrid(0, 200, 100,100, 10, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test907() {
	std::cout << "Test 907 : quadrilateral" << std::endl;
	// Define the four quadrilateral vertices for different shapes
	// First shape (quadrilateral )
	int16_t x0 = 10, y0 = 10; // First vertex
	int16_t x1 = 50, y1 = 10;
	int16_t x2 = 50, y2 = 50;
	int16_t x3 = 10, y3 = 50;
	// Second shape (Parallelogram )
	int16_t x4 = 180, y4 = 240;  // First vertex
	int16_t x5 = 230, y5 = 250;
	int16_t x6 = 220, y6 = 290;
	int16_t x7 = 170, y7 = 280;
	// Third shape (Trapezoid 1)
	int16_t x8 = 106, y8 = 107;  // First vertex
	int16_t x9 = 112, y9 = 128;
	int16_t x10 = 134, y10 = 128;
	int16_t x11 = 153, y11 = 107;
	// Fourth shape (Trapezoid 2)
	int16_t x12 = 16, y12 = 207;  // First vertex
	int16_t x13 = 29, y13 = 242;
	int16_t x14 = 85, y14 = 242;
	int16_t x15 = 46, y15 = 207;

	myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.RDLC_RED);
	myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.RDLC_GREEN);
	myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.RDLC_BLUE);
	myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test908(void) {
	std::cout << "Test 908 : Ellipse" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myTFT.drawEllipse(TFT_WIDTH / 4,TFT_HEIGHT / 4, 20, 40, false, myTFT.RDLC_WHITE);  // Top-left
	myTFT.drawEllipse(3 * TFT_WIDTH / 4,TFT_HEIGHT / 4, 25, 50, false, myTFT.RDLC_RED); // Top-right
	myTFT.drawEllipse(TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 60, 30, false, myTFT.RDLC_BLUE); // Bottom-left
	myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, false, myTFT.RDLC_YELLOW); // Bottom-right
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.drawEllipse(TFT_WIDTH / 4,TFT_HEIGHT / 4, 20, 40, true, myTFT.RDLC_WHITE);
	myTFT.drawEllipse(3 * TFT_WIDTH / 4,TFT_HEIGHT / 4, 25, 50, true, myTFT.RDLC_RED);
	myTFT.drawEllipse(TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 60, 30, true, myTFT.RDLC_BLUE);
	myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, true, myTFT.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test909(void) {
	std::cout << "Test 909 : Drawing Arc: drawArc and drawSimpleArc" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	int16_t centerX = 120;  // X-coordinate of the circle center
	int16_t centerY = 160;  // Y-coordinate of the circle center
	int16_t radius = 50;    // Radius of the circle
	std::cout << " Angle offset default : " << myTFT.getArcAngleOffset() << std::endl;
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.RDLC_BLUE);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myTFT.RDLC_RED);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.RDLC_CYAN);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.RDLC_MAGENTA);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	// drawSimpleArc :: Draw an arc with a radius of 50, from 0 to 300 degrees
	myTFT.drawSimpleArc(120, 160, 50, 0.0f, 300.0f, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test910(uint8_t revolutions)
{
	std::cout << "Test 910 : Draw line at angle function" << std::endl;
	char buffer[4]; // 3 digits + null terminator
	myTFT.setFont(font_sinclairS);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.drawCircle(64, 64, 60, myTFT.RDLC_YELLOW);
	myTFT.drawCircle(64, 64, 59, myTFT.RDLC_YELLOW);
	myTFT.setCursor(5, 150);
	myTFT.print("Degrees = ");
	myTFT.setCursor(5, 170);
	myTFT.print("Radians = ");
	myTFT.setCursor(5, 190);
	myTFT.print("Revolus = ");
	for (uint8_t j =0 ; j< revolutions ; j++)
	{
		for (int i = 0; i <= 361; i++)
		{
			//drawLineAngle(x, y, angle, start,length, offset,  color);
			myTFT.drawLineAngle(64, 64, i-1, 0 , 58, -90, myTFT.RDLC_BLACK);//erase last needle
			myTFT.drawLineAngle(64, 64, i, 0, 58, -90, myTFT.RDLC_BLUE);//draw needle
			delayMilliSecRDL(5);
			//print degrees
			myTFT.setCursor(90,150);
			sprintf(buffer, "%03d", i);
			myTFT.print(buffer);
			// print radians
			myTFT.setCursor(90,170);
			float radians = i * (std::numbers::pi / 180.0f); // Convert degrees to radians
			myTFT.print(radians , 4);
			myTFT.setCursor(90,190);
			myTFT.print(j);
		}
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void DisplayReset(void)
{
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_mega);
	myTFT.writeCharString(25, 50, teststr1);
	DisplayReset();

	myTFT.PowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************

/// @endcond
