/*!
	@file examples/tm1638/TEST_Model2/main.cpp
	@author Gavin Lyons
	@brief A demo file library for TM1638 module Works on Model 2
	Carries out series of tests demonstrating arduino library TM1638plus.
	Project Name: Display_Lib_RPI
	URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test
		-# Test 0 reset test
		-# Test 1 decimal and float numbers
		-# Test 2 Hexadecimal number
		-# Test 3a 3b & 3C using DisplaySegments method
		-# Test 4 strings
		-# Test 5 ASCIItoSegment method
		-# Test 6 Brightness and reset
		-# Test 7 scroll text
		-# Test 8 Buttons , ReadKey16() returns byte 1-16 decimal, press S16 to goto test9
		-# Test 9 Buttons , Readkey16Two() alternate buttons function.

	The tests will increment automatically with exception of 8 & 9,
	Pressing Switch 16 during test 8 moves to test 9 and pressing it during test 9
	ends program.
*/


#include <TM1638plus_Model2_RDL.hpp>
#include <ctime>
#include <signal.h> //catch user Ctrl+C
#include <cstdlib> //exit function

// GPIO I/O pins on the RPI connected to strobe, clock, data, pick on any I/O pin you want.
#define  STROBE_TM 21 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 20  // clock = GPIO connected to clock line of module
#define  DIO_TM 16 // data = GPIO connected to data line of module
#define  GPIO_CHIP_DEVICE 0 // Gpio Chip device number see ->ls /dev/gpio*
bool swap_nibbles = false; //Default is false if left out, see note in readme at URL

// Constructor object
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, GPIO_CHIP_DEVICE, swap_nibbles);

// For test setup
#define  myTestDelay 5
#define  myTestDelay1 2
#define  myTestDelay2 0.2

// ************* Function prototypes ***************
void Test1(void);
void Test2(void);
void Test3(void);
void Test4(void);
void Test5(void);
void Test6(void);
void Test7(void);
void Test8(void);
void Test9(void);
static uint64_t RPI_Millis( void );
void signal_callback_handler(int signum);

// *************** Main *****************
int main()
{
	signal(SIGINT, signal_callback_handler); // Ctrl + C
	printf("Test Begin :: Model 2 :: TM1638plus\r\n");
	delaySecRDL(myTestDelay1);
	tm.TMCommDelayset(10); // 10 us Delay 
	tm.displayBegin(); // Init the module
	tm.DisplayStr("testing", 0);
	delaySecRDL(myTestDelay1);

	tm.reset(); // Test 0 reset test
	Test1();  // Test 1 decimal and float numbers
	Test2();  // Test 2 Hexadecimal number
	Test3();  // Test 3a 3b & 3C using DisplaySegments method
	Test4();  // Test 4 strings
	Test5();  // Test 5 ASCIItoSegment method
	Test6();  // Test 6  Brightness and reset
	Test7();  // Test 7 scroll text
	Test8();  // Test 8 Buttons , ReadKey16() returns byte 1-16 decimal, press S16 to goto test9
	Test9();  // Test 9 Buttons , Readkey16Two() alternate buttons function.

	tm.reset();
	tm.displayClose();
	printf("Test End\r\n");
	return 0;
}
// *************** End of Main *****************



// ************** Function Space ***********
void Test1(void)
{
	printf("Test 1: Numbers \r\n");
	// Test 1a decimal number right aligned
	tm.DisplayDecNum(250, 1 << 2, tm.TMAlignTextRight); // "     2.50"
	delaySecRDL(myTestDelay);
	// Test 1b decimal number left aligned
	tm.DisplayDecNum(99791, 1 << 4, tm.TMAlignTextLeft); // "9979.1   "
	delaySecRDL(myTestDelay);
	// Test 1c decimal number leading zeros
	tm.DisplayDecNum(2882, 0 , tm.TMAlignTextZeros);  // 00002882
	delaySecRDL(myTestDelay);

	// Test 1d negative number
	tm.DisplayDecNum(-33, 0 , tm.TMAlignTextLeft); // "-33        "
	delaySecRDL(myTestDelay);

	// Test 1e  decimal numbers with the DisplayDecNumNibble right aligned
	tm.DisplayDecNumNibble(213 , 78, 0 , tm.TMAlignTextRight); // " 213  78"
	delaySecRDL(myTestDelay);
	// Test 1f  decimal numbers with the DisplayDecNumNibble left aligned
	tm.DisplayDecNumNibble(2 , 95, 1<<3 , tm.TMAlignTextLeft); // "2   9.5  "
	delaySecRDL(myTestDelay);
	// Test 1g  decimal numbers with the DisplayDecNumNibble leading zeros
	tm.DisplayDecNumNibble(134 , 47, 1<<1 , tm.TMAlignTextZeros); // "0134004.7"
	delaySecRDL(myTestDelay);
}

void Test2(void)
{
	printf("Test 2: Hexadecimal numbers \r\n");
	// Test 2a Hexadecimal number right aligned
	tm.DisplayHexNum(0x00FF, 0x056E, 0x00, tm.TMAlignTextRight); //"  FF 56E"
	delaySecRDL(myTestDelay);
	// Test 2a Hexadecimal number left aligned
	tm.DisplayHexNum(0x0ABC, 0x000F, 0x00, tm.TMAlignTextLeft); // "ABC F   "
	delaySecRDL(myTestDelay);
	// Test 2a Hexadecimal number leading zeros
	tm.DisplayHexNum(0x0EE1, 0x00F4, 1 << 4, tm.TMAlignTextZeros); // "0EE1.00F4"
	delaySecRDL(myTestDelay);
}

void Test3(void)
{
	// Test 3 manually set segments abcdefg(dp) = 01234567
	printf("Test 3: DisplaySegments method \r\n");
	// display a one in position one "       1"
	tm.DisplaySegments(0, 0x00); //a
	tm.DisplaySegments(1, 0x01); //b, for b turn on digit one only
	tm.DisplaySegments(2, 0x01); //c, for c turn on digit one only
	tm.DisplaySegments(3, 0x00); //d
	tm.DisplaySegments(4, 0x00); //e
	tm.DisplaySegments(5, 0x00); //f
	tm.DisplaySegments(6, 0x00); //g
	tm.DisplaySegments(7, 0x00); //DP
	delaySecRDL(myTestDelay);

	// Test 3b manually set segments
	// Display "00000005"
	tm.DisplaySegments(0, 0xFF); //a, turn a on for all digits
	tm.DisplaySegments(1, 0xFE); //b
	tm.DisplaySegments(2, 0xFF); //c
	tm.DisplaySegments(3, 0xFF); //d
	tm.DisplaySegments(4, 0xFE); //e
	tm.DisplaySegments(5, 0xFF); //f
	tm.DisplaySegments(6, 0x01); //g , for g middle segment, digit one only on
	tm.DisplaySegments(7, 0x00); //decimal point, turn off all decmial points
	delaySecRDL(myTestDelay);

	// Test 3c manually set segments scroll g
	// Display "-" countup to "--------"
	tm.reset();
	uint8_t dashvalue = 1;
	for (uint8_t j = 0; j < 8;  j++)
	{

		tm.DisplaySegments(6, dashvalue); // g scrolling
		dashvalue = (dashvalue*2)+1; // 1 to 256
		delaySecRDL(myTestDelay1);
	}
	delaySecRDL(myTestDelay1);

}

void Test4(void)
{
	// Test 4 strings
	printf("Test 4: Strings \r\n");
	tm.DisplayStr("helloYOU", 1); // "helloYOU."
	delaySecRDL(myTestDelay);
	tm.DisplayStr("      Hi", 0x08); // "     . Hi"
	delaySecRDL(myTestDelay);
	tm.DisplayStr("   ---   ", 0XE7); // ". . .---. . ."
	delaySecRDL(myTestDelay);
	tm.DisplayStr(" helloU2", 0); // " helloU2"
	delaySecRDL(myTestDelay);
	tm.DisplayStr("string", 0);  // "string   "
	delaySecRDL(myTestDelay);
}

void Test5(void)
{
	printf("Test 5: ASCII to segments method\r\n");
	// Test 5 ASCII to segments takes an array of bytes and displays them
	// without ref to the ASCII font table direct data to digits to displays 3F 3F 3F 6D 3F 3F 3F 6D = 00050005
	// gfedcba = 3F for zero https://en.wikipedia.org/wiki/Seven-segment_display

	const uint8_t values[8] = {0x3F, 0x3F, 0x3F, 0x6D, 0x3F, 0x3F, 0x3F, 0x6D}; // for ascii to segment test 00050005

	tm.ASCIItoSegment(values);

	delaySecRDL(myTestDelay);
}

void Test6(void)
{
	// Test 6  Brightness and reset
	printf("Test 6: Brightness Test\r\n");
	for (uint8_t brightness = 0; brightness < 8; brightness++)
	{
		tm.brightness(brightness);
		tm.DisplayStr("brightnes", 1);
		delaySecRDL(myTestDelay1);
	}
	tm.brightness(2);
	tm.reset();
}


// Just one possible method to scroll text there are many others.
void Test7(void)
{
	printf("Test 7: Scroll Test\r\n");
	char textScroll[17] = " Hello world 123";
	unsigned long previousMillis_display = 0;  // will store last time display was updated
	const long interval_display = 1000;            //   interval at which to update display (milliseconds)

	while(1)
	{
	tm.DisplayStr(textScroll, 0);
	unsigned long currentMillis = RPI_Millis();
	//  update data every interval_display delay
	if (currentMillis - previousMillis_display >= interval_display)
	{
		previousMillis_display = currentMillis;
		if (strlen(textScroll) > 0)
		{
			memmove(textScroll, textScroll+1, strlen(textScroll)); // delete first char in array.
		}else
		{
			return;
		}
	 }
	}
}


void Test8(void)
{
	printf("Test 8: Buttons :: Press S16 to go to test 9 \r\n");
	unsigned char buttons;
	while(1)
	{
		// Test 8 , buttons readkey16() function, no debounce see notes at URL for example to debounce.
		// returns 0-16 , 0 for nothing pressed.
		// NOTE: pressing  S16 will move to test 9
		buttons = tm.ReadKey16();
		tm.DisplayDecNum(buttons, 0 , tm.TMAlignTextRight);
		delaySecRDL( myTestDelay2);
		if (buttons == 16)
		{
			 //pressing 16 moves  to test 9
			delaySecRDL( myTestDelay2);
			return;
		}
		}
}

//returns word with binary value of switch. S16 = Bit 15 , S15 = bit 14 etc
void Test9(void)
{
	printf("Test 9: Buttons :: Press S16 to quit\r\n");
	uint16_t buttons=0;
	tm.DisplayStr("buttons2", 0);
	delaySecRDL( myTestDelay2);
	tm.reset();
	while(1)
		{
		// Test 9 buttons Read_key16_two() function
		// returns a uint16_t where each bit represents a switch.
		// S16S15S14S13S12S11S10S9S8S7S6S5S4S3SS2S1.
		// eg S1 =  0x0001
		// eg S8 =  0x0080
		// eg S16 = 0x8000
		// eg S1 + S16 = 0x8001
		// Can be used to detect multi key presses , see Notes section in readme.
		// For issues related to display when pressing multi keys together.
		buttons = tm.ReadKey16Two();
		tm.DisplayHexNum(0x0000, buttons, 0x00, tm.TMAlignTextZeros);
		delaySecRDL( myTestDelay2);
		if (buttons == 0x8000)
		{
			//pressing 16 to quit
			delaySecRDL( myTestDelay2);
			tm.DisplayStr("quiting", 0);
			delaySecRDL( myTestDelay1);
			return;
		}
		delaySecRDL( myTestDelay2);
		}
}

// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	tm.reset();
	tm.displayClose();
	printf("Test End, Ctrl C pressed\r\n");
	exit(signum);
}

//This returns milli-seconds as a 64-bit unsigned number, monotonically increasing,
//since system boot.
static uint64_t RPI_Millis( void )
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  (uint64_t)now.tv_sec * 1000U + ((uint64_t)now.tv_nsec/1000000U);
}
// ***************** EOF ****************
