/*!
	@file examples/color16_displays/ili9341/Bitmap_Tests/main.cpp
	@brief Library test file, bitmaps + FPS test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@details The FPS test is setup for a 128by128 pixel bitmap
		Five files 128by128 pixels are loaded and displayed as many times as possible in 
		10 seconds to calculate FPS. 
		811 frames, 10 sec, 81.0156 fps at 8Mhz SPI Baud rate, HW SPI
	@test
	-# Test 301 "clock demo" , icons, small bitmap
	-# Test 302 bi-color image 128x128
	-# Test 303A 24 bit 240by280 color image bitmap from the file system
	-# Test 303B 24 bit 128by128 color image bitmaps from the file system
	-# Test 304 16 bit 128by128 color image bitmaps from the file system
	-# Test 601 Frame rate per second (FPS) test. 24 bit bitmaps.
*/

// Section ::  libraries
#include <bcm2835.h> // for SPI GPIO and delays.
#include <ctime> // for test301
#include "Bi_Color_Bitmap_Data.hpp" // Data for test 301-302
#include "ILI9341_TFT_LCD_RDL.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 300

//size in pixels of bitmap used in FPS test
#define myBMPWidth 128
#define myBMPHeight 128
uint8_t numberOfFilesFPS = 5;

// Display size in pixels
#define myTFTWidth 240
#define myTFTHeight 320

// Section :: Globals
ILI9341_TFT myTFT;

//  Section ::  Function Headers
uint8_t Setup(void); // setup + user options

void Test301(void); // "clock demo" icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303A(void); // 24 color bitmap
void Test303B(void); // 24 color bitmap
void Test304(void); // 16 color bitmap
std::string UTC_string(void); // for clock demo
void TestFPS(void); // Frames per second 24 color bitmap test,
int64_t getTime(); // Utility for FPS test
uint8_t* loadImage(char* name); // Utility for FPS test

void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{

	if(Setup() != 0)return -1;
	myTFT.fillScreen(RDLC_BLACK);
	Test301();
	Test302();
	Test303A();
	Test303B();
	Test304();
	TestFPS();
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
	}

// ** USER OPTION 1 GPIO/SPI TYPE HW SPI **
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	uint16_t TFT_WIDTH = myTFTWidth;// Screen width in pixels
	uint16_t TFT_HEIGHT = myTFTHeight; // Screen height in pixels
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ =  8000000 ; //  freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
	if(myTFT.InitSPI(SCLK_FREQ, SPI_CE_PIN) != rpiDisplay_Success)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************
	std::cout << "ILI9341 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	delayMilliSecRDL(50);
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
	myTFT.drawIcon(15, 20, 16, RDLC_BLACK, RDLC_WHITE, pSignalIcon);
	myTFT.drawIcon(40, 20, 16, RDLC_BLACK, RDLC_WHITE, pMsgIcon);
	myTFT.drawIcon(60, 20, 8, RDLC_BLACK, RDLC_WHITE, pAlarmIcon);
	myTFT.drawIcon(210, 20, 16, RDLC_BLACK, RDLC_WHITE, pBatIcon);

	myTFT.drawIcon(5, 45, 12, RDLC_GREEN , RDLC_BLACK, pPowerIcon);
	myTFT.drawIcon(20, 45, 12, RDLC_RED, RDLC_YELLOW, pSpeedIcon);
	myTFT.setFont(font_arialBold);
	myTFT.setCursor(60,45);
	myTFT.print(GetRDLibVersionNum());
	if (myTFT.drawBitmap(180, 45, 40 , 16, RDLC_CYAN , RDLC_BLACK, pSunText) != rpiDisplay_Success)
	{
		std::cout << "Warning an Error occurred in drawBitmap." << std::endl;
		return ;
	}
	// Lines
	myTFT.drawFastHLine(0,15,240,RDLC_WHITE);
	myTFT.drawFastHLine(0,40,240,RDLC_WHITE);
	myTFT.drawFastHLine(0,90,240,RDLC_WHITE);
	myTFT.drawFastHLine(0,135,240,RDLC_WHITE);
	myTFT.drawFastHLine(0,180,240,RDLC_WHITE);
	myTFT.drawFastHLine(0,230,240,RDLC_WHITE);
	// draw clock
	while(count > 99)
	{
		std::string TimeString = UTC_string();
		std::cout<< TimeString << "\r" << std::flush;
		auto DateInfo = TimeString.substr(0, 10);
		auto TimeInfo = TimeString.substr(11,8);
		
		myTFT.setFont(font_groTesk);
		//print time
		myTFT.setTextColor(RDLC_RED, RDLC_BLACK);
		myTFT.setCursor(20,100);
		myTFT.print(TimeInfo);
		myTFT.setFont(font_groTesk);
		//print date
		myTFT.setTextColor(RDLC_GREEN, RDLC_BLACK);
		myTFT.setCursor(20,140);
		myTFT.print(DateInfo);
		
		myTFT.setFont(font_retro);
		// print count
		myTFT.setTextColor(RDLC_YELLOW, RDLC_BLACK);
		myTFT.setCursor(160,210);
		myTFT.print(count);
		count--;
		//print name
		myTFT.setCursor(10,210);
		myTFT.setTextColor(RDLC_CYAN, RDLC_BLACK);
		myTFT.print(teststr1);
	}
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(RDLC_BLACK);
	
}

// bitmap 2 colour , 128x128 bi color 1-bit bitmap
void Test302(void)
{
	std::cout << "Test 302: Bi-color bitmap 128X128 pixels" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 2";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	if(myTFT.drawBitmap(40, 40, 128 , 128, RDLC_WHITE , RDLC_GREEN, pBackupMenuBitmap) != rpiDisplay_Success)
		return;
	if(myTFT.drawBitmap(40, 180, 128 , 128, RDLC_RED , RDLC_YELLOW, pBackupMenuBitmap) != rpiDisplay_Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);
}

// bitmap 24 colour , File format = Windows BITMAPINFOHEADER offset 54
// bitmap is 240 by 280
void Test303A(void)
{
	std::cout << "Test 303-A: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

	FILE *pFile ;
	size_t pixelSize = 3; // 24 bit 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t* bmpBuffer = nullptr;
	uint16_t widthBitmap = 220;
	uint16_t heightBitmap = 280;
	bmpBuffer = (uint8_t*)malloc(( widthBitmap * heightBitmap) * pixelSize);
	
	if (bmpBuffer == nullptr)
	{
		std::cout << "Error Test 303-A : MALLOC could not assign memory " << std::endl;
		return;
	}
	
	pFile = fopen("bitmap/bitmap24images/24pic7_220X240.bmp", "r"); 
	if (pFile == nullptr)  // Check file exists
	{
		std::cout << "Error Test 303-A: File does not exist" << std::endl;
		free(bmpBuffer);
		return;
	}
	fseek(pFile, FileHeaderOffset, 0); // Put file in Buffer
	fread(bmpBuffer, pixelSize, widthBitmap *heightBitmap, pFile);
	fclose(pFile);

	if(myTFT.drawBitmap24(10, 10, bmpBuffer,widthBitmap ,heightBitmap) != rpiDisplay_Success)
	{// Check for success 0x00
		std::cout << "Warning an Error occurred in drawBitmap24" << std::endl;
		free(bmpBuffer);
		return;
	}

	delayMilliSecRDL(TEST_DELAY5);
	free(bmpBuffer);  // Free Up Buffer
}

// bitmap 24 colour , All files format = Windows BITMAPINFOHEADER offset 54
// All bitmaps are 128 by 128
void Test303B(void)
{
	std::cout << "Test 303-B: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeCharString(5, 5, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

	FILE *pFile ;
	size_t pixelSize = 3; // 24 bit 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t NumberOfFiles  = 5;
	uint8_t* bmpBuffer = nullptr;
	uint16_t widthBitmap = 128;
	uint16_t heightBitmap =128;
	bmpBuffer = (uint8_t*)malloc((widthBitmap * heightBitmap) * pixelSize);
	if (bmpBuffer == nullptr)
	{
		std::cout << "Error Test 303-B : MALLOC could not assign memory " << std::endl;
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
			free(bmpBuffer);
			return;
		}

		fseek(pFile, FileHeaderOffset, 0); // Put file in Buffer
		fread(bmpBuffer, pixelSize,widthBitmap * heightBitmap, pFile);
		fclose(pFile);

		if(myTFT.drawBitmap24(20, 29, bmpBuffer,widthBitmap, heightBitmap) != rpiDisplay_Success)
		{// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap24" << std::endl;
			free(bmpBuffer);
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
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
// All bitmaps are 128 by 128.
void Test304(void)
{
	std::cout << "Test 304: 16 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
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
	uint8_t* bmpBuffer1 = nullptr;
	bmpBuffer1 = (uint8_t*)malloc((bitmapWidth * bitmapHeight) * pixelSize);

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
		fread(bmpBuffer1, pixelSize, bitmapWidth * bitmapHeight, pFile);
		fclose(pFile);

		if (myTFT.drawBitmap16(40, 40, bmpBuffer1, bitmapWidth, bitmapHeight) != rpiDisplay_Success)
		{
		// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			free(bmpBuffer1);
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	free(bmpBuffer1); // Free Up Buffer
	myTFT.fillScreen(RDLC_BLACK);
} // end of test 


//Return UTC time as a std:.string with format "yyyy-mm-dd hh:mm:ss".
// used in  Clock demo
std::string UTC_string() 
{
	std::time_t time = std::time({});
	char timeString[std::size("yyyy-mm-dd hh:mm:ss UTC")];
	std::strftime(std::data(timeString), std::size(timeString), "%F %T UTC", std::gmtime(&time));
	return timeString;
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
	for (uint8_t i=0; i< numberOfFilesFPS ;i++) // Did any loadImage call return nullptr
	{
		if (img[i] == nullptr){ 
			for(uint8_t j=0; j< numberOfFilesFPS; j++) free(img[j]); // Free Up Buffer if set
			delayMilliSecRDL(TEST_DELAY1);
			return;
		}
	}
	int64_t start = getTime(), duration = 0;
	uint32_t frames = 0;
	double fps = 0;

	// Run for ~10sec
	while(duration < 10000000) {
		myTFT.drawBitmap24(25, 50, img[frames % numberOfFilesFPS], myBMPWidth, myBMPHeight);

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
	for(int i=0; i< numberOfFilesFPS; i++) free(img[i]);
}

/*!
 *@brief used in FPS 24 color bitmap test, 
*/
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

/*!
 *@brief used in FPS 24 color bitmap test, 
*/
int64_t getTime() {
	struct timespec tms;
	if (clock_gettime(CLOCK_REALTIME,&tms)) return -1;
	int64_t micros = tms.tv_sec * 1000000;
	micros += tms.tv_nsec/1000;
	if (tms.tv_nsec % 1000 >= 500) ++micros;
	return micros;
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_mega);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(15, 50, teststr1);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.PowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}


// *************** EOF ****************
