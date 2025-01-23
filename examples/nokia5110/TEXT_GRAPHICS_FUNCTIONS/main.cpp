/*!
	@file examples/nokia5110/TEXT_GRAPHICS_FUNCTIONS/main.cpp
	@author Gavin Lyons
	@brief Test file for Nokia 5110 display showing use of Text,graphics & functions.
		Project Name: Display_Lib_RPI

	@test
		-# Test 501 fill + clear screen
		-# Test 502 Sleep
		-# Test 503 Rotation
		-# Test 504 invert screen command
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
		-# Test 713 Display ASCII font 32-90 default font
		-# Test 714 Base number systems using print method
		-# Test 715 println + print & textwrap
		-# Test 716 print method String object
		-# Test 717 print method numbers
		-# Test 808 Text methods error Checking
		-# Test 902 Draw pixel
		-# Test 903 Draw lines
		-# Test 904 Draw shapes
*/


// libraries 
#include <lgpio.h>
#include <cstdio>
#include <iostream>
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver


// Test setup 
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// GPIO
const uint8_t RST_LCD = 25; // GPIO pin number pick any you want
const uint8_t DC_LCD = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEV = 4; // GPIO chip device RPI 5 = 4 , other RPIs = 0

//  LCD
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
#define LCD_INV  false // set to true to invert display pixel color
#define LCD_CST  0xB2 // contrast default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define LCD_BIAS 0x13 // LCD LCD_BIAS mode 1:48: Try 0x12 or 0x13 or 0x14

// Hardware SPI setup
int SPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

NOKIA_5110_RPI myLCD(MY_LCD_WIDTH, MY_LCD_HEIGHT , RST_LCD, DC_LCD);

// ************ Function Headers ****************
bool Setup(void);
void screenReset(void);
void EndTests(void);

void testFillScreen(void);
void testSleepMode(void);
void testRotate(void);
void testInvert(void);

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

void textDrawPixel(void);
void testdrawcircle(void);
void testfillcircle(void);
void testDrawLine(void);
void testDrawChar(void);
void testDrawRect(void);
void testFillRect(void);
void testDrawTriangle(void);
void testFillTriangle(void);
void testDrawRoundRect(void);
void testFillRoundRect(void);

void testErrorCheck(void);

// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;

	myLCD.LCDfillScreenPattern(0x31); // Splash screen
	screenReset();

	testFillScreen();
	testSleepMode();
	testRotate();
	testInvert();

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

	textDrawPixel();
	testDrawLine();
	std::cout << "Test 204 Draw shapes" << std::endl;
	testDrawRect();
	testFillRect();
	testdrawcircle();
	testfillcircle();
	testDrawRoundRect();
	testFillRoundRect();
	testDrawTriangle();
	testFillTriangle();

	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device
bool Setup(void)
{
	std::cout << "LCD Start"  << std::endl;
	std::cout<< "lgpio library version : " << lguVersion() << std::endl;
	std::cout<< "Nokia 5110 library version : " << GetRDLibVersionNum() << std::endl;
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, SPI_DEVICE, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rpiDisplay_Success)
	{
		std::cout<< "Error 1202: Setup :Cannot start spi" << std::endl;
		return false;
	}
	delayMilliSecRDL(250);
	myLCD.LCDdisplayClear();
	return true;
}

void EndTests(void)
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	std::cout << "LCD End" << std::endl;
}

void Test701(void)
{
	std::cout << "Test 701 Default font "<< std::endl;
	myLCD.setFont(font_default);
	char TestStr[] = "Default";
	char InvertedStr[] = "Invert";
	myLCD.writeChar(0,0 ,'a');
	myLCD.writeCharString(0,9, TestStr);
	myLCD.setCursor(5,19);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,29 ,'5');
	myLCD.writeCharString(0,29, InvertedStr);
	myLCD.setCursor(5,37);
	myLCD.print("invert p");
	screenReset();
}

void Test702(void)
{
	std::cout << "Test 702 Wide font, NO LOWERCASE LETTERS "<< std::endl;
	myLCD.setFont(font_gll);
	char TestStr[] = "GLL FONT";
	char InvertedStr[] = "INVERT";
	myLCD.writeChar(0,0 ,'7');
	myLCD.writeCharString(0,9, TestStr);
	myLCD.setCursor(5,19);
	myLCD.print(-45.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,29 ,'0');
	myLCD.writeCharString(0,29, InvertedStr);
	myLCD.setCursor(5,37);
	myLCD.print("INV 2!");
	screenReset();
}

void Test703(void){
	std::cout << "Test 703 PICO font"<< std::endl;
	myLCD.setFont(font_pico);
	myLCD.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Invert";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(0,14);
	myLCD.print(-3.98);
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,30, InvertedStr);
	screenReset();
}

void Test704(void){
	std::cout << "Test 704 Sinclair font "<< std::endl;
	myLCD.setFont(font_sinclairS);
	myLCD.setInvertFont(false);
	char TestStr[] = "Sinclair";
	char InvertedStr[] = "Invert";
	myLCD.writeChar(0,0 ,'u');
	myLCD.writeCharString(10,10, TestStr);
	myLCD.setCursor(5,19);
	myLCD.print(-3.14);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,29 ,'5');
	myLCD.writeCharString(10,29, InvertedStr);
	myLCD.setCursor(0,37);
	myLCD.print("p inv 3");
	screenReset();
}

void Test705(void)
{
	std::cout <<"Test 705 Orla font "<< std::endl;
	myLCD.setFont(font_orla);
	myLCD.setInvertFont(false);
	char TestStr[] = "Orla";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1,24);
	myLCD.print("inv");
	screenReset();
}

void Test706(void)
{
	std::cout << "Test 706 retro font"<< std::endl;
	myLCD.setFont(font_retro);
	myLCD.setInvertFont(false);
	char TestStr[] = "Retro";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(0,16);
	myLCD.print(14.19);
	myLCD.setInvertFont(true);
	myLCD.setCursor(0,32);
	myLCD.print("invert");
	screenReset();
}

void Test707(void){
	std::cout << "Test 707 Mega font"<< std::endl;
	myLCD.setFont(font_mega);
	myLCD.setInvertFont(false);
	char TestStr[] = "Mega";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(99);
	myLCD.setInvertFont(true);
	myLCD.setCursor(5,32);
	myLCD.print("inv");
	screenReset();
}

void Test708(void){
	std::cout << "Test 708 Arial bold font"<< std::endl;
	myLCD.setFont(font_arialBold);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial";
	char InvertedStr[] = "Inv";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.1415, 1); // print-3.1
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	screenReset();
}

void Test709(void)
{
	std::cout << "Test 709 hallFetica font"<< std::endl;
	myLCD.setFont(font_hallfetica);
	myLCD.setInvertFont(false);
	char TestStr[] = "Hall";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.17,1);
	myLCD.setInvertFont(true);
	myLCD.setCursor(5,32);
	myLCD.print("inv");
	screenReset();
}

void Test710(void){
	std::cout << "Test 710 Arial Round font"<< std::endl;
	myLCD.setFont(font_arialRound);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setInvertFont(true);
	myLCD.setCursor(1,25);
	myLCD.print("inv");
	screenReset();
}

void Test711(void)
{
	std::cout << "Test 711 GroTesk font"<< std::endl;
	myLCD.setFont(font_groTesk);
	myLCD.setInvertFont(false);
	char TestStr[] = "GroT";
	myLCD.writeCharString(0,0, TestStr);
	screenReset();
}

void Test712(void)
{
	std::cout << "Test 712 Sixteen segment font"<< std::endl;
	myLCD.setFont(font_sixteenSeg);
	myLCD.setInvertFont(false);

	myLCD.setCursor(0,0);
	myLCD.print(-3);
	screenReset();
	myLCD.setInvertFont(true);
	char TestStr1[] = "12";
	myLCD.writeCharString(0,0, TestStr1);
	screenReset();
}

void Test713()
{
	std::cout << "Test 713 print ASCII font 32-90 "<< std::endl;
	char i;
	myLCD.setFont(font_default);
	myLCD.setInvertFont(false);
	myLCD.setCursor(0, 0);
	// Print out the full ASCII font for default font
	for (i = 32; i < 90; i++)
	{
		myLCD.print(i);
	}
	screenReset();

}

void Test714(void)
{
	std::cout << "Test 714 Base number systems using print "<< std::endl;
	myLCD.setFont(font_default);
	myLCD.setCursor(0, 0);
	myLCD.print(47, RDL_DEC);
	myLCD.setCursor(0, 9);
	myLCD.print(47, RDL_HEX);
	myLCD.setCursor(0, 20);
	myLCD.print(47, RDL_BIN);
	myLCD.setCursor(0, 29);
	myLCD.print(47 ,RDL_OCT);
	screenReset();
}

void Test715(void)
{
	std::cout << "Test 715-a println + textwrap "<< std::endl;
	myLCD.setCursor(0, 0);
	myLCD.println("1st line");
	myLCD.print("2nd Line");
	myLCD.setCursor(0, 20);
	myLCD.setFont(font_gll);
	myLCD.print("123456789ABCDEFGHIJ");
	screenReset();
	std::cout << "Test 715-b writeText new line "<< std::endl;
	char TestStr1[] = "123456789ABF";
	myLCD.writeCharString(0,0, TestStr1);
	screenReset();
}

void Test716(void)
{
	std::cout << "Test 716 print method + string objects "<< std::endl;
	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myLCD.setFont(font_default);
	myLCD.setCursor(5,5);
	myLCD.print(timeInfo);
	// print a new line with println
	myLCD.setCursor(5,32);
	myLCD.println(newLine); // print a new line feed with println
	myLCD.print(newLine);
	screenReset();
}

void Test717(void)
{
	std::cout << "Test 717-a print method numbers" << std::endl;
	myLCD.setCursor(0, 0);
	myLCD.println(3.986,2 ); //print 3.99
	myLCD.println(4001);
	myLCD.println(-87.39);
	myLCD.println(-19);
	myLCD.println("hello");
	myLCD.setInvertFont(true);
	myLCD.print('P');
	myLCD.setInvertFont(false);
	screenReset();

	printf("Test 717-b print vectors\r\n");
	myLCD.setFont(font_gll);
	// For a vector of floats
	myLCD.setCursor(0, 0);
	std::vector<float> floatVec = {1.0, 22.004, -3.149823, 478.55434};
	myLCD.print(floatVec);     // Output 2 decimal places
	screenReset();
	myLCD.setCursor(0, 0);
	myLCD.print(floatVec, 2); // Output 2 decimal places
	screenReset();
	myLCD.setCursor(0, 0);
	myLCD.print(floatVec, 1); // Output 1 decimal places
	screenReset();
	// For a vector of integers
	std::vector<int> intVec = {47, 11, 34};
	myLCD.setCursor(0, 0);
	myLCD.print( intVec[0]);  // print 47
	myLCD.setCursor(0, 9);  
	myLCD.print( intVec[0], RDL_HEX);  // print 2F
	myLCD.setCursor(0, 20);
	myLCD.print( intVec[0] ,RDL_OCT); //print 57
	myLCD.setCursor(0, 30);
	myLCD.print( intVec[0], RDL_BIN); // print 101111
	screenReset();

	myLCD.setCursor(0, 0);
	myLCD.print( intVec); // 47 11 34
	screenReset();

	//For a vector of strings
	myLCD.setCursor(0, 0);
	std::vector<std::string> stringVec = {"HELLO", "VECTOR"};
	myLCD.print(stringVec); // Output: "HELLO VECTOR"
	screenReset();
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
	char testNonNumExtend[] = "-;A";
	bool errorFlag = false;
	
	// character out of font bounds
	// gll lower case + ]
	myLCD.setFont(font_gll);
	returnValues.push_back(myLCD.writeChar(32, 0, '!')); //success
	returnValues.push_back(myLCD.writeCharString(5,  5, testlowercase)); //throw gll font error 2
	screenReset();
	// Numeric extended bounds ; , A errors
	myLCD.setFont(font_sixteenSeg);
	returnValues.push_back(myLCD.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myLCD.writeChar(32, 0, ',')); //throw error 2
	screenReset();
	printf("========\r\n");
	// screen out of bounds
	myLCD.setFont(font_default);
	returnValues.push_back(myLCD.writeChar(0, 100, 'e')); //throw error 1
	returnValues.push_back(myLCD.writeChar(200, 0, 'f')); //throw error 1
	screenReset();
	myLCD.setFont(font_orla);
	returnValues.push_back(myLCD.writeChar(0, 100, 'A')); //throw error 1
	returnValues.push_back(myLCD.writeChar(200, 0, 'B')); //throw error 1
	screenReset();
	
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

void textDrawPixel(void) {
	std::cout << "Test 202 Draw pixels" << std::endl;
	myLCD.drawPixel(2, 5, RDL_BLACK);
	myLCD.drawPixel(10, 10, RDL_BLACK);
	myLCD.drawPixel(20, 20, RDL_BLACK);
	myLCD.drawPixel(30, 30, RDL_BLACK);
	myLCD.drawPixel(32, 35, RDL_BLACK);
	screenReset();
}

void testdrawcircle(void) {
	myLCD.drawCircle(20, 20, 10, RDL_BLACK);
	screenReset();
}

void testfillcircle(void)
{
	myLCD.fillCircle(60, 20, 10, RDL_BLACK);
	screenReset();
}

void testFillRect(void) {

	myLCD.fillRect(0, 20, 10, 30, RDL_BLACK);
	screenReset();
}

void testDrawTriangle(void) {
	myLCD.drawTriangle(30, 20, 50, 5, 80, 20, RDL_BLACK);
	screenReset();
}

void testFillTriangle(void) {

	myLCD.fillTriangle(10, 20, 40, 5, 80, 20, RDL_BLACK);
	screenReset();
}

void testDrawRoundRect(void) {
	myLCD.drawRoundRect(20, 20, 15 , 15, 5, RDL_BLACK);
	screenReset();
}

void testFillRoundRect(void) {
	myLCD.fillRoundRect(60, 20, 15, 15, 5, RDL_BLACK);
	screenReset();
}

void testDrawRect(void) {
	myLCD.drawRect(20, 15, 20, 15, RDL_BLACK);
	screenReset();
}

void testDrawLine(void) {

	std::cout << "Test 203 Draw lines " << std::endl;
	myLCD.drawLine(0, 0, 40, 20, RDL_BLACK);
	myLCD.drawFastHLine(50, 20, 10 , RDL_BLACK);
	myLCD.drawFastVLine(70, 20 ,10, RDL_BLACK);
	screenReset();
}

void testSleepMode(void) {

	std::cout << "Test 502 Sleep mode" << std::endl;
	char testStr1[]= "Sleep      5 secs!\r\n";
	char testStr2[]= "Awake!\r\n";

	myLCD.LCDdisplayClear();
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.LCDdisplayUpdate();

	delayMilliSecRDL(TEST_DELAY2);
	myLCD.LCDenableSleep();
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDdisableSleep();

	myLCD.print(testStr2);
	myLCD.LCDdisplayUpdate();
	delayMilliSecRDL(TEST_DELAY2);
	screenReset();
}

void testRotate(void) {
	std::cout << "Test 503 rotate mode" << std::endl;
	char testStr0[]= "Rotate  0\r\n";
	char testStr1[]= "Rotate   90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate   270\r\n";
	char testStr4[]= "Bottom\n";

	// rotation example
	myLCD.LCDdisplayClear();
	myLCD.setCursor(0, 0);

	myLCD.setRotation(displayBC_Degrees_90); // rotate 90 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(displayBC_Degrees_180); // rotate 180 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr2);
	myLCD.setCursor(0,30);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(displayBC_Degrees_270); // rotate 270 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr3);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	// revert back to no rotation
	myLCD.setRotation(displayBC_Degrees_0);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr0);
	myLCD.setCursor(0,30);
	myLCD.print(testStr4);
	screenReset();
}

void testInvert(void)
{
	std::cout << "Test 504 invert screen command" << std::endl;

	screenReset();
	myLCD.setCursor(5, 5);
	myLCD.print("INVERT 12");
	screenReset();

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDinvertDisplay(false);
	delayMilliSecRDL(TEST_DELAY5);
}

void testFillScreen(void){
	std::cout << "Test 501 fill screen" << std::endl;
	myLCD.LCDfillScreen();
	screenReset();
	myLCD.LCDfillScreenPattern(0x71);
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}

// *************** EOF ****************
