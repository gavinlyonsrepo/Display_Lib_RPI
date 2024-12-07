/*!
	@file examples/max7219/TESTS_SWSPI/main.cpp
	@author Gavin Lyons
	@brief A demo file for Max7219 seven segment displays ,
	Carries out series of tests to test the library. Software SPI
	Project Name: Display_Lib_RPI
	
	@test
		-# Test 1 Text strings display 
		-# Test 2 Brightness
		-# Test 3 Test Display Mode
		-# Test 4 ShutDown Mode
		-# Test 5 Character display 
		-# Test 6 Set Segment
		-# Test 7 Decimal number
		-# Test 8 Multiple Decimal points + Display Overflow  
		-# Test 9 Floating point
		-# Test 10 Counter
*/

// Libraries 
#include <cstdio> // Used for printf
#include <signal.h> //catch user Ctrl+C
#include <cstdlib> //exit function
#include <lgpio.h>
#include <MAX7219_7SEG_RDL.hpp> 

// GPIO I/O pins on the raspberry pi ,pick on any I/O you want.
uint8_t  CLK =16;  // clock GPIO, connected to clock line of module
uint8_t  CS =20;   // Chip Select GPIO, connected to CS line of module
uint8_t  DIN =21;  // data in GPIO, connected to DIN line of module
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0


// Delays for testing
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY1 1000

// Constructor object 
MAX7219_SS_RPI myMAX(CLK, CS ,DIN, GPIO_CHIP_DEVICE);

// Function Prototypes
bool Setup(void);
void EndTest(void);
void Test1(void);
void Test2(void);
void Test3(void);
void Test4(void);
void Test5(void);
void Test6(void);
void Test7(void);
void Test8(void);
void Test9(void);
void Test10(void);
void Test11(void);
void signal_callback_handler(int signum);

// Main loop
int main() 
{

	signal(SIGINT, signal_callback_handler); // Ctrl + C
	if (!Setup()) return -1;

	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	Test6();
	Test7();
	Test8();
	Test9();
	Test10();
	
	EndTest();
	return 0;
} 
// End of main

// Functions

// Setup test
bool Setup(void)
{
	printf("Test Begin :: MAX7219_7SEG_RPI\r\n");
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum()); 
	if (myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone) != rpiDisplay_Success)
	{
		return false;
	}
	myMAX.ClearDisplay();
	return true;
}

// Clean up before exit
void EndTest(void)
{
	myMAX.DisplayEndOperations();
	printf("Test End\r\n");
}

// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	myMAX.ClearDisplay();
	myMAX.DisplayEndOperations();
	printf("Test End, Ctrl C pressed\r\n");
	exit(signum);
}

void Test1(void)
{
	printf("Test 1:: DisplayText \r\n");
	// Hello world test on MAX7219
	char teststr1[] = "Start";
	char teststr2[] = "-62.95";
	char teststr3[] = "37.45";
	
	char teststr4[] = "12-44-21";
	char teststr5[] = "07/31/19";
	char teststr6[] = "11.12.2023";
	
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	delayMilliSecRDL(TEST_DELAY2);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr1, myMAX.AlignRight);
	delayMilliSecRDL(TEST_DELAY2);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr2, myMAX.AlignRight);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr3, myMAX.AlignLeft);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr4, myMAX.AlignLeft);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr5, myMAX.AlignLeft);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr6, myMAX.AlignRight);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test2(void)
{
	char teststr1[] = "Bright";
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	printf("Test 2:: Brightness \r\n");
	myMAX.SetBrightness(myMAX.IntensityMin);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.SetBrightness(myMAX.IntensityMax);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.SetBrightness(myMAX.IntensityDefault);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test3(void) 
{
	printf("Test 3:: Display Test Mode \r\n");
	myMAX.DisplayTestMode(true);
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.DisplayTestMode(false);
}

void Test4(void) 
{
	printf("Test 4:: Shutdown Mode\r\n");
	char teststr1[] = "shutdown";
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	delayMilliSecRDL(TEST_DELAY2);
	myMAX.ShutdownMode(true);
	delayMilliSecRDL(TEST_DELAY2);
	myMAX.ShutdownMode(false);
	delayMilliSecRDL(TEST_DELAY2);
	myMAX.ClearDisplay();
}

void Test5(void)
{
	printf("Test 5:: Display characters ab.cde1.23. \r\n");
	myMAX.DisplayChar(7, 'a', myMAX.DecPointOff);  // Digit 7 is LHS of display
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.DisplayChar(6, 'b', myMAX.DecPointOn);
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.DisplayChar(5, 'c', myMAX.DecPointOff);
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.DisplayChar(4, 'd', myMAX.DecPointOff);
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.DisplayChar(3, 'e', myMAX.DecPointOff);
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.DisplayChar(2, '1', myMAX.DecPointOn);
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.DisplayChar(1, '2', myMAX.DecPointOff);
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.DisplayChar(0, '3', myMAX.DecPointOn);
	delayMilliSecRDL(TEST_DELAY1);
	myMAX.ClearDisplay();
}

void Test6(void)
{
	printf("Test 6:: Set digit to Segments, 76543210 = dpabcdefg. \r\n");
	for (uint8_t digit = 0; digit <8 ; digit++)
	{ 
		myMAX.SetSegment(digit, 1<<digit);
		delayMilliSecRDL(TEST_DELAY1);
	}
	myMAX.ClearDisplay();
}

void Test7(void)
{
	printf("Test 7:: Decimal number \r\n");
	myMAX.DisplayIntNum(45, myMAX.AlignRight); // "        45"
	delayMilliSecRDL(TEST_DELAY5);
	
	myMAX.ClearDisplay();
	myMAX.DisplayIntNum(798311, myMAX.AlignLeft); // "798311  "
	delayMilliSecRDL(TEST_DELAY5);
	
	myMAX.DisplayIntNum(93391, myMAX.AlignRightZeros); // "00093391"
	delayMilliSecRDL(TEST_DELAY5);
	
	myMAX.DisplayIntNum(12345678, myMAX.AlignRight); // "12345678"
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7d tm.DisplayDecNumNIbble right aligned
	myMAX.DisplayDecNumNibble(134, 78, myMAX.AlignRight); // " 134" 78"
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7e tm.DisplayDecNumNIbble left aligned
	myMAX.DisplayDecNumNibble(123, 662, myMAX.AlignLeft); // "123 662 "
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7f tm.DisplayDecNumNIbble leading zeros
	myMAX.DisplayDecNumNibble(493, 62, myMAX.AlignRightZeros); // "04930062"
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test8(void)
{
	//TEST 8 Multiple dots test
	printf("Test 8: Multiple dots test \r\n");
	char teststr1[] = "Hello...";
	char teststr2[] = "...---...";
	myMAX.DisplayText(teststr1);   
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
	myMAX.DisplayText(teststr2);//SOS in morse
	delayMilliSecRDL(TEST_DELAY5);

	//TEST8b user overflow
	printf("Test 8B: overflow test \r\n");
	char teststr3[] = "1234567890abc";
	myMAX.DisplayText(teststr3); //should display just 12345678
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test9(void)
{
	printf("Test 9:  Float \r\n");
	float voltage = 12.45;
	char workStr[11];
	sprintf(workStr, "ADC=%.2f", voltage);
	
	myMAX.DisplayText(workStr); // ADC=12.45
	delayMilliSecRDL(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test10(void)
{
	printf("Test 10: counter Demo \r\n");
	char workStr[10];
	for (float counter = 0; counter < 3.0; counter += 0.2) 
	{
		sprintf(workStr, "%.1f", counter);
		myMAX.DisplayText(workStr, myMAX.AlignRight); 
		delayMilliSecRDL(TEST_DELAY1);
	}
	myMAX.ClearDisplay();
}


// == EOF ==
