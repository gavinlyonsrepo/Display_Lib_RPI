/*!
	@file examples/ssd1306/TEXT_GRAPHICS_FUNCTIONS/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED display showing use of Text, graphics and functions.
		Project Name: Display_Lib_RPI

	@test

		-# Test 501 Enable and disable Screen (Sleep mode)
		-# Test 502 Invert screen color
		-# Test 503 Contrast screen
		-# Test 504 Scroll Screen
		-# Test 505 Rotate
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
		-# Test 717 print method numbers + vectors
		-# Test 808 Text methods error Checking
		-# Test 901 Graphics test
*/

#include <lgpio.h>
#include <cstdio>
#include <vector> // for error checking test 808
#include "SSD1306_OLED_RDL.hpp"

// Screen related
#define MY_OLED_WIDTH  128
#define MY_OLED_HEIGHT 64
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
SSD1306_RDL myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT) ; // instantiate  an object

// Test timing parameters
#define DisplayDelay1 4000
#define DisplayDelay2 0
uint8_t graphicsCountLimit = 25;

// I2C related
bool I2C_debug = false;
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_DEVICE 1
#define OLED_I2C_FLAGS 0


// =============== Function prototype ================
bool SetupTest(void);
void myTests(void);
void EndTests(void);
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
void testErrorCheck(void);
void DisplayGraphics(void);

// ======================= Main ===================
int main(void)
{
	if(!SetupTest()) return -1;
	myTests();
	EndTests();
	return 0;
}
// ======================= End of main  ===================


// ======================= Function space ===================
bool SetupTest()
{
	printf("OLED Test Begin\r\n");
	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	// Open  on I2C device
	if(myOLED.OLED_I2C_ON(OLED_I2C_DEVICE, OLED_I2C_ADDRESS,OLED_I2C_FLAGS) != rpiDisplay_Success)
	{
		printf("Error 1201:Cannot open I2C device bus\n");
		return false;
	}
	// Check if OLED on Bus
	if(myOLED.OLEDCheckConnection() < 0)
	{
		printf("Error 1202 : Cannot See Device on Bus\n");
		return false;
	}

	delayMilliSecRDL(500);
	myOLED.OLEDbegin(I2C_debug); // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars, optional just for effect
	delayMilliSecRDL(1000);
	return true;
}


void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	printf("OLED Test End\r\n");
}

void myTests()
{
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.OLEDclearBuffer();

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
	testErrorCheck();
	DisplayGraphics();
}

void TestReset(void){
	myOLED.OLEDupdate();
	delayMilliSecRDL(DisplayDelay1);
	myOLED.OLEDclearBuffer();
}

void Test701(void)
{
	printf("OLED Test 701 Default font\r\n");
	myOLED.setFont(font_default);
	char TestStr[] = "Default font";
	char InvertedStr[] = "Invertedfont";
	myOLED.writeChar(0,0 ,'a');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-45.98);
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'5');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("print invert 3");
	TestReset();
}

void Test702(void)
{
	printf("OLED Test 702 gll font, NO LOWERCASE LETTERS\r\n");
	myOLED.setFont(font_gll);
	char TestStr[] = "GLL FONT";
	char InvertedStr[] = "INVERT FONT.";
	myOLED.writeChar(0,0 ,'7');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-45.98);
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'0');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("PRINT INV 2!");
	TestReset();
}

void Test703(void){
	printf("OLED Test 703 PICO font\r\n");
	myOLED.setFont(font_pico);
	myOLED.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Inverted font";
	myOLED.writeChar(0,0 ,'p');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-3.98);
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'6');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("print invert 3");
	TestReset();
}

void Test704(void){
	printf("OLED Test 704 Sinclair font\r\n");
	myOLED.setFont(font_sinclairS);
	myOLED.setInvertFont(false);
	char TestStr[] = "Sinclair font";
	char InvertedStr[] = "Inverted font";
	myOLED.writeChar(0,0 ,'u');
	myOLED.writeCharString(0,10, TestStr);
	myOLED.setCursor(5,20);
	myOLED.print(-3.14);
	myOLED.setInvertFont(true);
	myOLED.writeChar(0,30 ,'5');
	myOLED.writeCharString(0,40, InvertedStr);
	myOLED.setCursor(5,50);
	myOLED.print("print invert 3");
	TestReset();
}

void Test705(void)
{
	printf("OLED Test 705 Orla font\r\n");
	myOLED.setFont(font_orla);
	myOLED.setInvertFont(false);
	char TestStr[] = "Orla F";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setInvertFont(true);
	myOLED.setCursor(1,32);
	myOLED.print("invert");
	TestReset();
}

void Test706(void)
{
	printf("OLED Test 706 retro font\r\n");
	myOLED.setFont(font_retro);
	myOLED.setInvertFont(false);
	char TestStr[] = "Retro";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setCursor(0,16);
	myOLED.print(14.19);
	myOLED.setInvertFont(true);
	myOLED.writeCharString(0,32, TestStr);
	myOLED.setCursor(0,48);
	myOLED.print("invert");
	TestReset();
}

void Test707(void){
	printf("OLED Test 707 Mega font\r\n");
	myOLED.setFont(font_mega);
	myOLED.setInvertFont(false);
	char TestStr[] = "Mega f.";
	char InvertedStr[] = "Invert";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setCursor(5,16);
	myOLED.print(-3.14);
	myOLED.setInvertFont(true);
	myOLED.writeCharString(0,32, InvertedStr);
	myOLED.setCursor(5,48);
	myOLED.print("invert");
	TestReset();
}

void Test708(void){
	printf("OLED Test 708 Arial bold font\r\n");
	myOLED.setFont(font_arialBold);
	myOLED.setInvertFont(false);
	char TestStr[] = "Arial b";
	char InvertedStr[] = "Invert 1";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setCursor(5,16);
	myOLED.print(-3.1415, 2); // print-3.14
	myOLED.setInvertFont(true);
	myOLED.writeCharString(0,32, InvertedStr);
	myOLED.setCursor(5,48);
	myOLED.print("invert");
	TestReset();
}

void Test709(void)
{
	printf("OLED Test 709 hallFetica font\r\n");
	myOLED.setFont(font_hallfetica);
	myOLED.setInvertFont(false);
	char TestStr[] = "Hall f.";
	char InvertedStr[] = "Invert";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setCursor(5,16);
	myOLED.print(-3.17);
	myOLED.setInvertFont(true);
	myOLED.writeCharString(0,32, InvertedStr);
	myOLED.setCursor(5,48);
	myOLED.print("invert");
	TestReset();
}

void Test710(void){
	printf("OLED Test 710 Arial Round font\r\n");
	myOLED.setFont(font_arialRound);
	myOLED.setInvertFont(false);
	char TestStr[] = "Arial R";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setInvertFont(true);
	myOLED.setCursor(1,32);
	myOLED.print("invert");
	TestReset();
}

void Test711(void)
{
	printf("OLED Test 711 GroTesk font\r\n");
	myOLED.setFont(font_groTesk);
	myOLED.setInvertFont(false);
	char TestStr[] = "GroTesk1";
	myOLED.writeCharString(0,0, TestStr);
	myOLED.setInvertFont(true);
	myOLED.setCursor(0,32);
	myOLED.print("inv 12!");
	TestReset();
}

void Test712(void)
{
	printf("OLED Test 712 Sixteen segment font\r\n");
	myOLED.setFont(font_sixteenSeg);
	myOLED.setInvertFont(false);

	myOLED.setCursor(0,0);
	myOLED.print(-3.1, 1);
	TestReset();
	myOLED.setInvertFont(true);
	char TestStr1[] = "12:4";
	myOLED.writeCharString(0,0, TestStr1);
	TestReset();
}

void Test713()
{
	printf("OLED Test 713 print ASCII font 0-127 \r\n");
	myOLED.setFont(font_default);
	myOLED.setInvertFont(false);
	myOLED.setCursor(0, 0);

	myOLED.print("ASCII font ");
	myOLED.setCursor(0, 15);

	// Print first 127 chars of font
	for (char i = 32; i < 126; i++)
	{
		myOLED.print(i);
	}
	TestReset();

}

void Test714(void)
{
	printf("OLED Test 714 Base number systems using print \r\n");
	myOLED.setFont(font_default);
	myOLED.setCursor(0, 0);
	myOLED.print(47 , RDL_DEC);
	myOLED.setCursor(0, 16);
	myOLED.print(47 , RDL_HEX);
	myOLED.setCursor(0, 32);
	myOLED.print(47, RDL_BIN);
	myOLED.setCursor(0, 48);
	myOLED.print(47 , RDL_OCT);
	TestReset();
}

void Test715(void)
{
	printf("OLED Test 715-a println + textwrap \r\n");
	myOLED.setCursor(0, 0);
	myOLED.println("first line");
	myOLED.print("2nd Line");
	myOLED.setCursor(0, 32);
	myOLED.setFont(font_gll);
	myOLED.print("123456789ABCDEFGHIJ");
	TestReset();
	printf("OLED Test 715-b writeText new line \r\n");
	char TestStr1[] = "123456789ABCDEFGHIJ";
	myOLED.writeCharString(0,0, TestStr1);
	TestReset();
}

void Test716(void)
{
	printf("OLED Test 716 print method + string objects \r\n");
	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myOLED.setFont(font_default);
	myOLED.setCursor(5,5);
	myOLED.print(timeInfo);
	// print a new line with println
	myOLED.setCursor(5,40);
	myOLED.println(newLine); // print a new line feed with println
	myOLED.print(newLine);
	TestReset();
}

void Test717(void)
{
	printf("OLED Test 717-a  print method numbers\r\n");
	myOLED.setCursor(0, 0);
	myOLED.println(3.986,2 ); //print 3.99
	myOLED.println(4001);
	myOLED.println(-87.39);
	myOLED.println(-19);
	myOLED.println("hello");
	myOLED.setInvertFont(true);
	myOLED.print('P');
	myOLED.setInvertFont(false);
	TestReset();
	
	
	printf("OLED Test 717-b  print vectors\r\n");
	// For a vector of integers
	myOLED.setCursor(0, 0);
	std::vector<int> intVec = {1, 2, 3, 4};
	myOLED.print(intVec); // Output: "1 2 3 4"
	
	// For a vector of strings
	myOLED.setCursor(0, 20);
	std::vector<std::string> stringVec = {"Hello", "Vector"};
	myOLED.print(stringVec); // Output: "Hello Vector"
	TestReset();
}



// Function to display Graphics.
void  DisplayGraphics()
{
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	printf("OLED Test 901 graphics test \r\n");
	uint8_t count = 0;
	bool colour = 1;
	myOLED.setFont(font_default);
	myOLED.OLEDclearBuffer(); // Clear the buffer
	while (count < graphicsCountLimit)
	{
		colour = !colour;

		//print count
		myOLED.setCursor(0, 0);
		myOLED.print(count);
		myOLED.setCursor(45, 0);
		myOLED.print(colour);

		// Draw the X
		myOLED.drawLine(64,  0, 64, 64, RDL_BLACK);
		myOLED.drawFastVLine(62, 0, 64, RDL_BLACK);
		myOLED.drawFastHLine(0, 32, 128, RDL_BLACK);

		// Q1
		myOLED.fillRect(0, 10, 20, 20, colour);
		myOLED.fillCircle(40, 20, 10, !colour);
		myOLED.drawCircle(40, 20, 10, colour);
		// Q2
		myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
		myOLED.drawTriangle(80, 25, 90, 5, 100, 25, colour);
		myOLED.drawRect(105, 10, 15, 15, RDL_BLACK);
		// Q3
		myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
		myOLED.drawRoundRect(0, 40, 40, 20, 10, colour);
		// Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, RDL_BLACK);
			myOLED.OLEDupdate();
			delayMilliSecRDL(50);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
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
	myOLED.setFont(font_gll);
	returnValues.push_back(myOLED.writeChar(32, 0, '!')); //success
	returnValues.push_back(myOLED.writeCharString(5,  5, testlowercase)); //throw gll font error 2
	TestReset();
	// Numeric extended bounds ; , A errors
	myOLED.setFont(font_sixteenSeg);
	returnValues.push_back(myOLED.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myOLED.writeChar(32, 0, ',')); //throw error 2
	TestReset();
	printf("========\r\n");
	// screen out of bounds
	myOLED.setFont(font_default);
	returnValues.push_back(myOLED.writeChar(0, 100, 'e')); //throw error 1
	returnValues.push_back(myOLED.writeChar(150, 0, 'f')); //throw error 1
	TestReset();
	myOLED.setFont(font_orla);
	returnValues.push_back(myOLED.writeChar(0, 100, 'A')); //throw error 1
	returnValues.push_back(myOLED.writeChar(150, 0, 'B')); //throw error 1
	TestReset();
	
	returnValues.push_back(myOLED.writeCharString(5, 5, nullptr)); //throw error 
	
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

void Test500()
{
	// Set text parameters
	myOLED.setFont(font_default);

	//  ** Test 501 OLED display enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("Disable T501");
	printf("OLED Disable test 501\r\n");
	myOLED.OLEDupdate();

	delayMilliSecRDL(2000);
	myOLED.OLEDEnable(0);
	delayMilliSecRDL(2000);
	myOLED.OLEDEnable(1);
	delayMilliSecRDL(2000);
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("Inverse T502");
	printf("OLED Inverse test 502\r\n");
	myOLED.OLEDupdate();
	delayMilliSecRDL(1500);
	myOLED.OLEDInvert(1); // Inverted
	delayMilliSecRDL(1500);
	myOLED.OLEDInvert(0);
	delayMilliSecRDL(1500);

	// ** Test 503 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast T503");
	printf("OLED Contrast test 503\r\n");
	myOLED.OLEDupdate();
	delayMilliSecRDL(1500);
	myOLED.OLEDFillScreen(0x77, 0);
	myOLED.OLEDContrast(0x00);
	delayMilliSecRDL(1000);
	myOLED.OLEDContrast(0x80);
	delayMilliSecRDL(1000);
	myOLED.OLEDContrast(0xFF);
	delayMilliSecRDL(1000);
	myOLED.OLEDContrast(0x81);
	delayMilliSecRDL(1000);
	myOLED.OLEDclearBuffer();

	// ***** Test 504 Scroll **
	myOLED.setCursor(20,20 );
	myOLED.print("Scroll T504");
	printf("OLED Scroll test 504\r\n");
	myOLED.OLEDupdate();
	delayMilliSecRDL(2500);

	myOLED.OLEDStartScrollRight(0, 0x0F);
	delayMilliSecRDL(3000);
	myOLED.OLEDStopScroll();

	myOLED.OLEDStartScrollLeft(0, 0x0F);
	delayMilliSecRDL(3000);
	myOLED.OLEDStopScroll();

	myOLED.OLEDStartScrollDiagRight(0, 0x07);
	delayMilliSecRDL(3000);
	myOLED.OLEDStopScroll();

	myOLED.OLEDStartScrollDiagLeft(0, 0x07);
	delayMilliSecRDL(3000);
	myOLED.OLEDStopScroll();


	// ** Test 505 rotate test **

	myOLED.OLEDclearBuffer();
	printf("OLED Rotate test 505\r\n");
	myOLED.setRotation(displayBC_Degrees_90);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 90");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);

	myOLED.setRotation(displayBC_Degrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 180");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);

	myOLED.setRotation(displayBC_Degrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate   270");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);

	myOLED.setRotation(displayBC_Degrees_0); //default normal
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 0");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);

	myOLED.OLEDFillScreen(0x00, 0); // Clear the screen
	myOLED.OLEDupdate();
	myOLED.OLEDclearBuffer();
	delayMilliSecRDL(500);
}
