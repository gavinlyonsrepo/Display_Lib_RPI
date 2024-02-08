/*!
	@file examples/bicolor_displays/nokia5110/TextTests/main.cpp
	@author Gavin Lyons
	@brief Test file for Nokia 5110 display showing use of TEXT mode .
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

// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;

	myLCD.LCDfillScreenPattern(0x31); // Splash screen
	screenReset();

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
	bcm2835_delay(250);
	if(myLCD.LCDBegin(inverse, contrast, bias) != rpiDisplay_Success)
	{
		printf( "Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?\n");
		bcm2835_close(); // Close the bcm2835 library
		return false;
	}
	printf( "Nokia 5110 library version : %u\n",GetRDLibVersionNum());
	bcm2835_delay(250);
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
	myLCD.print(47 , DEC);
	myLCD.setCursor(0, 9);
	myLCD.print(47 , HEX); 
	myLCD.setCursor(0, 20);
	myLCD.print(47, BIN);
	myLCD.setCursor(0, 29);
	myLCD.print(47 , OCT);
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

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}

// *************** EOF ****************
