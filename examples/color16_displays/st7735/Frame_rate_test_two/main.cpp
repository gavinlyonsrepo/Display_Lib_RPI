/*! 
	@file examples/color16_displays/st7735/Frame_rate_test_two/main.cpp
	@brief Frame rate test. FPS text + graphics
	@note See USER OPTIONS 1-3 in SETUP function
	@details 58 FPS recorded V 1.0.1 at 8MHz
	@test 
		-# Test 603 Frame rate per second (FPS) test text + graphics
*/

// Section ::  libraries 
#include <bcm2835.h> // for SPI GPIO and delays.
#include <ctime> // for test FPS
#include "ST7735_TFT_LCD_RDL.hpp"

// Section :: Defines   
//  Test timing related defines 
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// Section :: Globals 
ST7735_TFT myTFT;

// vars for the test
uint16_t countLimit = 3000;
uint16_t count  = 0;
uint64_t  previousCounter =0;

// =============== Function prototype ================
int8_t Setup(void);
void TestFPS(void);
void EndTests(void);
void display(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main()
{
	if(Setup() != 0)return -1;
	TestFPS();
	EndTests();
	return 0;
}
// ======================= End of main  ===================

//  Section ::  Function Space 

int8_t Setup(void)
{
	std::cout << "TFT Start" << std::endl;
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}else{
		std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	}
	
// ** USER OPTION 1 GPIO/SPI TYPE HW OR SW **
	int8_t RST_TFT  =  25;
	int8_t DC_TFT   =  24;
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;// Screen width in pixels
	uint16_t TFT_HEIGHT = 128; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 PCB_TYPE + SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ =  8000000 ; // HW Spi freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
	// pass enum to param1 ,4 choices,see README
		if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SCLK_FREQ, SPI_CE_PIN) != rpiDisplay_Success)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************
	std::cout << "ST77XX library version : " << GetRDLibVersionNum()<< std::endl;
	delayMilliSecRDL(50);
	return 0;
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_retro);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	delayMilliSecRDL(TEST_DELAY5);
	myTFT.TFTPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

void TestFPS() {
	myTFT.fillScreen(RDLC_BLACK);
	std::cout << "FPS test: text + graphics ends at :: " << countLimit << std::endl;
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

	myTFT.setCursor(5, 20);
	myTFT.print("G Lyons");
	myTFT.setCursor(5, 30);
	myTFT.print(count);

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
	}

	myTFT.setCursor(5, 40);
	myTFT.print(fps);
	myTFT.print(" fps");
	myTFT.setCursor(5, 50);
	myTFT.print(GetRDLibVersionNum());

	myTFT.drawFastVLine(63, 0, 127, RDLC_BLUE);
	myTFT.drawFastHLine(0, 63, 127, RDLC_BLUE);
	myTFT.fillRect(20, 70, 20, 20, RDLC_RED);
	myTFT.fillCircle(100, 30, 10, RDLC_GREEN);
	myTFT.fillTriangle(70,90, 90, 70 , 110, 90, RDLC_YELLOW);

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
