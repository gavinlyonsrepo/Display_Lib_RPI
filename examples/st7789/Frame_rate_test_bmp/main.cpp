/*! 
	@file examples/st7789/Frame_rate_test_bmp/main.cpp
	@brief Frame rate test. FPS Bitmaps, frame rate per second
	@note See USER OPTIONS 1-3 in SETUP function
	@details This test is setup for a 128by128 bitmaps 
		Five files 128by128 pixels are loaded and displayed as many times as possible in 
		10 seconds to calculate FPS. 
	@test 
		-# Test 601 Frame rate per second (FPS) test. 24 bit bitmaps.
*/

// Section ::  libraries 
#include <iostream> // cout
#include <ctime> // for test FPS
#include "ST7789_TFT_LCD_RDL.hpp"
 
//  Test related  
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
uint8_t numberOfFiles = 5;
//size in pixels of bitmap 
#define myBMPWidth 128
#define myBMPHeight 128

// Section :: Globals 
ST7789_TFT myTFT;
int8_t RST_TFT  = 25; // Reset GPIO
int8_t DC_TFT   = 24; // DC GPIO
int  GPIO_CHIP_DEV = 0; // GPIO chip device number

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 280; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers 

int8_t Setup(void); // setup + user options
void TestFPS(void); // Frames per second 24 color bitmap test,
void EndTests(void);

int64_t getTime(); // Utility for FPS test
std::unique_ptr<uint8_t[]>  loadImage(const char*); // Utility for FPS test

//  Section ::  MAIN loop

int main(void) 
{
	if(Setup() != 0)return -1;
	TestFPS();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

int8_t Setup(void)
{
	std::cout << "TFT Start" << std::endl;
	std::cout << "ST7789 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI**
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

/*!
 *@brief Frames per second test , 24 color bitmap test, 
*/
void TestFPS(void) {
	myTFT.fillScreen(RDLC_RED);

	// Load images into buffers
	std::unique_ptr<uint8_t[]> img[5] = {
		loadImage("bitmap/bitmap24images/24pic1.bmp"),
		loadImage("bitmap/bitmap24images/24pic2.bmp"),
		loadImage("bitmap/bitmap24images/24pic3.bmp"),
		loadImage("bitmap/bitmap24images/24pic4.bmp"),
		loadImage("bitmap/bitmap24images/24pic5.bmp")
	};

	// Check if any loadImage call failed
	for (size_t i = 0; i < 5; ++i) {
		if (!img[i]) {
			std::cout << "Error: Image " << i + 1 << " failed to load." << std::endl;
			return;
		}
	}

	int64_t start = getTime(), duration = 0;
	uint32_t frames = 0;
	double fps = 0;

	// Run for ~10 seconds
	while (duration < 10000000) {
		// Use .get() to pass the raw pointer
		myTFT.drawBitmap24(25, 50, img[frames % 5].get(), myBMPWidth, myBMPHeight);

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




void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_retro);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

int64_t getTime() {
	struct timespec tms;
	if (clock_gettime(CLOCK_REALTIME,&tms)) return -1;
	int64_t micros = tms.tv_sec * 1000000;
	micros += tms.tv_nsec/1000;
	if (tms.tv_nsec % 1000 >= 500) ++micros;
	return micros;
}

std::unique_ptr<uint8_t[]> loadImage(const char* name) {
	size_t pixelSize = 3; // 24-bit color = 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;

	FILE *pFile = fopen(name, "r");
	if (pFile == nullptr) {
		std::cout << "Error: File does not exist" << std::endl;
		return nullptr;
	}

	std::unique_ptr<uint8_t[]> bmpBuffer;
	try {
		bmpBuffer = std::make_unique<uint8_t[]>(myBMPWidth * myBMPHeight * pixelSize);
	} catch (const std::bad_alloc&) {
		std::cout << "Error: Could not allocate memory" << std::endl;
		fclose(pFile);
		return nullptr;
	}

	fseek(pFile, FileHeaderOffset, SEEK_SET);
	fread(bmpBuffer.get(), pixelSize, myBMPWidth * myBMPHeight, pFile);
	fclose(pFile);

	return bmpBuffer; // Transfer ownership to the caller
}


// *************** EOF ****************
