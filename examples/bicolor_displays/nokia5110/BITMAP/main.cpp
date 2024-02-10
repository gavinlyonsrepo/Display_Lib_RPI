/*!
	@file: examples/bicolor_displays/nokia5110/BITMAP/main.cpp
	@brief Description library test file, carries out series of Bitmap tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@test
		-# Test 301 Bitmap full screen
		-# Test 302 Small bitmaps 
		-# Test 303 horizontal addressed data bitmaps

*/

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
#include "Bitmap_test_data.hpp" // Bitmap test data file
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

void testBitMap(void);
void testCustomChar(void);
void testSmallBitmap(void);
void testSmallBitmapHa(void);

// ************  MAIN ***************

int main()
{
	if(!Setup()) return -1;
	
	myLCD.LCDdisplayClear();
	myLCD.setDrawBitmapAddr(true);
	testBitMap();
	testSmallBitmap();
	testSmallBitmapHa();

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

void testBitMap(void)
{
	std::cout <<"Test 301 Bitmap full screen" << std::endl;
	myLCD.drawBitmap(0,0, backUpIcon84x48, myLCD_WIDTH, myLCD_HEIGHT, RDL_BLACK,RDL_WHITE, sizeof(backUpIcon84x48)); // Splash screen
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}


void testSmallBitmap(void)
{
	std::cout <<"Test 302 Small bitmaps " << std::endl;

	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, SignalIconVa, 16, 8, RDL_BLACK,RDL_WHITE, sizeof(SignalIconVa));
	myLCD.drawBitmap(60, 5, TemperatureImageVA, 16, 16, RDL_BLACK, RDL_WHITE, sizeof(TemperatureImageVA));
	screenReset();
}

void testSmallBitmapHa(void)
{
	std::cout <<"Test 303 Horizontally addressed data bitmaps " << std::endl;
	myLCD.setDrawBitmapAddr(false);
	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, BatIconHa, 16, 8, RDL_BLACK,RDL_WHITE,sizeof(BatIconHa));
	myLCD.drawBitmap(25, 5, BatIconHa, 16, 8, RDL_WHITE,RDL_BLACK,sizeof(BatIconHa));
	screenReset();
}

// *************** EOF ****************
