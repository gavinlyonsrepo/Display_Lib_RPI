/*!
	@file   examples/st7735/advanced_screen_buffer_mode/main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7735 library, 128x128 ST7735R Red Tab. Tests : advanced screen buffer mode,
		various tests.
	@note   See USER OPTIONS 0-3 in SETUP function
			dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
			or it will not compile, see readme for more details
	@test
		-# Advanced screen buffer mode various tests.
*/

// Section ::  libraries 
#include <iostream>
#include <time.h>
#include "Bitmap_test_data.hpp"
#include "ST7735_TFT_LCD_RDL.hpp"

/// @cond
// Section :: Globals
ST7735_TFT myTFT;
int8_t RST_TFT  = 25; // Reset GPIO
int8_t DC_TFT   = 24; // DC GPIO
int  GPIO_CHIP_DEV = 0; // GPIO chip device number

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t MY_TFT_WIDTH = 128;// Screen width in pixels
uint16_t MY_TFT_HEIGHT = 128; // Screen height in pixels

int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

#ifndef color16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: color16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif

//  Section ::  Function Headers
uint8_t Setup(void);
void TestBasic(void);
void TestRotate(void);
void TestBitmap(void);
void TestBitmapFPS(void);
void TestTextFPS(void);
void EndTests(void);
void ScreenReset(void);
static uint64_t counter( void );

//  Section ::  MAIN loop
int main(void)
{
	if (Setup() != 0)
		return -1;
	TestBasic();
	TestRotate();
	TestBitmap();
	TestBitmapFPS();
	TestTextFPS();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space
uint8_t Setup(void)
{
	std::cout << "TFT Start Test " << std::endl;
	std::cout << "Display_Lib_RPI library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , MY_TFT_WIDTH , MY_TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 PCB_TYPE + SPI settings**
	// pass enum to param one ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SPI_DEV, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		return -2;
	}
//*****************************
	if (myTFT.setBuffer() != rdlib::Success)
		return -3;					  // set up buffer
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	delayMilliSecRDL(50);
	return 0;
}


void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown();
	printf("TFT: Tests Over\n");
}

void ScreenReset(void)
{
	myTFT.writeBuffer();
	delayMilliSecRDL(2500);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
}

void TestBasic(void)
{
	printf("Test1\r\n");
	myTFT.clearBuffer(myTFT.RDLC_RED);
	myTFT.writeBuffer();
	delayMilliSecRDL(1000);
	myTFT.clearBuffer(myTFT.RDLC_GREEN);
	myTFT.writeBuffer();
	delayMilliSecRDL(1000);
	myTFT.clearBuffer(myTFT.RDLC_BLUE);
	myTFT.writeBuffer();
	delayMilliSecRDL(1000);

	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.setCursor(5, 5);
	myTFT.setFont(font_default);
	myTFT.print("Buffer Mode     Test");
	myTFT.writeBuffer();
	delayMilliSecRDL(5000);
}

void TestRotate(void)
{
	printf("Rotate\r\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	const char *rotationLabels[] = {"Rotate 0", "Rotate 90", "Rotate 180", "Rotate 270"};
	const decltype(myTFT.Degrees_0) rotations[] = {
		myTFT.Degrees_0,
		myTFT.Degrees_90,
		myTFT.Degrees_180,
		myTFT.Degrees_270};
	for (int i = 0; i < 4; ++i)
	{
		myTFT.TFTsetRotation(rotations[i]);
		myTFT.setCursor(20, 20);
		myTFT.print(rotationLabels[i]);
		myTFT.setCursor(20, 100);
		myTFT.print("Bottom");
		ScreenReset();
	}
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	ScreenReset();
}

void TestBitmap(void)
{
	printf("Sprite\n");
	// Sprite
	for (int col = 0; col < 2; ++col) // 0: no flip, 1: flip
	{
		bool flip = (col == 1);
		int x = col * 32;

		for (int row = 0; row < 3; ++row)
		{
			int y = row * 32;
			myTFT.drawSprite(x, y, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE, flip);
		}
	}
	ScreenReset();

	// Bitmap 1-bit
	printf("Bitmap 1-bit\n");
	myTFT.drawBitmap(0, 0, 128, 128, myTFT.RDLC_WHITE, myTFT.RDLC_GREEN, BackupMenuBitmap );
	ScreenReset();

	// Bitmap 16-bit, no error handling see examples/st7735/bitmap_tests/main.cpp for this.
	printf("Bitmap 16-bit\n");
	FILE *pFile ;
	std::vector<uint8_t> bmpBuffer;
	bmpBuffer.resize((128 * 128) * 2);
	pFile = fopen("bitmap/bitmap16images/16pic1.bmp", "r");
	fseek(pFile, 132, 0); // 132 = bmp offset
	fread(bmpBuffer.data(), 2, 128 * 128, pFile);
	fclose(pFile);
	myTFT.drawBitmap16(0, 0, bmpBuffer, 128, 128);
	ScreenReset();

	// Bitmap 24-bit, no error handling see examples/st7735/bitmap_tests/main.cpp for this.
	printf("Bitmap 24-bit\n");
	bmpBuffer.resize((128 * 128) * 3);
	pFile = fopen("bitmap/bitmap24images/24pic2.bmp", "r");
	fseek(pFile, 54, 0); // 54 = bmp offset
	fread(bmpBuffer.data(), 3, 128 * 128, pFile);
	fclose(pFile);
	myTFT.drawBitmap24(0, 0, bmpBuffer, 128, 128);
	ScreenReset();
}

void TestBitmapFPS(void)
{
	printf("Bitmap FPS\n");
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;

	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;

	char teststr1[] = "Test bitmap FPS, Output Results to console";
	myTFT.writeCharString(5, 5, teststr1);
	delayMilliSecRDL(5000);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_RED);
	FILE *pFile ;
	std::vector<uint8_t> bmpBuffer;
	bmpBuffer.resize((128 * 128) * 2);
	pFile = fopen("bitmap/bitmap16images/16pic1.bmp", "r");
	fseek(pFile, 132, 0); // 132 = bmp offset
	fread(bmpBuffer.data(), 2, 128 * 128, pFile);
	fclose(pFile);
	while (1)
	{
		unsigned long currentMillis =  counter();
		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			if (seconds == 20)
				break;
		}
		currentFramerate++;
		count++;
		//  ** Code to test **
		myTFT.drawBitmap16(0, 0, bmpBuffer, 128, 128);
		myTFT.setCursor(0, 0);
		myTFT.print(fps);
		myTFT.writeBuffer();
		//   **
	} // end of while
	// Report results to usb
	printf("Seconds :: %u \n", seconds);
	printf("Count :: %u \n", count);
	printf("FPS :: %u \n", fps);
	// Print to screen
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.setCursor(0, 0);
	myTFT.print("Seconds : ");
	myTFT.println(seconds);
	myTFT.print("Count : ");
	myTFT.println(count);
	myTFT.print("FPS : ");
	myTFT.print(fps);
	myTFT.writeBuffer();
	delayMilliSecRDL(5000);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void TestTextFPS(void)
{
	printf("Text FPS\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setFont(font_pico);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	while (1)
	{
		unsigned long currentMillis = counter();

		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			if (count >= 1000){
				printf("FPS : %u\n", fps);
				printf("Seconds : %u\n", seconds);
				return; // end if count gets to this
			}
		}
		currentFramerate++;
		count++;
		//  ** Code to test **
		// print seconds
		myTFT.setCursor(5, 5);
		myTFT.println("Secs:");
		myTFT.println(seconds);
		// print fps
		myTFT.println("FPS:");
		myTFT.println(fps);
		// print count
		myTFT.println("Count:");
		myTFT.println(count);
		myTFT.println("Testing!");
		myTFT.println(rdlib::LibraryVersion());
		myTFT.writeBuffer();
		//  ****
	}
}

//This returns milli-seconds as a 64-bit unsigned number, monotonically increasing, 
//probably since system boot.
//The actual resolution looks like microseconds. 
static uint64_t counter( void )
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  (uint64_t)now.tv_sec * 1000U + ((uint64_t)now.tv_nsec/1000000U);
}
/// @endcond
