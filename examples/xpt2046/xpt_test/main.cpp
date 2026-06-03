/*! 
	@file examples/xpt2046/xpt_test/main.cpp
	@brief Simple example to test XPT2046 touchscreen IC
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# 1001 Touch screen example XPT2046
*/

#include <atomic>  // Ctrl + C exit
#include <csignal> // Ctrl + C exit
#include <thread>  // Ctrl + C exit

#include "XPT2046_TS_TFT_LCD_RDL.hpp"

/// @cond

std::atomic<bool> stopRequested{false}; // Stop signal , Ctrl + c etc

// Touchscreen device XPT 2046
XPT_2046_RDL myXPT;
// init XPT2046 touch screen 
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
int HWSPI_CHANNEL_XPT = 1; // A SPI channel, >= 0. Which Chip enable pin to use
uint8_t IRQPIN   = 22;  // Interrupt GPIO ,T_IRQ
int8_t RESPIN = 11; // // MOSI used to init device on init ,T_CLK reset pin if not used set to -1
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

void EndTest();
void handleSignal(int){
	stopRequested = true; // for CtrL +C
}

int main ()
{
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	// Setup
	std::signal(SIGINT, handleSignal); // for user press Ctrl+C
	std::signal(SIGTERM, handleSignal);// for kill command
	printf("Start, Press ctrl + c to quit\n");

	if (myXPT.XPTSPIInit(HWSPI_DEVICE, HWSPI_CHANNEL_XPT, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE, IRQPIN, RESPIN) != rdlib::Success)
	{
		printf("ERROR :: Could not Start XPT 2046 sensor\n");
		return -1;
	}

	// Test Touch Pad
	int x, y;

	for (;;) 
	{
		delayMilliSecRDL(10); // Settle time
		bool TouchPenIRQ = myXPT.XPTIRQIsPressed();
		if (TouchPenIRQ == true){ // touch pen is down
			myXPT.XPTReadXY(&x, &y);
			printf("Touch : x=%5d y=%5d\n", x, y);
			delayMilliSecRDL(10);
		} else { }// touch pen is up
		if (stopRequested) break;
	} // end for

	if (stopRequested)
		std::cout << "Exit Signal received" << std::endl;
	EndTest();
	return 0;
}

void EndTest(void)
{
	myXPT.XPTSPIend();
	printf("End\n");
}


/// @endcond
