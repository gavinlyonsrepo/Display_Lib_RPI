/*!
	@file examples/ssd1306/clock_demo/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED display, Test file showing a "clock demo" , 128X64 screen
		Project Name: Display_Lib_RPI
	@test
		-# Test 401 Clock Demo
*/

#include <iostream>
#include <ctime>

#include <csignal> //catch user Ctrl+C
#include "SSD1306_OLED_RDL.hpp"
#include "Bitmap_test_data.hpp" // Test data for bitmaps

/// @cond

// Screen Size 
#define MY_OLED_WIDTH  128
#define MY_OLED_HEIGHT 64
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))

// I2C related
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_DEVICE 1
#define OLED_I2C_FLAGS 0

// Define Screen buffer
uint8_t screenBuffer[FULLSCREEN];

 // instantiate  an object
SSD1306_RDL myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT) ;

// =============== Function prototype ================
bool SetupTest(void);
void Test(void);
void EndTests(void);
void DisplayClock(void);
void SplashScreen(void);
std::string UTC_string(void);
void signal_callback_handler(int signum);

// ======================= Main ===================
int main()
{
	signal(SIGINT, signal_callback_handler);
	if (SetupTest() != true) return -1;
	Test();
	EndTests();
	return 0;
}
// ======================= End of Main ===================


// ======================= Function space ===================
bool SetupTest()
{
	std::cout<<"OLED Test Begin" << std::endl;
	std::cout<<"SSD1306 library Version Number :: " <<  rdlib::LibraryVersion() << std::endl;
	std::cout<<"lgpio   library Version Number :: "  << lguVersion()   << std::endl;

	// Open  on I2C device
	if(myOLED.OLED_I2C_ON(OLED_I2C_DEVICE, OLED_I2C_ADDRESS,OLED_I2C_FLAGS) != rdlib::Success)
	{
		std::cout<<"Error 1201:Cannot open I2C device bus" << std::endl;
		return false;
	}
	// Check if OLED on Bus
	if(myOLED.OLEDCheckConnection() < 0)
	{
		std::cout<<"Error 1202 : Cannot See Device on Bus"  << std::endl;
		return false;
	}

	delayMilliSecRDL(500);
	myOLED.OLEDbegin(); // initialize the OLED
	return true;
}

void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	std::cout<<"OLED Test End" << std::endl;
}

void Test()
{
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer ) != rdlib::Success) return;
	myOLED.setDrawBitmapAddr(false); // horizontal bitmaps.
	SplashScreen();
	DisplayClock();
}

void DisplayClock(void)
{
	
	uint16_t count =0;
	std::cout<<"OLED Clock Demo :: Press Ctrl + c to quit." << std::endl;
	while(count < 9999)
	{
		std::string TimeString = UTC_string();
		auto DateInfo = TimeString.substr(2, 8);
		auto TimeInfo = TimeString.substr(11,8);

		myOLED.drawBitmap(0,  0, SignalIconHa, 16, 8, myOLED.BLACK , myOLED.WHITE);
		myOLED.drawBitmap(20, 0, MsgIconHa, 16, 8, myOLED.BLACK, myOLED.WHITE);
		myOLED.drawBitmap(37, 0, AlarmIconHa, 8, 8, myOLED.BLACK, myOLED.WHITE);
		myOLED.drawBitmap(110, 0, BatIconHa, 16, 8, myOLED.BLACK, myOLED.WHITE);

		myOLED.drawLine(0,10,128,10,myOLED.BLACK);
		myOLED.drawLine(0,35,128,35,myOLED.BLACK);
		myOLED.drawLine(0,63,128,63,myOLED.BLACK);

		myOLED.drawLine(0,35,0,63,myOLED.BLACK);
		myOLED.drawLine(127,35,127,63,myOLED.BLACK);

		myOLED.drawLine(75,35,75,63,myOLED.BLACK);

		myOLED.setFont(font_mega);
		myOLED.setCursor(0,17);
		myOLED.print(TimeInfo);

		myOLED.setFont(font_default);
		myOLED.setCursor(5,44);
		myOLED.print(DateInfo);

		myOLED.setCursor(90,54);
		myOLED.print(count);

		myOLED.drawBitmap(80, 40, MsgIconHa, 16, 8, myOLED.BLACK, myOLED.WHITE);
		myOLED.drawBitmap(100, 40, MsgIconHa, 16, 8, myOLED.BLACK, myOLED.WHITE);
		delayMilliSecRDL(1000);
		myOLED.OLEDupdate();
		myOLED.OLEDclearBuffer();
		count++;
	}

	delayMilliSecRDL(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}

void SplashScreen(void)
{
	myOLED.drawBitmap(0, 0, backupicon128x64, MY_OLED_WIDTH, MY_OLED_HEIGHT, myOLED.BLACK , myOLED.WHITE);
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

//Return UTC time as a std:.string with format "yyyy-mm-dd hh:mm:ss".
std::string UTC_string() 
{
	std::time_t time = std::time({});
	char timeString[std::size("yyyy-mm-dd hh:mm:ss UTC")];
	std::strftime(std::data(timeString), std::size(timeString), "%F %T UTC", std::gmtime(&time));
	return timeString;
}

// Terminate program on ctrl + C 
void signal_callback_handler(int signum)
{
	EndTests();
	exit(signum);
}

/// @endcond
