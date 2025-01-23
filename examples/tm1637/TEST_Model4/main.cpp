/*!
	@file examples/tm1637/TEST_Model4/main.cpp
	@author Gavin Lyons
	@brief A demo file library for TM1637 module Works on Model 4
		Carries out series of tests demonstrating arduino library TM1637plus.
		Project Name: Display_Lib_RPI
	@details
			setSegments function those not touch the ASCII font table and takes raw data
			Segment data used by other functions is is from the ASCII table in font file 
			gfedcba (seven segments ) In  gfedcba representation,
			a byte value of 0x06 would turn on segments "c" and "b", which would display a "1".
			https://en.wikipedia.org/wiki/Seven-segment_display
			URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@note
			Only tested on 4 digit + semi colon model, 
	@test
		-# Test 1 setSegments and encode digit functions
		-# Test 2 decimal number testing
		-# Test 3 string display
		-# Test 4 Brightness & on/off
	

*/


#include <TM1637plus_Model4_RDL.hpp>


// GPIO I/O pins on the RPI connected to clock, data, pick on any I/O pin you want.
#define  CLOCK_TM 21  // clock = GPIO connected to clock line of module
#define  DIO_TM 20 // data = GPIO connected to data line of module
#define  GPIO_CHIP_DEVICE 0 // try 0 or 4 , ls /dev/gpio*
#define  COMM_DELAY_US 75 // Internal Serial Communications delay in uS
#define  NUM_OF_DIGITS 4 // number of digits in display.

// Constructor object
TM1637plus_Model4 tm(CLOCK_TM , DIO_TM, GPIO_CHIP_DEVICE, COMM_DELAY_US, NUM_OF_DIGITS);

// For test timing setup
#define  myTestDelay 5
#define  myTestDelay1 1


// ************* Function prototypes ***************
bool Setup(void);
void Test1(void);
void Test2(void);
void Test3(void);
void Test4(void);
void EndTests(void);

// *************** Main *****************
int main(void)
{
	if (!Setup())
	{
		return -1;
	}
	Test1();
	Test2();
	Test3();
	Test4();
	EndTests();
	return 0;
}
// *************** End of Main *****************



// ************** Function Space ***********
bool Setup(void)
{
	printf("Test Begin :: Model 4 :: TM1637plus\r\n");
	delaySecRDL(myTestDelay1);
	if (tm.displayBegin() != rpiDisplay_Success)
	{
		return false;
	}
	tm.setBrightness(0x07, true);
	return true;
}

void EndTests(void)
{
	tm.displayClear();
	tm.setBrightness(0x00, false);
	tm.displayClose();
	printf("Test End\r\n");
}

void Test1(void)
{
	printf("Test 1A: SetSegments 1 2 3 4 \r\n");
	uint8_t data[] = {0x06, 0x5B, 0x4F, 0x66 };
	tm.setSegments(data,4,0);
	delaySecRDL(myTestDelay);

	printf("Test 1B: encode digits  5 6 7 8 \r\n");
	// Selectively set different digits
	data[0] = tm.encodeCharacter('5');
	data[1] = tm.encodeCharacter('6');
	data[2] = tm.encodeCharacter('7');
	data[3] = tm.encodeCharacter('8');
	tm.setSegments(data,4,0);
	delaySecRDL(myTestDelay);

	printf("Test 1C: SetSegments H E L P \r\n");
	data[0] = 0x76;
	data[1] = 0x79;
	data[2] = 0x38;
	data[3] = 0x73;
	tm.setSegments(data,4,0);
	delaySecRDL(myTestDelay);

}

void Test2(void)
{

	printf("Test 2A: decimal number leading zeros off\r\n");
	tm.DisplayDecimal(1, false, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimal(23, false, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimal(784, false, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimal(6281, false, 4, 0);
	delaySecRDL(myTestDelay);

	printf("Test 2B: decimal number leading zeros on\r\n");
	tm.DisplayDecimal(1, true, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimal(23, true, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimal(784, true, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimal(6281, true, 4, 0);
	delaySecRDL(myTestDelay);

	printf("Test 2C: decimal number leading zeros off,  with semi:colon on \r\n");
	tm.DisplayDecimalwDot(1, 0x80 >> 1, false, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimalwDot(23, 0x80 >> 1, false, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimalwDot(784, 0x80 >> 1, false, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimalwDot(6281, 0x80 >> 1, false, 4, 0);
	delaySecRDL(myTestDelay);

	printf("Test 2D: decimal number leading zeros on ,  with semi:colon on\r\n");
	tm.DisplayDecimalwDot(1, 0x80 >> 1, true, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimalwDot(23, 0x80 >> 1, true, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimalwDot(784, 0x80 >> 1, true, 4, 0);
	delaySecRDL(myTestDelay);
	tm.DisplayDecimalwDot(6281, 0x80 >> 1, true, 4, 0);
	delaySecRDL(myTestDelay);
	tm.displayClear();
}

void Test3(void)
{
	printf("Test 3A: string 3 digit\r\n");
	char number[] = "-23";
	tm.DisplayString(number, 0,  3, 1); // Display " -23" starting at position 1
	delaySecRDL(myTestDelay);

	printf("Test 3B: string 2 digit + semi colon\r\n");
	char min[] = "07";
	char sec[] = "53";
	tm.DisplayString(min, 0x80 >> 1, 2, 0); // Display "07" starting at position 0
	tm.DisplayString(sec, 0x80 >> 1, 2, 2); // Display "53" starting at position 2
	delaySecRDL(myTestDelay);

	printf("Test 3C: string 4 digit\r\n");
	char string[] = "OPEN";
	tm.DisplayString(string, 0, 4, 0); // Display "OPEN" starting at position 0
	delaySecRDL(myTestDelay);

}

void Test4(void)
{
	printf("Test 4A: brightness\r\n");
	uint8_t i = 0;
	uint8_t data[] = {0xFF, 0xFF, 0xFF, 0xFF };  // 8888
	for(i = 0; i < 7; i++) 
	{
		tm.setBrightness(i, true);
		tm.setSegments(data,4,0);
		delaySecRDL(myTestDelay1);
	}
	
	printf("Test 4B: on/off\r\n");
	//off
	tm.setBrightness(7, false);
	tm.setSegments(data,4,0);
	delaySecRDL(myTestDelay);
	//on
	tm.setBrightness(7, true);
	tm.setSegments(data,4,0);
	delaySecRDL(myTestDelay1);
	//off
	tm.setBrightness(7, false);
	tm.setSegments(data,4,0);
	delaySecRDL(myTestDelay1);
}
