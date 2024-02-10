/*!
	@file examples/seven_segment_displays/max7219/TESTS_HWSPI/main.cpp
	@author Gavin Lyons
	@brief A demo file for Max7219 seven segment displays ,
	Carries out series of tests to test the library. Hardware SPI
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
#include <bcm2835.h>
#include <cstdio> // Used for printf
#include <MAX7219_7SEG_RDL.hpp> 

// Delays for testing
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY1 1000

// Hardware SPI setup
uint32_t SPI_SCLK_FREQ =  5000; // HW Spi only , freq in kiloHertz , MAX 125 Mhz MIN 30Khz
uint8_t SPI_CEX_GPIO   =  0;     // HW Spi only which HW SPI chip enable pin to use,  0 or 1

// Constructor object 
MAX7219_SS_RPI myMAX(SPI_SCLK_FREQ, SPI_CEX_GPIO);

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


// Main loop
int main() 
{
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
	if(!bcm2835_init())  // Init the bcm2835 library
	{
		printf("Error 1201 :: bcm2835_init failed. Are you running as root??\n");
		return false;
	}
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("MAX7219_7SEG Library version number :: %u\r\n", GetRDLibVersionNum()); 
	if(myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone) != rpiDisplay_Success)
	{
		printf("Error 1202 :: bcm2835_spi_begin failed. Are you running as root??\n");
		return false;
	}
	myMAX.ClearDisplay();
	return true;
}

// Clean up before exit
void EndTest(void)
{
	myMAX.DisplayEndOperations();
	bcm2835_close();  // Close the bcm2835 library
	printf("Test End\r\n");
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
	MAX7219_MilliSecondDelay(TEST_DELAY2);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr1, myMAX.AlignRight);
	MAX7219_MilliSecondDelay(TEST_DELAY2);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr2, myMAX.AlignRight);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr3, myMAX.AlignLeft);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr4, myMAX.AlignLeft);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr5, myMAX.AlignLeft);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr6, myMAX.AlignRight);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test2(void)
{
	char teststr1[] = "Bright";
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	printf("Test 2:: Brightness \r\n");
	myMAX.SetBrightness(myMAX.IntensityMin);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.SetBrightness(myMAX.IntensityMax);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.SetBrightness(myMAX.IntensityDefault);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test3(void) 
{
	printf("Test 3:: Display Test Mode \r\n");
	myMAX.DisplayTestMode(true);
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.DisplayTestMode(false);
}

void Test4(void) 
{
	printf("Test 4:: Shutdown Mode\r\n");
	char teststr1[] = "shutdown";
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	MAX7219_MilliSecondDelay(TEST_DELAY2);
	myMAX.ShutdownMode(true);
	MAX7219_MilliSecondDelay(TEST_DELAY2);
	myMAX.ShutdownMode(false);
	MAX7219_MilliSecondDelay(TEST_DELAY2);
	myMAX.ClearDisplay();
}

void Test5(void)
{
	printf("Test 5:: Display characters ab.cde1.23. \r\n");
	myMAX.DisplayChar(7, 'a', myMAX.DecPointOff);  // Digit 7 is LHS of display
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.DisplayChar(6, 'b', myMAX.DecPointOn);
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.DisplayChar(5, 'c', myMAX.DecPointOff);
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.DisplayChar(4, 'd', myMAX.DecPointOff);
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.DisplayChar(3, 'e', myMAX.DecPointOff);
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.DisplayChar(2, '1', myMAX.DecPointOn);
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.DisplayChar(1, '2', myMAX.DecPointOff);
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.DisplayChar(0, '3', myMAX.DecPointOn);
	MAX7219_MilliSecondDelay(TEST_DELAY1);
	myMAX.ClearDisplay();
}

void Test6(void)
{
	printf("Test 6:: Set digit to Segments, 76543210 = dpabcdefg. \r\n");
	for (uint8_t digit = 0; digit <8 ; digit++)
	{ 
		myMAX.SetSegment(digit, 1<<digit);
		MAX7219_MilliSecondDelay(TEST_DELAY1);
	}
	myMAX.ClearDisplay();
}

void Test7(void)
{
	printf("Test 7:: Decimal number \r\n");
	myMAX.DisplayIntNum(45, myMAX.AlignRight); // "        45"
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	
	myMAX.ClearDisplay();
	myMAX.DisplayIntNum(798311, myMAX.AlignLeft); // "798311  "
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	
	myMAX.DisplayIntNum(93391, myMAX.AlignRightZeros); // "00093391"
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	
	myMAX.DisplayIntNum(12345678, myMAX.AlignRight); // "12345678"
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7d tm.DisplayDecNumNIbble right aligned
	myMAX.DisplayDecNumNibble(134, 78, myMAX.AlignRight); // " 134" 78"
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7e tm.DisplayDecNumNIbble left aligned
	myMAX.DisplayDecNumNibble(123, 662, myMAX.AlignLeft); // "123 662 "
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7f tm.DisplayDecNumNIbble leading zeros
	myMAX.DisplayDecNumNibble(493, 62, myMAX.AlignRightZeros); // "04930062"
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test8(void)
{
	//TEST 8 Multiple dots test
	printf("Test 8: Multiple dots test \r\n");
	char teststr1[] = "Hello...";
	char teststr2[] = "...---...";
	myMAX.DisplayText(teststr1);   
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
	myMAX.DisplayText(teststr2);//SOS in morse
	MAX7219_MilliSecondDelay(TEST_DELAY5);

	//TEST8b user overflow
	printf("Test 8B: overflow test \r\n");
	char teststr3[] = "1234567890abc";
	myMAX.DisplayText(teststr3); //should display just 12345678
	MAX7219_MilliSecondDelay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test9(void)
{
	printf("Test 9:  Float \r\n");
	float voltage = 12.45;
	char workStr[11];
	sprintf(workStr, "ADC=%.2f", voltage);
	
	myMAX.DisplayText(workStr); // ADC=12.45
	MAX7219_MilliSecondDelay(TEST_DELAY5);
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
		MAX7219_MilliSecondDelay(TEST_DELAY1);
	}
	myMAX.ClearDisplay();
}


// == EOF ==
