/*!
	@file examples/bicolor_displays/nokia5110/TEXT_GRAPHICS_FUNCTIONS/main.cpp
	@author Gavin Lyons
	@brief Test file for Nokia 5110 display showing use of Text,graphics & functions.
		Project Name: Display_Lib_RPI

	@test
		-# Test 501 fill + clear screen
		-# Test 502 Sleep
		-# Test 503 Rotation
		-# Test 504 invert screen command
		-# Test 701 Default font
		-# Test 702 Wide font
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
		-# Test 713 Display ASCII font 0-255 default font
		-# Test 714 Base number systems using print method
		-# Test 715 println + print & textwrap
		-# Test 716 print method String object
		-# Test 717 print method numbers
		-# Test 720 Text methods error Checking
		-# Test 902 Draw pixel
		-# Test 903 Draw lines
		-# Test 904 Draw shapes
*/


// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <cstdio>
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver


// *********** Test setup Defines **************
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// **************** GPIO ***************
#define RST_LCD 25
#define DC_LCD 24

// **************** LCD ***************
#define myLCD_WIDTH 84
#define myLCD_HEIGHT 48
#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

// ********************* HW SPI *****************
const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1

NOKIA_5110_RPI myLCD(myLCD_WIDTH, myLCD_HEIGHT , RST_LCD, DC_LCD, SPICLK_FREQ, SPI_CE_PIN);

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
	printf("Test 204 Draw shapes\n");
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
	printf("LCD Start\n");
	if(!bcm2835_init())
	{
		printf( "Error 1201 : Setup :Problem with init bcm2835 library\n");
		return false;
	}else{
		printf( "bcm2835 library version : %u\n",bcm2835_version());
	}
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(inverse, contrast, bias) != rpiDisplay_Success)
	{
		printf( "Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?\n");
		bcm2835_close(); // Close the bcm2835 library
		return false;
	}
	printf( "Nokia 5110 library version : %u\n",GetRDLibVersionNum());
	delayMilliSecRDL(250);
	return true;
}

void EndTests(void)
{
	myLCD.LCDSPIoff(); //Stop the hardware SPI
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	printf("LCD End\n");
}

void Test701(void)
{
	printf("LCD Test 701 Default font\r\n");
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
	printf("LCD Test 702 Wide font, NO LOWERCASE LETTERS\r\n");
	myLCD.setFont(font_wide);
	char TestStr[] = "WIDE FONT";
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
	printf("LCD Test 703 PICO font\r\n");
	myLCD.setFont(font_pico);
	myLCD.setInvertFont(false);
	char TestStr[] = "Pico font";
	char InvertedStr[] = "Inverted";
	myLCD.writeChar(0,0 ,'p');
	myLCD.writeCharString(0,9, TestStr);
	myLCD.setCursor(5,19);
	myLCD.print(-3.98);
	myLCD.setInvertFont(true);
	myLCD.writeChar(0,27 ,'6');
	myLCD.writeCharString(0,27, InvertedStr);
	myLCD.setCursor(5,40);
	myLCD.print("print invert");
	screenReset();
}

void Test704(void){
	printf("LCD Test 704 Sinclair font\r\n");
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
	printf("LCD Test 705 Orla font\r\n");
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
	printf("LCD Test 706 retro font\r\n");
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
	printf("LCD Test 707 Mega font\r\n");
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
	printf("LCD Test 708 Arial bold font\r\n");
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
	printf("LCD Test 709 hallFetica font\r\n");
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
	printf("LCD Test 710 Arial Round font\r\n");
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
	printf("LCD Test 711 GroTesk font\r\n");
	myLCD.setFont(font_groTesk);
	myLCD.setInvertFont(false);
	char TestStr[] = "GroT";
	myLCD.writeCharString(0,0, TestStr);
	screenReset();
}

void Test712(void)
{
	printf("LCD Test 712 Sixteen segment font\r\n");
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
	printf("LCD Test 713-a print ASCII font 0-255 \r\n");
	char i;
	myLCD.setFont(font_default);
	myLCD.setInvertFont(false);
	myLCD.setCursor(0, 0);
	// Print out the full ASCII font for default font
	for (i = 0; i < 83; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.print(i);
	}
	screenReset();

	myLCD.setCursor(0, 0);
	for (i = 84; i < 168; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.print(i);
	}
	screenReset();

	myLCD.setCursor(0, 0);
	for (i = 168; i < 250; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.print(i);
	}
	screenReset();
}

void Test714(void)
{
	printf("LCD Test 714 Base number systems using print \r\n");
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
	printf("LCD Test 715-a println + textwrap \r\n");
	myLCD.setCursor(0, 0);
	myLCD.println("first line");
	myLCD.print("2nd Line");
	myLCD.setCursor(0, 20);
	myLCD.setFont(font_wide);
	myLCD.print("123456789ABCDEFGHIJ");
	screenReset();
	printf("LCD Test 715-b writeText new line \r\n");
	char TestStr1[] = "123456789ABF";
	myLCD.writeCharString(0,0, TestStr1);
	screenReset();
}

void Test716(void)
{
	printf("LCD Test 716 print method + string objects \r\n");
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
	printf("LCD Test 717 print method numbers\r\n");
	myLCD.setCursor(0, 0);
	myLCD.println(3.986,2 ); //print 3.99
	myLCD.println(4001);
	myLCD.println(-87.39);
	myLCD.println(-19);
	myLCD.println("hello");
	myLCD.setInvertFont(true);
	myLCD.print('P');
	screenReset();
}

void testErrorCheck(void)
{
	// Error checking
	printf("==== Test 720 Start Error checking ====\r\n");
	printf("Result = 2 2 2 1 1 1 1 ===\r\n");
	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = ":;A";

	// character out of font bounds
	// wide & thick lower case + ]
	myLCD.setFont(font_wide);
	myLCD.writeCharString(5,  5, testlowercase); //throw wide font error 2
	myLCD.writeChar(32, 0, '!');
	screenReset();
	// Numeric extended bounds ; , A errors
	myLCD.setFont(font_sixteenSeg);
	myLCD.writeCharString(0, 0, testNonNumExtend); //throw font error 2
	myLCD.writeChar(32, 0, ','); //throw error 2
	screenReset();
	printf("========\r\n");
	// screen out of bounds
	myLCD.setFont(font_default);
	myLCD.writeChar(0, 100, 'e'); //throw error 1
	myLCD.writeChar(150, 0, 'f'); //throw error 1
	screenReset();
	myLCD.setFont(font_orla);
	myLCD.writeChar(0, 100, 'A'); //throw error 1
	myLCD.writeChar(150, 0, 'B'); //throw error 1
	screenReset();
	printf("==== Stop Error checking ====\r\n");
}

void textDrawPixel(void) {
	printf("Test 202 Draw pixels\n");
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

	printf("Test 203 Draw lines\n");
	myLCD.drawLine(0, 0, 40, 20, RDL_BLACK);
	myLCD.drawFastHLine(50, 20, 10 , RDL_BLACK);
	myLCD.drawFastVLine(70, 20 ,10, RDL_BLACK);
	screenReset();
}

void testSleepMode(void) {

	printf("Test 502 Sleep mode\n");
	char testStr1[]= "Sleep 5 secs!\r\n";
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
	printf("Test 503 rotate mode\n");
	char testStr0[]= "Rotate  0\r\n";
	char testStr1[]= "Rotate   90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate   270\r\n";
	char testStr4[]= "Bottom !\n";

	// rotation example
	myLCD.LCDdisplayClear();
	myLCD.setCursor(0, 0);

	myLCD.setRotation(displayBC_Degrees_90); // rotate 90 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.setCursor(0,76);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(displayBC_Degrees_180); // rotate 180 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr2);
	myLCD.setCursor(0,39);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(displayBC_Degrees_270); // rotate 270 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr3);
	myLCD.setCursor(0,76);
	myLCD.print(testStr4);
	screenReset();

	// revert back to no rotation
	myLCD.setRotation(displayBC_Degrees_0);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr0);
	myLCD.setCursor(0,39);
	myLCD.print(testStr4);
	screenReset();
}

void testInvert(void)
{
	printf("Test 504 invert screen command\n");

	screenReset();
	myLCD.setCursor(5, 5);
	myLCD.print("INVERT 123");
	screenReset();

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDinvertDisplay(false);
	delayMilliSecRDL(TEST_DELAY5);
}

void testFillScreen(void){
	printf("Test 501 fill screen\n");
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
