

/*!
	@file examples/bicolor_displays/ssd1306/SPEED_TEST/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library,  showing fps frame rate per second
		Project Name: Display_Lib_RPI
	
	@test
		-# Test 601 FPS test frame rate per second
		
	@details
		-# results data for test file
		-# I2C_Speed = 0 Fps 7 bcm2835_i2c_set_baudrate(100000) 
		-# I2C_Speed = 2500 Fps 6 bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500) 
		-# I2C_Speed = 626 Fps 16 bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626) 
		-# I2C_Speed = 150 Fps 24 bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_150) 
		-# I2C_Speed = 148 FPS 25 bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_148) 
*/

#include <ctime>
#include <cstdio>
#include <bcm2835.h>
#include "SSD1306_OLED_RDL.hpp"

#define myOLEDwidth  128
#define myOLEDheight 64
#define FULLSCREEN (myOLEDwidth * (myOLEDheight/8))
SSD1306 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object

// vars for the test
uint16_t count  = 0;
uint16_t countLimit = 1000;
bool colour = 1;
uint64_t  previousCounter =0;
uint8_t  screenBuffer[FULLSCREEN]; 


// =============== Function prototype ================
bool SetupTest(void); 
void myLoop(void);
void display_buffer(long , int );
void EndTests(void);
static uint64_t counter( void );

	
// ======================= Main ===================
int main(void)
{

	if (SetupTest() != true) return -1;
	myLoop();
	EndTests();
	return 0;
}
// ======================= End of main  ===================

void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	bcm2835_close(); // Close the library
	printf("OLED End\r\n");
}

bool SetupTest()
{
	const uint16_t I2C_Speed = BCM2835_I2C_CLOCK_DIVIDER_626;; // bcm2835I2CClockDivider enum ,see readme.
	const uint8_t I2C_Address = 0x3C;
	bool I2C_debug = false;
	printf("OLED Test Begin\r\n");
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}
	delayMilliSecRDL(250);
	// Turn on I2C bus (optional it may already be on)
	if(myOLED.OLED_I2C_ON() != rpiDisplay_Success)
	{
		printf("Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?\n");
		bcm2835_close(); // Close the library
		return false;
	}
	
	printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	myOLED.OLEDbegin(I2C_Speed, I2C_Address, I2C_debug); // initialize the OLED
	printf("Debug status is : %u\r\n", myOLED.OLEDDebugGet());
	printf("I2C Debug Error : %u\r\n", myOLED.OLEDI2CErrorGet()); // Print I2C error flag
	printf("I2C Error Timeout mS : %u \r\n", myOLED.OLEDI2CErrorTimeoutGet()); // Print I2C error Timeout
	printf("I2C Error retry attempts counts : %u \r\n", myOLED.OLEDI2CErrorRetryNumGet()); // Print I2C error retry count
	myOLED.OLEDFillScreen(0xF1, 0); // splash screen bars, optional just for effect
	delayMilliSecRDL(1000);
	
	return true;
}

void myLoop() 
{
		printf("OLED Frame rate per second(FPS) test , ends at %u \r\n", countLimit);
		myOLED.setFont(font_default);
		if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
		while (count < countLimit)
		{
			static long framerate = 0;
			display_buffer(framerate, count);
			framerate++;
			count++;
		}
}

void display_buffer(long currentFramerate, int count)
{
	// Values to count frame rate per second
	static long lastFramerate = 0;
	static uint16_t fps;
	uint64_t currentCounter = counter();
	if(currentCounter - previousCounter >= 1000000000)
	{
		//(Rolls over every 1 second)
		fps = currentFramerate - lastFramerate;
		lastFramerate = currentFramerate ;
		previousCounter = currentCounter;
		colour = !colour;
	}
	
	myOLED.OLEDclearBuffer();
	
	// ** TEST CODE **
	myOLED.setCursor(0, 0);
	myOLED.print("SSD1306");
	myOLED.setCursor(0, 10);
	myOLED.print("G Lyons");
	myOLED.setCursor(0, 20);
	myOLED.print(GetRDLibVersionNum());
	
	myOLED.setCursor(0, 30);
	myOLED.print(myOLED.getI2Cspeed());
	myOLED.setCursor(0, 40);
	myOLED.print(count);
	myOLED.setCursor(0, 50);
	myOLED.print(fps);
	myOLED.print(" fps");

	myOLED.drawFastVLine(64, 0, 63, RDL_WHITE);
	myOLED.fillRect(70, 10, 20, 20, colour);
	myOLED.fillCircle(110, 20, 10, !colour);
	myOLED.drawRoundRect(80, 40, 40, 20, 10, RDL_WHITE);
	// ** END of TEST CODE ** 
	
	myOLED.OLEDupdate();
}

// This func returns nano-seconds as a 64-bit unsigned number, 
// monotonically increasing, since system boot.
// The actual resolution looks like microseconds. returns nanoseconds
static uint64_t counter( void )
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec;
}
