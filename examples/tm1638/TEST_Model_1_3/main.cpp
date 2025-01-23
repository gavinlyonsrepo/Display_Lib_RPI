/*!
	@file examples/tm1638/TEST_Model_1_3/main.cpp
	@author Gavin Lyons
	@brief A demo file library for TM1638 module Works on Model 1 and Model 3
	Carries out series of tests demonstrating arduino library TM1638plus.
	Project Name: Display_Lib_RPI
	URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@note Change the define in USER OPTION SECTION to change between model 1 & model 3
	@test
		 -# Test0 =    reset
		 -# Test1 =    Brightness
		 -# Test2 =    ASCII display
		 -# Test3 =    Set a single segment
		 -# Test4 =    Hex digits
		 -# Test5 =    Text String with Decimal point
		 -# Test6 =    TEXT + ASCII combo
		 -# Test7 =    Integer Decimal number
		 -# Test8 =    Text String + Float hack
		 -# Test9 =    Text String + decimal number
		 -# Test10 =    Multiple Decimal points
		 -# Test11 =    Display Overflow
		 -# Test12 =    Scrolling text
		 -# Test13 =    setLED and setLEDs
		 -# Test14 =    Buttons + LEDS NOTE: Press S1 & S8 together to quit
*/

// ***************** USER OPTION SECTION *********************
// Pick one define and one only to switch between model 1 & model 3
#define TM1638_MODEL_ONE_TEST // Comment this in if testing model 1
//#define TM1638_MODEL_THREE_TEST // Comment this in if testing model 3
//************************************************************

#include <cstdio>
#include <ctime>
#include <signal.h> //catch user Ctrl+C
#include <cstdlib> //exit function

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
//pick on any I/O you want.
#define  STROBE_TM 21 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 20  // clock = GPIO connected to clock line of module
#define  DIO_TM 16 // data = GPIO connected to data line of module
#define  GPIO_CHIP_DEVICE 0 // gpio chip device number see -> ls /dev/gpio*

// Model 1
#ifdef TM1638_MODEL_ONE_TEST
#include <TM1638plus_Model1_RDL.hpp>
//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO)
TM1638plus_Model1 tm(STROBE_TM, CLOCK_TM , DIO_TM, GPIO_CHIP_DEVICE);
#endif

// model 3
#ifdef TM1638_MODEL_THREE_TEST
#include <TM1638plus_Model3_RDL.hpp>
//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO)
TM1638plus_Model3 tm(STROBE_TM, CLOCK_TM , DIO_TM,  GPIO_CHIP_DEVICE);
#endif

// Some vars and defines for the tests.
#define myTestDelay  5
#define myTestDelay1 1
uint8_t  testcount = 0;

void Test0(void);  // void Test 0 reset
void Test1(void);  // Brightness
void Test2(void);  // ASCII display
void Test3(void);  // Set a single segment
void Test4(void);  // Hex digits
void Test5(void);  // Text String with Decimal point
void Test6(void);  // TEXT + ASCII combo
void Test7(void);  // Integer Decimal number
void Test8(void);  // Text String + Float hack
void Test9(void);  // Text String + decimal number
void Test10(void);  // Multiple Decimal points
void Test11(void);  // Display Overflow
void Test12(void);  // Scrolling text
void Test13(void);  // setLED and setLEDs
void Test14(void);  // Buttons + LEDS NOTE: Press S1 & S8 together to quit

void doLEDs(uint8_t value);
static uint64_t RPI_Millis( void );
void signal_callback_handler(int signum);

int main()
{
#ifdef TM1638_MODEL_ONE_TEST
	printf("Test Begin :: Model 1 :: TM1638plus_RPI\r\n");
#endif
#ifdef TM1638_MODEL_THREE_TEST
	printf("Test Begin :: Model 3 :: TM1638plus_RPI\r\n");
#endif

	signal(SIGINT, signal_callback_handler); // Ctrl + C

	tm.displayBegin();
	delaySecRDL(myTestDelay1);
	
	Test0();  // Test 0 reset
	Test1();  // Brightness
	Test2();  // ASCII display
	Test3();  // Set a single segment
	Test4();  // Hex digits
	Test5();  // Text String with Decimal point
	Test6();  // TEXT + ASCII combo
	Test7();  // Integer Decimal number
	Test8();  // Text String + Float hack
	Test9();  // Text String + decimal number
	Test10();  // Multiple Decimal points
	Test11();  // Display Overflow
	Test12();  // Scrolling text
	Test13();  // setLED and setLEDs
	Test14();  // Buttons + LEDS NOTE: Press S1 & S8 together to quit
	
	tm.reset();
	tm.displayClose();
	printf("Test End\r\n");
	return 0;
}

void Test0()
{
	// Test 0 reset test
	printf("Test 0: Reset\r\n");
	tm.setLED(0, 1);
	tm.displayText("testing");
	delaySecRDL(myTestDelay);
	tm.reset();
}

void Test1() {
	// Test 1  Brightness and reset
	printf("Test 1: Brightness \r\n");
	for (uint8_t brightness = 0; brightness < 8; brightness++)
	{
		tm.brightness(brightness);
		tm.displayText("00000000");
		delaySecRDL(myTestDelay1);
	}
	tm.reset();
	// restore default brightness
	tm.brightness(0x02);
}

void Test2() {
	//Test 2 ASCII , display 2.341
	printf("Test 2: ASCII\r\n");
	tm.displayASCIIwDot(0, '2');
	tm.displayASCII(1, '3');
	tm.displayASCII(2, '4');
	tm.displayASCII(3, '1');
	delaySecRDL(myTestDelay1);
	tm.reset();
}

void Test3() {
	//TEST 3 single segment (digit position, (dp)gfedcba)
	// (dp)gfedcba =  seven segments positions
	// Displays a single seg in (dp)gfedcba) in each pos 0-7
	uint8_t pos = 0;
	printf("Test 3: Set a single segment\r\n");
	for (pos = 0 ; pos<8 ; pos++)
	{
		tm.display7Seg(pos, 1<<(7-pos)); 
		delaySecRDL(myTestDelay1);
	}
}

void Test4() {
	// Test 4 Hex digits.
	printf("Test 4: Hexadecimal\r\n");
	tm.displayHex(0, 0);
	tm.displayHex(1, 1);
	tm.displayHex(2, 2);
	tm.displayHex(3, 3);
	tm.displayHex(4, 4);
	tm.displayHex(5, 5);
	tm.displayHex(6, 6);
	tm.displayHex(7, 7);  
	delaySecRDL(myTestDelay); // display 12345678

	tm.displayHex(0, 8);
	tm.displayHex(1, 9);
	tm.displayHex(2, 0x0A);
	tm.displayHex(3, 0x0B);
	tm.displayHex(4, 0x0C);
	tm.displayHex(5, 0x0D);
	tm.displayHex(6, 0x0E);
	tm.displayHex(7, 0x0F);
	delaySecRDL(myTestDelay); // display 89ABCDEF
	tm.reset();
	
	tm.displayHex(1, 0xFE);
	tm.displayHex(7, 0x10);
	delaySecRDL(myTestDelay);// display " E      0"
}

void Test5() {
	// Test 5 TEXT  with dec point
	// abcdefgh with decimal point for c and d
	printf("Test 5: Text String with Decimal point\r\n");
	tm.displayText("abc.d.efgh");
	delaySecRDL(myTestDelay);
}

void Test6() {
	// Test6  TEXT + ASCII combo
	// ADC=.2.948
	printf("Test 6: TEXT + ASCII combo\r\n");
	char text1[] = "ADC=.";
	tm.displayText(text1);
	tm.displayASCIIwDot(4, '2');
	tm.displayASCII(5, '9');
	tm.displayASCII(6, '4');
	tm.displayASCII(7, '8');
	delaySecRDL(myTestDelay);
	tm.reset();
}

void Test7() {
	printf("Test 7: Integer\r\n");
	// TEST 7a Integer right aligned
	tm.displayIntNum(45, tm.TMAlignTextRight); // "        45"
	delaySecRDL(myTestDelay);
	tm.reset();
	// TEST 7b Integer left aligned 
	tm.displayIntNum(798311, tm.TMAlignTextLeft); // "798311  "
	delaySecRDL(myTestDelay);
	tm.reset();
	// TEST 7c Integer // leading zeros
	tm.displayIntNum(93391, tm.TMAlignTextZeros); // "00093391"
	delaySecRDL(myTestDelay);

	
	// TEST 7d tm.DisplayDecNumNIbble right aligned
	tm.DisplayDecNumNibble(134, 78, tm.TMAlignTextRight); // " 134" 78"
	delaySecRDL(myTestDelay);
	tm.reset();
	// TEST 7e tm.DisplayDecNumNIbble left aligned
	tm.DisplayDecNumNibble(123, 662, tm.TMAlignTextLeft); // "123 662 "
	delaySecRDL(myTestDelay);
	tm.reset();
	// TEST 7f tm.DisplayDecNumNIbble leading zeros
	tm.DisplayDecNumNibble(493, 62, tm.TMAlignTextZeros); // "04930062"
	delaySecRDL(myTestDelay);
	tm.reset();
}

void Test8() {
	// TEST 8  TEXT STRING + integer SSSSIIII
	printf("Test 8: TEXT STRING + integer\r\n");
	char workStr[11];
	uint16_t  data = 234;
	sprintf(workStr, "ADC=.%04d", data); // "ADC=.0234"
	tm.displayText(workStr);
	delaySecRDL(myTestDelay);
}

void Test9() {
	// TEST 9 Text String + Float  SSSSFFFF ,  just one possible method.
	printf("Test 9: Text String + Float \r\n");
	float voltage = 12.45;
	char workStr[11];
	sprintf(workStr, "ADC=%.2f", voltage);
	tm.displayText(workStr); // ADC=12.45
	delaySecRDL(myTestDelay);
	tm.reset();
}

void Test10()
{
	//TEST 10 Multiple dots test
	printf("Test 10: Multiple dots test \r\n");
	tm.displayText("Hello...");
	delaySecRDL(myTestDelay);
	tm.displayText("...---..."); //SOS in morse
	delaySecRDL(myTestDelay);
}

void Test11()
{
	//TEST11 user overflow
	printf("Test 11: overflow test \r\n");
	tm.displayText("1234567890abc"); //should display just 12345678
	delaySecRDL(myTestDelay1);
	tm.reset();
}


void Test12() {
	//TEST 12 scrolling text, just one possible method.
	printf("Test 12: Scroll test  \r\n");
	char textScroll[17] = "Scroll test 1234";
	unsigned long previousMillis_display = 0;  // will store last time display was updated
	const long interval_display = 1000;  //   interval at which to update display (milliseconds)

	while(1)
	{
	tm.displayText(textScroll);
	unsigned long currentMillis = RPI_Millis();
	
	if (currentMillis - previousMillis_display >= interval_display)
	{
		previousMillis_display = currentMillis;
		if (strlen(textScroll) > 0)
		{
			memmove(textScroll, textScroll+1, strlen(textScroll));
			tm.displayText("        "); //Clear display or last character will drag across screen
		}else
		{
			return;
		}
	 }
	}
}

// Model 1
#ifdef TM1638_MODEL_ONE_TEST
void Test13()
{
	//Test 13 LED display
	printf("Test 13: LED \r\n");
	uint8_t LEDposition = 0;

	// Test 13A Turn on redleds one by one, left to right, with setLED where 0 is L1 and 7 is L8 (L8 RHS of display)
	for (LEDposition = 0; LEDposition < 8; LEDposition++) {
		tm.setLED(LEDposition, 1);
		delaySecRDL(1);
		tm.setLED(LEDposition, 0);
	}

	// TEST 13b test setLEDs function (0xLXX) ( L8-L1 , XX )
	// NOTE passed L8-L1 and on display L8 is on right hand side. i.e. 0x01 turns on L1. LXXX XXXX

	tm.setLEDs(0xFF); //  all LEDs on 
	delaySecRDL(3);
	tm.setLEDs(0x01); // Displays as LXXX XXXX (L1-L8) , NOTE on display L8 is on right hand side.
	delaySecRDL(3);
	tm.setLEDs(0xF0); //  Displays as XXXX LLLL (L1-L8) , NOTE on display L8 is on right hand side.
	delaySecRDL(3);
	tm.setLEDs(0x07); //  Displays as LLLX XXXX (L1-L8) , NOTE on display L8 is on right hand side.
	delaySecRDL(3);
	tm.setLEDs(0x41); //  Displays as LXXX XXLX (L1-L8) , NOTE on display L8 is on right hand side.
	delaySecRDL(3);
	tm.setLEDs(0x00); // all off
	delaySecRDL(3);

}
#endif 

// model 3
#ifdef TM1638_MODEL_THREE_TEST
void Test13()
{
	//Test 13 LED display
	uint8_t LEDposition = 0;
	printf("Test 13: LED \r\n");
	// Test 13A Turn on green leds with setLED
	for (LEDposition = 0; LEDposition < 8; LEDposition++) {
		tm.setLED(LEDposition, tm.TM_GREEN_LED);
		delaySecRDL(0.5);
		tm.setLED(LEDposition, tm.TM_OFF_LED);
	}

	// Test 13b turn on red LEDs with setLED
	for (LEDposition = 0; LEDposition < 8; LEDposition++) {
		tm.setLED(LEDposition, tm.TM_RED_LED);
		delaySecRDL(0.5);
		tm.setLED(LEDposition, tm.TM_OFF_LED);
	}

	// TEST 13c 
	// test setLEDs function (0xgreenred) (0xGGRR) (LED8-LED1, LED8-LED1)
	// Upper byte switch LED green colour ON, lower byte = switch LED red colour ON
	// NB Note on the unit, LED8 is on the right hand side so result is mirrored.
	// Example:
	// E0 = green on , 07 = red on 
	// E0  = 1110 0000 , 07 = 0000 0111 = 11100111 = GGGXXRRR = LED8-LED1
	// Shows on display as  LED1-LED8 turns on RRRXXGGG as LED 8 is on right hand side.

	tm.setLEDs(0xE007); //L1-L8 turns on RRRX XGGG on display
	delaySecRDL(3);

	tm.setLEDs(0xF00F); // L1-L8 turns on RRRR GGGG on display
	delaySecRDL(3);
	tm.setLEDs(0xFE01); // L1-L8 turns on RGGG GGGG on display
	delaySecRDL(3);
	tm.setLEDs(0x00FF); //all red   RRRR RRRR
	delaySecRDL(3);
	tm.setLEDs(0xFF00); //all green GGGG GGGG
	delaySecRDL(3);
	tm.setLEDs(0x0000); //all off   XXXX XXXX
	delaySecRDL(3);

}
#endif

void Test14() {
	//Test 14 buttons and LED test, press switch number S-X to turn on LED-X, where x is 1-8.
	// NOTE: Press S1 & S8 together to quit
	printf("Test 14: Buttons :: Press S1 & S8 together to quit\r\n");
	tm.displayText("buttons ");
	delaySecRDL(2);
	while (1) // Loop here until user quits 
	{
		uint8_t buttons = tm.readButtons();
			/* buttons contains a byte with values of button s8s7s6s5s4s3s2s1
			 HEX  :  Switch no : Binary
			 0x01 : S1 Pressed  0000 0001 
			 0x02 : S2 Pressed  0000 0010 
			 0x04 : S3 Pressed  0000 0100 
			 0x08 : S4 Pressed  0000 1000 
			 0x10 : S5 Pressed  0001 0000 
			 0x20 : S6 Pressed  0010 0000 
			 0x40 : S7 Pressed  0100 0000 
			 0x80 : S8 Pressed  1000 0000  
			*/
		doLEDs(buttons);
		tm.displayIntNum(buttons, tm.TMAlignTextRight); 
		delaySecRDL(0.2); 
		if (buttons == 129) break; // if Press S1 & S8 together =  quit loop
	}
	tm.reset();
	tm.displayText("end test");
	delaySecRDL(2); 
}

// scans the individual bits of value sets a LED based on which button pressed
void doLEDs(uint8_t value) {
	for (uint8_t LEDposition = 0; LEDposition < 8; LEDposition++) {
		tm.setLED(LEDposition, value & 1);
		value = value >> 1;
	}
}

//This returns milli-seconds as a 64-bit unsigned number, monotonically increasing, 
//since system boot. 
static uint64_t RPI_Millis( void )
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  (uint64_t)now.tv_sec * 1000U + ((uint64_t)now.tv_nsec/1000000U);
}

// Terminate program on ctrl + C 
void signal_callback_handler(int signum)
{
	tm.reset();
	tm.displayClose();
	printf("Test End, Ctrl C pressed\r\n");
	exit(signum);
}
