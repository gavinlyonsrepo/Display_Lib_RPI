/*!
	@file examples/color16_displays/ili9341/Mandelbrot_set/main.cpp
	@brief  Speed test using drawPixel to draw a mandelbrot set
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function, run as sudo
	@details at SPI SPEED 25Mhz = Time Taken :: ~670 mS
	@test
		-# 2000 Mandelbro set test
*/

// Section ::  libraries
#include <ctime>
#include <bcm2835.h>
#include "ILI9341_TFT_LCD_RDL.hpp"

// Section :: Globals
ILI9341_TFT myTFT;


//  Section ::  Function Headers

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
void drawMandelbrot(void);
static uint64_t MilliCount(void); // ~millis()
void EndTests(void);

//  Section ::  MAIN loop

int main()
{

	if(SetupHWSPI() != 0) return -1; //Hardware SPI
	myTFT.setRotation(myTFT.TFT_Degrees_90);
	drawMandelbrot();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	std::cout << "TFT Start Test 101 HWSPI" << std::endl;
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}
// ** USER OPTION 1 GPIO HW SPI **
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ =  25000000 ; //  freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
	if(myTFT.InitSPI(SCLK_FREQ, SPI_CE_PIN) != rpiDisplay_Success)
	{
		bcm2835_close(); //Close lib & /dev/mem, deallocating mem
		return 3;
	}
//*****************************
	std::cout <<"ILI9341 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	delayMilliSecRDL(100);
	return 0;
}

void drawMandelbrot(void)
{
	std::cout << "Start MandelBrot Test"<< std::endl;


	uint32_t startTime,elapsedTime; // Time calculations

	const int16_t bits        = 20;   // Fractional resolution
	const int16_t pixelWidth  = 320;  // TFT  Width in pixels
	const int16_t pixelHeight = 240;  // TFT  Height in pixels
	const int16_t iterations  = 128;  // Fractal iteration limit

	float centerReal  = -0.6; // Image center point in complex plane
	float centerImag  =  0.0; // Image center point in complex plane
	float rangeReal   =  3.0; // Image coverage in complex plane
	float rangeImag   =  3.0; // Image coverage in complex plane


	int64_t n, a, b, a2, b2, posReal, posImag;
	int32_t startReal   = (int64_t)((centerReal - rangeReal * 0.5)   * (float)(1 << bits));
	int32_t startImag   = (int64_t)((centerImag + rangeImag * 0.5)   * (float)(1 << bits));
	int32_t incReal     = (int64_t)((rangeReal / (float)pixelWidth)  * (float)(1 << bits));
	int32_t incImag     = (int64_t)((rangeImag / (float)pixelHeight) * (float)(1 << bits));

	startTime = MilliCount(); // Start Timer Calculation

	posImag = startImag;
	for (int y = 0; y < pixelHeight; y++)
	{
		posReal = startReal;
		for (int x = 0; x < pixelWidth; x++)
		{
			a = posReal;
			b = posImag;
			for (n = iterations; n > 0 ; n--)
			{
				a2 = (a * a) >> bits;
				b2 = (b * b) >> bits;
				if ((a2 + b2) >= (4 << bits))
					break;
				b  = posImag + ((a * b) >> (bits - 1));
				a  = posReal + a2 - b2;
			}
			myTFT.drawPixel(x, y, (n * 29)<<8 | (n * 67));
			posReal += incReal;
		}
		posImag -= incImag;
	}

	elapsedTime = MilliCount() -startTime; // End Timer Calculation
	std::cout << "Time Taken :: " << elapsedTime << " ms" << std::endl;
	delayMilliSecRDL(7000);

}

void EndTests(void)
{
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.PowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "End MandelBrot Test" << std::endl;
}

/*!
	@brief This returns milli-seconds as a 64-bit unsigned number, monotonically increasing,
	since system boot.
	@return milliseconds.
	@details 20922069, multiple now.tv_sec * 1000 to convert to millis then add + ((uint64_t)now.tv_nsec/1000000U
		to get millis resolution
*/
static uint64_t MilliCount(void)
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  (uint64_t)now.tv_sec * 1000U + ((uint64_t)now.tv_nsec/1000000U);
}


// *************** EOF ****************
