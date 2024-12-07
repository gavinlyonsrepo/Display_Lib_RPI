/*! 
	@file examples/ili9341/Touch_Screen/main.cpp
	@brief Simple example to test XPT2046 touchscreen IC and ili9341 TFT LCD together
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 1001 Touch screen example XPT2046 & ILI9341
*/

// Section ::  libraries 
#include <iostream> // cout
#include <csignal> //catch user Ctrl+C
#include "ILI9341_TFT_LCD_RDL.hpp" // TFT LCD
#include "XPT2046_TS_TFT_LCD_RDL.hpp" // TouchScreen

// Section :: Globals 

// LCD ILI9341_
ILI9341_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL_LCD = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

// Touchscreen device XPT 2046
XPT_2046_RDL myXPT;
// init XPT2046 touch screen 
int HWSPI_CHANNEL_XPT = 1;   // A SPI channel, >= 0. Which Chip enable pin to use
uint8_t IRQPIN        = 22;  // Interrupt GPIO ,T_IRQ
int8_t RESPIN         = -1;  // reset pin, if not used set to -1

// Section ::  Function Headers 

uint8_t Setup(void); // setup + user options 
void DrawButttons(void);
void TouchTest(void);
void EndTests(void);
void signal_callback_handler(int signum); // Exit on Ctrl +C

//  Section ::  MAIN loop

int main() 
{
	signal(SIGINT, signal_callback_handler);
	if(Setup() != 0) return -1; //Hardware TFT SPI
	DrawButttons();
	TouchTest();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

// setup
uint8_t Setup(void)
{
	std::cout << "TFT Start Test 1001 Touch Screen Demo" << std::endl;
	std::cout << "ili9341 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.InitSPI(HWSPI_DEVICE, HWSPI_CHANNEL_LCD, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	myTFT.fillScreen(RDLC_BLACK);
	
	if (myXPT.XPTSPIInit(HWSPI_DEVICE, HWSPI_CHANNEL_XPT, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE, IRQPIN, RESPIN) != rpiDisplay_Success)
	{
		return 4;
	}
	return 0;
}

void DrawButttons(void)
{
	// Draw buttons
	myTFT.fillRoundRect(10,20, 60, 60 , 5 , RDLC_RED);
	myTFT.fillRoundRect(90,20, 60, 60 , 5 , RDLC_GREEN);
	myTFT.fillRoundRect(170,20, 60, 60 , 5 , RDLC_BLUE);
	// draw text
	myTFT.setFont(font_retro);
	myTFT.setTextColor(RDLC_CYAN, RDLC_BLACK);
	myTFT.setCursor(12,170);
	myTFT.print("Touch Screen Test");
	myTFT.setCursor(12,210);
	myTFT.print("Touch me!");
}

void TouchTest(void) 
{
	std::cout << "Touch Test" << std::endl;
	std::cout << "Press Ctrl + c to quit" << std::endl;
	int TouchX, TouchY;
	bool TouchPenIRQ = false;
	// Test Touch Pad
	for (;;) 
	{
		delayMilliSecRDL(10); /* Settle time when pen goes up */
		TouchPenIRQ = myXPT.XPTIRQIsPressed();
		if (TouchPenIRQ == true) { /*  PenIRQ is LOW : touch! pen is down */
			myXPT.XPTReadXY(&TouchX, &TouchY);
			printf("Touch : x=%5d y=%5d\n", TouchX, TouchY);
			// draw text box fill rectangle 
			myTFT.fillRect(10,208, 160,60, RDLC_BLACK);
			// draw text
			myTFT.setCursor(12,210);
			if (TouchX < 1750 && TouchX > 1400 && TouchY < 550 && TouchY > 250)
			{
				myTFT.setTextColor(RDLC_RED, RDLC_BLACK);
				myTFT.print("You Touched Red");
			}else if (TouchX < 1150 && TouchX > 800 && TouchY < 550 && TouchY > 250)
			{
				myTFT.setTextColor(RDLC_GREEN, RDLC_BLACK);
				myTFT.print("You Touched Green");
			}else if (TouchX < 600 && TouchX > 200 && TouchY < 550 && TouchY > 250)
			{
				myTFT.setTextColor(RDLC_BLUE, RDLC_BLACK);
				myTFT.print("You Touched Blue");
			}else{
				myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
				myTFT.print("You Touched Black");
			}
			myTFT.setTextColor(RDLC_WHITE, RDLC_BLACK);
			myTFT.setCursor(12,230);
			myTFT.print("X = ");
			myTFT.print(TouchX);
			myTFT.setCursor(12,250);
			myTFT.print("Y = ");
			myTFT.print(TouchY);
			delayMilliSecRDL(10);
		} else { 
				// Pen is up.
		}
	} // end for

}

void EndTests(void)
{
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.PowerDown(); // Power down TFT device
	myXPT.XPTSPIend();
	std::cout << "TFT End" << std::endl;
}


// Terminate program on ctrl + C 
void signal_callback_handler(int signum)
{
	EndTests();
	exit(signum);
}

// *************** EOF ****************
