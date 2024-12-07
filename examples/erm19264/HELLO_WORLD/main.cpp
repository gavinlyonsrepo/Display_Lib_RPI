/*!
	@file  examples/erm19264/HELLO_WORLD/main.cpp
	@brief Example file for ERM19264_LCD display, showing use of hello world, HW SPI
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test
		-# 100 Hello world
*/

#include <lgpio.h>
#include <cstdio>
#include "ERM19264_UC1609_LCD_RDL.hpp"

//GPIO
const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

// Screen
const uint8_t MY_LCD_WIDTH  = 192;
const uint8_t MY_LCD_HEIGHT = 64;
#define MY_SCREEN_SIZE (MY_LCD_WIDTH * (MY_LCD_HEIGHT/8)) 
const uint8_t LCDcontrast = 0x49; // Contrast 00 to FF , 0x49 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

// SPi
// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

ERM19264_UC1609 myLCD(MY_LCD_WIDTH ,MY_LCD_HEIGHT, RST, CD ); //instantiate object

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
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum()); 
	delayMilliSecRDL(100);
	if(myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		printf("Error 1202: Cannot start spi\n");
		return false;
	}
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	delayMilliSecRDL(1000);
	return true;
}

bool myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[MY_SCREEN_SIZE];
	if (myLCD.LCDSetBufferPtr(MY_LCD_WIDTH, MY_LCD_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success)
	{
		return false;
	}

	myLCD.LCDclearBuffer();   // Clear buffer
	myLCD.setCursor(10,10);
	myLCD.setFont(font_groTesk);
	myLCD.print("Hello world");
	myLCD.LCDupdate();  //write to active buffer
	delayMilliSecRDL(5000);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	myLCD.LCDSPIoff();
	printf("LCD End\r\n");
}

// ============== EOF =========
