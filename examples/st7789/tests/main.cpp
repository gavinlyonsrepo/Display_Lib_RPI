/*!
	@file   examples/st7789/tests/main.cpp
	@brief  Library test file, tests bitmaps, text, graphics, functions & framerate.
	@author Gavin Lyons.
	@note   See USER OPTIONS 1-3 in SETUP function
	@test
		-# Test 300 Sprites demo, small bitmap
		-# Test 301 "clock demo" , icons, small bitmap
		-# Test 302 bi-color image 128x128
		-# Test 303 24 bit color image bitmaps from the file system
		-# Test 304 16 bit color image bitmaps from the file system
		-# Test 305 16 bit color image bitmap from data
		-# Test 500 RGB color OK?
		-# Test 502 Rotate
		-# Test 503 change modes test -> Invert, display on/off and Sleep.
		-# Test 504 scroll
		-# Test 601 Frame rate per second (FPS) test. bitmaps.
		-# Test 603 Frame rate per second (FPS) test. text graphics.
		-# Test 701 print out fonts with writeCharString
		-# Test 705 print method all fonts
		-# Test 706 Misc print class tests (string object, println invert, wrap, base nums)
		-# Test 707 Vectors with print class
		-# Test 902 rectangles
		-# Test 903 Circle
		-# Test 904 Triangles
		-# Test 905 Polygons
		-# Test 906 Dot Grid
		-# Test 907 Quadrilateral
		-# Test 908 Ellipse
		-# Test 909 Draw Arc and draw simple arc
		-# Test 910 Draw Line at angle function
*/

// Section ::  libraries
#include <iostream> // cout
#include <ctime> // for test301
#include "Bitmap_test_data.hpp" // Data for bitmap tests
#include "ST7789_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
ST7789_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 350 // bitmap test 301 clock demo time
// Framerate test parameters
// bitmap
uint8_t numberOfFiles = 5;
#define runtimeBmp  10000000 //uS
#define myBMPWidth 128
#define myBMPHeight 128
// text + graphics
#define runtime  20000000 //uS

//  Section ::  Function Headers
uint8_t Setup(void);
void EndTests(void);
void DisplayReset(void);
void displayMenu(void);
// bitmap tests
void Test300(void); // Sprite 
void Test301(void); // "clock demo" icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303(void); // 24 color bitmap
void Test304(void); // 16 color bitmap
void Test305(void); // 16 color bitmap (from data)
std::string UTC_string(void); // for clock demo
// Functions tests
void Test500(void);
void Test502(void);
void Test503(void);
void Test504(void);
// FPS tests
void Test601(int64_t runtime_us);
void Test603(int64_t runtime_us);
int64_t getTime(); // Utility for 601 FPS test
std::vector<uint8_t>   loadImage(const char*); // Utility for 601 FPS test
void display(double fps, uint32_t frames); // Utility for 603 FPS test
// Text tests
void Test701(void);
void Test705(void);
void Test706(void);
void Test707(void);
// Graphic tests
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
				Test303();
				Test304();
				Test305();
			break;
			case 2:
				Test500();
				Test502();
				Test503();
				Test504();
			break;
			case 3:
				Test601(runtimeBmp);
				Test603(runtime);
			break;
			case 4:
				Test701();
				Test705();
				Test706();
				Test707();
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
	std::cout << "TFT Start" << std::endl;
	std::cout << "ST7789 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI settings**
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

void Test500(void)
{
	std::cout << "Test 500: Color Test:: Red,green,blue,yellow,white, black background" << std::endl;
	myTFT.setFont(font_mega);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_GREEN,myTFT.RDLC_BLACK);
	myTFT.fillRoundRect(8, 50, 24, 60, 8, myTFT.RDLC_RED);
	myTFT.fillRoundRect(32, 50, 24, 60, 8, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(56, 50, 24, 60, 8, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(80, 50, 24, 60, 8, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(104, 50, 24, 60, 8, myTFT.RDLC_WHITE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
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
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.writeCharString(55, 55, teststr0);
	myTFT.writeCharString(55, 200, teststr0);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.writeCharString(55, 55, teststr1);
	myTFT.writeCharString(55, 200, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_180);
	myTFT.writeCharString(55, 55, teststr2);
	myTFT.writeCharString(55, 200, teststr2);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_270);
	myTFT.writeCharString(55, 55, teststr3);
	myTFT.writeCharString(55, 200, teststr3);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test503()
{
	std::cout << "Test 503: Mode Tests" << std::endl;
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(8, 50, 24, 60, 8, myTFT.RDLC_RED);
	myTFT.fillRoundRect(32, 50, 24, 60, 8, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(56, 50, 24, 60, 8, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(80, 50, 24, 60, 8, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(104, 50, 24, 60, 8, myTFT.RDLC_WHITE);
	myTFT.writeCharString(10, 120, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	// Invert on and off
	myTFT.TFTchangeInvertMode(true);
	std::cout << "Test 503-1: Invert on " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTchangeInvertMode(false);
	std::cout << "Test 503-2: Invert off " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	
	// Display on and off
	myTFT.TFTenableDisplay(false);
	std::cout << "Test 503-3: Display off" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTenableDisplay(true);
	std::cout << "Test 503-4: Turn Display back on" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	
	// Sleep on and off
	myTFT.TFTsleepDisplay(true);
	std::cout << "Test 503-5: Sleep on" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTsleepDisplay(false);
	std::cout << "Test 503-6: Sleep off" << std::endl;
	delayMilliSecRDL(TEST_DELAY2);
}

void Test504(void)
{
	DisplayReset();
	printf("Test 504: Scroll\r\n");
	myTFT.setFont(font_default);
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
	delayMilliSecRDL(1000);
	}
	myTFT.TFTNormalMode();
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	DisplayReset();

}

/**
 * @brief Measures the FPS (frames per second) of a display test.
 * Runs for the specified duration, rendering frames and calculating FPS.
 * @param runtime_us Duration of the test in microseconds (e.g., 20000000 for 20 seconds).
 */
void Test603(int64_t runtime_us) {
	// Initialize display settings
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setFont(font_orla);
	// Print test start message
	std::cout << "Test 601 FPS test: text + graphics :: " 
			  << (runtime_us / 1000000) << " seconds" << std::endl;
	// Initialize timing and FPS calculation variables
	int64_t start = getTime();  // Get start time in microseconds
	int64_t duration = 0;       // Track elapsed time
	uint32_t frames = 0;        // Count number of frames
	double fps = 0;             // Store calculated FPS

	// Run the test for the specified duration
	while (duration < runtime_us) {
		// Call display function to test
		display(fps, frames);
		// Update elapsed time
		duration = getTime() - start;
		// Calculate and print FPS every 50 frames
		if ((++frames % 50) == 0) {
			fps = (double)frames / ((double)duration / 1000000);  // FPS = frames / elapsed seconds
			std::cout << fps << std::endl;
		}
	}
	// Compute final FPS and display results
	duration = getTime() - start;
	fps = (double)frames / ((double)duration / 1000000);
	std::cout << frames << " frames, " << duration / 1000000 << " sec, " << fps << " fps" << std::endl;
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}


// Frames per second test , 24 color bitmap test, 
void Test601(int64_t runtime_us) {
	myTFT.fillScreen(myTFT.RDLC_RED);
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	std::cout << "Test 603 FPS test: bitmaps ::" << (runtime_us / 1000000) << " seconds" << std::endl;
	// Load images into buffers
	std::vector<uint8_t> img[5] = {
		loadImage("bitmap/bitmap24images/24pic1.bmp"),
		loadImage("bitmap/bitmap24images/24pic2.bmp"),
		loadImage("bitmap/bitmap24images/24pic3.bmp"),
		loadImage("bitmap/bitmap24images/24pic4.bmp"),
		loadImage("bitmap/bitmap24images/24pic5.bmp")
	};

	// Check if any loadImage call failed
	for (size_t i = 0; i < 5; ++i) {
		if (img[i].empty()) {
			std::cout << "Error: Image " << i + 1 << " failed to load." << std::endl;
			return;
		}
	}

	int64_t start = getTime(), duration = 0;
	uint32_t frames = 0;
	double fps = 0;

	// Run for ~10 seconds
	while (duration < runtime_us) 
	{
		myTFT.drawBitmap24(25, 50, img[frames % 5], myBMPWidth, myBMPHeight);
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


void  display(double fps, uint32_t frames)
{
	myTFT.setCursor(25, 30);
	myTFT.print("G Lyons");
	myTFT.setCursor(25, 70);
	myTFT.print(frames);

	myTFT.setCursor(25, 110);
	myTFT.print(fps, 2);
	myTFT.print(" fps");
	myTFT.setCursor(25, 150);
	myTFT.print(rdlib::LibraryVersion());

	myTFT.drawFastVLine(160, 20, 200, myTFT.RDLC_BLUE);
	myTFT.fillRect(200, 70, 20, 20, myTFT.RDLC_RED);
	myTFT.fillCircle(200, 30, 10, myTFT.RDLC_GREEN);
	myTFT.fillTriangle(180,200, 200, 180 , 220, 200, myTFT.RDLC_YELLOW);
}

/**
 * @brief Gets the current time in microseconds since the Unix epoch.
 * Uses clock_gettime with CLOCK_REALTIME to fetch the system time.
 * @return int64_t Time in microseconds, or -1 on failure.
 */
int64_t getTime() {
	// Structure to store the current time
	struct timespec tms;
	// Get the current real-world time; return -1 if the call fails
	if (clock_gettime(CLOCK_REALTIME, &tms)) return -1;
	// Convert seconds to microseconds
	int64_t micros = tms.tv_sec * 1000000;
	// Convert nanoseconds to microseconds and add
	micros += tms.tv_nsec / 1000;
	// Round up if the nanosecond remainder is >= 500
	if (tms.tv_nsec % 1000 >= 500) ++micros;
	// Return the time in microseconds
	return micros;
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
		bmpBuffer.resize(TFT_WIDTH * TFT_HEIGHT * pixelSize);
	} catch (const std::bad_alloc&) {
		std::cout << "Error: Could not allocate memory" << std::endl;
		fclose(pFile);
		return {};
	}

	fseek(pFile, FileHeaderOffset, SEEK_SET);
	fread(bmpBuffer.data(), pixelSize, TFT_WIDTH * TFT_HEIGHT, pFile);
	fclose(pFile);

	return bmpBuffer; // Return by value (RVO applies)
}

void Test701(void) {

	std::cout << "Test 701: Print out all fonts with writeCharString" << std::endl;
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
	myTFT.writeCharString(5, 45, teststr1);
	myTFT.setFont(font_gll);
	myTFT.writeCharString(5, 55, teststr2);
	myTFT.setFont(font_pico);
	myTFT.writeCharString(5, 70, teststr3);
	myTFT.setFont(font_sinclairS);
	myTFT.writeCharString(5, 130, teststr4);
	myTFT.setFont(font_orla);
	myTFT.writeCharString(5, 170, teststr5);
	myTFT.setFont(font_retro);
	myTFT.writeCharString(5, 200, teststr6);
	DisplayReset();

	myTFT.setFont(font_mega);
	myTFT.writeCharString(5, 52, teststr7);
	myTFT.setFont(font_arialBold);
	myTFT.writeCharString(5, 80, teststr8);
	myTFT.setFont(font_hallfetica);
	myTFT.writeCharString(5, 120, teststr9);
	myTFT.setFont(font_arialRound);
	myTFT.writeCharString(5, 200, teststr10);
	DisplayReset();

	myTFT.setFont(font_groTesk);
	myTFT.writeCharString(5, 55, teststr11);
	myTFT.setFont(font_sixteenSeg);
	myTFT.writeCharString(5,120, teststr12);
	DisplayReset();

	myTFT.setFont(font_sevenSeg);
	myTFT.writeCharString(5, 55, teststr13);
	myTFT.setFont(font_inconsola);
	myTFT.writeCharString(5, 120, teststr14);
	DisplayReset();
	
	myTFT.setFont(font_groTeskBig);
	myTFT.writeCharString(5, 55, teststr15);
	myTFT.setFont(font_mint);
	myTFT.writeCharString(5, 120, teststr16);
	DisplayReset();
}

void Test705(void)
{
	std::cout << "Test 705: Print class methods" << std::endl;

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);

	myTFT.setCursor(5,55);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);

	myTFT.setCursor(5,75);
	myTFT.setFont(font_gll);
	myTFT.print("GLL ");
	myTFT.print(123.284,1); // print 123.3

	myTFT.setCursor(5,125);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);

	myTFT.setCursor(5,170);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);

	myTFT.setCursor(5,200);
	myTFT.setFont(font_retro);
	myTFT.print("retro  ");
	myTFT.print(-3.14);

	DisplayReset();

	// Test font  mega Arial bold and Hallf.
	myTFT.setCursor(5,55);
	myTFT.setFont(font_mega);
	myTFT.println("mega ");
	myTFT.print(61);

	myTFT.setCursor(5,100);
	myTFT.setFont(font_arialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);

	myTFT.setCursor(5,160);
	myTFT.setFont(font_hallfetica);
	myTFT.println("hall f");
	myTFT.print(1.48);

	DisplayReset();

	// Test Font orla + arial round

	myTFT.setFont(font_orla);
	myTFT.setCursor(5,55);
	myTFT.println(-7.16);
	myTFT.print("Orla");

	myTFT.setCursor(5,120);
	myTFT.setFont(font_arialRound);
	myTFT.println(-8.16);
	myTFT.print("a rnd");

	DisplayReset();

	// Test font grotesk + sixteen segment
	myTFT.setCursor(5,75);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);

	myTFT.setCursor(0,180);
	myTFT.setFont(font_sixteenSeg);
	myTFT.print(1245);

	DisplayReset();
	
	// Test font seven segment 
	myTFT.setCursor(5,55);
	myTFT.setFont(font_sevenSeg);
	myTFT.println(12);
	DisplayReset();

	// Test font mint + gll
	myTFT.setCursor(5,55);
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
	//Inverted print fonts 1-6
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
	myTFT.setFont(font_default);

	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(5,55);
	myTFT.print(-49);

	myTFT.setFont(font_mega);
	myTFT.setCursor(5,90);
	myTFT.print(112.09);
	myTFT.setCursor(5,120);
	myTFT.print("ABCD");

	myTFT.setFont(font_gll);
	myTFT.setCursor(5,200);
	myTFT.print("ER");

	DisplayReset();

	// Inverted print fonts 7-12
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_RED);

	myTFT.setFont(font_arialBold);
	myTFT.setCursor(5,55);
	myTFT.print("INVERT");
	myTFT.setCursor(5,85);
	myTFT.print(-94.982, 2);

	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(5,120);
	myTFT.print("INVERT");
	myTFT.setCursor(5,200);
	myTFT.print(123456);
	DisplayReset();


	// Test print with DEC BIN OCT HEX
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(font_mega);
	myTFT.setCursor(5,55);
	myTFT.print(numPos , myTFT.RDL_DEC); // 47
	myTFT.setCursor(5,75);
	myTFT.print(numPos , myTFT.RDL_BIN); // 10111
	myTFT.setCursor(5,120);
	myTFT.print(numPos , myTFT.RDL_OCT); // 57
	myTFT.setCursor(5,160);
	myTFT.print(numPos , myTFT.RDL_HEX); // 2F

	DisplayReset();

	// Test print a string object with print
	myTFT.setFont(font_mega);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(5, 55);
	myTFT.print(timeInfo);
	// print a new line with println
	myTFT.setCursor(5,120);
	myTFT.println(newLine); // print a new line feed with println
	myTFT.print(newLine);

	//text wrap with print
	myTFT.setCursor(5,70);
	myTFT.print("12345678901234567890ABCDEFGHIJ");
	DisplayReset();
	
	// test a vector
}

void Test707(void)
{
	std::cout << "Test 708: print class -: vectors" << std::endl;
	myTFT.setFont(font_mega);
	// For a vector of floats
	myTFT.setCursor(5, 40);
	std::vector<float> floatVec = {1.0, 22.004, -3.149823, 478.55434};
	myTFT.println(floatVec, 1); // Output 1 decimal places
	//myTFT.setCursor(5, 80);
	myTFT.println(floatVec);    // Output 2 decimal places(default)
	//myTFT.setCursor(5, 120);
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
	myTFT.drawRoundRect(15, 60, 50, 50, 5, myTFT.RDLC_CYAN);
	myTFT.fillRoundRect(70, 60, 50, 50, 10, myTFT.RDLC_WHITE);

	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test903(void) {
	std::cout << "Test 903 & 904 : Triangles and circles" << std::endl;
	myTFT.drawCircle(40, 200, 15, myTFT.RDLC_GREEN);
	myTFT.fillCircle(80, 200, 15, myTFT.RDLC_YELLOW);
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
	delayMilliSecRDL(TEST_DELAY2);
	// decagon
	myTFT.drawPolygon(50, 100, 10, 50, 0, true, myTFT.RDLC_RED);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, true, myTFT.RDLC_GREEN);
	// hexagon rotated
	myTFT.drawPolygon(50, 200, 6, 50, 45, true, myTFT.RDLC_WHITE);
	// pentagon
	myTFT.drawPolygon(150, 200, 5, 50, 0, true, myTFT.RDLC_TAN);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test906(void) {
	std::cout << "Test 906 : dot grid" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(0, 0, TFT_WIDTH, TFT_HEIGHT, 5, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test907() {
	std::cout << "Test 907 : quadrilateral" << std::endl;
	// Define the 3 quadrilateral vertices for different shapes
	//  (Parallelogram )
	int16_t x4 = 180, y4 = 240;  // First vertex
	int16_t x5 = 230, y5 = 250;
	int16_t x6 = 220, y6 = 290;
	int16_t x7 = 170, y7 = 280;
	//  (Trapezoid 1)
	int16_t x8 = 106, y8 = 107;  // First vertex
	int16_t x9 = 112, y9 = 128;
	int16_t x10 = 134, y10 = 128;
	int16_t x11 = 153, y11 = 107;
	//  (Trapezoid 2)
	int16_t x12 = 16, y12 = 207;  // First vertex
	int16_t x13 = 29, y13 = 242;
	int16_t x14 = 85, y14 = 242;
	int16_t x15 = 46, y15 = 207;

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
	myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, false, myTFT.RDLC_YELLOW); // Bottom-right
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.drawEllipse(TFT_WIDTH / 4,TFT_HEIGHT / 4, 20, 40, true, myTFT.RDLC_WHITE);
	myTFT.drawEllipse(3 * TFT_WIDTH / 4,TFT_HEIGHT / 4, 25, 50, true, myTFT.RDLC_RED);
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
	myTFT.drawArc(centerX, centerY, radius, 10, 0.0f, 180.0f, myTFT.RDLC_GREEN);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.RDLC_BLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 5, 0.0f, 90.0f, myTFT.RDLC_RED);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 5, 90.0f, 180.0f, myTFT.RDLC_YELLOW);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 5, 180.0f, 270.0f, myTFT.RDLC_CYAN);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 5, 270.0f, 360.0f, myTFT.RDLC_MAGENTA);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	// drawSimpleArc :: Draw an arc with a radius of 50, from 0 to 300 degrees
	myTFT.drawSimpleArc(120, 160, 50, 0.0f, 300.0f, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test910(uint8_t revolutions)
{
	std::cout << "Test 910 : Draw line at angle function" << std::endl;
	myTFT.drawCircle(84, 84, 60, myTFT.RDLC_GREEN);
	myTFT.drawCircle(84, 84, 59, myTFT.RDLC_GREEN);
	for (uint8_t j =0 ; j< revolutions ; j++)
	{
		for (int i = 0; i <= 361; i++)
		{
			//drawLineAngle(x, y, angle, start,length, offset,  color);
			myTFT.drawLineAngle(84, 84, i-1, 0 , 58, -90, myTFT.RDLC_BLACK);//erase last needle
			myTFT.drawLineAngle(84, 84, i, 0, 58, -90, myTFT.RDLC_GREEN);//draw needle
			delayMilliSecRDL(15);
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
	myTFT.writeCharString(25, 90, teststr1);
	DisplayReset();

	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
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

// *************** EOF ****************

/// @endcond
