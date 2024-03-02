/*! 
	@file examples/color16_displays/xpt2046/xpt_Test/main.cpp
	@brief Simple example to test XPT2046 touchscreen IC
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function, run as sudo
	@test 
		-# 1001 Touch screen example XPT2046
*/

#include <csignal> //catch user Ctrl+C
#include "bcm2835.h"
#include "XPT2046_TS_TFT_LCD_RDL.hpp"

XPT_2046_RDL myXPT;

#define CSPIN1 1 // Chip select pin 0 or 1 , T_CS
#define RESPIN 11 // MOSI used to init device on init ,T_CLK
#define IRQPIN 22 // Interrupt GPIO ,T_IRQ

void EndTest();
void signal_callback_handler(int signum);

int main ()
{

	// Setup
	printf("Start, Press ctrl + c to quit\n");
	signal(SIGINT, signal_callback_handler);
	if (bcm2835_init() != 1) return 1;
	if(myXPT.XPTSPIInit(CSPIN1, IRQPIN, true, RESPIN) != rpiDisplay_Success)
	{
		bcm2835_close();
		return 1;
	}

	// Test Touch Pad
	int x, y;
	bool TouchPenIRQ = false;
	for (;;) {
		delayMilliSecRDL(10); // Settle time
		TouchPenIRQ = myXPT.XPTIRQIsPressed();
		if (TouchPenIRQ == true){ // touch pen is down
			myXPT.XPTReadXY(&x, &y);
			printf("Touch : x=%5d y=%5d\n", x, y);
			delayMilliSecRDL(10);
		} else { // touch pen is up

		}
	} // end for

	EndTest();
	return 0;
}

void EndTest(void)
{
	myXPT.XPTSPIend();
	bcm2835_close();
	printf("End\n");
}

// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	EndTest();
	exit(signum);
}
