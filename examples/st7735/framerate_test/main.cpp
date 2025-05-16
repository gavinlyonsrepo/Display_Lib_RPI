/*! 
	@file examples/st7735/framerate_test/main.cpp
	@brief Frame rate test. FPS bitmap, text + graphics , 128x128 ST7735R Red Tab.
	@note See USER OPTIONS 1-3 in SETUP function
	@details This test (601) is setup for a 128by128 bitmaps 
		Five files 128by128 pixels are loaded and displayed as many times as possible in 
		10 seconds to calculate FPS. 
	@test 
		-# Test 601 Frame rate per second (FPS) test. 24 bit bitmaps.
		-# Test 603 Frame rate per second (FPS) test text
		-# Test 604 Frame rate per second (FPS) test graphics
*/

// Section ::  libraries 
#include <iostream> // cout
#include <ctime> // for test FPS
#include "ST7735_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Defines   
//  Test timing related defines 
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
// bitmap
uint8_t numberOfFiles = 5;
#define runtimeBmp  10000000 //uS
#define myBMPWidth 128
#define myBMPHeight 128
// text + graphics
#define runtime  20000000 //uS

// Section :: Globals 

ST7735_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEV = 0; // GPIO chip device number usually 0

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 128;// Screen width in pixels
uint16_t TFT_HEIGHT = 128; // Screen height in pixels

int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

// =============== Function prototype ================
uint8_t Setup(void);
void EndTests(void);
//Test 601 bitmaps
void TestBMP_FPS(int64_t runtime_us);
int64_t getTime(); // Utility for FPS test
std::vector<uint8_t>   loadImage(const char*); // Utility for FPS test

// Test 603 604 text and graphics
void TestFPS(int64_t runtime_us, uint8_t testNumber);
void displayText(double fps, uint32_t frames);
void displayGraphics(void);

// ======================= Main ===================
int main()
{
	if(Setup() != 0)return -1;
	std::cout << "=============" << std::endl;
	std::cout << "Test 603 FPS test: text" << std::endl;
	TestFPS(runtime, 1 );
	std::cout << "=============" << std::endl;
	std::cout << "Test 604 FPS test: graphics" << std::endl;
	TestFPS(runtime, 2 );
	std::cout << "=============" << std::endl;
	TestBMP_FPS(runtimeBmp);
	std::cout << "=============" << std::endl;
	EndTests();
	std::cout << "=============" << std::endl;
	return 0;
}
// ======================= End of main  ===================

//  Section ::  Function Space 
uint8_t Setup(void)
{
	std::cout << "TFT Start" << std::endl;
	std::cout << "Display_Lib_RPI library version :" << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// **********************************
// ** USER OPTION 3 PCB_TYPE + SPI settings**
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SPI_DEV, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	myTFT.setFont(font_pico);
	return 0;
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_retro);
	myTFT.writeCharString(5, 55, teststr1);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

#include <iostream>
#include <cstdint>

/**
 * @brief Measures the FPS (frames per second) of a display test.
 * Runs for the specified duration, rendering frames and calculating FPS.
 * @param runtime_us Duration of the test in microseconds (e.g., 20000000 for 20 seconds).
 * @param testNumber , which test to run 1 for text 2 for graphics
 */
void TestFPS(int64_t runtimeUs, uint8_t testNumber = 1) {
	// Initialize display settings
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setFont(font_pico);
	std::cout << (runtimeUs / 1000000) << " seconds" << std::endl;
	// Initialize timing and FPS calculation variables
	int64_t start = getTime();  // Get start time in microseconds
	int64_t duration = 0;       // Track elapsed time
	uint32_t frames = 0;        // Count number of frames
	double fps = 0;             // Store calculated FPS

	// Run the test for the specified duration
	while (duration < runtimeUs) {
		// Call display function to test
		switch (testNumber)
		{
			case 1 : displayText(fps, frames); break;
			case 2 : displayGraphics(); break;
		}
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
}


// Frames per second test , 24 color bitmap test, 
void TestBMP_FPS(int64_t runtime_us) {
	myTFT.fillScreen(myTFT.RDLC_RED);
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	std::cout << "Test 601 FPS test: bitmaps ::" << (runtime_us / 1000000) << " seconds" << std::endl;
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
		myTFT.drawBitmap24(0, 0, img[frames % 5], myBMPWidth, myBMPHeight);
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


void  displayText(double fps, uint32_t frames)
{
	myTFT.setCursor(5, 5);
	myTFT.println("Frames:");
	myTFT.println(frames);
	myTFT.println("FPS:");
	myTFT.println(fps);
	myTFT.println("SPI speed:");
	myTFT.println(SPI_SPEED);
	myTFT.println("Testing!");
	myTFT.println(rdlib::LibraryVersion());
}

void  displayGraphics(void)
{
	myTFT.drawFastVLine(63, 0, 127, myTFT.RDLC_BLUE);
	myTFT.drawFastHLine(0, 63, 127, myTFT.RDLC_BLUE);
	myTFT.fillQuadrilateral(
		20, 20,  // P0: top-left
		60, 20,  // P1: top-right
		50, 40,  // P2: bottom-right (offset x by -10)
		10, 40,  // P3: bottom-left  (offset x by -10)
	myTFT.RDLC_CYAN
	);
	myTFT.fillRect(20, 70, 20, 20, myTFT.RDLC_RED);
	myTFT.fillCircle(100, 30, 10, myTFT.RDLC_GREEN);
	myTFT.fillTriangle(70,90, 90, 70 , 110, 90, myTFT.RDLC_YELLOW);
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
/// @endcond
