
// Example file name : main.cpp
// Description:
// Test file for ER_OLEDM1_CH1115 library, showing use of hello world
// URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
// *****************************

#include <bcm2835.h>
#include <stdio.h>
#include "ER_OLEDM1_CH1115.hpp"

//GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want 
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;

const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight, RES, DC); // instantiate an object 

	
// =============== Function prototype ================
void setup(void);
void myTest(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv) 
{
	if(!bcm2835_init())
	{
		printf("OLED :: ERROR failed to init bcm2835 library.\r\n");
		return -1;
	}
	setup();
	myTest();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void setup() 
{
	bcm2835_delay(50);
	printf("OLED Begin\r\n");
	myOLED.OLEDbegin(OLEDcontrast, SPICLK_FREQ , SPI_CE_PIN); // initialize the OLED
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	bcm2835_delay(3000);
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	bcm2835_close(); //Close lib & /dev/mem, deallocating mem 
	printf("OLED End\r\n");
}

void myTest() {
	
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[(myOLEDwidth * (myOLEDheight / 8))+1]; // 1024 bytes = 128 * 64/8
	myOLED.OLEDbuffer = (uint8_t*) &screenBuffer;  // Assign the pointer to the buffer
	myOLED.OLEDclearBuffer(); // Clear buffer
	
	myOLED.setTextColor(FOREGROUND);
	myOLED.setFontNum(OLEDFontType_Default);
	myOLED.setCursor(20, 20);
	myOLED.print("Hello world");
	myOLED.OLEDupdate();  //write to active buffer
	bcm2835_delay(7000);
}

// ============== EOF =========
