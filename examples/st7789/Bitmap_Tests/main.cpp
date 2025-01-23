/*!
	@file examples/st7789/Bitmap_Tests/main.cpp
	@brief Library test file, bitmaps
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 300 Sprites demo, small bitmap
	-# Test 301 "clock demo" , icons, small bitmap
	-# Test 302 bi-color image 128x128
	-# Test 303 24 bit color image bitmaps from the file system
	-# Test 304 16 bit color image bitmaps from the file system
	-# Test 305 16 bit color image bitmap from data
	-# Test 802 Error checking bitmap functions, results to console

*/

// Section ::  libraries
#include <ctime> // for test301
#include <iostream> // cout
#include "Bitmap_test_data.hpp" // Data for test 301-302
#include "ST7789_TFT_LCD_RDL.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 350

// Display size in pixels
#define myTFTWidth 240
#define myTFTHeight 280


// Section :: Globals
ST7789_TFT myTFT;

int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 280; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers
uint8_t Setup(void); // setup + user options
void Test300(void); // Sprite 
void Test301(void); // "clock demo" icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303(void); // 24 color bitmap
void Test304(void); // 16 color bitmap
void Test305(void); // 16 color bitmap (from data)
void Test802(void); //error checking
std::string UTC_string(void); // for clock demo
void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{

	if(Setup() != 0)return -1;
	myTFT.fillScreen(RDLC_BLACK);
	Test300();
	Test301();
	Test302();
	Test303();
	Test304();
	Test305();
	Test802();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	std::cout << "TFT Start" << std::endl;
	std::cout << "ST7789 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
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

	//print name
	myTFT.setCursor(10,210);
	myTFT.setTextColor(RDLC_CYAN, RDLC_BLACK);
	myTFT.print(teststr1);
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
	}
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(RDLC_BLACK);
	
}

// bitmap 2 colour , 128x128 bi color bitmap
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
	delayMilliSecRDL(TEST_DELAY5);
}

// bitmap 24 colour , All file format = Windows BITMAPINFOHEADER offset 54
// bitmap are 220 by 240 
void Test303(void)
{
	std::cout << "Test 303: 24 bit color image bitmaps from the file system" << std::endl;
	myTFT.fillScreen(RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
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
		if (myTFT.drawBitmap24(10, 20, bmpBuffer.data(), 220, 240) != rpiDisplay_Success)
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

		if (myTFT.drawBitmap16(40, 40, bmpBuffer1.data(), bitmapWidth, bitmapHeight) != rpiDisplay_Success)
		{
		// Check for success 0x00
			std::cout << "Warning an Error occurred in drawBitmap16" << std::endl;
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	myTFT.fillScreen(RDLC_BLACK);
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
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 16 data";
	myTFT.writeCharString(25, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);
	
	if(myTFT.drawBitmap16(60, 60, const_cast<uint8_t*>(pSpriteTest16), 32 , 32) != rpiDisplay_Success)
		return;
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(RDLC_BLACK);
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
	returnValues.push_back(myTFT.drawBitmap24(40, 310, const_cast<uint8_t*>(pSpriteTest16), 32, 32));
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

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_mega);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(15, 50, teststr1);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}


// *************** EOF ****************
