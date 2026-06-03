
/*!
	@file   examples/nokia5110/tests/main.cpp
	@author Gavin Lyons
	@brief  Unified test file carrying out Framerate, Typography, Graphics, and Bitmap tests.
			PCD8544 Nokia 5110 SPI LCD display Library
			Project Name: Display_Lib_RPI
	@test
		-# Test 301 Bitmap full screen
		-# Test 302 Vertically addressed data small bitmaps
		-# Test 303 horizontal addressed data small bitmaps
		-# Test 501 fill + clear screen
		-# Test 502 Sleep
		-# Test 503 Rotation
		-# Test 504 invert screen command
		-# Test 601 FPS test (frame rate per second)
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
		-# Test 901 Graphics shapes
		-# Test 902 Draw pixel
		-# Test 903 Draw lines
		-# Test 904 Advanced Graphic shapes
*/

// Libraries
#include <cstdio>
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include "Bitmap_test_data.hpp"   // Bitmap test data file
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

/// @cond

// Test Setup Delays
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// Framerate Variables (From main.cpp)
uint16_t count = 0;
bool colour = 1;
uint64_t previousCounter = 0;
uint16_t countLimit = 1500;

// GPIO Layout
const uint8_t RST_LCD = 25; 
const uint8_t DC_LCD = 24; 
int GPIO_CHIP_DEV = 0; 

// LCD Properties
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
#define FULLSCREEN (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
#define LCD_INV  false 
#define LCD_CST  0xB2 
#define LCD_BIAS 0x13 

// Hardware SPI Configuration
int SPI_DEVICE = 0; 
int SPI_CHANNEL = 0; 
int SPI_SPEED =  1000000; 
int SPI_FLAGS = 0; 

NOKIA_5110_RPI myLCD(MY_LCD_WIDTH, MY_LCD_HEIGHT, RST_LCD, DC_LCD);

// ======================= Function Prototypes =======================
bool Setup(void);
void EndTests(void);
void screenReset(void);

// Bitmap Test Functions
void Test301(void);
void Test302(void);
void Test303(void);

// Framerate Test Functions
void Test601(void);
void display_buffer(long currentFramerate, int count);
static uint64_t counter(void);

// text & Typography Test Functions
void Test501(void);
void Test502(void);
void Test503(void);
void Test504(void);
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

// Geometry & Graphics Test Functions
void Test901(void);
void Test902(void);
void Test903(void);
void Test904(void);

// ======================= Main Orchestrator =======================
int main()
{
	if (!Setup()) return -1;
	// 1. Bitmaps Tests
	myLCD.LCDclearBuffer();
	myLCD.setDrawBitmapAddr(true);
	Test301();
	Test302();
	Test303();
	// 2. Control & Screen Mode Tests
	Test501();
	Test502();
	Test503();
	Test504();
	// 3. Framerate test
	Test601();
	// 4. Fonts & Text Formatting Tests
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
	// 5. Primitive & Advanced Shapes Tests
	Test901();
	Test902();
	Test903();
	Test904();

	EndTests();
	return 0;
}

// ======================= Initialization & Cleanup =======================

bool Setup(void)
{
	std::cout << "LCD Start" << std::endl;
	std::cout << "lgpio library version : " << lguVersion() << std::endl;
	std::cout << "Nokia 5110 library version : " << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, SPI_DEVICE, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		std::cout << "Error 1201: Setup :Cannot start spi" << std::endl;
		return false;
	}
	if (myLCD.LCDSetBufferPtr(screenBuffer) != rdlib::Success)
	{
		std::cout << "Error 1202: Setup :Cannot Assign Buffer" << std::endl;
		return false;
	}
	delayMilliSecRDL(250);
	myLCD.LCDfillScreen();
	return true;
}

void EndTests(void)
{
	myLCD.LCDfillScreen();
	myLCD.LCDclearBuffer();
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	std::cout << "LCD End" << std::endl;
}

void screenReset(void) {
	myLCD.LCDupdate();
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDclearBuffer();
}

// ======================= Framerate Test Block =======================

void Test601() {
	std::cout << "Test 601 FPS test (frame rate per second)" << std::endl;
	std::cout << "Count Limit  = " << countLimit << std::endl;
	myLCD.setFont(font_default);
	while (count < countLimit)
	{
		static long framerate = 0;
		display_buffer(framerate, count);
		framerate++;
		count++;
	}
	myLCD.LCDclearBuffer();
}

void display_buffer(long currentFramerate, int count)
{
			// cppcheck-suppress variableScope
	static long lastFramerate = 0;
	static uint16_t fps;
	uint64_t currentCounter = counter();
	if(currentCounter - previousCounter >= 1000000000)
	{
		fps = currentFramerate - lastFramerate;
		lastFramerate = currentFramerate;
		previousCounter = currentCounter;
		colour = !colour;
	}
	
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);
	myLCD.print("Nokia");

	myLCD.setCursor(0, 10);
	myLCD.print("G Lyons");

	myLCD.setCursor(0, 20);
	myLCD.print(count);

	myLCD.setCursor(0, 30);
	myLCD.print(fps);
	myLCD.print(" fps");
	myLCD.setCursor(0, 40);
	myLCD.print(rdlib::LibraryVersion());

	myLCD.fillRect(60, 1, 20, 20, colour);
	myLCD.fillCircle(60, 35, 10, !colour);
	
	myLCD.LCDupdate();
}

static uint64_t counter(void)
{
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec; 
}

// ======================= Text & Typographic Tests =======================

void Test701(void)
{
	std::cout << "Test 701 Default font " << std::endl;
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
	std::cout << "Test 702 gll font, NO LOWERCASE LETTERS " << std::endl;
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
	std::cout << "Test 703 PICO font" << std::endl;
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
	std::cout << "Test 704 Sinclair font " << std::endl;
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
	std::cout << "Test 705 Orla font " << std::endl;
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
	std::cout << "Test 706 retro font" << std::endl;
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
	std::cout << "Test 707 Mega font" << std::endl;
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
	std::cout << "Test 708 Arial bold font" << std::endl;
	myLCD.setFont(font_arialBold);
	myLCD.setInvertFont(false);
	char TestStr[] = "Arial";
	char InvertedStr[] = "Inv";
	myLCD.writeCharString(0,0, TestStr);
	myLCD.setCursor(5,16);
	myLCD.print(-3.1415, 1); 
	myLCD.setInvertFont(true);
	myLCD.writeCharString(0,32, InvertedStr);
	screenReset();
}

void Test709(void)
{
	std::cout << "Test 709 hallFetica font" << std::endl;
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
	std::cout << "Test 710 Arial Round font" << std::endl;
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
	std::cout << "Test 711 GroTesk font" << std::endl;
	myLCD.setFont(font_groTesk);
	myLCD.setInvertFont(false);
	char TestStr[] = "GroT";
	myLCD.writeCharString(0,0, TestStr);
	screenReset();
}

void Test712(void)
{
	std::cout << "Test 712 Sixteen segment font" << std::endl;
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
	std::cout << "Test 713 print ASCII font 32-90 " << std::endl;
	char i;
	myLCD.setFont(font_default);
	myLCD.setInvertFont(false);
	myLCD.setCursor(0, 0);
	for (i = 32; i < 90; i++)
	{
		myLCD.print(i);
	}
	screenReset();
}

void Test714(void)
{
	std::cout << "Test 714 Base number systems using print " << std::endl;
	myLCD.setFont(font_default);
	myLCD.setCursor(0, 0);
	myLCD.print(47, myLCD.RDL_DEC);
	myLCD.setCursor(0, 9);
	myLCD.print(47, myLCD.RDL_HEX);
	myLCD.setCursor(0, 20);
	myLCD.print(47, myLCD.RDL_BIN);
	myLCD.setCursor(0, 29);
	myLCD.print(47 ,myLCD.RDL_OCT);
	screenReset();
}

void Test715(void)
{
	std::cout << "Test 715-a println + textwrap " << std::endl;
	myLCD.setCursor(0, 0);
	myLCD.println("1st line");
	myLCD.print("2nd Line");
	myLCD.setCursor(0, 20);
	myLCD.setFont(font_gll);
	myLCD.print("123456789ABCDEFGHIJ");
	screenReset();
	std::cout << "Test 715-b writeText new line " << std::endl;
	char TestStr1[] = "123456789ABF";
	myLCD.writeCharString(0,0, TestStr1);
	screenReset();
}

void Test716(void)
{
	std::cout << "Test 716 print method + string objects " << std::endl;
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myLCD.setFont(font_default);
	myLCD.setCursor(5,5);
	myLCD.print(timeInfo);
	myLCD.setCursor(5,32);
	myLCD.println(newLine); 
	myLCD.print(newLine);
	screenReset();
}

void Test717(void)
{
	std::cout << "Test 717-a print method numbers" << std::endl;
	myLCD.setCursor(0, 0);
	myLCD.println(3.986,2 ); 
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
	myLCD.setCursor(0, 0);
	std::vector<float> floatVec = {1.0, 22.004, -3.149823, 478.55434};
	myLCD.print(floatVec);     
	screenReset();
	myLCD.setCursor(0, 0);
	myLCD.print(floatVec, 2); 
	screenReset();
	myLCD.setCursor(0, 0);
	myLCD.print(floatVec, 1); 
	screenReset();
	
	std::vector<int> intVec = {47, 11, 34};
	myLCD.setCursor(0, 0);
	myLCD.print( intVec[0]);  
	myLCD.setCursor(0, 9);
	myLCD.print( intVec[0], myLCD.RDL_HEX);  
	myLCD.setCursor(0, 20);
	myLCD.print( intVec[0] ,myLCD.RDL_OCT); 
	myLCD.setCursor(0, 30);
	myLCD.print( intVec[0], myLCD.RDL_BIN); 
	screenReset();

	myLCD.setCursor(0, 0);
	myLCD.print( intVec); 
	screenReset();

	myLCD.setCursor(0, 0);
	std::vector<std::string> stringVec = {"HELLO", "VECTOR"};
	myLCD.print(stringVec); 
	screenReset();
}

// ======================= Display Modes & Control =======================

void Test501(void){
	std::cout << "Test 501 fill screen" << std::endl;
	myLCD.LCDfillScreen(0x71);
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDfillScreen();
}

void Test502(void) {
	std::cout << "Test 502 Sleep mode" << std::endl;
	char testStr1[]= "Sleep      5 secs!\r\n";
	char testStr2[]= "Awake!\r\n";

	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.LCDupdate();

	delayMilliSecRDL(TEST_DELAY2);
	myLCD.LCDenableSleep();
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDdisableSleep();

	myLCD.print(testStr2);
	myLCD.LCDupdate();
	delayMilliSecRDL(TEST_DELAY2);
	screenReset();
}

void Test503(void) {
	std::cout << "Test 503 rotate mode" << std::endl;
	char testStr0[]= "Rotate  0\r\n";
	char testStr1[]= "Rotate   90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate   270\r\n";
	char testStr4[]= "Bottom\n";

	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);

	myLCD.setRotation(myLCD.BC_Degrees_90); 
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(myLCD.BC_Degrees_180); 
	myLCD.setCursor(0, 0);
	myLCD.print(testStr2);
	myLCD.setCursor(0,30);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(myLCD.BC_Degrees_270); 
	myLCD.setCursor(0, 0);
	myLCD.print(testStr3);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(myLCD.BC_Degrees_0);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr0);
	myLCD.setCursor(0,30);
	myLCD.print(testStr4);
	screenReset();
}

void Test504(void)
{
	std::cout << "Test 504 invert screen command" << std::endl;
	screenReset();
	myLCD.setCursor(5, 5);
	myLCD.print("INVERT 12");
	screenReset();

	myLCD.LCDinvertDisplay(true);
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDinvertDisplay(false);
	delayMilliSecRDL(TEST_DELAY5);
}

// ======================= Geometry & Graphics Tests =======================

void Test901(void){
	std::cout << "Test 901 Draw  shapes" << std::endl;
	myLCD.drawCircle(20, 20, 10, myLCD.BLACK);
	screenReset();
	myLCD.fillCircle(60, 20, 10, myLCD.BLACK);
	screenReset();
	myLCD.fillRect(0, 20, 10, 30, myLCD.BLACK);
	screenReset();
	myLCD.drawTriangle(30, 20, 50, 5, 80, 20, myLCD.BLACK);
	screenReset();
	myLCD.fillTriangle(10, 20, 40, 5, 80, 20, myLCD.BLACK);
	screenReset();
	myLCD.drawRoundRect(20, 20, 15 , 15, 5, myLCD.BLACK);
	screenReset();
	myLCD.fillRoundRect(60, 20, 15, 15, 5, myLCD.BLACK);
	screenReset();
	myLCD.drawRect(20, 15, 20, 15, myLCD.BLACK);
	screenReset();
}

void Test902(void) {
	std::cout << "Test 902 Draw pixels" << std::endl;
	myLCD.drawPixel(2, 5, myLCD.BLACK);
	myLCD.drawPixel(10, 10, myLCD.BLACK);
	myLCD.drawPixel(20, 20, myLCD.BLACK);
	myLCD.drawPixel(30, 30, myLCD.BLACK);
	myLCD.drawPixel(32, 35, myLCD.BLACK);
	screenReset();
}

void Test903(void) {
	std::cout << "Test 903 Draw lines " << std::endl;
	myLCD.drawLine(0, 0, 40, 20, myLCD.BLACK);
	myLCD.drawFastHLine(50, 20, 10 , myLCD.BLACK);
	myLCD.drawFastVLine(70, 20 ,10, myLCD.BLACK);
	screenReset();
}

void Test904(void) {
	std::cout << "Test 904 Draw more shapes" << std::endl;

	std::cout << "Polygons" << std::endl;
	myLCD.drawPolygon(42, 24, 6, 18, 0, false, myLCD.BLACK);
	myLCD.drawPolygon(42, 24, 6, 18, 0, true, myLCD.BLACK);
	delayMilliSecRDL(200);
	screenReset();

	std::cout << "Dot grid" << std::endl;
	myLCD.drawDotGrid(0, 0, MY_LCD_WIDTH, MY_LCD_HEIGHT, 5, myLCD.BLACK);
	screenReset();

	std::cout << "Quadrilateral" << std::endl;
	int16_t x8 = 20, y8 = 20;
	int16_t x9 = 25, y9 = 40;
	int16_t x10 = 55, y10 = 40;
	int16_t x11 = 64, y11 = 20;
	myLCD.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myLCD.BLACK);
	screenReset();
	myLCD.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myLCD.BLACK);
	screenReset();
	
	std::cout << "Ellipse" << std::endl;
	myLCD.drawEllipse(42, 24, 18, 12, false, myLCD.BLACK);
	screenReset();
	myLCD.drawEllipse(42, 24, 18, 12, true, myLCD.BLACK);
	screenReset();

	std::cout << "Drawing Arc: drawArc and drawSimpleArc" << std::endl;
	int16_t centerX = 42;
	int16_t centerY = 30;
	int16_t radius = 20;

	std::cout << "  Angle offset default: " << myLCD.getArcAngleOffset() << std::endl;
	myLCD.drawArc(centerX, centerY, radius, 5, 180.0f, 360.0f, myLCD.BLACK);
	screenReset();
	delayMilliSecRDL(2000);

	myLCD.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myLCD.BLACK);
	screenReset();

	myLCD.drawSimpleArc(30, 24, 15, 0.0f, 300.0f, myLCD.BLACK);
	screenReset();
}

// ======================= Bitmap Tests =======================

void Test301(void)
{
	std::cout << "Test 301 Bitmap full screen" << std::endl;
	myLCD.drawBitmap(0,0, backUpIcon84x48, MY_LCD_WIDTH, MY_LCD_HEIGHT, myLCD.BLACK,myLCD.WHITE); 
	screenReset();
}

void Test302(void)
{
	std::cout << "Test 302 Vertically addressed data bitmaps " << std::endl;
	myLCD.drawBitmap(5, 5, SignalIconVa, 16, 8, myLCD.BLACK,myLCD.WHITE);
	myLCD.drawBitmap(60, 5, TemperatureImageVA, 16, 16, myLCD.BLACK, myLCD.WHITE);
	screenReset();
}

void Test303(void)
{
	std::cout << "Test 303 Horizontally addressed data bitmaps " << std::endl;
	myLCD.setDrawBitmapAddr(false);
	myLCD.drawBitmap(5, 5, BatIconHa, 16, 8, myLCD.BLACK,myLCD.WHITE);
	myLCD.drawBitmap(25, 5, BatIconHa, 16, 8, myLCD.WHITE,myLCD.BLACK);
	screenReset();
}

/// @endcond
