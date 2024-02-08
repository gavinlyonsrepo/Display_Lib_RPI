/*!
	@file  examples/bicolor_displays/ch1115/HELLO_WORLD/main.cpp
	@brief Example file for ERM1_CH1115_OLED display, showing use of hello world, HW SPI
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test 
		-# 100 Hello world
*/

#include <bcm2835.h>
#include <cstdio> // For printf
#include "ERM1_CH1115_OLED_RDL.hpp"

//GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
// Screen
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // 1024 bytes = 128 * 64/8

const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight, RES, DC); // instantiate an object

// =============== Function prototype ================
bool Setup(void);
void myTest(void);
void EndTest(void);

// ======================= Main ===================
int main()
{
	if(!Setup()) return -1;
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
	if(myOLED.OLEDbegin(OLEDcontrast, SPICLK_FREQ , SPI_CE_PIN) != rpiDisplay_Success) // initialize the OLED
	{
		printf("Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?\r\n");
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return false;
	}
	printf("CH1115 OLED library version : %u\r\n", GetRDLibVersionNum());
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

void myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize]; 
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	
	myOLED.OLEDclearBuffer(); // Clear buffer
	myOLED.setFont(font_default);
	myOLED.setCursor(20, 20);
	myOLED.print("Hello world.");
	myOLED.OLEDupdate();  //write to active buffer
	bcm2835_delay(7000);
}

// ============== EOF =========
