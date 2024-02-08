/*!
	@file examples/seven_segment_displays/tm1638/HELLOWORLD_Model3/main.cpp
	@author Gavin Lyons
	@brief A demo file library for TM1638 module Works on Model 3
	Carries out series most basic test , "hello world" helo wrld
	Project Name: Display_Lib_RPI
	URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
*/

#include <TM1638plus_Model3_RDL.hpp> //include the library
#include <bcm2835.h>
#include <cstdio>

// GPIO I/O pins on the raspberry pi connected to strobe, clock, data,
//pick on any I/O you want.
#define  STROBE_TM 23 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 24  // clock = GPIO connected to clock line of module
#define  DIO_TM 25 // data = GPIO connected to data line of module
 
// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, )
TM1638plus_Model3 tm(STROBE_TM, CLOCK_TM , DIO_TM);


int main() 
{
	printf("Test Begin\r\n");
	printf("Display_Lib_RPI library Version Number :%u\r\n", GetRDLibVersionNum());
	if(!bcm2835_init())
	{
		printf("bcm2835 library failed to init, Run as root\r\n");
		return -1;
	}else
	{
		printf("bcm2835 library Version Number :%u\r\n", bcm2835_version());
	}
	
	tm.displayBegin(); 
	tm.displayText("helowrld");
	bcm2835_delay(5000);
	
	tm.reset();
	bcm2835_close(); // Close the library
	printf("Test End\r\n");
	return 0;
}
