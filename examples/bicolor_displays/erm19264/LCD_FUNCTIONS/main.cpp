/*!
	@file  examples/bicolor_displays/erm19264/LCD_FUNCTIONS/main.cpp
	@brief Example file for ERM19264_LCD display, showing use of various functions
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@details
			measured frame rate = 66 fps, 1:49 min to 10000, v101, rpi3
	@test
		-#  501 All Pixels on
		-#  502 Invert screen
		-#  503 Flip 180 with LCD rotate command screen
		-#  504 Scroll Screen
		-#  505 Enable and disable Screen
		-#  506 Rotate software buffer
		-#  507 Power down (LCD OFF)
*/

#include <bcm2835.h>
#include <cstdio>
#include "ERM19264_UC1609_LCD_RDL.hpp"

// LCD setup
const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;
#define myScreenSize (myLCDwidth * (myLCDheight/8))

const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t LCDcontrast = 0x49; //Constrast 00 to FF , 0x80 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD ) ;

// =============== Function prototype ================
bool setup(void);
void myTest(void);
void EndTest(void);

// ======================= Main ===================
int main()
{
	if(!setup()) {return -1;}
	myTest();
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
		printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
		bcm2835_delay(100);

	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, SPICLK_FREQ , SPI_CE_PIN) != rpiDisplay_Success)
	{
		printf("Error 1202: bcm2835_spi_begin :Cannot start spi, Running as root?\n");
		bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
		return false;
	}
	printf("ERM19264 Library version number :: %u \r\n", GetRDLibVersionNum());
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	bcm2835_delay(1000);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
	printf("LCD End\r\n");
}

void myTest()
{
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize];
	if (myLCD.LCDSetBufferPtr(myLCDwidth, myLCDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success)
	{
		bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
		exit(-1);
	}
	myLCD.LCDclearBuffer();   // Clear buffer

	// Test 501 LCD all pixels on
	myLCD.setCursor(20, 30);
	myLCD.print("All Pixels on");
	myLCD.LCDupdate();
	bcm2835_delay(4000);
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	myLCD.LCDallpixelsOn(1);
	bcm2835_delay(2000);
	myLCD.LCDallpixelsOn(0);
	bcm2835_delay(2000);

	// Test 502 inverse
	myLCD.setCursor(20, 30);
	myLCD.print("inverse test  ");
	myLCD.LCDupdate();
	myLCD.LCDinvert(0); // Normal
	bcm2835_delay(2000);
	myLCD.LCDinvert(1); // Inverted
	bcm2835_delay(4000);
	myLCD.LCDinvert(0);

	// Test 503 Flip 180 with LCD rotate command screen
	myLCD.LCDclearBuffer();
	myLCD.setCursor(20, 30);
	myLCD.print("flip 180  test");
	myLCD.LCDupdate();
	bcm2835_delay(2000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_ONE);
	myLCD.LCDupdate();
	bcm2835_delay(5000);
	myLCD.LCDrotate(UC1609_ROTATION_NORMAL);
	myLCD.LCDupdate();
	bcm2835_delay(2000);

	// Test 504 LCD scroll
	myLCD.LCDclearBuffer();
	//myLCD.LCDupdate();
	myLCD.setCursor(0, 40);
	myLCD.print("scroll test");
	for (uint8_t i = 0 ; i < 62 ; i ++)
	{
		myLCD.LCDscroll(i);
		myLCD.LCDupdate();
		bcm2835_delay(100);
	}
	myLCD.LCDscroll(0);

	//Test 505 LCD enable and disable
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 30);
	myLCD.print("LCD Disable test");
	myLCD.LCDupdate();
	bcm2835_delay(5000);
	myLCD.LCDEnable(0);
	bcm2835_delay(5000);
	myLCD.LCDEnable(1);
	myLCD.LCDclearBuffer();

	// ** Test 506 Rotate test **
	myLCD.LCDclearBuffer();
	myLCD.setRotation(displayBC_Degrees_90);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 90");
	myLCD.setCursor(5,180);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	bcm2835_delay(3000);

	myLCD.setRotation(displayBC_Degrees_180);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 180");
	myLCD.setCursor(5,56);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	bcm2835_delay(3000);

	myLCD.setRotation(displayBC_Degrees_270);
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate   270");
	myLCD.setCursor(5,180);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	bcm2835_delay(3000);

	myLCD.setRotation(displayBC_Degrees_0); //default normal
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5,5 );
	myLCD.print("rotate 0");
	myLCD.setCursor(5,56);
	myLCD.print("bottom");
	myLCD.LCDupdate();
	bcm2835_delay(3000);



	// Test 507 Powerdown
	myLCD.LCDclearBuffer();
	myLCD.setCursor(5, 10);
	myLCD.print("End Tests");
	myLCD.setCursor(5, 35);
	myLCD.print("Power down in 5");
	myLCD.LCDupdate();
	bcm2835_delay(5000);
}

// *********** EOF ************
