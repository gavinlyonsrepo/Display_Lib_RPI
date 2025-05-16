/*!
	@file examples/gc9a01/text_graphics_functions/main.cpp
	@brief Library test file, tests Text,graphics & functions.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 501 RGB color OK?
	-# Test 502 Scroll
	-# Test 503 Rotate
	-# Test 504 change modes test -> Invert, display on/off and Sleep.
	-# Test 701 Print out all fonts with writeCharString
	-# Test 702 Print out all fonts with print
	-# Test 703 Misc print class tests(string object, println invert, wrap, etc)
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
#include "GC9A01_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

// Section :: Globals
GC9A01_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 240; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers
uint8_t Setup(void);
void EndTests(void);
void DisplayReset(void);
void ColorBlock(void);
// Functions tests
void Test501(void);
void Test502(void);
void Test503(void);
void Test504(void);
// Text tests
void Test701(void);
void Test702(void);
void Test703(void);
// Graphic tests
void Test902(void);
void Test903(void);
void Test904(void);
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);


//  Section ::  MAIN loop
int main(void)
{
	if(Setup() != 0)return -1;

	Test501();
	Test502();
	Test503();
	Test504();
	Test701();
	Test702();
	Test703();
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
	std::cout << "TFT Start" << std::endl;
	std::cout << "GC9A01 library version : " << rdlib::LibraryVersion()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.TFTInitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI settings**
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

void Test501(void)
{
	std::cout << "Test 501: Color Test:: Red,green,blue,yellow,white, black background" << std::endl;
	myTFT.setFont(font_mega);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_GREEN,myTFT.RDLC_BLACK);
	ColorBlock();
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test502(void)
{
	printf("Test 502: Scroll\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setFont(font_retro);
	char teststr0[] = "Scroll Test";
	myTFT.writeCharString(75, 75, teststr0);

	const uint16_t screenH = 240;
	const uint16_t scrollArea = screenH - 0; // no fixed areas
	myTFT.TFTsetScrollArea(0, scrollArea, 0);

	for (uint16_t y = 0; y < screenH; y++) {
		myTFT.TFTsetScrollStart(y);
		delayMilliSecRDL(20);
	}
	myTFT.TFTsetScrollStart(0); // reset
	myTFT.TFTScrollModeLeave(); // leave scroll mode
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test503()
{
	printf("Test 503: Rotate\n");
	myTFT.setFont(font_retro);
	char teststr0[] = "Rotate 0"; //normal
	char teststr1[] = "Rotate 90"; // 90
	char teststr2[] = "Rotate 180"; // 180
	char teststr3[] = "Rotate 270"; // 270

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.writeCharString(15, 80, teststr0);
	myTFT.writeCharString(15, 160, teststr0);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.writeCharString(15, 80, teststr1);
	myTFT.writeCharString(15, 160, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_180);
	myTFT.writeCharString(15, 80, teststr2);
	myTFT.writeCharString(15, 160, teststr2);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_270);
	myTFT.writeCharString(15, 80, teststr3);
	myTFT.writeCharString(15, 160, teststr3);
	delayMilliSecRDL(TEST_DELAY2);

	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test504()
{
	printf("Test 504: Mode Tests\n");
	char teststr1[] = "Invert    Test";
	char teststr2[] = "Display on/off";
	char teststr3[] = "Sleep   on/off";
	ColorBlock();
	myTFT.writeCharString(10, 80, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	// Invert on and off
	myTFT.TFTchangeInvertMode(true);
	printf("Test 504-1: Invert on \n");
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTchangeInvertMode(false);
	printf("Test 504-2: Invert off \n");
	delayMilliSecRDL(TEST_DELAY5);

	// Display on and off
	myTFT.TFTenableDisplay(false);
	printf("Test 504-3: Display off\n");
	myTFT.writeCharString(10, 80, teststr2);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTenableDisplay(true);
	printf("Test 504-4: Turn Display back on\n");
	delayMilliSecRDL(TEST_DELAY5);
	// modes
	myTFT.writeCharString(10, 80, teststr3);
	delayMilliSecRDL(TEST_DELAY5); 
	// 1. sleep mode
	myTFT.TFTsetPowerMode(GC9A01_TFT::PowerState_e::SleepNormalIdleOff);
	printf("Test 504-5: Power mode Sleep ON Idle Off\n");
	delayMilliSecRDL(TEST_DELAY5);
	delayMilliSecRDL(TEST_DELAY5);
	//back to sleep off - normal on - idle off
	myTFT.TFTsetPowerMode(GC9A01_TFT::PowerState_e::NormalIdleOff);	
	delayMilliSecRDL(TEST_DELAY5);
}

void Test701(void) {

	printf("Test 701: Print out all fonts with writeCharString\n");
	char teststr1[] = "Default ";
	char teststr2[] = "GLL ";
	char teststr3[] = "Pico ";
	char teststr4[] = "Sinclair ";
	char teststr5[] = "Orla ";
	char teststr6[] = "Retro ";
	char teststr7[] = "Mega";
	char teststr8[] = "Arial b";
	char teststr9[] = "Hall ";

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Start at ~20px Y offset and use ~20px spacing
	int startY = 40;
	int spacing = 20;
	int x = 40;
	int y = startY;

	myTFT.setFont(font_default);
	myTFT.writeCharString(x, y, teststr1);
	y += spacing;
	myTFT.setFont(font_gll);
	myTFT.writeCharString(x, y, teststr2);
	y += spacing;
	myTFT.setFont(font_pico);
	myTFT.writeCharString(x, y, teststr3);
	y += spacing;
	myTFT.setFont(font_sinclairS);
	myTFT.writeCharString(x, y, teststr4);
	y += spacing;
	myTFT.setFont(font_orla);
	myTFT.writeCharString(x, y, teststr5);
	y += spacing;
	myTFT.setFont(font_retro);
	myTFT.writeCharString(x, y, teststr6);
	y += spacing;
	myTFT.setFont(font_mega);
	myTFT.writeCharString(x, y, teststr7);
	y += spacing;
	myTFT.setFont(font_arialBold);
	myTFT.writeCharString(x, y, teststr8);
	y += spacing;
	myTFT.setFont(font_hallfetica);
	myTFT.writeCharString(x, y, teststr9);
	y += spacing;
	myTFT.setFont(font_arialRound);
	DisplayReset();
}

void Test702(void)
{
	printf("Test 702: Print class methods\n");

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);

	int x = 40;
	int y = 40;
	int spacing = 25;

	// --- First Block ---
	myTFT.setCursor(x, y);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_gll);
	myTFT.print("GLL ");
	myTFT.print(123.284, 1); // print 123.3
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_retro);
	myTFT.print("retro ");
	myTFT.print(-3.14);

	DisplayReset();

	// --- Second Block ---
	y = 60;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_mega);
	myTFT.print("mega ");
	myTFT.print(61);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_arialBold);
	myTFT.print("A bold ");
	myTFT.print(12.08);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_hallfetica);
	myTFT.print("hall f ");
	myTFT.print(1.48);

	DisplayReset();

	// --- Third Block ---
	y = 40;
	myTFT.setFont(font_orla);
	myTFT.setCursor(x, y);
	myTFT.print(-7.16);
	myTFT.print("Orla ");
	y += spacing;
	myTFT.setCursor(x, y + spacing);
	myTFT.setFont(font_arialRound);
	myTFT.print(-8.16);
	myTFT.print("a rnd ");

	DisplayReset();

	// --- Final Block ---
	y = 60;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);
	DisplayReset();
}

void Test703(void)
{
	printf("Test 703: Misc print class\n");
	// --- Inverted print fonts 1–6 ---
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(10, 20);
	myTFT.print(-49);
	myTFT.setFont(font_mega);
	myTFT.setCursor(10, 60);
	myTFT.print(112.09);
	myTFT.setCursor(10, 90);
	myTFT.print("ABCD");
	myTFT.setFont(font_gll);
	myTFT.setCursor(10, 140);
	myTFT.print("ER");

	DisplayReset();

	// --- Inverted print ---
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_RED);
	myTFT.setFont(font_arialBold);
	myTFT.setCursor(40, 40);
	myTFT.print("INVERT");
	myTFT.setCursor(40, 80);
	myTFT.print(-94.982, 2);
	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(40, 100);
	myTFT.print("INVERT");
	myTFT.setCursor(40, 140);
	myTFT.print(123456);

	DisplayReset();

	// --- Test std::string print and wrapping ---
	myTFT.setFont(font_mega);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(60, 60);
	myTFT.print(timeInfo);
	myTFT.setCursor(60, 60);
	myTFT.println(newLine); // newline
	myTFT.print(newLine);
	myTFT.setCursor(10, 100);
	myTFT.print("12345678901234567890ABCDEFGHIJ"); // wrap long text

	DisplayReset();
}

void Test902(void) {
	printf("Test 902: rectangles \n");
	rdlib::Return_Codes_e returnValue;
	myTFT.drawRectWH(45, 25, 20, 20, myTFT.RDLC_RED);
	returnValue = myTFT.fillRectangle(85, 25, 20, 20, myTFT.RDLC_YELLOW);
	if (returnValue != rdlib::Success)
	{
		printf("Warning : Test TFTfillRectangle, An error occurred returnValue = %u \n", returnValue);
	}
	myTFT.fillRect(115, 25, 20, 20, myTFT.RDLC_GREEN);
	myTFT.drawRoundRect(15, 100, 70, 70, 20, myTFT.RDLC_CYAN);
	myTFT.fillRoundRect(120, 100, 50, 50, 10, myTFT.RDLC_WHITE);

	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test903(void) {
	printf("Test 903 & 904 : Triangles and circles\n");
	myTFT.drawCircle(40, 180, 15, myTFT.RDLC_GREEN);
	myTFT.fillCircle(80, 180, 15, myTFT.RDLC_YELLOW);
}

void Test904(void) {
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80,myTFT.RDLC_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.RDLC_RED);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test905(void)
{
	printf("Test 905 : polygons\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, false, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY5);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, true, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test906(void) {
	printf("Test 906 : mesh\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(50, 50, 100,100, 5, myTFT.RDLC_CYAN);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test907() {
	printf("Test 907 : quadrilateral\n");
	// Define the four quadrilateral vertices for different shapes
	// First shape: Quadrilateral (top-left region)
	int16_t x0 = 30, y0 = 30;
	int16_t x1 = 80, y1 = 30;
	int16_t x2 = 80, y2 = 80;
	int16_t x3 = 30, y3 = 80;
	// Second shape: Parallelogram (bottom-right region)
	int16_t x4 = 150, y4 = 170;
	int16_t x5 = 190, y5 = 180;
	int16_t x6 = 180, y6 = 210;
	int16_t x7 = 140, y7 = 200;
	// Third shape: Trapezoid 1 (center region)
	int16_t x8  = 95, y8  = 110;
	int16_t x9  = 100, y9  = 130;
	int16_t x10 = 140, y10 = 130;
	int16_t x11 = 150, y11 = 110;
	// Fourth shape: Trapezoid 2 (bottom-left region)
	int16_t x12 = 40, y12 = 180;
	int16_t x13 = 55, y13 = 215;
	int16_t x14 = 100, y14 = 215;
	int16_t x15 = 70, y15 = 180;

	myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.RDLC_RED);
	myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.RDLC_GREEN);
	myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.RDLC_BLUE);
	myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test908(void) {
	printf("Test 908 : Ellipse\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	// Draw unfilled ellipses
	myTFT.drawEllipse(60, 60, 18, 30, false, myTFT.RDLC_WHITE);   // Top-left
	myTFT.drawEllipse(180, 60, 18, 30, false, myTFT.RDLC_RED);    // Top-right
	myTFT.drawEllipse(60, 180, 30, 18, false, myTFT.RDLC_BLUE);   // Bottom-left
	myTFT.drawEllipse(180, 180, 30, 18, false, myTFT.RDLC_YELLOW); // Bottom-right
	delayMilliSecRDL(TEST_DELAY5);

	// Draw filled ellipses in same positions
	myTFT.drawEllipse(60, 60, 18, 30, true, myTFT.RDLC_WHITE);
	myTFT.drawEllipse(180, 60, 18, 30, true, myTFT.RDLC_RED);
	myTFT.drawEllipse(60, 180, 30, 18, true, myTFT.RDLC_BLUE);
	myTFT.drawEllipse(180, 180, 30, 18, true, myTFT.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test909(void) {
	printf("Test 909 : Drawing Arc: drawArc\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	int16_t centerX = 120;  // X-coordinate of the circle center
	int16_t centerY = 120;  // Y-coordinate of the circle center
	int16_t radius = 50;    // Radius of the circle
	printf(" Angle offset default : %i \n", myTFT.getArcAngleOffset());
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.RDLC_BLUE);
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myTFT.RDLC_RED);
	delayMilliSecRDL(TEST_DELAY);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.RDLC_CYAN);
	delayMilliSecRDL(TEST_DELAY);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.RDLC_MAGENTA);
	delayMilliSecRDL(TEST_DELAY);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}


void DisplayReset(void)
{
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_mega);
	myTFT.writeCharString(25, 90, teststr1);
	DisplayReset();
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}

void ColorBlock(void)
{	myTFT.fillRoundRect(8, 100, 24, 60, 8, myTFT.RDLC_RED);
	myTFT.fillRoundRect(32, 100, 24, 60, 8, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(56, 100, 24, 60, 8, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(80, 100, 24, 60, 8, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(104, 100, 24, 60, 8, myTFT.RDLC_WHITE);
}

/// @endcond
