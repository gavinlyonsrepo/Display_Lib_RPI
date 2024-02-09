/*!
	@file examples/bicolor_displays/ch1115/TEXTGRAPHICS_FUNCTIONS/main.cpp
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
		-# Test 901 Graphic tests
*/

#include <bcm2835.h>
#include <cstdio>
#include "ERM1_CH1115_OLED_RDL.hpp"

// Test related 
#define DisplayDelay1 3000
uint8_t graphicsCountLimit = 25;

// OLED
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8)) // 1024 bytes = 128 * 64/8

const uint32_t SPICLK_FREQ = 64; // Spi clock divid bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC) ; // instantiate  an object

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
void Test901(void);

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
	printf("OLED Begin\r\n");
	if(!bcm2835_init())
	{
		printf("Error 1201 : Setup : Problem with init bcm2835 library\r\n");
		return false;
	}else{
		printf("bcm2835 library version : %u\r\n", bcm2835_version());
	}
	bcm2835_delay(50);
	if(myOLED.OLEDbegin(OLEDcontrast, SPICLK_FREQ , SPI_CE_PIN) != rpiDisplay_Success) // initialize the OLED
	{
		printf("Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?\r\n");
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return false;
	}
	printf("CH1115 OLED library version : %u\r\n", GetRDLibVersionNum());
	bcm2835_delay(50);
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	bcm2835_delay(1000);
	return true;
}


void EndTests()
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	bcm2835_close(); //Close lib & /dev/mem, deallocating mem
	printf("OLED End\r\n");
}

void myTests()
{
	// Define a buffer to cover whole screen
	uint8_t  screenBuffer[FULLSCREEN];
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
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
	Test901();

}

void TestReset(void){
	myOLED.OLEDupdate();
	bcm2835_delay(DisplayDelay1);
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
	printf("OLED Test 702 Wide font, NO LOWERCASE LETTERS\r\n");
	myOLED.setFont(font_wide);
	char TestStr[] = "WIDE FONT";
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
	printf("OLED Test 713-a print ASCII font 0-127 \r\n");
	myOLED.setFont(font_default);
	myOLED.setInvertFont(false);
	myOLED.setCursor(0, 0);

	myOLED.print("ASCII font ");
	myOLED.setCursor(0, 15);

	// Print first 127 chars of font
	for (char i = 0; i < 126; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myOLED.print(i);
	}
	TestReset();

	myOLED.setCursor(0, 0);
	myOLED.print("ASCII font 128-255");
	uint8_t x = 0;
	uint8_t y = 15;
	myOLED.setCursor(x, y);
	
	// Test 711-b
	printf("OLED Test 713-b print ASCII font 128-255 \r\n");
	for (uint8_t i = 128; i < 254; i++)
	{
		if (x > 110)
		{
			x = 0;
			y += 8;
		}
		myOLED.writeChar(x, y , i);
		x += 6;
	}
	TestReset();
}

void Test714(void)
{
	printf("OLED Test 714 Base number systems using print \r\n");
	myOLED.setFont(font_default);
	myOLED.setCursor(0, 0);
	myOLED.print(47 , DEC);
	myOLED.setCursor(0, 16);
	myOLED.print(47 , HEX); 
	myOLED.setCursor(0, 32);
	myOLED.print(47, BIN);
	myOLED.setCursor(0, 48);
	myOLED.print(47 , OCT);
	TestReset();
}

void Test715(void)
{
	printf("OLED Test 715-a println + textwrap \r\n");
	myOLED.setCursor(0, 0);
	myOLED.println("first line");
	myOLED.print("2nd Line");
	myOLED.setCursor(0, 32);
	myOLED.setFont(font_wide);
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
	printf("OLED Test 717 print method numbers\r\n");	
	myOLED.setCursor(0, 0);
	myOLED.println(3.986,2 ); //print 3.99
	myOLED.println(4001);
	myOLED.println(-87.39);
	myOLED.println(-19);
	myOLED.println("hello");
	myOLED.setInvertFont(true);
	myOLED.print('P');
	TestReset();
}

void testErrorCheck(void)
{
	// Error checking
	printf("==== Test 720 Start Error checking ====\r\n");
	printf("Result = 2 2 2 1 1 1 1 ===\r\n");
	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = "-:;A";

	// character out of font bounds
	// wide & thick lower case + ]
	myOLED.setFont(font_wide);
	myOLED.writeCharString(5,  5, testlowercase); //throw wide font error 2
	myOLED.writeChar(32, 0, '!'); 
	TestReset();
	// Numeric extended bounds ; , A errors
	myOLED.setFont(font_sixteenSeg);
	myOLED.writeCharString(0, 0, testNonNumExtend); //throw font error 2
	myOLED.writeChar(32, 0, ','); //throw error 2
	TestReset();
	printf("========\r\n");
	// screen out of bounds
	myOLED.setFont(font_default);
	myOLED.writeChar(0, 100, 'e'); //throw error 1
	myOLED.writeChar(150, 0, 'f'); //throw error 1
	TestReset();
	myOLED.setFont(font_orla);
	myOLED.writeChar(0, 100, 'A'); //throw error 1
	myOLED.writeChar(150, 0, 'B'); //throw error 1
	TestReset();
	printf("==== Stop Error checking ====\r\n");
}

void Test500(void)
{
	// Set text parameters
	myOLED.setFont(font_default);
	//  ** Test 501 OLED enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("OLED Disable test 501");
	myOLED.OLEDupdate();

	bcm2835_delay(3000); //display message
	myOLED.OLEDEnable(0); //turn off display (sleep mode 500uA measured)
	bcm2835_delay(3000); //wait for 3
	myOLED.OLEDEnable(1); //turn on
	bcm2835_delay(2000); //display message
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("OLED Inverse test 502");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDInvert(1); // Inverted
	bcm2835_delay(4000);
	myOLED.OLEDInvert(0);
	bcm2835_delay(1000);

	// ** Test 503 OLED flip **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Flip test 503");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDFlip(1);
	bcm2835_delay(3000);
	myOLED.OLEDFlip(0);
	bcm2835_delay(2000);

	// ** Test 504 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test 504");
	myOLED.OLEDupdate();
	bcm2835_delay(2500);
	myOLED.OLEDFillScreen(0x77); // fill screen

	myOLED.OLEDContrast(0x00);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0x80);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(0xFF);
	bcm2835_delay(1000);
	myOLED.OLEDContrast(OLEDcontrast);
	bcm2835_delay(1000);
	myOLED.OLEDclearBuffer();

	// ** Test 505 OLED scroll **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 40);
	myOLED.print("Scroll test 505 ");
	myOLED.OLEDupdate();
	bcm2835_delay(2500);

	// See .cpp file for more info on these parmeters.
	uint8_t timeInterval = 0x00; // 6 frames 0x00 - 0x07
	uint8_t scrollDirection = 0x26; //right 0x26 or 0x27
	uint8_t scrollMode = 0x28; // contiunous 0x28-0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
	myOLED.OLEDscroll(1); //start scroll
	bcm2835_delay(15000);
	myOLED.OLEDscroll(0); // stop Scroll

	bcm2835_delay(1);

	timeInterval = 0x02; // 64 frames , 0x00 - 0x07
	scrollDirection = 0x27; // left , 0x26 or 0x27
	scrollMode = 0x29; // contiunous 0x28 one shot 0x29 , one col 0x2A,
	myOLED.OLEDscrollSetup(timeInterval, scrollDirection , scrollMode);
	myOLED.OLEDscroll(1); //start
	bcm2835_delay(15000);
	myOLED.OLEDscroll(0); // stop

	// ** TEST 506 fade ~ breath effect **
	myOLED.OLEDclearBuffer();
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Fade effect test 506");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDfadeEffect();
	myOLED.OLEDFillScreen(0xE3);
	bcm2835_delay(10000);
	myOLED.OLEDfadeEffect(0x00); // switch off fade
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);

	// ** Test 507 fill page function **
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.print("Fill effect test 507");
	int8_t pageNum= 0;
	for (pageNum = 0 ; pageNum < 8 ; pageNum ++)
	{
		myOLED.OLEDFillPage(pageNum, 0x17); // Write pattern (0001 0111) to all 8 page's.
		bcm2835_delay(500);
	}
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	
	// ** Test 508 Rotate test **
	myOLED.OLEDclearBuffer();
	printf("OLED Rotate test 508\r\n");
	myOLED.setRotation(displayBC_Degrees_90);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 90");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.setRotation(displayBC_Degrees_180);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 180");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.setRotation(displayBC_Degrees_270);
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate   270");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.setRotation(displayBC_Degrees_0); //default normal 
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5,5 );
	myOLED.print("rotate 0");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	
	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.OLEDupdate();
	myOLED.OLEDclearBuffer();
	bcm2835_delay(500);
}

// Function to display Graphics test
void  Test901()
{
	printf("OLED Test 901 graphics test \r\n");
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	//
	bool colour = 1;
	uint8_t count = 0;
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
			bcm2835_delay(50);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
}
