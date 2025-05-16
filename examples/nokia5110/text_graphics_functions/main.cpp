/*!
	@file examples/nokia5110/text_graphics_functions/main.cpp
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
		-# Test 901 Graphics shapes
		-# Test 902 Draw pixel
		-# Test 903 Draw lines
		-# Test 904 Advanced Graphic shapes
*/


// libraries

#include <cstdio>
#include <iostream>
#include "NOKIA_5110_LCD_RDL.hpp" // PCD8544 controller driver

/// @cond


// Test setup
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// GPIO
const uint8_t RST_LCD = 25; // GPIO pin number pick any you want
const uint8_t DC_LCD = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEV = 0; // GPIO chip device number usually 0

//  LCD
#define MY_LCD_WIDTH 84
#define MY_LCD_HEIGHT 48
#define FULLSCREEN (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
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

void Test901(void);
void Test902(void);
void Test903(void);
void Test904(void);

// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;

	Test501();
	Test502();
	Test503();
	Test504();

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

	Test901();
	Test902();
	Test903();
	Test904();
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
	std::cout<< "Nokia 5110 library version : " << rdlib::LibraryVersion() << std::endl;
	delayMilliSecRDL(250);
	if(myLCD.LCDBegin(LCD_INV, LCD_CST, LCD_BIAS, SPI_DEVICE, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		std::cout<< "Error 1201: Setup :Cannot start spi" << std::endl;
		return false;
	}
	if (myLCD.LCDSetBufferPtr(screenBuffer) != rdlib::Success)
	{
		std::cout<< "Error 1202: Setup :Cannot Assign Buffer" << std::endl;
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
	std::cout << "Test 702 gll font, NO LOWERCASE LETTERS "<< std::endl;
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
	myLCD.print( intVec[0], myLCD.RDL_HEX);  // print 2F
	myLCD.setCursor(0, 20);
	myLCD.print( intVec[0] ,myLCD.RDL_OCT); //print 57
	myLCD.setCursor(0, 30);
	myLCD.print( intVec[0], myLCD.RDL_BIN); // print 101111
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
	// Draw hexagon 
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
	// Draw upper half arc (180° to 360°)
	myLCD.drawArc(centerX, centerY, radius, 5, 180.0f, 360.0f, myLCD.BLACK);
	screenReset();
	delayMilliSecRDL(2000);

	// Draw third quarter arc (180° to 270°)
	myLCD.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myLCD.BLACK);
	screenReset();

	// Draw simple arc
	myLCD.drawSimpleArc(30, 24, 15, 0.0f, 300.0f, myLCD.BLACK);
	screenReset();
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

	// rotation example
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);

	myLCD.setRotation(myLCD.BC_Degrees_90); // rotate 90 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(myLCD.BC_Degrees_180); // rotate 180 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr2);
	myLCD.setCursor(0,30);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(myLCD.BC_Degrees_270); // rotate 270 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr3);
	myLCD.setCursor(0,70);
	myLCD.print(testStr4);
	screenReset();

	// revert back to no rotation
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

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDinvertDisplay(false);
	delayMilliSecRDL(TEST_DELAY5);
}

void Test501(void){
	std::cout << "Test 501 fill screen" << std::endl;
	myLCD.LCDfillScreen(0x71);
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDfillScreen();
}

void screenReset(void) {
	myLCD.LCDupdate();
	delayMilliSecRDL(TEST_DELAY5);
	myLCD.LCDclearBuffer();
}

// *************** EOF ****************

/// @endcond
