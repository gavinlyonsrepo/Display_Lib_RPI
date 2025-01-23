/*!
	@file examples/erm19264/TEXT_GRAPHICS_FUNCTIONS/main.cpp
	@author Gavin Lyons
	@brief Test file for ERM19264 display showing use of Text fonts,graphics and functions
		Project Name: Display_Lib_RPI

	@test
		-# Test 501 All Pixels on
		-# Test 502 Invert screen
		-# Test 503 Flip 180 with LCD rotate command screen
		-# Test 504 Scroll Screen
		-# Test 505 Enable and disable Screen
		-# Test 506 Rotate software buffer
		-# Test 701 Default font
		-# Test 702 gll font
		-# Test 703 Pico font
		-# Test 704 Sinclair font
		-# Test 705 Orla Font
		-# Test 706 Retro Font
		-# Test 707 Mega font
		-# Test 708 Arial bold font
		-# Test 709 hallFetica font
		-# Test 710 Arial round font
		-# Test 711 GroTesk font
		-# Test 712 Sixteen Segment font
		-# Test 713 Display ASCII font 32-127 default font
		-# Test 714 Base number systems using print method
		-# Test 715 println + print & textwrap
		-# Test 716 print method String object
		-# Test 717 print method numbers
		-# Test 718 Fonts grotesk big , inconsola, mint and seven segment
		-# Test 808 Text methods error Checking
*/

#include <lgpio.h>
#include <cstdio>
#include "ERM19264_UC1609_LCD_RDL.hpp"

//GPIO
const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

// Screen
const uint8_t MY_LCD_WIDTH  = 192;
const uint8_t MY_LCD_HEIGHT = 64;
#define  MY_SCREEN_SIZE (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8))
const uint8_t LCDcontrast = 0x49; // Contrast 00 to FF , 0x49 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

// SPi
// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

// instantiate  an object for HW SPI
ERM19264_UC1609 myLCD(MY_LCD_WIDTH ,MY_LCD_HEIGHT , RST, CD) ;

// Vars for test
#define DisplayDelay1 9000
#define DisplayDelay2 5
uint8_t graphicsCountLimit = 25;

// =============== Function prototype ================
bool setup(void);
void myTest(void);
void EndTest(void);
void TestReset(void);

void Test500(void);
void Test701(void);
void Test702(void);
void Test703(void);
void Test704(void);
void Test705(void);
void Test706(void);
void Test707(void);
void Test708(void);
void Test709(void);
void Test710(void);
void Test711(void);
void Test712(void);
void Test713(void);
void Test714(void);
void Test715(void);
void Test716(void);
void Test717(void);
void Test718(void);
void testErrorCheck(void);
void DisplayGraphics();

// ======================= Main ===================
int main()
{
	if(!setup()) {return -1;}
	myTest();
	EndTest();

	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
bool setup() {
	printf("LCD Test Begin\r\n");
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum());
	delayMilliSecRDL(100);
	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		printf("Error 1202: Cannot start spi\n");
		return false;
	}
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	delayMilliSecRDL(1000);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	printf("LCD End\r\n");
}

void TestReset()
{
	myLCD.LCDupdate();  // Write to the buffer
	delayMilliSecRDL(DisplayDelay1);
	myLCD.LCDclearBuffer();
}

void myTest()
{
	// Define a buffer to cover whole screen
	uint8_t screenBuffer[ MY_SCREEN_SIZE];
	if (myLCD.LCDSetBufferPtr(MY_LCD_WIDTH, MY_LCD_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myLCD.LCDclearBuffer();

	Test500();
	Test701();
	Test702();
	Test703();
	Test704();
	Test705();
	Test706();
	Test707();
	Test708();
	Test709();
	Test710();
	Test711();
	Test712();
	Test713();
	Test714();
	Test715();
	Test716();
	Test717();
	Test718();

	testErrorCheck();
	DisplayGraphics();
}

void Test701(void)
{
	printf("LCD Test 701 Default font\r\n");
	myLCD.setFont(font_default);
	char TestStr[] = "Default font";
	char InvertedStr[] = "Invertedfont";
	myLCD.writeChar(0,0 ,'a');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-33.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'5');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test702(void)
{
	printf("LCD Test 702 gll font, NO LOWERCASE LETTERS\r\n");
	myLCD.setFont(font_gll);
	char TestStr[] = "GLL FONT";
	char InvertedStr[] = "INVERT FONT.";
	myLCD.writeChar(0,0 ,'7');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'0');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("PRINT INV 2!");
	TestReset();
}

void Test703(void){
	printf("LCD Test 703 PICO font\r\n");
	myLCD.setFont(font_pico);
	myLCD.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Inverted font";
	myLCD.writeChar(0,0 ,'p');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-3.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'6');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test704(void){
	printf("LCD Test 704 Sinclair font\r\n");
	myLCD.setFont(font_sinclairS);
	myLCD.setInvertFont(false);
	char TestStr[] = "Sinclair font";
	char InvertedStr[] = "Inverted font";
	myLCD.writeChar(0,0 ,'u');
	myLCD.writeCharString(0,10, TestStr);
	myLCD.setCursor(5,20);
	myLCD.print(-2.19);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,30 ,'5');
	myLCD.writeCharString(0,40, InvertedStr);
	myLCD.setCursor(5,50);
	myLCD.print("print invert 3");
	TestReset();
}

void Test705(void)
{
	printf("LCD Test 705 Orla font\r\n");
	myLCD.setFont(font_orla);
	myLCD.setInvertFont(false);
	char TestStr[] = "Orla F";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1,32);
	myLCD.print("invert");
	TestReset();
}

void Test706(void)
{
	printf("LCD Test 706 retro font\r\n");
	myLCD.setFont(font_retro);
	myLCD.setInvertFont(false);
	char TestStr[] = "Retro";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(0,16);
	myLCD.print(14.19);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, TestStr);
	myLCD.setCursor(0,48);
	myLCD.print("invert");
	TestReset();
}

void Test707(void){
	printf("LCD Test 707 Mega font\r\n");
	myLCD.setFont(font_mega);
	myLCD.setInvertFont(false);
	char TestStr[] = "Mega f.";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-8.14);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	myLCD.setCursor(5,48);
	myLCD.print("invert");
	TestReset();
}

void Test708(void){
	printf("LCD Test 708 Arial bold font\r\n");
	myLCD.setFont(font_arialBold);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial b";
	char InvertedStr[] = "Invert 1";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.1415, 2); // print-3.14
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	myLCD.setCursor(5,48);
	myLCD.print("invert");
	TestReset();
}

void Test709(void)
{
	printf("LCD Test 709 hallFetica font\r\n");
	myLCD.setFont(font_hallfetica);
	myLCD.setInvertFont(false);
	char TestStr[] = "Hall f.";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.17);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	myLCD.setCursor(5,48);
	myLCD.print("invert");
	TestReset();
}

void Test710(void){
	printf("LCD Test 710 Arial Round font\r\n");
	myLCD.setFont(font_arialRound);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial R";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1,32);
	myLCD.print("invert");
	TestReset();
}

void Test711(void)
{
	printf("LCD Test 711 GroTesk font\r\n");
	myLCD.setFont(font_groTesk);
	myLCD.setInvertFont(false);
	char TestStr[] = "GroTesk1";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(0,32);
	myLCD.print("inv 12!");
	TestReset();
}

void Test712(void)
{
	printf("LCD Test 712 Sixteen segment font\r\n");
	myLCD.setFont(font_sixteenSeg);
	myLCD.setInvertFont(false);

	myLCD.setCursor(0,0);
	myLCD.print(-3.141, 3);
	TestReset();
	myLCD.setInvertFont(true);
	char TestStr1[] = "12:30";
	myLCD.writeCharString(0,0, TestStr1);
	TestReset();
}

void Test713()
{
	printf("LCD Test 713 print ASCII font 32-126 \r\n");
	myLCD.setFont(font_default);
	myLCD.setInvertFont(false);
	myLCD.setCursor(0, 0);

	myLCD.print("ASCII font ");
	myLCD.setCursor(0, 15);

	// Print first 127 chars of font
	for (char i = 32; i < 126; i++)
	{
		myLCD.print(i);
	}
	TestReset();

}

void Test714(void)
{
	printf("LCD Test 714 Base number systems using print \r\n");
	myLCD.setFont(font_default);
	myLCD.setCursor(0, 0);
	myLCD.print(47 , RDL_DEC);
	myLCD.setCursor(0, 16);
	myLCD.print(47 , RDL_HEX);
	myLCD.setCursor(0, 32);
	myLCD.print(47, RDL_BIN);
	myLCD.setCursor(0, 48);
	myLCD.print(47 , RDL_OCT);
	TestReset();
}

void Test715(void)
{
	printf("LCD Test 715-a println + textwrap \r\n");
	myLCD.setCursor(0, 0);
	myLCD.println("first line");
	myLCD.print("2nd Line");
	myLCD.setCursor(60, 32);
	myLCD.setFont(font_gll);
	myLCD.print("123456789ABCDEFGHIJ");
	TestReset();
	printf("LCD Test 715-b writeText new line \r\n");
	char TestStr1[] = "123456789ABCDEFGHIJ";
	myLCD.writeCharString(60,0, TestStr1);
	TestReset();
}

void Test716(void)
{
	printf("LCD Test 716-a print method + string objects \r\n");
	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myLCD.setFont(font_default);
	myLCD.setCursor(5,5);
	myLCD.print(timeInfo);
	// print a new line with println
	myLCD.setCursor(5,40);
	myLCD.println(newLine); // print a new line feed with println
	myLCD.print(newLine);
	TestReset();
	
	printf("LCD Test 716-b  print vectors\r\n");
	// For a vector of integers
	myLCD.setCursor(10, 20);
	std::vector<int> intVec = {1, 2, 3, 4};
	myLCD.print(intVec[1]); // Output: "2 "
	
	// For a vector of strings
	myLCD.setCursor(10, 40);
	std::vector<std::string> stringVec = {"HELLO", "VECTOR"};
	myLCD.print(stringVec); // Output: "HELLO VECTOR"
	TestReset();
}

void Test717(void)
{
	printf("LCD Test 717 print method numbers\r\n");
	myLCD.setCursor(0, 0);
	myLCD.println(3.986,2 ); //print 3.99
	myLCD.println(4001);
	myLCD.println(-87.39);
	myLCD.println(-19);
	myLCD.println("hello");
	myLCD.setInvertFont(true);
	myLCD.print('P');
	TestReset();
}

void Test718(void){
	printf("LCD Test 718 mint, inconsola, 7 seg and grotesk big font  \r\n");
	myLCD.setFont(font_mint);
	myLCD.setCursor(5,5);
	myLCD.print("21.35`C"); // Note ` gives degree symbol
	TestReset();

	myLCD.setFont(font_inconsola);
	myLCD.setCursor(5,5);
	myLCD.print("11.49`C"); // Note ` gives degree symbol
	TestReset();

	myLCD.setFont(font_sevenSeg);
	myLCD.setCursor(5,5);
	myLCD.print(-9.123);
	TestReset();

	myLCD.setFont(font_groTeskBig);
	myLCD.setCursor(5,5);
	myLCD.print("BIG");
	TestReset();
}

void testErrorCheck(void)
{
	// Error checking
	printf("==== Test 808 Start Error checking ====\r\n");
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		rpiDisplay_Success, rpiDisplay_CharFontASCIIRange, rpiDisplay_CharFontASCIIRange, rpiDisplay_CharFontASCIIRange,
		rpiDisplay_CharScreenBounds, rpiDisplay_CharScreenBounds, rpiDisplay_CharScreenBounds, rpiDisplay_CharScreenBounds,
		rpiDisplay_CharArrayNullptr
	};
	
	// Vector to store return values
	std::vector<uint8_t> returnValues; 

	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = "-:;A";
	bool errorFlag = false;
	
	// character out of font bounds
	// gll lower case + ]
	myLCD.setFont(font_gll);
	returnValues.push_back(myLCD.writeChar(32, 0, '!')); //success
	returnValues.push_back(myLCD.writeCharString(5,  5, testlowercase)); //throw gll font error 2
	TestReset();
	// Numeric extended bounds ; , A errors
	myLCD.setFont(font_sixteenSeg);
	returnValues.push_back(myLCD.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myLCD.writeChar(32, 0, ',')); //throw error 2
	TestReset();
	printf("========\r\n");
	// screen out of bounds
	myLCD.setFont(font_default);
	returnValues.push_back(myLCD.writeChar(0, 100, 'e')); //throw error 1
	returnValues.push_back(myLCD.writeChar(200, 0, 'f')); //throw error 1
	TestReset();
	myLCD.setFont(font_orla);
	returnValues.push_back(myLCD.writeChar(0, 100, 'A')); //throw error 1
	returnValues.push_back(myLCD.writeChar(200, 0, 'B')); //throw error 1
	TestReset();
	
	returnValues.push_back(myLCD.writeCharString(5, 5, nullptr)); //throw error 
	
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

// Function to display Graphics test 901
void  DisplayGraphics()
{
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	//
	bool colour = 1;
	uint8_t count = 0;
	myLCD.setFont(font_default);
	myLCD.LCDclearBuffer(); // Clear the buffer
	while (count < graphicsCountLimit)
	{
		colour = !colour;

		//print count
		myLCD.setCursor(0, 0);
		myLCD.print(count);
		myLCD.setCursor(45, 0);
		myLCD.print(colour);
		// Draw the X
		myLCD.drawLine(96,  0, 96, 64, RDL_BLACK);
		myLCD.drawFastVLine(94, 0, 64, RDL_BLACK);
		myLCD.drawFastHLine(0, 32, 192, RDL_BLACK);

		//Q1
		myLCD.fillRect(0, 10, 20, 20, colour);
		myLCD.fillCircle(40, 20, 10, RDL_BLACK);
		myLCD.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
		//Q2
		myLCD.drawRect(100, 10, 20, 20, RDL_BLACK);
		myLCD.drawCircle(140, 20, 10, colour);
		myLCD.drawTriangle(160, 30, 170, 10, 180, 30, RDL_BLACK);
		//Q3
		myLCD.fillRoundRect(0, 40, 40, 20, 10, !colour);
		myLCD.fillRoundRect(45, 40, 40, 20, 10, colour);
		//Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myLCD.drawRect(100 + i, 40 + i, 80 - i * 2, 20 - i * 2, RDL_BLACK);
			myLCD.LCDupdate();
			delayMilliSecRDL(50);
		}
	myLCD.LCDclearBuffer();
	count++;
	}

} // end Display graphics

void Test500()
{

	// Test 501 LCD all pixels on
	myLCD.setCursor(20, 30);
	myLCD.print("All Pixels on");
	myLCD.LCDupdate();
	delayMilliSecRDL(4000);
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	myLCD.LCDallpixelsOn(1);
	delayMilliSecRDL(2000);
	myLCD.LCDallpixelsOn(0);
	delayMilliSecRDL(2000);

	// Test 502 inverse
	myLCD.setCursor(20, 30);
	myLCD.print("inverse test  ");
	myLCD.LCDupdate();
	myLCD.LCDinvert(0); // Normal
	delayMilliSecRDL(2000);
	myLCD.LCDinvert(1); // Inverted
	delayMilliSecRDL(4000);
	myLCD.LCDinvert(0);

	// Test 503 Flip 180 with LCD rotate command screen
	myLCD.LCDclearBuffer();
	myLCD.setCursor(20, 30);
	myLCD.print("flip 180  test");
	myLCD.LCDupdate();
	delayMilliSecRDL(2000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_ONE);
	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
	myLCD.LCDrotate(UC1609_ROTATION_NORMAL);
	myLCD.LCDupdate();
	delayMilliSecRDL(2000);

	// Test 504 LCD scroll
	myLCD.LCDclearBuffer();
	//myLCD.LCDupdate();
	myLCD.setCursor(0, 40);
	myLCD.print("scroll test");
	for (uint8_t i = 0 ; i < 62 ; i ++)
	{
		myLCD.LCDscroll(i);
		myLCD.LCDupdate();
		delayMilliSecRDL(100);
	}
	myLCD.LCDscroll(0);

	//Test 505 LCD enable and disable
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 30);
	myLCD.print("LCD Disable test");
	myLCD.LCDupdate();
	delayMilliSecRDL(5000);
	myLCD.LCDEnable(0);
	delayMilliSecRDL(5000);
	myLCD.LCDEnable(1);
	myLCD.LCDclearBuffer();

	// ** Test 506 Rotate test **
	myLCD.LCDclearBuffer();
	myLCD.setRotation(displayBC_Degrees_90);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 90");
	myLCD.setCursor(5,180);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.setRotation(displayBC_Degrees_180);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 180");
	myLCD.setCursor(5,56);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.setRotation(displayBC_Degrees_270);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate   270");
	myLCD.setCursor(5,180);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.setRotation(displayBC_Degrees_0); //default normal
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 0");
	myLCD.setCursor(5,56);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	delayMilliSecRDL(3000);

	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	delayMilliSecRDL(500);
}

// *************** EOF *********************
