/*!
	@file examples/src/OLED_FUNCTIONS/main.cpp
	@brief Example file for ER_OLEDM1_CH1115_RPI library, showing use of various function. HW SPI.
	@note
		-URL : https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
	@test
		-# Test 501 Enable and disable Screen (Sleep mode , 500uA)
		-# Test 502 Invert screen colour
		-# Test 503 Rotate vertically screen 180 (flip )
		-# Test 504 Screen Contrast
		-# Test 505 Scroll Screen
		-# Test 506 Fade effect
		-# Test 507 Fill a page 0-7 + clear screen
*/

#include <bcm2835.h>
#include <cstdio>
#include "ER_OLEDM1_CH1115.hpp"

// ==== Globals ====
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
#define myScreenSize (myOLEDwidth * (myOLEDheight/8))

const uint32_t SPICLK_FREQ = 64; // Spi clock divider  bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC ) ;

// =============== Function prototypes ================
bool Setup(void);
void myLoop(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!Setup()) return -1;
	myLoop();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ============= Function Space ===================
bool Setup(void)
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
	if(!myOLED.OLEDbegin(OLEDcontrast, SPICLK_FREQ , SPI_CE_PIN)) // initialize the OLED
	{
		printf("Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?\r\n");
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return false;
	}
	printf("CH1115 OLED library version : %u\r\n", myOLED.OLEDLibVerNumGet());
	bcm2835_delay(50);
	myOLED.OLEDFillScreen(0x77); //splash screen bars
	bcm2835_delay(1000);
	return true;
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	bcm2835_close(); //Close lib & /dev/mem, deallocating mem
	printf("OLED End\r\n");
}

void myLoop()
{

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize ]; // 1024 bytes = 128 * 64/8
	if (!myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)/sizeof(uint8_t)))
		return;

	myOLED.OLEDclearBuffer(); // Clear buffer

	// Set text parameters
	myOLED.setTextColor(FOREGROUND);
	myOLED.setTextSize(1);

	//  ** Test 501 OLED enable and disable **
	myOLED.setCursor(0, 30);
	myOLED.print("OLED Disable test 1");
	myOLED.OLEDupdate();

	bcm2835_delay(3000); //display message
	myOLED.OLEDEnable(0); //turn off display (sleep mode 500uA measured)
	bcm2835_delay(3000); //wait for 3
	myOLED.OLEDEnable(1); //turn on
	bcm2835_delay(2000); //display message
	myOLED.OLEDclearBuffer();

	// ** Test 502 inverse **
	myOLED.setCursor(0, 10);
	myOLED.print("OLED Inverse test 2");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDInvert(1); // Inverted
	bcm2835_delay(4000);
	myOLED.OLEDInvert(0);
	bcm2835_delay(1000);


	// ** Test 503 OLED flip **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(5, 5);
	myOLED.print("Flip test 3");
	myOLED.OLEDupdate();
	bcm2835_delay(3000);
	myOLED.OLEDFlip(1);
	bcm2835_delay(3000);
	myOLED.OLEDFlip(0);
	bcm2835_delay(2000);

	// ** Test 504 contrast **
	myOLED.OLEDclearBuffer();
	myOLED.setCursor(0, 0);
	myOLED.print("Contrast test 4");
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
	myOLED.print("Scroll test 5 ");
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
	myOLED.print("Fade effect test 6");
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
	int8_t pageNum= 0;
	for (pageNum = 0 ; pageNum < 8 ; pageNum ++)
	{
		myOLED.OLEDFillPage(pageNum, 0x17); // Write pattern (0001 0111) to all 8 page's.
		bcm2835_delay(500);
	}

	myOLED.OLEDFillScreen(0x00); // Clear the screen
	myOLED.print("End of tests");
	myOLED.OLEDupdate();
	myOLED.OLEDclearBuffer();
	bcm2835_delay(2500);
}

// *********** EOF ***********
