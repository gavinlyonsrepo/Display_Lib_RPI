/*!
	@file examples/ch1115/text_graphics_functions/main.cpp
	@author Gavin Lyons
	@brief Test file for  ER_OLEDM1_CH1115 display showing use of Text,graphics & functions.
		Project Name: Display_Lib_RPI

	@test
		-# Test 501 Enable and disable Screen (Sleep mode , 500uA)
		-# Test 502 Invert screen colour
		-# Test 503 Flip vertically screen 180 with LCD command
		-# Test 504 Screen Contrast
		-# Test 505 Scroll Screen
		-# Test 506 Fade effect
		-# Test 507 Fill a page 0-7 + clear screen
		-# Test 508 Rotate screen by rotating software buffer
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
		-# Test 718 Fonts thick, seven segment + mint
		-# Test 901 standard graphic tests
		-# Test 905 Polygons
		-# Test 906 Dot Grid
		-# Test 907 Quadrilateral
		-# Test 908 Ellipse
		-# Test 909 Draw Arc and draw simple arc
*/

#include <cstdio>
#include "ERM1_CH1115_OLED_RDL.hpp"

/// @cond

// Test related 
#define DisplayDelay1 3000
uint8_t graphicsCountLimit = 25;

//GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

// Screen 
const uint8_t MY_OLED_WIDTH  = 128;
const uint8_t MY_OLED_HEIGHT = 64;
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8)) // 1024 bytes = 128 * 64/8
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

// SPi
// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

ERMCH1115 myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT , RES, DC) ; // instantiate  an object

// =============== Function prototype ================
bool Setup(void);
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
void Test718(void);
void Test901(void);
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);

// ======================= Main ===================
int main(void)
{
	if(!Setup()) return -1;
	myTests();
	EndTests();
	return 0;
}
// ======================= End of main  ===================


// ======================= Function space ===================
bool Setup(void)
{
	printf("OLED Begin\r\n");
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", rdlib::LibraryVersion()); 
	delayMilliSecRDL(50);

	if(myOLED.OLEDbegin(OLEDcontrast, HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE ) != rdlib::Success) // initialize the OLED
	{
		printf("Error 1202: Setup : Cannot start spi, \r\n");
		return false;
	}
	delayMilliSecRDL(50);
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	delayMilliSecRDL(1500);
	return true;
}


void EndTests()
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	printf("OLED End\r\n");
}

void myTests()
{
	// Define a buffer to cover whole screen
	uint8_t  screenBuffer[FULLSCREEN];
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer) != rdlib::Success) return;
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
	Test718();
	Test901();
	Test905();
	Test906();
	Test907();
	Test908();
	Test909();
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
	printf("OLED Test 713 print ASCII font 32-127 \r\n");
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
	myOLED.print(47 , myOLED.RDL_DEC);
	myOLED.setCursor(0, 16);
	myOLED.print(47 , myOLED.RDL_HEX); 
	myOLED.setCursor(0, 32);
	myOLED.print(47, myOLED.RDL_BIN);
	myOLED.setCursor(0, 48);
	myOLED.print(47 , myOLED.RDL_OCT);
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
	printf("OLED Test 717-a print method numbers\r\n");
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
	myOLED.print(intVec[2]); // Output: "3 "
	
	// For a vector of strings
	myOLED.setCursor(0, 20);
	std::vector<std::string> stringVec = {"Hello", "Vector"};
	myOLED.print(stringVec[1]); // Output: "Vector "
	TestReset();
}

void Test718(void)
{
	printf("OLED Test 718 mint \r\n");	
	myOLED.setCursor(0, 0);
	myOLED.setFont(font_mint);
	myOLED.println("Mint");
	TestReset();
}


void Test500(void)
{
	// Set text parameters
	myOLED.setFont(font_default);
	//  ** Test 501 OLED enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("Disable test");
	myOLED.OLEDupdate();

	delayMilliSecRDL(3000); //display message
	myOLED.OLEDEnable(0); //turn off display (sleep mode 500uA measured)
	delayMilliSecRDL(3000); //wait for 3
	myOLED.OLEDEnable(1); //turn on
	delayMilliSecRDL(2000); //display message
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("Inverse test");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	myOLED.OLEDInvert(1); // Inverted
	delayMilliSecRDL(4000);
	myOLED.OLEDInvert(0);
	delayMilliSecRDL(1000);

	// ** Test 503 OLED flip **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Flip test");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	myOLED.OLEDFlip(1);
	delayMilliSecRDL(3000);
	myOLED.OLEDFlip(0);
	delayMilliSecRDL(2000);

	// ** Test 504 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test");
	myOLED.OLEDupdate();
	delayMilliSecRDL(2500);
	myOLED.OLEDFillScreen(0x77); // fill screen

	myOLED.OLEDContrast(0x00);
	delayMilliSecRDL(1000);
	myOLED.OLEDContrast(0x80);
	delayMilliSecRDL(1000);
	myOLED.OLEDContrast(0xFF);
	delayMilliSecRDL(1000);
	myOLED.OLEDContrast(OLEDcontrast);
	delayMilliSecRDL(1000);
	myOLED.OLEDclearBuffer();

	// ** Test 505 OLED scroll **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 40);
	myOLED.print("Scroll test ");
	myOLED.OLEDupdate();
	delayMilliSecRDL(2500);

	// See .cpp file for more info on these parmeters.
	uint8_t timeInterval = 0x00; // 6 frames 0x00 - 0x07
	uint8_t scrollDirection = 0x26; //right 0x26 or 0x27
	uint8_t scrollMode = 0x28; // contiunous 0x28-0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
	myOLED.OLEDscroll(1); //start scroll
	delayMilliSecRDL(15000);
	myOLED.OLEDscroll(0); // stop Scroll

	delayMilliSecRDL(1);

	timeInterval = 0x02; // 64 frames , 0x00 - 0x07
	scrollDirection = 0x27; // left , 0x26 or 0x27
	scrollMode = 0x29; // contiunous 0x28 one shot 0x29 , one col 0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
	myOLED.OLEDscroll(1); //start
	delayMilliSecRDL(15000);
	myOLED.OLEDscroll(0); // stop

	// ** TEST 506 fade ~ breath effect **
	myOLED.OLEDclearBuffer();
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Fade effect    test");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	myOLED.OLEDfadeEffect();
	myOLED.OLEDFillScreen(0xE3);
	delayMilliSecRDL(10000);
	myOLED.OLEDfadeEffect(0x00); // switch off fade
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);

	// ** Test 507 fill page function **
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.print("Fill effect     test");
	delayMilliSecRDL(1500);
	int8_t pageNum= 0;
	for (pageNum = 0 ; pageNum < 8 ; pageNum ++)
	{
		myOLED.OLEDFillPage(pageNum, 0x17); // Write pattern (0001 0111) to all 8 page's.
		delayMilliSecRDL(500);
	}
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	
	// ** Test 508 Rotate test **
	myOLED.OLEDclearBuffer();
	printf("OLED Rotate test 508\r\n");
	myOLED.setRotation(myOLED.BC_Degrees_90);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 90");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	
	myOLED.setRotation(myOLED.BC_Degrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 180");
	myOLED.setCursor(5,50);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	
	myOLED.setRotation(myOLED.BC_Degrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate   270");
	myOLED.setCursor(5,110);
	myOLED.print("bottom");
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	
	myOLED.setRotation(myOLED.BC_Degrees_0); //default normal 
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 0");

	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.OLEDupdate();
	myOLED.OLEDclearBuffer();
	delayMilliSecRDL(500);
}

// Function to display Graphics test
void  Test901()
{
	printf("OLED Test 901 graphics test \r\n");
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	//
	uint8_t count = 0;
	bool colour = 1;
	myOLED.OLEDclearBuffer(); // Clear the buffer
	myOLED.setFont(font_default);
	while (count < graphicsCountLimit)
	{
		colour = !colour;

		//print count
		myOLED.setCursor(0, 0);
		myOLED.print(count);
		myOLED.setCursor(45, 0);
		myOLED.print(colour);
		// Draw the X
		myOLED.drawLine(64,  0, 64, 64, myOLED.BLACK);
		myOLED.drawFastVLine(62, 0, 64, myOLED.BLACK);
		myOLED.drawFastHLine(0, 32, 128, myOLED.BLACK);

		// Q1
		myOLED.fillRect(0, 10, 20, 20, colour);
		myOLED.fillCircle(40, 20, 10, !colour);
		myOLED.drawCircle(40, 20, 10, colour);

		// Q2
		myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
		myOLED.drawTriangle(80, 25, 90, 5, 100, 25, colour);
		myOLED.drawRect(105, 10, 15, 15, myOLED.BLACK);
		// Q3
		myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
		myOLED.drawRoundRect(0, 40, 40, 20, 10, colour);
		// Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, myOLED.BLACK);
			myOLED.OLEDupdate();
			delayMilliSecRDL(50);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
}

void Test905(void)
{
	std::cout << "Test 905 : polygons" << std::endl;
	//drawPolygon( x,  y, sides,  diameter, rotation,fill, color);
	// hexagon 6 sides
	myOLED.drawPolygon(60, 30, 6, 25, 0, false, myOLED.BLACK);
	myOLED.drawPolygon(60, 30, 6, 25, 0, true, myOLED.BLACK);
	delayMilliSecRDL(200);
	TestReset();
}

void Test906(void) {
	std::cout << "Test 906 : dot grid" << std::endl;
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myOLED.drawDotGrid(0, 0, MY_OLED_WIDTH, MY_OLED_HEIGHT, 5, myOLED.BLACK);
	TestReset();
}

void Test907() {
	std::cout << "Test 907 : quadrilateral" << std::endl;
	// Trapezoid 1 
	int16_t x8 = 30, y8 = 30;   // First vertex
	int16_t x9 = 35, y9 = 50;
	int16_t x10 = 60, y10 = 50;
	int16_t x11 = 75, y11 = 30;
	myOLED.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myOLED.BLACK);
	TestReset();
}

void Test908(void) {
	std::cout << "Test 908 : Ellipse" << std::endl;
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myOLED.drawEllipse(60, 30, 25, 20, false, myOLED.BLACK);  // Top-left
	delayMilliSecRDL(DisplayDelay1);
	myOLED.drawEllipse(60, 30, 25, 20, true, myOLED.BLACK);
	TestReset();
}

void Test909(void) {
	std::cout << "Test 909 : Drawing Arc: drawArc and drawSimpleArc" << std::endl;
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
	myOLED.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myOLED.BLACK);
	TestReset();
	delayMilliSecRDL(2000);
	// Draw the third quarter (180° to 270°)
	myOLED.drawArc(centerX, centerY, radius, 3, 180.0f, 270.0f, myOLED.BLACK);
	TestReset();
	// drawSimpleArc :: Draw an arc with a radius of 50, from 0 to 300 degrees
	myOLED.drawSimpleArc(30, 30, 25, 0.0f, 300.0f, myOLED.BLACK);
	TestReset();
}
/// @endcond
