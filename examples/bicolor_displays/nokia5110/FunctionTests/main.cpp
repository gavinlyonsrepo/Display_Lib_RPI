/*!
	@file: examples/bicolor_displays/nokia5110/FunctionTests/main.cpp
	@brief Description library test file, carries out series of Function tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 501 fill + clear screen
		-# Test 502 Sleep
		-# Test 503 Rotation
		-# Test 504 invert screen command
*/


// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
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

void testFillScreen(void);
void testSleepMode(void);
void testRotate(void);
void testInvert(void);


// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;

	testFillScreen();
	testSleepMode();
	testRotate();
	testInvert();

	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device
bool Setup(void)
{
	std::cout << "LCD Start"  << std::endl;
	if(!bcm2835_init())
	{
		std::cout<< "Error 1201 : Setup :Problem with init bcm2835 library" << std::endl;
		return false;
	}else{
		std::cout<< "bcm2835 library version : " << bcm2835_version() << std::endl;
	}
	bcm2835_delay(250);
	if(myLCD.LCDBegin(inverse, contrast, bias) != rpiDisplay_Success)
	{
		std::cout<< "Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?" << std::endl;
		bcm2835_close(); // Close the bcm2835 library
		return false;
	}
	std::cout<< "Nokia 5110 library version : " << GetRDLibVersionNum() << std::endl;
	bcm2835_delay(250);
	return true;
}

void EndTests(void)
{
	myLCD.LCDSPIoff(); //Stop the hardware SPI
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End" << std::endl;
}

void testSleepMode(void) {

	std::cout <<"Test 502 Sleep mode" << std::endl;
	char testStr1[]= "Sleep 5 secs!\r\n";
	char testStr2[]= "Awake!\r\n";

	myLCD.LCDdisplayClear();
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.LCDdisplayUpdate();

	bcm2835_delay(TEST_DELAY2);
	myLCD.LCDenableSleep();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisableSleep();

	myLCD.print(testStr2);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY2);
	screenReset();
}


void testRotate(void) {

	std::cout <<"Test 503 rotate mode" << std::endl;
	char testStr0[]= "Rotate  0\r\n";
	char testStr1[]= "Rotate   90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate   270\r\n";
	char testStr4[]= "Bottom !\n";

	// rotation example
	myLCD.LCDdisplayClear();
	myLCD.setCursor(0, 0);

	myLCD.setRotation(displayBC_Degrees_90); // rotate 90 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.setCursor(0,76);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(displayBC_Degrees_180); // rotate 180 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr2);
	myLCD.setCursor(0,39);
	myLCD.print(testStr4);
	screenReset();

	myLCD.setRotation(displayBC_Degrees_270); // rotate 270 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr3);
	myLCD.setCursor(0,76);
	myLCD.print(testStr4);
	screenReset();

	// revert back to no rotation
	myLCD.setRotation(displayBC_Degrees_0);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr0);
	myLCD.setCursor(0,39);
	myLCD.print(testStr4);
	screenReset();
}


void testFillScreen(void){
	std::cout <<"Test 501 fill screen"  << std::endl;
	myLCD.LCDfillScreen();
	screenReset();
	myLCD.LCDfillScreenPattern(0x71);
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}

void testInvert(void)
{
	std::cout <<"Test 504 invert screen command" << std::endl;

	screenReset();
	myLCD.setCursor(5, 5);
	myLCD.print("INVERT 123");
	screenReset();

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDinvertDisplay(false);
	bcm2835_delay(TEST_DELAY5);
}


// *************** EOF ****************
