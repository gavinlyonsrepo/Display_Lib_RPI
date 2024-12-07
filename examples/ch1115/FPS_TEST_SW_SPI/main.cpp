/*!
	@file examples/ch1115/FPS_TEST_SW_SPI/main.cpp
	@brief Example file for ER_OLEDM1_CH1115 display, measuring Frame rate per second)FPS, SW SPI
	@test
		-# Test 602 FPS SW SPI
*/
 

#include <ctime>
#include <cstdio>
#include "ERM1_CH1115_OLED_RDL.hpp"


// ==== Globals ====
uint16_t countLimit = 1500;
const uint8_t MY_OLED_WIDTH  = 128;
const uint8_t MY_OLED_HEIGHT = 64;
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.
#define myScreenSize (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))

// GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
const uint8_t DIN = 5; // GPIO pin number pick any you want
const uint8_t SCLK = 6; // GPIO pin number pick any you want
const uint8_t CS = 21; // GPIO pin number pick any you want
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

// software SPI
ERMCH1115 myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT , RES, DC, CS, SCLK,DIN) ; 

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
bool Setup(void);
void myTest(void);
void EndTest(void);
void display(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main()
{
	if(!Setup()) return -1;
	myTest();
	EndTest();
	return 0;
}
// ======================= End of main  ===================


bool Setup(void)
{
	printf("OLED Begin\r\n");
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", GetRDLibVersionNum()); 
	delayMilliSecRDL(50);

	if(myOLED.OLEDbegin(OLEDcontrast, GPIO_CHIP_DEVICE ) != rpiDisplay_Success) // initialize the OLED
	{
		printf("Error 1202: Setup GPIO failed, \r\n");
		return false;
	}
	delayMilliSecRDL(50);
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	delayMilliSecRDL(1500);
	printf("Software SPI Freq delay is set to %u uS \n", myOLED.OLEDHighFreqDelayGet());
	return true;
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	printf("OLED End\r\n");
}

void myTest() {

	printf("FPS SW SPI:: test ends at %u\r\n", countLimit);
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize ]; // 1024 bytes = 128 * 64/8
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer, sizeof(screenBuffer)) != rpiDisplay_Success) return;

	myOLED.OLEDclearBuffer(); // Clear buffer

	while (count < countLimit)
	{
		static long framerate = 0;
		display(framerate, count);
		framerate++;
		count++;
		delayMilliSecRDL(1);
	}

}


// Function to display left hand side buffer
void display(long currentFramerate, int count)
{
	myOLED.OLEDclearBuffer();

	myOLED.setCursor(0, 10);
	myOLED.print("1024 bytes");
	myOLED.setCursor(0, 20);
	myOLED.print("G Lyons");
	myOLED.setCursor(0, 30);
	myOLED.print(count);

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

	myOLED.setCursor(0, 40);
	myOLED.print(fps);
	myOLED.print(" fps");
	myOLED.setCursor(0, 50);
	myOLED.print(GetRDLibVersionNum());

	myOLED.drawFastVLine(64, 0, 63, RDL_BLACK);
	myOLED.fillRect(70, 10, 20, 20, colour);
	myOLED.fillCircle(110, 20, 10, !colour);
	myOLED.drawRoundRect(80, 40, 40, 20, 10, RDL_BLACK);
	myOLED.drawPixel(65, 60, RDL_BLACK);

	myOLED.OLEDupdate();
}

//This returns nano-seconds as a 64-bit unsigned number, monotonically increasing,
//probably since system boot.
//The actual resolution looks like microseconds. returns nanoseconds
static uint64_t counter( void )
{
  struct timespec now;
  clock_gettime( CLOCK_MONOTONIC, &now );
  return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec;
}
