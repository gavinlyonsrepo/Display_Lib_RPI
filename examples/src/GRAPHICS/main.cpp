/*!
	@file examples/src/GRAPHICS/main.cpp
	@brief Example file for ER_OLEDM1_CH1115_RPI library, carrying out graphics test , HW SPI
	@note
		URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
	@test
		-# Test 401 Graphic tests
*/

#include <bcm2835.h>
#include <cstdio>
#include "ER_OLEDM1_CH1115.hpp"

// ===== Test data Defines ====
#define DisplayDelay1 5000
#define DisplayDelay2 0
#define PI 3.14

// ==== Globals ====
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want 
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
#define myScreenSize (myOLEDwidth * (myOLEDheight/8))

const uint32_t SPICLK_FREQ = 64; // Spi clock divid bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight , RES, DC) ; // instantiate  an object

// =============== Function prototype ================
bool Setup(void);
void myTest(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!Setup())return -1;
	myTest();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================

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
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	bcm2835_delay(1500);
	return true;
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	bcm2835_close(); //Close lib & /dev/mem, deallocating mem
	printf("OLED End\r\n");
}

// Function to display Graphics test
void  myTest()
{
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize]; // 1024 bytes = 128 * 64/8
	if (!myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)/sizeof(uint8_t)))
	{
		return;
	}
	
	myOLED.OLEDclearBuffer(); // Clear buffer
	
	//Q1 ||  Q2
	//---------
	//Q3 ||  Q4
	//
	bool colour = 1;
	uint8_t count = 25;
	myOLED.OLEDclearBuffer(); // Clear the buffer
	while (count != 0)
	{
		colour = !colour;

		// Draw the X
		myOLED.drawLine(64,  0, 64, 64, FOREGROUND);
		myOLED.drawFastVLine(62, 0, 64, FOREGROUND);
		myOLED.drawFastHLine(0, 32, 128, FOREGROUND);

		// Q1
		myOLED.fillRect(0, 10, 20, 20, colour);
		myOLED.fillCircle(40, 20, 10, FOREGROUND);

		// Q2
		myOLED.fillTriangle(80, 25, 90, 5, 100, 25, !colour);
		myOLED.drawRect(105, 10, 15, 15, FOREGROUND);
		// Q3
		myOLED.fillRoundRect(0, 40, 40, 20, 10, !colour);
		// Q4
		char i;
		for (i = 0; i < 10; i ++)
		{
			myOLED.drawRect(70 + i, 40 + i, 50 - i * 2, 20 - i * 2, FOREGROUND);
			myOLED.OLEDupdate();
			bcm2835_delay(50);
		}
		myOLED.OLEDclearBuffer();
		count--;
		myOLED.setCursor(0,0);
		myOLED.print(count);
	}
}
