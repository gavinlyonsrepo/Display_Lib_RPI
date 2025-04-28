/*!
	@file examples/ssd1331/text_graphics_functions/main.cpp
	@brief Library test file, tests Text,graphics & functions.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function and in myOLED constructor
	@test
	-# Test 500 RGB color OK?
	-# Test 501 change modes test -> Invert, display on/off and Sleep.
	-# Test 502 Rotate
	-# Test 503 windows command tests
	-# Test 504 Scroll
	-# Test 705 print method all fonts
	-# Test 706 Misc print class tests (string object, println invert, wrap, base nums)
	-# Test 902 rectangles
	-# Test 903 Circle
	-# Test 904 Triangles
	-# Test 905 Polygons
	-# Test 906 Dot Grid
	-# Test 907 Quadrilateral
	-# Test 908 Ellipse
	-# Test 909 Draw Arc and draw simple arc
*/

// Section ::  libraries
#include <iostream> // cout
#include "SSD1331_OLED_RDL.hpp"

/// @cond

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode

SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);

int8_t RST_OLED  = 25;
int8_t DC_OLED   = 24;
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

uint16_t OLED_WIDTH = 96;// Screen width in pixels
uint16_t OLED_HEIGHT = 64; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers

uint8_t Setup(void);
void EndTests(void);
void DisplayReset(void);

void Test500(void);
void Test501(void);
void Test502(void);
void Test503(void);
void Test504(void); 

void Test701(void);
void Test705(void);
void Test706(void);

void Test902(void);
void Test903(void);
void Test904(void);
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);
void colorBlock(void);

//  Section ::  MAIN loop

int main(void)
{
	if(Setup() != 0)return -1;

	Test500();
	Test501();
	Test502();
	Test503();
	Test504();
	Test701();
	Test705();
	Test706();
	Test902();
	Test903();
	Test904();
	Test905();
	Test906();
	Test907();
	Test908();
	Test909();

	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	std::cout << "OLED Start" << std::endl;
	std::cout << "SSD1331 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myOLED.OLEDSetupGPIO(RST_OLED, DC_OLED);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myOLED.OLEDInitScreenSize(OLED_WIDTH , OLED_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI settings**
	if(myOLED.OLEDInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
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
	std::cout << "If wrong colour check color order setting in constructor" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.setTextColor(myOLED.RDLC_GREEN,myOLED.RDLC_BLACK);
	colorBlock();
	delayMilliSecRDL(TEST_DELAY5);

}

void colorBlock(void)
{
	myOLED.fillRect(0, 0, 60, 8, myOLED.RDLC_RED);
	myOLED.fillRect(0, 8, 60, 8, myOLED.RDLC_GREEN);
	myOLED.fillRect(0, 16, 60, 8, myOLED.RDLC_BLUE);
	myOLED.fillRect(0, 24, 60, 8, myOLED.RDLC_YELLOW);
	myOLED.fillRect(0, 32, 60, 8, myOLED.RDLC_WHITE);
}

void Test501()
{
	std::cout << "Test 501: Mode Tests" << std::endl;
	// Invert on and off
	myOLED.OLEDchangeDisplayMode(myOLED.DISPLAY_INVERSE);
	std::cout << "Test 501-1: Invert on " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.OLEDchangeDisplayMode(myOLED.DISPLAY_NORMAL);
	std::cout << "Test 501-2: Invert off " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);

	// ALL pixels on and off
	myOLED.OLEDchangeDisplayMode(myOLED.DISPLAY_ALL_OFF);
	std::cout << "Test 501-3: All pixels off " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.OLEDchangeDisplayMode(myOLED.DISPLAY_ALL_ON);
	std::cout << "Test 501-4: All pixels on  " << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.OLEDchangeDisplayMode(myOLED.DISPLAY_NORMAL);
	delayMilliSecRDL(TEST_DELAY2);

	// sleep mode
	myOLED.OLEDsleepDisplay();
	std::cout << "Test 501-5: Display off, sleep mode" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.OLEDenableDisplay();
	std::cout << "Test 501-6: Turn Display back on" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);

	// dim mode
	myOLED.OLEDDimDisplay();
	std::cout << "Test 501-7: Display dim mode on" << std::endl;
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.OLEDenableDisplay();
	std::cout << "Test 501-8: Display dim mode off" << std::endl;
	delayMilliSecRDL(TEST_DELAY2);
}


void Test502(void)
{
	std::cout << "Test 502: Window command Tests" << std::endl;
	std::cout << "Test 502-1 line draw command OLED command (not using graphics class)" << std::endl;
	myOLED.OLEDDrawLineCmd(0, 0, 50, 50, myOLED.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY5);
	std::cout << "Test 502-2 draw rect OLED command (not using graphics class)" << std::endl;
	myOLED.OLEDDrawRectCmd(0, 0, 80, 40, myOLED.RDLC_YELLOW, false);
	delayMilliSecRDL(TEST_DELAY5);
	std::cout << "Test 502-3 fill rect OLED command (not using graphics class)" << std::endl;
	myOLED.OLEDDrawRectCmd(0, 0, 95, 63, myOLED.RDLC_CYAN, true);
	delayMilliSecRDL(TEST_DELAY5);
	std::cout << "Test 502-4 Window clear command" << std::endl;
	myOLED.OLEDClearWindowCmd(0, 0, 40, 40);
	delayMilliSecRDL(TEST_DELAY5);

	std::cout << "Test 502-5 Window copy command" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.fillRect(0, 0, 20, 20, myOLED.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY);
	myOLED.OLEDCopyWindowCmd(0,0, 20, 20, 76,0);
	delayMilliSecRDL(TEST_DELAY5);

	std::cout << "Test 502-6 Window Dim command" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY);
	myOLED.OLEDDimWindowCmd(0, 0, 40, 40);
	delayMilliSecRDL(TEST_DELAY5);
	
}

void Test503()
{
	std::cout << "Test 503: Rotate" << std::endl;
	myOLED.setFont(font_default);
	myOLED.setTextColor(myOLED.RDLC_RED,myOLED.RDLC_BLACK);
	char teststr0[] = "Rot 0";
	char teststr1[] = "Rot 90";
	char teststr2[] = "Rot 180";
	char teststr3[] = "R 270";

	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.OLEDsetRotation(myOLED.Degrees_0);
	myOLED.writeCharString(0, 0, teststr0);
	myOLED.writeCharString(0, 50, teststr0);
	delayMilliSecRDL(TEST_DELAY2);

	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.OLEDsetRotation(myOLED.Degrees_90);
	myOLED.writeCharString(0, 0, teststr1);
	myOLED.writeCharString(0, 50, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.OLEDsetRotation(myOLED.Degrees_180);
	myOLED.writeCharString(0, 0, teststr2);
	myOLED.writeCharString(0, 50, teststr2);
	delayMilliSecRDL(TEST_DELAY2);

	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.OLEDsetRotation(myOLED.Degrees_270);
	myOLED.writeCharString(0, 0, teststr3);
	myOLED.writeCharString(0, 50, teststr3);
	delayMilliSecRDL(TEST_DELAY2);

	myOLED.OLEDsetRotation(myOLED.Degrees_0);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}


void Test504(void)
{
	printf("Test 504: Scroll\r\n");
	 // test horizontal scroll slow 
	printf("Test 504-1: Scroll test horizontal\r\n");
	colorBlock();
	uint8_t  horOffset = 95;
	uint8_t startRowAddr = 0; 
	uint8_t numOfRows = 62;
	uint8_t verOffset = 0; 
	myOLED.OLEDScrollSetup(horOffset, startRowAddr,  numOfRows, verOffset , myOLED.SCROLL_FRAME_6);
	myOLED.OLEDScroll(myOLED.SCROLL_ACTIVATE);
	delayMilliSecRDL(10000);
	myOLED.OLEDScroll(myOLED.SCROLL_DEACTIVE);
	myOLED.fillScreen(myOLED.RDLC_BLACK);

	// test horizontal scroll slow 
	printf("Test 504-2: Scroll test Vertical\r\n");
	colorBlock();
	horOffset = 0;
	numOfRows = 0;
	verOffset = 62; 
	myOLED.OLEDScrollSetup(horOffset, startRowAddr,  numOfRows, verOffset , myOLED.SCROLL_FRAME_10);
	myOLED.OLEDScroll(myOLED.SCROLL_ACTIVATE);
	delayMilliSecRDL(10000);
	myOLED.OLEDScroll(myOLED.SCROLL_DEACTIVE);
	DisplayReset();
}

void Test701(void) {

	std::cout << "Test 701: Print out some fonts with writeCharString" << std::endl;
	char teststr1[] = "Default ";
	char teststr2[] = "GLL ";
	char teststr7[] = "Mega";
	char teststr8[] = "Arial";
	myOLED.fillScreen(myOLED.RDLC_BLACK);

	myOLED.setFont(font_default);
	myOLED.writeCharString(5, 5, teststr1);
	myOLED.setFont(font_gll);
	myOLED.writeCharString(5, 15, teststr2);
	DisplayReset();

	myOLED.setFont(font_mega);
	myOLED.writeCharString(5, 5, teststr7);
	myOLED.setFont(font_arialBold);
	myOLED.writeCharString(5, 30, teststr8);
	DisplayReset();

}

void Test705(void)
{
	std::cout << "Test 705: Print class methods" << std::endl;
	myOLED.setTextColor(myOLED.RDLC_WHITE, myOLED.RDLC_BLACK);

	myOLED.setCursor(5,5);
	myOLED.setFont(font_default);
	myOLED.print("Default ");
	myOLED.print(-43);
	DisplayReset();
	myOLED.setCursor(5,5);
	myOLED.setFont(font_gll);
	myOLED.print("GLL ");
	myOLED.print(123.284,1); // print 123.3
	DisplayReset();
	myOLED.setCursor(5,5);
	myOLED.setFont(font_retro);
	myOLED.print("retro ");
	myOLED.print(-3.14);
	DisplayReset();

	// Test Font orla 
	myOLED.setFont(font_orla);
	myOLED.setCursor(5,5);
	myOLED.println(-7.16);
	myOLED.print("Orla");
	DisplayReset();

	// Test font seven segment
	myOLED.setCursor(0,0);
	myOLED.setFont(font_sevenSeg);
	myOLED.println(12);
	DisplayReset();

}

void Test706(void)
{
	std::cout << "Test 706: Misc print class(string object, println invert, wrap, base nums etc)" << std::endl;
	//Inverted print fonts 1-6
	myOLED.setTextColor(myOLED.RDLC_RED, myOLED.RDLC_YELLOW);
	myOLED.setFont(font_default);

	myOLED.setFont(font_sinclairS);
	myOLED.setCursor(5,55);
	myOLED.print(-49);

	myOLED.setFont(font_mega);
	myOLED.setCursor(5,5);
	myOLED.print("ABCD");
	DisplayReset();

	// Inverted print font
	myOLED.setTextColor(myOLED.RDLC_YELLOW, myOLED.RDLC_RED);
	myOLED.setFont(font_gll);
	myOLED.setCursor(5,5);
	myOLED.print("INVERT");
	myOLED.setCursor(5,5);
	myOLED.print(94.98259112, 5);
	DisplayReset();

	// Test print with DEC BIN OCT HEX
	myOLED.setTextColor(myOLED.RDLC_WHITE, myOLED.RDLC_BLACK);
	uint8_t numPos = 47;
	myOLED.setFont(font_default);
	myOLED.setCursor(5,5);
	myOLED.print(numPos , myOLED.RDL_DEC); // 47
	myOLED.setCursor(5,20);
	myOLED.print(numPos , myOLED.RDL_BIN); // 10111
	myOLED.setCursor(5,35);
	myOLED.print(numPos , myOLED.RDL_OCT); // 57
	myOLED.setCursor(5,50);
	myOLED.print(numPos , myOLED.RDL_HEX); // 2F
	DisplayReset();

	// Test print a string object with print
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myOLED.setCursor(5, 5);
	myOLED.print(timeInfo);
	// print a new line with println
	myOLED.setCursor(5,20);
	myOLED.println(newLine); // print a new line feed with println
	myOLED.print(newLine);

	//text wrap with print
	myOLED.setCursor(5,40);
	myOLED.print("1234567890123456");
	DisplayReset();
}

void Test902(void) {
	std::cout << "Test 902: rectangles " << std::endl;
	rdlib::Return_Codes_e returnValue;
	myOLED.drawRectWH(0, 0, 20, 20, myOLED.RDLC_RED);
	returnValue = myOLED.fillRectangle(0, 25, 20, 20, myOLED.RDLC_YELLOW);
	if (returnValue != rdlib::Success)
	{
		std::cout << "Warning : Test OLEDfillRectangle, An error occurred returnValue =" << +returnValue << std::endl;
	}
	myOLED.fillRect(35, 40, 20, 20, myOLED.RDLC_GREEN);
	
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.drawRoundRect(5, 5, 50, 50, 5, myOLED.RDLC_CYAN);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.fillRoundRect(5, 5, 50, 50, 18, myOLED.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void Test903(void) {
	std::cout << "Test 903 & 904 : Triangles and circles" << std::endl;
	myOLED.drawCircle(30, 35, 15, myOLED.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.fillCircle(30, 30, 15, myOLED.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void Test904(void) {
	myOLED.fillTriangle(15, 20, 75, 20, 45, 55, myOLED.RDLC_RED);
	myOLED.drawTriangle(5, 40, 30, 60, 10, 60, myOLED.RDLC_GREEN);
	myOLED.drawTriangle(66, 40, 90, 50, 80, 60, myOLED.RDLC_BLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void Test905(void)
{
	std::cout << "Test 905 : polygons" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	//drawPolygon( x,  y, sides,  diameter, rotation,fill, color);
	// hexagon 6 sides
	myOLED.drawPolygon(60, 30, 6, 25, 0, false, myOLED.RDLC_WHITE);
	myOLED.drawPolygon(60, 30, 6, 25, 0, true, myOLED.RDLC_WHITE);
	delayMilliSecRDL(TEST_DELAY2);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void Test906(void) {
	std::cout << "Test 906 : dot grid" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myOLED.drawDotGrid(0, 0, OLED_WIDTH, OLED_HEIGHT, 5, myOLED.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void Test907() {
	std::cout << "Test 907 : quadrilateral" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	// Trapezoid 1 
	int16_t x8 = 30, y8 = 30;   // First vertex
	int16_t x9 = 35, y9 = 50;
	int16_t x10 = 60, y10 = 50;
	int16_t x11 = 75, y11 = 30;
	myOLED.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myOLED.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);

}

void Test908(void) {
	std::cout << "Test 908 : Ellipse" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myOLED.drawEllipse(60, 30, 25, 20, false, myOLED.RDLC_WHITE);  // Top-left
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.drawEllipse(60, 30, 25, 20, true, myOLED.RDLC_WHITE);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void Test909(void) {
	std::cout << "Test 909 : Drawing Arc: drawArc and drawSimpleArc" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	int16_t centerX = 50;  // X-coordinate of the circle center
	int16_t centerY = 50;  // Y-coordinate of the circle center
	int16_t radius = 45;    // Radius of the circle
	std::cout << " Angle offset default : " << myOLED.getArcAngleOffset() << std::endl;
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full upper half (180° to 360°)
	myOLED.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myOLED.RDLC_BLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	// Draw the third quarter (180° to 270°)
	myOLED.drawArc(centerX, centerY, radius, 5, 180.0f, 270.0f, myOLED.RDLC_CYAN);
	// Draw the fourth quarter (270° to 360°)
	myOLED.drawArc(centerX, centerY, radius, 5, 270.0f, 360.0f, myOLED.RDLC_MAGENTA);
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);

	// drawSimpleArc :: Draw an arc with a radius of 50, from 0 to 300 degrees
	myOLED.drawSimpleArc(30, 30, 25, 0.0f, 300.0f, myOLED.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY2);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void DisplayReset(void)
{
	delayMilliSecRDL(TEST_DELAY5);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.setTextColor(myOLED.RDLC_WHITE, myOLED.RDLC_BLACK);
	myOLED.setFont(font_default);
	myOLED.writeCharString(0, 0, teststr1);
	DisplayReset();

	myOLED.OLEDPowerDown(); // Power down device
	std::cout << "OLED End" << std::endl;
}

// *************** EOF ****************

/// @endcond
