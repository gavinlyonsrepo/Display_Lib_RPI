

/*!
	@file examples/ssd1306/SPEED_TEST/main.cpp
	@author Gavin Lyons
	@brief Test file for SSD1306_OLED library,  showing fps frame rate per second
		Project Name: Display_Lib_RPI
	@test
		-# Test 601 FPS test frame rate per second
*/

#include <ctime>
#include <cstdio>
#include <lgpio.h>
#include "SSD1306_OLED_RDL.hpp"

// Screen related
#define MY_OLED_WIDTH  128
#define MY_OLED_HEIGHT 64
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
SSD1306_RDL myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT) ; // instantiate  an object

// I2C related
bool I2C_debug = true;
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_DEVICE 1
#define OLED_I2C_FLAGS 0

// vars for the test
uint16_t count  = 0;
uint16_t countLimit =700;
bool colour = 1;
uint64_t  previousCounter =0;


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
	printf("OLED End\r\n");
}

bool SetupTest()
{
	
	printf("OLED Test Begin\r\n");
	printf("SSD1306 library Version Number :: %u\r\n",GetRDLibVersionNum());
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	// Open an I2C device
	if(myOLED.OLED_I2C_ON(OLED_I2C_DEVICE, OLED_I2C_ADDRESS,OLED_I2C_FLAGS) != rpiDisplay_Success)
	{
		printf("Error 1201:Cannot open I2C device bus\n");
		return false;
	}
	// Check if OLED on Bus
	if(myOLED.OLEDCheckConnection() < 0)
	{
		printf("Error 1202 : Cannot See Device on Bus\n");
		return false;
	}

	myOLED.OLEDbegin(I2C_debug); // initialize the OLED
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
		if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;
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
	myOLED.print(myOLED.OLEDI2CErrorGet());
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
