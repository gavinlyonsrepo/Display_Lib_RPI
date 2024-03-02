/*! 
	@file examples/color16_displays/ili9341/Touch_Screen/main.cpp
	@brief Simple example to test XPT2046 touchscreen IC and ili9341 TFT LCD together
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function, run as sudo
	@test 
		-# 1001 Touch screen example XPT2046 & ILI9341
*/

// Section ::  libraries 
#include <bcm2835.h> 
#include <csignal> //catch user Ctrl+C
#include "ILI9341_TFT_LCD_RDL.hpp" // TFT LCD
#include "XPT2046_TS_TFT_LCD_RDL.hpp" // TouchScreen

// Section :: Globals 
ILI9341_TFT myTFT;
XPT_2046_RDL myXPT;

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
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}
// ** TFT USER OPTION 1 GPIO HW SPI **
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** TFT USER OPTION 2 Screen Setup **
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// **TFT USER OPTION 3 SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ =  8000000 ; //  freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
	if(myTFT.InitSPI(SCLK_FREQ, SPI_CE_PIN) != rpiDisplay_Success)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************

	std::cout <<"RDL library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	delayMilliSecRDL(100);
	myTFT.fillScreen(RDLC_BLACK);
	
	// init XPT2046 touch screen 
	uint8_t CSPIN1  = 1;    // Chip select pin, T_CS
	uint8_t IRQPIN   = 22;  // Interrupt GPIO ,T_IRQ
	myXPT.XPTSPIInit(CSPIN1, IRQPIN, false, -1);
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
		if (TouchPenIRQ == true) { /* P1.22 == PenIRQ is LOW : touch! pen is down */
			myXPT.XPTReadXY(&TouchX, &TouchY);
			printf("Touch : x=%5d y=%5d\n", TouchX, TouchY);
			// turn back on LCD SPI settings
			myTFT.TFTSPIHWSettings();
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
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}


// Terminate program on ctrl + C 
void signal_callback_handler(int signum)
{
	EndTests();
	exit(signum);
}

// *************** EOF ****************
