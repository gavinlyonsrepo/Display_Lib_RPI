/*! 
	@file examples/xpt2046/xpt_Test/main.cpp
	@brief Simple example to test XPT2046 touchscreen IC
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 1001 Touch screen example XPT2046
*/

#include <csignal> //catch user Ctrl+C
#include <lgpio.h>
#include "XPT2046_TS_TFT_LCD_RDL.hpp"

// Touchscreen device XPT 2046
XPT_2046_RDL myXPT;
// init XPT2046 touch screen 
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
int HWSPI_CHANNEL_XPT = 1; // A SPI channel, >= 0. Which Chip enable pin to use
uint8_t IRQPIN   = 22;  // Interrupt GPIO ,T_IRQ
int8_t RESPIN = 11; // // MOSI used to init device on init ,T_CLK reset pin if not used set to -1
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

void EndTest();
void signal_callback_handler(int signum);

int main ()
{
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	// Setup
	printf("Start, Press ctrl + c to quit\n");
	signal(SIGINT, signal_callback_handler);
	if (myXPT.XPTSPIInit(HWSPI_DEVICE, HWSPI_CHANNEL_XPT, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE, IRQPIN, RESPIN) != rpiDisplay_Success)
	{
		printf("ERROR :: Could not Start XPT 2046 sensor\n");
		return -1;
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
	printf("End\n");
}

// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	EndTest();
	exit(signum);
}
