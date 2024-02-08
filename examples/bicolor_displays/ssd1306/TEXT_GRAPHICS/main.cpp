/*!
	@file examples/bicolor_displays/ssd1306/TEXT_GRAPHICS/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED display showing use of TEXT mode & graphics.
		Project Name: Display_Lib_RPI

	@test
		1. Test 701 Default font
		2. Test 702 Wide font
		3. Test 703 Pico font
		4. Test 704 Sinclair font
		5. Test 705 Orla Font
		6. Test 706 Retro Font
		5. Test 707 Mega font
		6. Test 708 Arial bold font
		7. Test 709 hallFetica font
		8. Test 710 Arial round font
		9. Test 711 GroTesk font
		10. Test 712 Sixteen Segment font
		11. Test 713 Display ASCII font 0-255 default font
		12. Test 714 Base number systems using print method
		13. Test 715 println + print & textwrap 
		14. Test 716 print method String object 
		15. Test 717 print method numbers 
		16. Test 720 Text methods error Checking
		17. Test 901 Graphics test
*/

#include <bcm2835.h>
#include <cstdio>
#include "SSD1306_OLED_RDL.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8))
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object

// Test timing parameters
#define DisplayDelay1 4000
#define DisplayDelay2 0
uint8_t graphicsCountLimit = 25;

// =============== Function prototype ================
bool SetupTest(void);
void myTests(void);
void EndTests(void);
void TestReset(void);


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
	const uint16_t I2C_Speed = BCM2835_I2C_CLOCK_DIVIDER_626; //  bcm2835I2CClockDivider enum , see readme.
	const uint8_t I2C_Address = 0x3C;
	bool I2C_debug = false;
	printf("OLED Test Begin\r\n");

	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}

	// Turn on I2C bus (optionally it may already be on)
	if(!myOLED.OLED_I2C_ON())
	{
		printf("Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?\n");
		bcm2835_close(); // Close the library
		return false;
	}

	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	bcm2835_delay(500);
	myOLED.OLEDbegin(I2C_Speed, I2C_Address, I2C_debug); // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars, optional just for effect
	bcm2835_delay(1000);
	return true;
}


void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	bcm2835_close(); // Close the bcm2835 library
	printf("OLED Test End\r\n");
}

void myTests()
{
	// Define a buffer to cover whole screen
	uint8_t  screenBuffer[FULLSCREEN];
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.OLEDclearBuffer();

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



// Function to display Graphics.
void  DisplayGraphics()
{
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	printf("OLED Test 901 graphics test \r\n");
	bool colour = 1;
	uint8_t count = 0;
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
			bcm2835_delay(50);
		}
		myOLED.OLEDclearBuffer();
		count++;
	}
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

