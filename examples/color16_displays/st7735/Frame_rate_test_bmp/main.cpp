/*! 
	@file examples/color16_displays/st7735/Frame_rate_test_bmp/main.cpp
	@brief Frame rate test. FPS Bitmaps, frame rate per second
	@note See USER OPTIONS 1-3 in SETUP function
	@details This test is setup for a 128by128 screen
		Five files 128by128 pixels are loaded and displayed as many times as possible in 
		10 seconds to calculate FPS. 
		811 frames, 10 sec, 81.0156 fps at 8Mhz SPI Baud rate, HW SPI
	@test 
		-# Test 601 Frame rate per second (FPS) test. 24 bit bitmaps.
*/

// Section ::  libraries 
#include <bcm2835.h> // for SPI GPIO and delays.
#include <ctime> // for test FPS
#include "ST7735_TFT_LCD_RDL.hpp"

// Section :: Defines   
//  Test related defines 
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
uint8_t numberOfFiles = 5;
//size in pixels of bitmap 
#define myBMPWidth 128
#define myBMPHeight 128

// Section :: Globals 
ST7735_TFT myTFT;

//  Section ::  Function Headers 

int8_t Setup(void); // setup + user options
void TestFPS(void); // Frames per second 24 color bitmap test,
void EndTests(void);

int64_t getTime(); // Utility for FPS test
uint8_t* loadImage(char* name); // Utility for FPS test

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
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}else{
		std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	}
	
// ** USER OPTION 1 GPIO/SPI TYPE HW OR SW **
	int8_t RST_TFT  =  25;
	int8_t DC_TFT   =  24;
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
	uint32_t SCLK_FREQ =  8000000 ; // HW Spi freq in Hertz , MAX 125 Mhz MIN 30Khz
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

/*!
 *@brief Frames per second test , 24 color bitmap test, 
*/
void TestFPS(void) {

	myTFT.fillScreen(RDLC_RED);
	
	// Load images into buffers
	uint8_t* img[5] = { 
		loadImage((char*)"bitmap/bitmap24images/24pic1.bmp"),
		loadImage((char*)"bitmap/bitmap24images/24pic2.bmp"),
		loadImage((char*)"bitmap/bitmap24images/24pic3.bmp"),
		loadImage((char*)"bitmap/bitmap24images/24pic4.bmp"),
		loadImage((char*)"bitmap/bitmap24images/24pic5.bmp")
	};
	for (uint8_t i=0; i< numberOfFiles ;i++) // Did any loadImage call return nullptr
	{
		if (img[i] == nullptr){ 
			for(uint8_t j=0; j< numberOfFiles; j++) free(img[j]); // Free Up Buffer if set
			TFT_MILLISEC_DELAY(TEST_DELAY1);
			return;
		}
	}
	int64_t start = getTime(), duration = 0;
	uint32_t frames = 0;
	double fps = 0;

	// Run for ~10sec
	while(duration < 10000000) {
		myTFT.drawBitmap24(0, 0, img[frames % numberOfFiles], myBMPWidth, myBMPHeight);

		duration = getTime() - start;

		if((++frames % 50) == 0) {
			fps = (double)frames / ((double)duration / 1000000);
			std::cout << fps << std::endl;
		}
	}

	// Get final Stats and print
	duration = getTime() - start;
	fps = (double)frames / ((double)duration / 1000000);
	std::cout << frames << " frames, " << duration / 1000000 << " sec, " << fps << " fps" << std::endl;

	// Free Up Buffers
	for(int i=0; i< numberOfFiles; i++) free(img[i]);
}


void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_retro);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
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

uint8_t* loadImage(char* name) {
	FILE *pFile ;
	size_t pixelSize = 3; // 24 bit 3 bytes per pixel
	uint8_t* bmpBuffer1 = nullptr;
	uint8_t FileHeaderOffset = 54;
	pFile = fopen(name, "r");
	if (pFile == nullptr) {
		std::cout << "Error TestFPS : File does not exist" << std::endl;
		return nullptr;
	} else {
		bmpBuffer1 = (uint8_t*)malloc((myBMPWidth * myBMPHeight) * pixelSize);
		if (bmpBuffer1 == nullptr)
		{
			std::cout << "Error TestFPS : MALLOC could not assign memory " << std::endl;
			return nullptr;
		}
		fseek(pFile,  FileHeaderOffset, 0);
		fread(bmpBuffer1, pixelSize, myBMPWidth * myBMPHeight, pFile);
		fclose(pFile);
	}
	
	return bmpBuffer1;
}

// *************** EOF ****************
