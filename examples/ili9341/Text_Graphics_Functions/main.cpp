/*!
	@file examples/ili9341/Text_Graphics_Functions/main.cpp
	@brief Library test file, tests Text,graphics & functions.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 500 RGB color OK?
	-# Test 502 Rotate
	-# Test 503 change modes test -> Invert, display on/off and Sleep.
	-# Test 701 print out fonts 1-12
	-# Test 702 defined 16-bit Colors, text
	-# Test 703 print entire ASCII font 32 to 127, default font
	-# Test 704 font invert + test character draw using draw functions
	-# Test 705 print method all fonts
	-# Test 706 Misc print class tests (string object, println invert, wrap, base nums etc)
	-# Test 707 Misc draw functions (Invert, wrap)s
	-# Test 708 Vectors with print class
	-# Test 808 Error checking text functions
	-# Test 902 rectangles
	-# Test 903 Circle
	-# Test 904 Triangles
*/

// Section ::  libraries
#include <iostream> // cout
#include "ILI9341_TFT_LCD_RDL.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

// Section :: Globals
ILI9341_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

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

void Test500(void); // Color RGB
void Test501(void); 
void Test502(void); // Rotate
void Test503(void); // change modes test -> Invert, display on/off.

void Test701(void);
void Test702(void);
void Test703(void);
void Test704(void);
void Test705(void);
void Test706(void);
void Test707(void);
void Test708(void);

void Test808(void);

void Test902(void);  // rectangles
void Test903(void);  // Circle
void Test904(void);  // Triangles

//  Section ::  MAIN loop

int main(void)
{
	if(Setup() != 0)return -1;

	Test500();
	Test501();
	Test502();
	Test503();

	Test701();
	Test702();
	Test703();
	Test704();
	Test705();
	Test706();
	Test707();

	Test808();

	Test902();
	Test903();
	Test904();

	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	std::cout << "TFT Start Test " << std::endl;
	std::cout << "ili9341 library version : " << GetRDLibVersionNum()<< std::endl;
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

void Test500(void)
{
	std::cout << "Test 500: Color Test:: Red,green,blue,yellow,white, black background" << std::endl;
	myTFT.setFont(font_mega);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_GREEN,RDLC_BLACK);
	myTFT.fillRoundRect(8, 10, 24, 60, 8, RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, RDLC_WHITE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
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

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_0);
	myTFT.writeCharString(15, 15, teststr0);
	myTFT.writeCharString(15, 300, teststr0);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_90);
	myTFT.writeCharString(15, 15, teststr1);
	myTFT.writeCharString(15, 220, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_180);
	myTFT.writeCharString(15, 15, teststr2);
	myTFT.writeCharString(15, 300, teststr2);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setRotation(myTFT.TFT_Degrees_270);
	myTFT.writeCharString(15, 15, teststr3);
	myTFT.writeCharString(15, 220, teststr3);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.setRotation(myTFT.TFT_Degrees_0);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test503()
{
	std::cout << "Test 503: Mode Tests" << std::endl;
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(8, 10, 24, 60, 8, RDLC_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, RDLC_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, RDLC_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, RDLC_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, RDLC_WHITE);
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

	myTFT.fillScreen(RDLC_BLACK);

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

	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeCharString(5, 0, teststr1);
	myTFT.setTextColor(RDLC_BLUE, RDLC_BLACK);
	myTFT.writeCharString(5, 16, teststr2);
	myTFT.setTextColor(RDLC_RED, RDLC_BLACK);
	myTFT.writeCharString(5, 33, teststr3);
	myTFT.setTextColor(RDLC_GREEN, RDLC_BLACK);
	myTFT.writeCharString(5, 49, teststr4);
	myTFT.setTextColor( RDLC_CYAN, RDLC_BLACK);
	myTFT.writeCharString(5, 66, teststr5);
	myTFT.setTextColor(RDLC_MAGENTA, RDLC_BLACK);
	myTFT.writeCharString(5, 100, teststr6);
	myTFT.setTextColor(RDLC_YELLOW, RDLC_BLACK);
	myTFT.writeCharString(5, 130, teststr7);
	myTFT.setTextColor(RDLC_GREY, RDLC_BLACK);
	myTFT.writeCharString(5, 170, teststr8);
	myTFT.setTextColor(RDLC_TAN, RDLC_BLACK);
	myTFT.writeCharString(5, 200, teststr9);
	myTFT.setTextColor(RDLC_BROWN, RDLC_BLACK);
	myTFT.writeCharString(5, 220, teststr10);
	myTFT.setTextColor( RDLC_ORANGE, RDLC_BLACK);
	myTFT.writeCharString(5, 260, teststr11);
	myTFT.setTextColor(RDLC_DGREEN, RDLC_BLACK);
	myTFT.writeCharString(5, 300, teststr12);
	DisplayReset();
}

void Test703(void) {
	std::cout << "Test 703: Print entire ASCII font 32 to 127" << std::endl;
	uint8_t row = 5;
	uint8_t col = 5;
	myTFT.setFont(font_mega);
	myTFT.setTextColor(RDLC_GREEN, RDLC_BLACK);
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
	myTFT.setTextColor(RDLC_RED, RDLC_YELLOW);
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
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);

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
	myTFT.setTextColor(RDLC_RED, RDLC_YELLOW);
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
	myTFT.setTextColor(RDLC_YELLOW, RDLC_RED);

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
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(font_pico);
	myTFT.setCursor(5,5);
	myTFT.print(numPos , RDL_DEC); // 47
	myTFT.setCursor(5,25);
	myTFT.print(numPos , RDL_BIN); // 10111
	myTFT.setCursor(5,45);
	myTFT.print(numPos , RDL_OCT); // 57
	myTFT.setCursor(5,65);
	myTFT.print(numPos , RDL_HEX); // 2F

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
	myTFT.setTextColor(RDLC_RED, RDLC_BLACK);
	myTFT.writeChar(2, 2, '7');
	myTFT.setTextColor(RDLC_YELLOW, RDLC_BLACK);
	myTFT.writeCharString(2, 40, teststr1);

	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeChar(2, 75, '8');
	myTFT.setTextColor(RDLC_GREEN, RDLC_RED);
	myTFT.writeCharString(2, 100, teststr2);

	DisplayReset();

	myTFT.setFont(font_arialRound);
	myTFT.setTextColor(RDLC_RED, RDLC_BLACK);
	myTFT.writeChar(2, 2, '9');
	myTFT.setTextColor(RDLC_YELLOW, RDLC_BLACK);
	myTFT.writeCharString(2, 26, teststr3);

	myTFT.setFont(font_arialBold);
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.writeChar(2, 56, 'A');
	myTFT.setTextColor(RDLC_GREEN, RDLC_RED);
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
	myTFT.println( intVec[0], RDL_HEX);  // print 2F
	myTFT.println( intVec[0] ,RDL_OCT); //print 57
	myTFT.println( intVec[0], RDL_BIN); // print 101111
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

void Test808(void)
{
	// Error checking
	printf("==== Test 808 Start Error checking ====\n");
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		rpiDisplay_CharFontASCIIRange, rpiDisplay_CharFontASCIIRange, rpiDisplay_Success, rpiDisplay_CharArrayNullptr,
		rpiDisplay_Success, rpiDisplay_CharFontASCIIRange, rpiDisplay_CharFontASCIIRange,
		rpiDisplay_CharFontASCIIRange, rpiDisplay_CharScreenBounds, rpiDisplay_CharScreenBounds,
		rpiDisplay_CharScreenBounds, rpiDisplay_CharScreenBounds, rpiDisplay_CharArrayNullptr
	};
	
	// Vector to store return values
	std::vector<uint8_t> returnValues; 

	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = "-;A";
	bool errorFlag = false;
	myTFT.setFont(font_gll);

	// (1) Print statement test, gll font lower case letters
	// We check print class error flag as print statement does not return errors it returns num of characters
	// it attempted to print. Always clear error flag before checking getWriteError after new print
	myTFT.setCursor(40,40);
	myTFT.print("ABc"); // Print AB , return 3 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return error
	myTFT.clearWriteError(); // Reset error flag
	myTFT.print("abC");  // print C , return 3 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return error
	myTFT.clearWriteError(); // Reset error flag
	myTFT.print("12345"); // print 12345 , return 5 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return pass
	myTFT.clearWriteError(); // Reset error flag
	myTFT.print(nullptr);  // return 0 (num of characters)
	returnValues.push_back(myTFT.getWriteError());  // return error
	myTFT.clearWriteError(); // Reset error flag
	DisplayReset();
	printf("========\r\n");
	// (2) writeChar + writeCharString
	// gll lower case + ] character out of font bounds
	returnValues.push_back(myTFT.writeChar(32, 0, '!')); //success
	returnValues.push_back(myTFT.writeCharString(5,  5, testlowercase)); //throw gll font error 2
	DisplayReset();
	// Numeric extended bounds ; , A errors
	myTFT.setFont(font_sixteenSeg);
	returnValues.push_back(myTFT.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myTFT.writeChar(32, 0, ',')); //throw error 2
	DisplayReset();
	printf("========\r\n");
	// screen out of bounds
	myTFT.setFont(font_default);
	returnValues.push_back(myTFT.writeChar(0, 400, 'e')); //throw error 1
	returnValues.push_back(myTFT.writeChar(400, 0, 'f')); //throw error 1
	DisplayReset();
	myTFT.setFont(font_orla);
	returnValues.push_back(myTFT.writeChar(0, 400, 'A')); //throw error 1
	returnValues.push_back(myTFT.writeChar(400, 0, 'B')); //throw error 1
	DisplayReset();
	
	returnValues.push_back(myTFT.writeCharString(5, 5, nullptr)); //throw error 
	
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

void Test902(void) {
	std::cout << "Test 902: rectangles " << std::endl;
	rpiDisplay_Return_Codes_e returnValue;
	myTFT.drawRectWH(25, 25, 20, 20, RDLC_RED);
	returnValue = myTFT.fillRectangle(85, 25, 20, 20, RDLC_YELLOW);
	if (returnValue != rpiDisplay_Success)
	{
		std::cout << "Warning : Test TFTfillRectangle, An error occurred returnValue =" << +returnValue << std::endl;
	}
	myTFT.fillRect(115, 25, 20, 20, RDLC_GREEN);
	myTFT.drawRoundRect(15, 60, 50, 50, 5, RDLC_CYAN);
	myTFT.fillRoundRect(70, 60, 50, 50, 10, RDLC_WHITE);

	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void Test903(void) {
	std::cout << "Test 903 & 904 : Triangles and circles" << std::endl;
	myTFT.drawCircle(40, 20, 15, RDLC_GREEN);
	myTFT.fillCircle(80, 20, 15, RDLC_YELLOW);
}

void Test904(void) {
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80,RDLC_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, RDLC_RED);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void DisplayReset(void)
{
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(RDLC_BLACK);
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
	myTFT.setFont(font_mega);
	myTFT.writeCharString(25, 50, teststr1);
	DisplayReset();

	myTFT.PowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
