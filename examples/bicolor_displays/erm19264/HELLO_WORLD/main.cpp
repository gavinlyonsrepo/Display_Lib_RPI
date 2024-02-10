/*!
	@file  examples/bicolor_displays/erm19264/HELLO_WORLD/main.cpp
	@brief Example file for ERM19264_LCD display, showing use of hello world, HW SPI
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test
		-# 100 Hello world
*/


#include <bcm2835.h>
#include <cstdio>
#include "ERM19264_UC1609_LCD_RDL.hpp"

const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;
#define myScreenSize (myLCDwidth * (myLCDheight/8))

const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t LCDcontrast = 0x49; // Contrast 00 to FF , 0x49 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight, RST, CD ); //instantiate object

// =============== Function prototype ================
bool setup(void);
bool myTest(void);
void EndTest(void);

// ======================= Main ===================
int main(void)
{
	if(!setup()) {return -1;}
	if(!myTest()) {return -1;}
	EndTest();

	return 0;
}
// ======================= End of main  ===================


// ===================== Function Space =====================
bool setup() {
	printf("LCD Test Begin\r\n");
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}
	bcm2835_delay(100);
	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, SPICLK_FREQ , SPI_CE_PIN) != rpiDisplay_Success)
	{
		printf("Error 1202: bcm2835_spi_begin :Cannot start spi, Running as root?\n");
		bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
		return false;
	}
	
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("ERM19264 Library version number :: %u \r\n", GetRDLibVersionNum());
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	bcm2835_delay(1000);
	return true;
}

bool myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize];
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success)
	{
		bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
		return false;
	}

	myLCD.LCDclearBuffer();   // Clear buffer
	myLCD.setCursor(10,10);
	myLCD.print("Hello world");
	myLCD.LCDupdate();  //write to active buffer
	delay(6000);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
	printf("LCD End\r\n");
}

// ============== EOF =========
