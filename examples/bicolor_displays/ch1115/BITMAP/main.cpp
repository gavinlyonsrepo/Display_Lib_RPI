/*!
	@file examples/bicolor_displays/ch1115/BITMAP/main.cpp
	@brief Example file for ER_OLEDM1_CH1115 display, showing how to display bitmaps. HW SPI.
	@test
		-# Test 301 OLEDBitmap method
		-# Test 302 OLEDBuffer Method Bitmap splashscreen technique
		-# Test 303 drawBitmap() method, Vertical addressing setDrawBitmapAddr(true) 
		-# Test 304 drawBitmap() method, Horizontal  addressing  setDrawBitmapAddr(false) 
*/

#include <bcm2835.h>
#include <cstdio>
#include "Bitmap_test_data.hpp" // Bitmap test data file
#include "ERM1_CH1115_OLED_RDL.hpp"

// ==== Globals ====
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
const uint8_t myOLEDwidth  = 128;
const uint8_t myOLEDheight = 64;
#define myScreenSize (myOLEDwidth * (myOLEDheight/8))

const uint32_t SPICLK_FREQ  = 64; // Spi clock divider : bcm2835SPIClockDivider bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

ERMCH1115 myOLED(myOLEDwidth ,myOLEDheight, RES, DC); // instantiate an object

// '128x64px "g lyons" + shapes ,  The buffer will be assigned this data at init
// vertical addressing . Note this is the screen buffer filled with a Bitmap , can only be used once startup.
// This is not a bitmap but bitmap data pre-loaded into the screen buffer array.
uint8_t  screenBuffer128x64[1024] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0x60, 0x60, 0x20, 0x20, 0x20, 0x20, 0x60,
	0x40, 0xc0, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0xe0, 0xe0, 0xe0, 0x20, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xf8, 0xfe, 0xff, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x60, 0xe1, 0xe7, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x38, 0xf8, 0xf8, 0xc8, 0x00, 0x00,
	0x00, 0x00, 0xc8, 0x78, 0x08, 0x00, 0x00, 0xc0, 0xe0, 0x70, 0x10, 0x08, 0x08, 0x08, 0x08, 0x18,
	0x70, 0xe0, 0xc0, 0x00, 0x00, 0x10, 0xf0, 0xf8, 0xf8, 0x20, 0x10, 0x18, 0x18, 0x38, 0xf8, 0xf0,
	0x00, 0x00, 0x00, 0x00, 0xf0, 0xd8, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x0f, 0x1f, 0x3e, 0x70, 0x60, 0x40, 0xc0, 0x80, 0x80, 0x80, 0x80, 0x80,
	0xc0, 0x60, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0xc0, 0xc0, 0xe0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3e, 0xf8,
	0x70, 0x0e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x3f, 0x7e, 0x60, 0xc0, 0x80, 0x80, 0x80, 0xc0,
	0x60, 0x3f, 0x1f, 0x02, 0x00, 0x80, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff,
	0x80, 0x00, 0x00, 0x00, 0xe1, 0x83, 0x83, 0x87, 0x86, 0x8e, 0x7c, 0x38, 0x00, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x38, 0x30, 0x18, 0x06, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xff, 0xff, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6,
	0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6, 0xf6,
	0xf6, 0xf6, 0xf6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1f, 0x1f, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
	0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,
	0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,
	0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1f, 0x1f, 0x1f, 0x1f, 0x00
};

// =============== Function prototype ================
bool Setup(void);
void myLoop(void);
void Test301(void);
void Test302(void);
void Test303(void);
void Test304(void);
void EndTest(void);

// ======================= Main ===================
int main()
{
	if(!Setup()) return -1;
	myLoop();
	EndTest();
	return 0;
}

// === Bitmap Data for tests ===


// === Function Space ===
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
	myOLED.OLEDFillScreen(0x00); // clear screen
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
	Test301(); // Method (1) OLED bitmap method, writes to screen directly(no buffer)
	Test302(); // Method (2) Assign bitmap to a buffer at init
	Test303(); // Method (3) Drawbitmap to buffer method, vertical data addressing
	Test304(); // Method (4) Drawbitmap to buffer method, horizontal data addressing
}


void Test301(void)
{
	// Method (1) OLED bitmap method, write bitmap directly to screen
	printf(" Test 301 OLED bitmap method, write bitmap directly to screen\n");
	myOLED.OLEDBitmap(5, 5 , 16, 8, SignalIconVa);
	bcm2835_delay(5000);
	myOLED.OLEDFillScreen(0x00);
}


void Test302(void)
{
	// Method (2) assign bitmap to a buffer at init
	printf(" Test 302 OLED buffer method, assign bitmap at initialization \n");
	if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer128x64, sizeof(screenBuffer128x64)) != rpiDisplay_Success)
	{
		EndTest();
		exit(-1);
	}
	myOLED.OLEDupdate();
	bcm2835_delay(5000);
	myOLED.OLEDclearBuffer();
}


void Test303(void)
{
	printf(" Test 303 setDrawBitmapAddr method,  for Bitmap Data Vertical addressed\n");
	myOLED.setDrawBitmapAddr(true); // for Bitmap Data Vertical  addressed
	myOLED.drawBitmap(0, 0, SignalIconVa, 16, 8, RDL_BLACK, RDL_WHITE, sizeof(SignalIconVa));
	myOLED.drawBitmap(30, 20, SignalIconVa, 16, 8, RDL_WHITE, RDL_BLACK, sizeof(SignalIconVa));
	myOLED.drawBitmap(70, 30, TemperatureImageVA, 16, 16, RDL_WHITE, RDL_BLACK, sizeof(TemperatureImageVA));
	myOLED.OLEDupdate();
	bcm2835_delay(5000);
	myOLED.OLEDclearBuffer();

}

void Test304(void)
{
	printf(" Test 304 setDrawBitmapAddr method,  for Bitmap Data Horziontal addressed\n");
	myOLED.setDrawBitmapAddr(false); // for Bitmap Data Horziontal addressed

	myOLED.drawBitmap(10, 25, SignalIconHa, 16, 8, RDL_BLACK, RDL_WHITE, sizeof(SignalIconHa));
	myOLED.drawBitmap(100, 20, SignalIconHa, 16, 8, RDL_BLACK, RDL_WHITE, sizeof(SignalIconHa));
	myOLED.drawBitmap(60, 20, SignalIconHa, 16, 8, RDL_WHITE, RDL_BLACK, sizeof(SignalIconHa));

	myOLED.OLEDupdate();
	bcm2835_delay(5000);
	myOLED.OLEDclearBuffer();

	myOLED.drawBitmap(0, 0, bigImage64x64, 64, 64, RDL_BLACK, RDL_WHITE, sizeof(bigImage64x64));

	myOLED.OLEDupdate();
	bcm2835_delay(5000);
	myOLED.OLEDclearBuffer();
}

