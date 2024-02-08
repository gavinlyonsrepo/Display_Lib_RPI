/*!
	@file /examples/seven_segment_displays/tm1638/HELLOWORLD_Model2/main.cpp
	@author Gavin Lyons
	@brief A demo file library for TM1638 module Works on Model 2 
	Carries out series most basic test , "hello world" helo wrld
	Project Name: Display_Lib_RPI
	URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
*/

#include <TM1638plus_Model2_RDL.hpp>
#include <bcm2835.h>
#include <cstdio>

// GPIO I/O pins on the Arduino connected to strobe, clock, data, pick on any I/O pin you want.
#define  STROBE_TM 23 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 24  // clock = GPIO connected to clock line of module
#define  DIO_TM 25 // data = GPIO connected to data line of module
bool swap_nibbles = false; //Default is false if left out, see issues section in readme at URL

// Constructor object Init the module
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles);


int main() 
{
	printf("Test Begin :: Model 2 :: TM1638plus\r\n");
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
	tm.DisplayStr("helowrld", 0);  // Display "helowrld" in 7 segments with 0 decimal points set.
	delay(5000);
	
	tm.reset();
	bcm2835_close(); // Close the library
	printf("Test End\r\n");
	return 0;
}


