/*!
	@file examples/ili9341/Bitmap_Tests/main.cpp
	@brief Library test file, bitmaps + FPS test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@details The FPS test is setup for a 128by128 pixel bitmap
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
	-# Test 601 Frame rate per second (FPS) test. 24 bit bitmaps.
	-# Test 802 Error checking bitmap functions, results to console
*/

// Section ::  libraries
#include <iostream> // cout
#include <ctime> // for test301
#include "Bitmap_test_data.hpp" // Data for test 301-302
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


int8_t RST_TFT  = 25; // GPIO reset pin
int8_t DC_TFT   = 24; // GPIO data or command 
int  GPIO_CHIP_DEVICE = 0;//A GPIO chip device, >= 0.

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

// Section :: Globals
ILI9341_TFT myTFT;

//  Section ::  Function Headers
uint8_t Setup(void); // setup + user options

void Test300(void); // Sprite
void Test301(void); // "clock demo" icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303A(void); // 24 color bitmap
void Test303B(void); // 24 color bitmap
void Test304A(void); // 16 color bitmap
void Test304B(void); // 16 color bitmap
std::string UTC_string(void); // for clock demo
void TestFPS(void); // Frames per second 24 color bitmap test,
int64_t getTime(); // Utility for FPS test
std::unique_ptr<uint8_t[]> loadImage(const char* name); // Utility for FPS test
void Test802(void); //error checking

void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{

	if(Setup() != 0)return -1;
	myTFT.fillScreen(RDLC_BLACK);
	Test300();
	Test301();
	Test302();
	Test303A();
	Test303B();
	Test304A();
	Test304B();
	Test802(); 
	TestFPS();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	std::cout << "TFT Start" << std::endl;
	std::cout << "ili9341 ibrary version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.InitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
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
	// Draw as sprite, without background , 32 X 32 .background color = RDLC_LBLUE
	// Green background screen
	myTFT.fillScreen(RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.drawSprite(80, 100, pSpriteTest16, 32, 32, RDLC_LBLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color =RDLC_LBLUE
	// Bitmap background screen
	if(myTFT.drawBitmap(40, 40, 128 , 128, RDLC_WHITE , RDLC_RED, pBackupMenuBitmap) != rpiDisplay_Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);

	myTFT.drawSprite(80, 30, pSpriteTest16, 32, 32, RDLC_LBLUE);
	myTFT.drawSprite(60, 60, pSpriteTest16, 32, 32, RDLC_LBLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
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
	char teststr1[] = "Bitmap bi-color, 128 X 128";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	if(myTFT.drawBitmap(40, 40, 128 , 128, RDLC_WHITE , RDLC_GREEN, pBackupMenuBitmap) != rpiDisplay_Success)
		return;
	if(myTFT.drawBitmap(40, 180, 128 , 128, RDLC_RED , RDLC_YELLOW, pBackupMenuBitmap) != rpiDisplay_Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);
}

// bitmap 24 colour , File format = Windows BITMAPINFOHEADER offset 54
// bitmap is 220 by 240
void Test303A(void)
{
	std::cout << "Test 303-A: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	char teststr1[] = "Bitmap 24,  220 X 240";
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeCharString(10,10, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

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

	if(myTFT.drawBitmap24(10, 20, bmpBuffer.data(),widthBitmap ,heightBitmap) != rpiDisplay_Success)
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
	myTFT.fillScreen(RDLC_BLACK);
	char teststr1[] = "Bitmap 24, 128 X 128";
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeCharString(5, 5, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

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

		if(myTFT.drawBitmap24(20, 29, bmpBuffer.data(),widthBitmap, heightBitmap) != rpiDisplay_Success)
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
	myTFT.fillScreen(RDLC_BLACK);
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

		if (myTFT.drawBitmap16(40, 40, bmpBuffer.data(), bitmapWidth, bitmapHeight) != rpiDisplay_Success)
		{
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	myTFT.fillScreen(RDLC_BLACK);
} // end of test 


// test function for 16 bit color bitmap made in GIMP (RGB 565 16 bit color)
// File 1 fourbyteburger240X320.bmp BITMAPV5HEADER offset 132 , 240x320 pixels
// Color space information written
void Test304B(void)
{
	std::cout << "Test 304-B: 16 bit color image bitmap from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
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

		if (myTFT.drawBitmap16(0, 0, bmpBuffer.data(), bitmapWidth, bitmapHeight) != rpiDisplay_Success)
		{
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

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


void Test802(void)
{
	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		rpiDisplay_Success,
		rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapNullptr, //icon
		rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapNullptr, //sprite
		rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapNullptr, rpiDisplay_BitmapHorizontalSize, //1-bit bitmap
		rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapNullptr, //16-bit bitmap
		rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapScreenBounds, rpiDisplay_BitmapNullptr  //24-bit bitmap
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test variables
	char testString5[] = "Error Check Test 802, results to console";
	bool errorFlag = false;

	// === Tests===
	printf("=== START Error checking. Expecting errors ===\r\n");
	// Perform function calls and store return values

	// Print message + sanity check for success
	myTFT.setFont(font_default);
	returnValues.push_back(myTFT.writeCharString(5, 55, testString5)); 
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.fillScreen(RDLC_BLACK);
	//TFTdrawIcon
	returnValues.push_back(myTFT.drawIcon(350, 40, 16, RDLC_BLACK, RDLC_WHITE, pSignalIcon));
	returnValues.push_back(myTFT.drawIcon(180, 350, 16, RDLC_BLACK, RDLC_WHITE, pSignalIcon));
	returnValues.push_back(myTFT.drawIcon(40, 40, 16, RDLC_BLACK, RDLC_WHITE, nullptr));
	//TFTdrawSprite
	returnValues.push_back(myTFT.drawSprite(350, 50,pSpriteTest16, 32, 32, RDLC_LBLUE));
	returnValues.push_back(myTFT.drawSprite(40, 350,pSpriteTest16, 32, 32, RDLC_LBLUE));
	returnValues.push_back(myTFT.drawSprite(40, 180, nullptr, 32, 32, RDLC_LBLUE));
	//TFTdrawBitmap
	returnValues.push_back(myTFT.drawBitmap(350, 65, 128, 128, RDLC_WHITE, RDLC_GREEN, BackupMenuBitmap));
	returnValues.push_back(myTFT.drawBitmap(50, 350, 128, 128, RDLC_WHITE, RDLC_GREEN, BackupMenuBitmap));
	returnValues.push_back(myTFT.drawBitmap(50, 65, 128, 128, RDLC_WHITE, RDLC_GREEN, nullptr));
	returnValues.push_back(myTFT.drawBitmap(20, 20, 70, 128, RDLC_WHITE, RDLC_GREEN, BackupMenuBitmap));
	//TFTdrawBitmap16 , We use const cast for testing only
	returnValues.push_back(myTFT.drawBitmap16(350, 50, const_cast<uint8_t*>(pSpriteTest16), 32, 32));
	returnValues.push_back(myTFT.drawBitmap16(40, 350, const_cast<uint8_t*>(pSpriteTest16), 32, 32));
	returnValues.push_back(myTFT.drawBitmap16(40, 180, nullptr, 32, 32));
	//TFTdrawBitmap24 , We use const cast for testing only
	returnValues.push_back(myTFT.drawBitmap24(340, 50, const_cast<uint8_t*>(pSpriteTest16), 32, 32));
	returnValues.push_back(myTFT.drawBitmap24(40, 330, const_cast<uint8_t*>(pSpriteTest16), 32, 32));
	returnValues.push_back(myTFT.drawBitmap24(40, 180, nullptr, 32, 32));
	
	//== SUMMARY SECTION===
	printf("\nError Checking Summary.\n");
	// Check return values against expected errors
	for (size_t i = 0; i < returnValues.size(); ++i) {
		if (i >= expectedErrors.size() || returnValues[i] != expectedErrors[i]) {
			errorFlag = true;
			printf("Unexpected error code: %d at test case %zu (expected: %d)\n", 
				returnValues[i], i + 1, (i < expectedErrors.size() ? expectedErrors[i] : -1));
		}
	}

	// Print all expectedErrors for summary
	for (uint8_t value : expectedErrors ) 
	{
		printf("%d ", value);
	}
	printf("\n");
	// Print all returnValues for summary
	for (uint8_t value : returnValues) 
	{
		printf("%d ", value);
	}
	if (errorFlag == true ){
		printf("\nError Checking has FAILED.\n");
	}else{
		printf("\nError Checking has PASSED.\n");
	}
	printf("\n=== STOP Error checking. ===\r\n");
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

	// Run for ~10sec
	while(duration < 10000000) {
		myTFT.drawBitmap24(25, 50, img[frames % 5].get(), myBMPWidth, myBMPHeight);

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

}

/*!
 *@brief used in FPS 24 color bitmap test, 
*/
std::unique_ptr<uint8_t[]> loadImage(const char* name) 
{
	size_t pixelSize = 3; // 24-bit color = 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;

	FILE *pFile = fopen(name, "r");
	if (pFile == nullptr) 
	{
		std::cout << "Error: File does not exist" << std::endl;
		return nullptr;
	}

	std::unique_ptr<uint8_t[]> bmpBuffer;
	try 
	{
		bmpBuffer = std::make_unique<uint8_t[]>(myBMPWidth * myBMPHeight * pixelSize);
	} catch (const std::bad_alloc&) 
	{
		std::cout << "Error: Could not allocate memory" << std::endl;
		fclose(pFile);
		return nullptr;
	}

	fseek(pFile, FileHeaderOffset, SEEK_SET);
	fread(bmpBuffer.get(), pixelSize, myBMPWidth * myBMPHeight, pFile);
	fclose(pFile);

	return bmpBuffer; // Transfer ownership to the caller
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
	std::cout << "TFT End" << std::endl;
}


// *************** EOF ****************
