/*!
	@file    examples/gc9107/tests/main.cpp
	@brief   Example cpp file for GC9107 display Library test file, tests :
			 Bitmaps, text,graphics & functions.
	@details Uses advanced screen buffer mode. Hardware SPI
			 Setup for 60x160 display
			 See USER OPTIONS in SETUP function
	@test
		-# Test 300 Sprites demo, small bitmap
		-# Test 301 icons, small bitmap
		-# Test 302 bi-color image
		-# Test 304 16 bit color image bitmap from the file system
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
#include "GC9107_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
std::atomic<bool> stopRequested{false};

GC9107_TFT myTFT;

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
void Test304(void);
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
	myTFT.TFTsetRotation(myTFT.Degrees_0);
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
				myTFT.TFTsetRotation(myTFT.Degrees_90);
				Test300();
				Test301();
				Test302();
				Test304();
				myTFT.TFTsetRotation(myTFT.Degrees_0);
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
	std::cout << "Start Test HWSPI" << std::endl;

	int8_t RST_TFT = 25;
	int8_t DC_TFT = 24;
	int GPIO_CHIP_DEVICE = 0;  // GPIO chip device number usually 0
	int HWSPI_DEVICE = 0;	   // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0;	   // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED = 8000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0;	   // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	// Size of Display (note 60x160 display has VRAM of 128x160)
	uint16_t TFT_WIDTH = 60;   // width  size in pixels
	uint16_t TFT_HEIGHT = 160; // height size in pixels
	uint8_t X_Offset = 34;     // X offset
	uint8_t Y_Offset = 0;      // Y offset
	// GRAM memory base mapping resolution, see GC9107_LTSM::GM_memory_base_e enum
	GC9107_TFT::GM_memory_base_e TFTmemoryBase = GC9107_TFT::GM_memory_base_e::MEMORY_BASE_GM_128x160;

	// ** USER OPTION 0 GPIO **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);

	// *** USER OPTION 1 Screen Setup ***
	myTFT.TFTInitScreen(TFT_WIDTH, TFT_HEIGHT, TFTmemoryBase, GC9107_TFT::MADCTL_FLAGS_t::RGB);

	// *** USER OPTION 2 Screen offsets***
	myTFT.TFTsetPanelOffset(X_Offset, Y_Offset);

	// ** USER OPTION 3 SPI settings **
	if (myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}

	delayMilliSecRDL(100);
	return SetupBufferMode();;
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
	myTFT.drawSprite(0, 0, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	DisplayReset();
	myTFT.drawSprite(0,0, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE, true);
	DisplayReset();
	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color =myTFT.RDLC_LBLUE
	// Bitmap background screen
	if(myTFT.drawBitmap(60, 0, 64 , 64, myTFT.RDLC_WHITE , myTFT.RDLC_RED, bigImage64x64 ) != rdlib::Success)
		return;
	myTFT.drawSprite(60, 0, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	myTFT.drawSprite(90, 0, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	DisplayReset();
}

void Test301(void)
{
	if (stopRequested) return; 
	std::cout << "Test 301: icons, small bitmap" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.drawIcon(42, 0, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, SignalIconVa);
	myTFT.drawIcon(62, 0, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, MsgIconVa);
	myTFT.drawIcon(80, 0, 8, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, AlarmIconVa);
	myTFT.drawIcon(100, 0, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, BatIconVa);
	myTFT.drawIcon(25, 0, 12, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK, PowerIcon);
	myTFT.drawIcon(0, 0, 12, myTFT.RDLC_RED, myTFT.RDLC_YELLOW, SpeedIcon);
	DisplayReset();

	if (myTFT.drawBitmap(0, 0, 40 , 16, myTFT.RDLC_RED , myTFT.RDLC_YELLOW, SunText) != rdlib::Success)
	{
		std::cout << "Warning an Error occurred in drawBitmap." << std::endl;
		return ;
	}
	DisplayReset();
}

// bitmap 2 colour , 128x128 bi color bitmap
void Test302(void)
{
	std::cout << "Test 302: Bi-color bitmap 64x64 pixels" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	if(myTFT.drawBitmap(0, 0, 64, 64, myTFT.RDLC_WHITE , myTFT.RDLC_GREEN,  bigImage64x64) != rdlib::Success)
		return;
	DisplayReset();
}

// test function or 16 bit color bitmaps made in GIMP (RGB 565 16 bit color)
// 1 files BITMAPV5HEADERoffset 138(124+14) Made in The GIMP
// Note 96 by 64 bitmap size,  4 bits will be cropped (screen is 60 height)
void Test304(void)
{
	if (stopRequested) return; 
	std::cout << "Test 304: 16 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	FILE *pFile ;
	size_t pixelSize = 2; // 16 bit 2 bytes per pixel
	uint8_t offsetBMPHeader = 138;
	uint8_t bitmapWidth = 96;
	uint8_t bitmapHeight = 64;

	// Use std::vector for safe memory management of buffer
	std::vector<uint8_t> bmpBuffer_v;
	try 
	{
		// Attempt to allocate memory for the vector
		bmpBuffer_v.resize((bitmapWidth * bitmapHeight) * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error Test 303: Memory allocation failed for bmpBuffer1" << std::endl;
		return;
	}
	// Open the file and check it
	pFile = fopen("bitmap/bitmap16images/16pic4_96x64.bmp", "r");
	if (pFile == nullptr)
	{
		std::cout << "Error Test 404 : File does not exist" << std::endl;
		return;
	}
	// Aquire the bitmap data from file and place it in vector
	fseek(pFile, offsetBMPHeader, 0);
	fread(bmpBuffer_v.data(), pixelSize, bitmapWidth * bitmapHeight, pFile);
	fclose(pFile);
	// Draw the bitmap & Check for success 0x00
	if (myTFT.drawBitmap16(0, 0, bmpBuffer_v, bitmapWidth, bitmapHeight) != rdlib::Success)
	{
		std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
		return;
	}
	DisplayReset();


	myTFT.fillScreen(myTFT.RDLC_BLACK);
} // end of test 


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
	myTFT.fillRoundRect(0, 100, 12, 60, 4, myTFT.RDLC_RED);
	myTFT.fillRoundRect(12, 100, 12, 60, 4, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(24, 100, 12, 60, 4, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(36, 100, 12, 60, 4, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(48, 100, 12, 60, 4, myTFT.RDLC_WHITE);
}

void Test501(void)
{
	if (stopRequested) return; 
	std::cout << "Test 501: Scroll" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setFont(font_retro);
	char teststr0[] = "Scroll Test";
	myTFT.writeCharString(0, 100, teststr0);
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
	char teststr0[] = "R 0"; //normal
	char teststr1[] = "R 90"; // 90
	char teststr2[] = "R 180"; // 180
	char teststr3[] = "R 270"; // 270

	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.writeCharString(0, 0, teststr0);
	myTFT.writeCharString(0, 140, teststr0);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.writeCharString(0, 0, teststr1);
	myTFT.writeCharString(0, 40, teststr1);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_180);
	myTFT.writeCharString(0, 0, teststr2);
	myTFT.writeCharString(0, 140, teststr2);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_270);
	myTFT.writeCharString(0, 0, teststr3);
	myTFT.writeCharString(0, 40, teststr3);
	DisplayReset();

	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
}

void Test503()
{
	if (stopRequested) return; 
	std::cout << "Test 503: Mode Tests" << std::endl;
	char teststr1[] = "Invert";
	char teststr2[] = "Display";
	char teststr3[] = "Sleep  ";
	ColorBlock();
	myTFT.writeCharString(0, 32, teststr1);
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
	myTFT.writeCharString(0, 32, teststr2);
	myTFT.writeBuffer();
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTenableDisplay(true);
	std::cout << "Test 503-4: Turn Display back on" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	// modes
	myTFT.writeCharString(0, 32, teststr3);
	myTFT.writeBuffer();
	delayMilliSecRDL(TEST_DELAY5); 
	// 1. sleep mode
	myTFT.TFTsetPowerMode(GC9107_TFT::PowerState_e::SleepNormalIdleOff);
	std::cout << "Test 503-7: Power mode Sleep ON Idle Off" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	delayMilliSecRDL(TEST_DELAY5);
	//back to sleep off - normal on - idle off
	myTFT.TFTsetPowerMode(GC9107_TFT::PowerState_e::NormalIdleOff);	
	delayMilliSecRDL(TEST_DELAY5);
}

// Text
void Test701(void) {
	if (stopRequested) return; 
	std::cout << "Test 701: Print out some fonts with writeCharString" << std::endl;
	char teststr1[] = "Default";
	char teststr2[] = "GLL";
	char teststr3[] = "Pico";
	char teststr4[] = "Sinclair";
	char teststr5[] = "Orla";
	char teststr6[] = "Retro";
	char teststr7[] = "Mega";

	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	int startY = 0;
	int spacing = 20;
	int x = 0;
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

	int x = 0;
	int y = 0;
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
	y = 0;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_mega);
	myTFT.print("meg");
	DisplayReset();
	// --- Third Block ---
	y = 0;
	myTFT.setFont(font_orla);
	myTFT.setCursor(x, y);
	myTFT.print("OA");
	y += spacing;
	myTFT.setCursor(x, y + spacing);
	myTFT.setFont(font_arialRound);
	myTFT.print(-8);
	DisplayReset();
	// --- Final Block ---
	y = 0;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_groTesk);
	myTFT.println("GRO");
	DisplayReset();
}

void Test703(void)
{
	if (stopRequested) return; 
	std::cout << "Test 703: Misc print class(string object, println invert, wrap, etc)" << std::endl;
	// --- Inverted print fonts 1–6 ---
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(0, 20);
	myTFT.print(-49);
	DisplayReset();
	// --- Inverted print ---
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_RED);
	myTFT.setFont(font_default);
	myTFT.setCursor(0, 40);
	myTFT.print("INV");
	DisplayReset();
	// --- Test std::string print and wrapping ---
	myTFT.setFont(font_default);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(0, 20);
	myTFT.print(timeInfo);
	myTFT.setCursor(0, 40);
	myTFT.println(newLine); // newline
	myTFT.print(newLine);
	myTFT.setCursor(0, 80);
	myTFT.print("1234567890"); // wrap long text

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
	returnValue = myTFT.fillRectangle(5, 30, 20, 20, myTFT.RDLC_YELLOW);
	if (returnValue != rdlib::Return_Codes_e::Success)
	{
		std::cout << "Warning : Test TFTfillRectangle, An error occurred returnValue " +returnValue << std::endl;
	}
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.drawRectWH(5, 5, 10, 10, myTFT.RDLC_RED);
	myTFT.fillRect(5, 55, 20, 20, myTFT.RDLC_GREEN);
	myTFT.drawRoundRect(5, 80, 50, 50, 8, myTFT.RDLC_CYAN);
	myTFT.fillRoundRect(5, 135, 40, 20, 6, myTFT.RDLC_WHITE);

	DisplayReset();

	std::cout << "Test 903 & 904 : Triangles and circles" << std::endl;
	myTFT.drawCircle(29, 55, 15, myTFT.RDLC_GREEN);
	myTFT.fillCircle(29, 105, 15, myTFT.RDLC_YELLOW);
	DisplayReset();
	// Triangle 1: pointing down, upper half
	myTFT.drawTriangle(
		 5, 20,  // top-left
		 55, 20, // top-right
		 29, 70, // bottom-centre
		myTFT.RDLC_CYAN);
	// Triangle 2: pointing up, lower half
	myTFT.fillTriangle(
		 5, 145,	// bottom-left
		 55, 145, // bottom-right
		 29, 90,	// top-centre
		myTFT.RDLC_RED);
	DisplayReset();
}


void Test905(void)
{
	if (stopRequested) return; 
	std::cout << "Test 905 : polygons" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// octagon
	myTFT.drawPolygon(30, 80, 8, 20, 0, false, myTFT.RDLC_GREEN);
	DisplayReset();
	// octagon
	myTFT.drawPolygon(30, 80, 8, 20, 0, true, myTFT.RDLC_GREEN);
	DisplayReset();
}

void Test906(void) {
	if (stopRequested) return; 
	std::cout << "Test 906 : mesh" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(1, 5, 59, 155, 5, myTFT.RDLC_CYAN);
	DisplayReset();
}

void Test907() {
	if (stopRequested) return; 
	std::cout << "Test 907 : quadrilateral" << std::endl;
	// Define the four quadrilateral vertices for different shapes
	// Shape 1: Rectangle (top)
	int16_t x0 =  5, y0 = 5;
	int16_t x1 =  50, y1 = 5;
	int16_t x2 =  50, y2 = 40;
	int16_t x3 =  5, y3 = 40;
	// Shape 2: Parallelogram (upper-middle)
	int16_t x4 =  20, y4 = 50;
	int16_t x5 =  55, y5 = 50;
	int16_t x6 =  40, y6 = 80;
	int16_t x7 =  5, y7 = 80;
	// Shape 3: Trapezoid wide-top (lower-middle)
	int16_t x8 =  5, y8 = 95;
	int16_t x9 =  55, y9 = 95;
	int16_t x10 =  45, y10 = 125;
	int16_t x11 =  15, y11 = 125;
	// Shape 4: Irregular quad (bottom)
	int16_t x12 =  5, y12 = 135;
	int16_t x13 =  50, y13 = 140;
	int16_t x14 =  45, y14 = 158;
	int16_t x15 =  10, y15 = 155;
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
	// === Unfilled pass ===
	myTFT.drawEllipse( 15, 55, 12, 25, false, myTFT.RDLC_WHITE);	  // top-left
	myTFT.drawEllipse( 45, 55, 12, 25, false, myTFT.RDLC_RED);	  // top-right
	myTFT.drawEllipse( 15, 120, 14, 12, false, myTFT.RDLC_BLUE);	  // bottom-left
	myTFT.drawEllipse( 45, 120, 14, 12, false, myTFT.RDLC_YELLOW); // bottom-right
	DisplayReset();
	// === Filled pass - same positions ===
	myTFT.drawEllipse( 15, 55, 12, 25, true, myTFT.RDLC_WHITE);
	myTFT.drawEllipse( 45, 55, 12, 25, true, myTFT.RDLC_RED);
	myTFT.drawEllipse( 15, 120, 14, 12, true, myTFT.RDLC_BLUE);
	myTFT.drawEllipse( 45, 120, 14, 12, true, myTFT.RDLC_YELLOW);
	DisplayReset();
}

void Test909(void) {
	if (stopRequested) return; 
	std::cout << "Test 909 : Drawing Arc: drawArc" << std::endl;
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	int16_t centerX = 40;  // X-coordinate of the circle center
	int16_t centerY = 80;  // Y-coordinate of the circle center
	int16_t radius = 20;    // Radius of the circle
	std::cout << " Angle offset default : " << myTFT.getArcAngleOffset() << std::endl;
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 4, 0.0f, 180.0f, myTFT.RDLC_GREEN);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 6, 180.0f, 360.0f, myTFT.RDLC_BLUE);
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
