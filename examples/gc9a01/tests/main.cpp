/*!
	@file examples/gc9a01/tests/main.cpp
	@brief Library test file, tests Bitmaps,text ,graphics & functions.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 300 Sprites demo, small bitmap
	-# Test 301 icons, small bitmap
	-# Test 302 bi-color image 128x128
	-# Test 303 24 bit color image bitmaps from the file system
	-# Test 304 16 bit color image bitmaps from the file system
	-# Test 305 16 bit color image bitmap from data
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
#include <atomic>  // Ctrl+C exit
#include <csignal> // Ctrl+C exit
#include <thread>  // Ctrl+C exit
#include "Bitmap_test_data.hpp" // Data for test 30X
#include "GC9A01_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY1 1000

// Section :: Globals
GC9A01_TFT myTFT;
std::atomic<bool> stopRequested{false};

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 240; // Screen height in pixels

//  Section ::  Function Headers
uint8_t Setup(void);
void EndTests(void);
void DisplayReset(void);
void DisplayMenu(void);
void ColorBlock(void);
void handleSignal(int){
	stopRequested = true;
}

// Bitmap tests
void Test300(void); // Sprite
void Test301(void); // icons, small bi-color bitmaps
void Test302(void); // 2 color bitmap
void Test303(void); // 24 color bitmap
void Test304(void); // 16 color bitmap
void Test305(void); // 16 color bitmap (from data)
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
void Test902_904(void);
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);


//  Section ::  MAIN loop
int main(void)
{
	rdlib_config::loadConfig();
	if(Setup() != 0) return -1;
	std::signal(SIGINT, handleSignal); // for Ctrl+C
	std::signal(SIGTERM, handleSignal);// for kill command
int choice;

do {
	if (stopRequested) break;

	myTFT.fillScreen(myTFT.RDLC_BLACK);
	DisplayMenu();

	printf("Enter a number (1-5): ");

	if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
		printf("Invalid input. Please enter a number from list.\n");

		// Clear invalid input
		while (getchar() != '\n');

		continue;
	}

	// Clear leftover newline
	while (getchar() != '\n');

	switch (choice) {
		case 1:
			Test300(); Test301(); Test302();
			Test303(); Test304(); Test305();
			break;

		case 2:
			Test501(); Test502();
			Test503(); Test504();
			break;

		case 3:
			Test701(); Test702(); Test703();
			break;

		case 4:
			Test902_904(); Test905();
			Test906(); Test907();
			Test908(); Test909();
			break;

		case 5:
			printf("Exiting menu\n");
			break;
	}

} while (choice != 5);


	if (stopRequested)
		printf("Stop Signal Received.\n");
	EndTests();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space

uint8_t Setup(void)
{
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	printf("TFT Start tests\n");

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

void DisplayMenu() {
	printf("Tests Menu:\n");
	printf("1. Bitmap Tests\n");
	printf("2. Function Tests\n");
	printf("3. Text Tests\n");
	printf("4. Graphic Tests\n");
	printf("5. Quit\n");
	printf("Enter your choice: ");
}

void Test501(void)
{
		if (stopRequested) return;
	printf("Test 501: Color Test:: Red,green,blue,yellow,white, black background\n");
	myTFT.setFont(font_mega);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_GREEN,myTFT.RDLC_BLACK);
	ColorBlock();
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test502(void)
{
	if (stopRequested) return;
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
	if (stopRequested) return;
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
	if (stopRequested) return;
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
	if (stopRequested) return;
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
	if (stopRequested) return;
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

void Test902_904(void) {
	if (stopRequested) return;
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

	printf("Test 903 & 904 : Triangles and circles\n");
	myTFT.drawCircle(40, 180, 15, myTFT.RDLC_GREEN);
	myTFT.fillCircle(80, 180, 15, myTFT.RDLC_YELLOW);
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80,myTFT.RDLC_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.RDLC_RED);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test905(void)
{
	if (stopRequested) return;
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
	if (stopRequested) return;
	printf("Test 906 : mesh\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(50, 50, 100,100, 5, myTFT.RDLC_CYAN);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test907() {
	if (stopRequested) return;
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
	if (stopRequested) return;
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
	if (stopRequested) return;
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
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.RDLC_YELLOW);
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.RDLC_CYAN);
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.RDLC_MAGENTA);
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test300(void)
{
	if (stopRequested) return;
	printf("Test 300: Sprites demo\n");
	// Test 300-A test 16-bit color Sprite
	// Draw as sprite, without background , 32 X 32 .background color = myTFT.RDLC_LBLUE
	// Green background screen
	myTFT.fillScreen(myTFT.RDLC_GREEN);
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.drawSprite(80, 100, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	myTFT.drawSprite(100, 140, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE, true);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Test 300-B test 16-bit color Sprite
	// Draw as sprite, without background , 32 X 32 .background color =myTFT.RDLC_LBLUE
	// Bitmap background screen
	if(myTFT.drawBitmap(40, 40, 128 , 128, myTFT.RDLC_WHITE , myTFT.RDLC_RED, BackupMenuBitmap) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);

	myTFT.drawSprite(80, 30, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	myTFT.drawSprite(60, 60, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void Test301(void)
{
	if (stopRequested) return;
	printf("Test 301: icons, small bitmap\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.drawIcon(42, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, SignalIconVa);
	myTFT.drawIcon(62, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, MsgIconVa);
	myTFT.drawIcon(80, 40, 8, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, AlarmIconVa);
	myTFT.drawIcon(100, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, BatIconVa);
	myTFT.drawIcon(25, 60, 12, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK, PowerIcon);
	myTFT.drawIcon(45, 60, 12, myTFT.RDLC_RED, myTFT.RDLC_YELLOW, SpeedIcon);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	if (myTFT.drawBitmap(160, 45, 40 , 16, myTFT.RDLC_RED , myTFT.RDLC_YELLOW, SunText) != rdlib::Success)
	{
		printf("Warning an Error occurred in drawBitmap.\n");
		return ;
	}
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

}

// bitmap 2 colour , 128x128 bi color bitmap
void Test302(void)
{
	if (stopRequested) return;
	printf("Test 302: Bi-color bitmap 128X128 pixels\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 2";
	myTFT.writeCharString(45, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	if(myTFT.drawBitmap(40, 40, 128 , 128, myTFT.RDLC_WHITE , myTFT.RDLC_GREEN, BackupMenuBitmap) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY5);
}

// bitmap 24 colour , All file format = Windows BITMAPINFOHEADER offset 54
// bitmap are 220 by 240
void Test303(void)
{
	if (stopRequested) return;
	printf("Test 303: 24 bit color image bitmaps from the file system\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 24";
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.writeCharString(45, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY1);

	size_t pixelSize = 3; // 24-bit color = 3 bytes per pixel
	uint8_t FileHeaderOffset = 54;
	uint8_t NumberOfFiles = 1;

	// Use std::vector for safe memory management of buffer
	std::vector<uint8_t> bmpBuffer;
	try
	{
		// Attempt to allocate memory for the vector
		bmpBuffer.resize(128 * 128 * pixelSize);
	} catch (const std::bad_alloc&)
	{
		printf("Error Test 303: Memory allocation failed for bmpBuffer\n");
		return;
	}

	for (uint8_t i = 0; i < NumberOfFiles; i++)
	{
		FILE *pFile = nullptr;

		// Select file
		switch (i)
		{
			case 0:
				pFile = fopen("bitmap/bitmap24images/24pic2.bmp", "r");
				break;
		}

		if (pFile == nullptr) // Check if file exists
		{
			printf("Error Test 303: File does not exist\n");
			return;
		}

		// Read bitmap data into buffer
		fseek(pFile, FileHeaderOffset, SEEK_SET);
		fread(bmpBuffer.data(), pixelSize, 128 * 128, pFile);
		fclose(pFile);

		// Draw bitmap
		if (myTFT.drawBitmap24(40, 40, bmpBuffer, 128, 128) != rdlib::Success)
		{
			printf("Warning: An error occurred in drawBitmap24\n");
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
	if (stopRequested) return;
	printf("Test 304: 16 bit color image bitmaps from the file system\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	char teststr1[] = "Bitmap 16";
	myTFT.writeCharString(45, 25, teststr1);
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
		printf("Error Test 303: Memory allocation failed for bmpBuffer1\n");
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
			printf("Error Test 404 : File does not exist\n");
			return;
		}
		fseek(pFile, offsetBMPHeader, 0);
		fread(bmpBuffer1.data(), pixelSize, bitmapWidth * bitmapHeight, pFile);
		fclose(pFile);

		if (myTFT.drawBitmap16(40, 40, bmpBuffer1, bitmapWidth, bitmapHeight) != rdlib::Success)
		{
		// Check for success 0x00
			printf("Warning an Error occurred in drawBitmap16\n");
			return;
		}
		delayMilliSecRDL(TEST_DELAY5);
	} // end of for loop

	myTFT.fillScreen(myTFT.RDLC_BLACK);
} // end of test


// bitmap 16 colour , Data from array as opposed to file system, When creating data flip image upside down
void Test305(void)
{
	if (stopRequested) return;
	printf("Test 305: 16-color bitmap 32x32 pixels from raw data as opposed to file-system(default), \n");
	printf("Reusing the sprite data from test300, note it will be displayed upside down\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	char teststr1[] = "Bitmap 16 data";
	myTFT.writeCharString(45, 25, teststr1);
	delayMilliSecRDL(TEST_DELAY2);

	if(myTFT.drawBitmap16(60, 60, SpriteTest16, 32 , 32) != rdlib::Success)
		return;
	delayMilliSecRDL(TEST_DELAY2);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void DisplayReset(void)
{
	if (stopRequested) {
		return;
	}
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
	delayMilliSecRDL(TEST_DELAY1);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	printf("TFT End tests\n");
}

void ColorBlock(void)
{	myTFT.fillRoundRect(8, 100, 24, 60, 8, myTFT.RDLC_RED);
	myTFT.fillRoundRect(32, 100, 24, 60, 8, myTFT.RDLC_GREEN);
	myTFT.fillRoundRect(56, 100, 24, 60, 8, myTFT.RDLC_BLUE);
	myTFT.fillRoundRect(80, 100, 24, 60, 8, myTFT.RDLC_YELLOW);
	myTFT.fillRoundRect(104, 100, 24, 60, 8, myTFT.RDLC_WHITE);
}

/// @endcond
