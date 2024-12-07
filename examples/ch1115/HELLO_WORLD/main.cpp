/*!
	@file  examples/ch1115/HELLO_WORLD/main.cpp
	@brief Example file for ERM1_CH1115_OLED display, showing use of hello world, HW SPI
			Project Name: Display_Lib_RPI
	@note
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@test 
		-# 100 Hello world
*/

#include <lgpio.h>
#include <cstdio> // For printf
#include "ERM1_CH1115_OLED_RDL.hpp"

//GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

// Screen
const uint8_t MY_OLED_WIDTH  = 128;
const uint8_t MY_OLED_HEIGHT = 64;
#define myScreenSize (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8)) // 1024 bytes = 128 * 64/8
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

// SPi
// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device


ERMCH1115 myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT, RES, DC); // instantiate an object

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
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum()); 
	delayMilliSecRDL(50);

	if(myOLED.OLEDbegin(OLEDcontrast, HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE ) != rpiDisplay_Success) // initialize the OLED
	{
		printf("Error 1202: Setup : Cannot start spi \r\n");
		return false;
	}
	delayMilliSecRDL(50);
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	delayMilliSecRDL(1500);
	return true;
}


void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	printf("OLED End\r\n");
}

void myTest() {

	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize]; 
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.OLEDclearBuffer(); // Clear buffer
	myOLED.setFont(font_default);
	myOLED.setCursor(20, 20);
	myOLED.print("Hello World.");
	myOLED.OLEDupdate();  //write to active buffer
	delayMilliSecRDL(7000);
}

// ============== EOF =========
