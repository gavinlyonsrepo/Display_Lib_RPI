/*! 
	@file examples/st7735/Multiple_displays/main.cpp
	@brief Multiple displays on hardware  SPI one ST7735 lcd and One cH1115 oled test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 1050 multiple displays Hardware SPI
*/

// Section ::  libraries 
#include <iostream>
#include <lgpio.h>
#include "ST7735_TFT_LCD_RDL.hpp"
#include "ERM1_CH1115_OLED_RDL.hpp"

// Section :: Globals 

//OLED Setup
const uint8_t RES = 21; // GPIO pin number pick any you want
const uint8_t DC = 20; // "

const uint8_t MY_OLED_WIDTH  = 128;
const uint8_t MY_OLED_HEIGHT = 64;
#define myScreenSize (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8)) // 1024 bytes = 128 * 64/8
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

int SPI_CHANNEL_ONE = 1; // SPI channel Chip enable pin, ch1115 to SPCE1

// LCD Setup
int8_t RST_TFT  = 25; // GPIO pin number pick any you want
int8_t DC_TFT   = 24; // "

int SPI_CHANNEL_ZERO = 0; // SPI channel Chip enable pin, st7735 to SPCE0

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 128;// Screen width in pixels
uint16_t TFT_HEIGHT = 128; // Screen height in pixels

// Common  to both displays
int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
int  GPIO_CHIP_DEV = 0; // Gpio chip device number
int SPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

 // instantiate the objects
ST7735_TFT myTFT;
ERMCH1115 myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT, RES, DC);

//  Section ::  Function Headers 

uint8_t SetupLCDSPI(void); // setup + user options for St7735 LCD
uint8_t SetupOLEDSPI(void); // setup + user options for Ch1115 OLED
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	std::cout << "Start Test" << std::endl;
	std::cout << "Display_Lib_RPI library version :" << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;
	if(SetupLCDSPI() != 0)  return -1; //Hardware SPI St7735 LCD 
	if(SetupOLEDSPI() != 0)  return -1; //Hardware SPI Ch1115 LCD 
	HelloWorld();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

// Hardware SPI setup LCD
uint8_t SetupLCDSPI(void)
{
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 PCB_TYPE + SPI settings**
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SPI_DEV, SPI_CHANNEL_ZERO, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rpiDisplay_Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	return 0;
}

uint8_t  SetupOLEDSPI(void)
{
	delayMilliSecRDL(50);
	if(myOLED.OLEDbegin(OLEDcontrast, SPI_DEV, SPI_CHANNEL_ONE, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rpiDisplay_Success) // initialize the OLED
	{
		std::cout << "Error 1202: Setup : Cannot start spi" << std::endl;
		return -1;
	}
	delayMilliSecRDL(50);
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	delayMilliSecRDL(1500);
	return 0;
}

void HelloWorld(void) 
{
	std::cout << "Hello World" << std::endl;
	// LCD
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setCursor(10,10);
	myTFT.setFont(font_default);
	myTFT.print("Hello world");
	// OLED
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize]; 
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
	myOLED.OLEDclearBuffer(); // Clear buffer
	myOLED.setFont(font_default);
	myOLED.setCursor(0, 0);
	myOLED.print("HELLO");
	myOLED.OLEDupdate();  //write to active buffer
	delayMilliSecRDL(10000);
}

void EndTests(void)
{
	//  LCD
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	// OLED
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	std::cout << "Test End" << std::endl;
}

// *************** EOF ****************
