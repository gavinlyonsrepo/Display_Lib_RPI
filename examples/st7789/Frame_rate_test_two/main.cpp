/*! 
	@file examples/st7789/Frame_rate_test_two/main.cpp
	@brief Frame rate test. FPS text + graphics
	@note See USER OPTIONS 1-3 in SETUP function
	@test 
		-# Test 603 Frame rate per second (FPS) test text + graphics
*/

// Section ::  libraries 
#include <iostream> // cout
#include <ctime> // for test FPS
#include "ST7789_TFT_LCD_RDL.hpp"

// Section :: Defines   
//  Test timing related defines 
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// Section :: Globals 

ST7789_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 4; // RPI 5 = 4 , other RPIs = 0

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 280; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device


// vars for the test
uint16_t countLimit = 200;
uint16_t count  = 0;
uint64_t  previousCounter =0;

// =============== Function prototype ================
int8_t SetupHWSPI(void);
void TestFPS(void);
void EndTests(void);
void display(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main()
{
	if(SetupHWSPI() != 0)return -1;
	TestFPS();
	EndTests();
	return 0;
}
// ======================= End of main  ===================

//  Section ::  Function Space 
// Hardware SPI setup
int8_t SetupHWSPI(void)
{
	std::cout << "TFT Start" << std::endl;
	std::cout << "ST7789 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.writeCharString(25, 50, teststr1);
	delayMilliSecRDL(2500);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "TFT End" << std::endl;
}


void TestFPS() {
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_90);
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.setFont(font_orla);
	
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


// Function to display left hand side
void display(long currentFramerate, int count)
{

	myTFT.setCursor(25, 30);
	myTFT.print("G Lyons");
	myTFT.setCursor(25, 70);
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

	myTFT.setCursor(25, 110);
	myTFT.print(fps);
	myTFT.print(" fps");
	myTFT.setCursor(25, 150);
	myTFT.print(GetRDLibVersionNum());

	myTFT.drawFastVLine(160, 20, 200, RDLC_BLUE);
	myTFT.fillRect(200, 70, 20, 20, RDLC_RED);
	myTFT.fillCircle(200, 30, 10, RDLC_GREEN);
	myTFT.fillTriangle(180,200, 200, 180 , 220, 200, RDLC_YELLOW);

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
