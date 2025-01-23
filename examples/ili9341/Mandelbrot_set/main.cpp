/*!
	@file examples/ili9341/Mandelbrot_set/main.cpp
	@brief  Speed test using drawPixel to draw a mandelbrot set
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# 400 Mandelbro set test
*/

// Section ::  libraries
#include <ctime>
#include <iostream> // cout
#include "ILI9341_TFT_LCD_RDL.hpp"

// Section :: Globals
ILI9341_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0;

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

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
	std::cout << "TFT Start Test " << std::endl;
	std::cout << "ili9341 library version : " << GetRDLibVersionNum()<< std::endl;
	std::cout <<"Lgpio library version :" << lguVersion() << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.InitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rpiDisplay_Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}

void drawMandelbrot(void)
{
	std::cout << "Start MandelBrot Test"<< std::endl;


	uint32_t startTime,elapsedTime; // Variables to track start and elapsed time

	// Constants for image and fractal settings
	const int16_t bits        = 20;   // Fractional resolution used for precision (higher bits = finer precision)
	const int16_t pixelWidth  = 320;  // TFT  Width in pixels
	const int16_t pixelHeight = 240;  // TFT  Height in pixels
	const int16_t iterations  = 128;   // Maximum number of iterations to check for each pixel

	// Mandelbrot set properties: Center point and coverage area in the complex plane
	float centerReal  = -0.6; // Image center point in complex plane
	float centerImag  =  0.0; // Image center point in complex plane
	float rangeReal   =  3.0; // Image coverage in complex plane
	float rangeImag   =  3.0; // Image coverage in complex plane

	// Buffer to store pixel color values (16-bit per pixel)
	uint8_t buffer[pixelWidth * pixelHeight*2];
	//uint16_t [pixelWidth * pixelHeight];
	
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
			//buffer[y * pixelWidth + x] = (n * 29)<<8 | (n * 67);
			// Calculate the color (16-bit value)
			uint16_t color = (n * 29) << 8 | (n * 67);

			// Write to the buffer (split into high and low bytes)
			buffer[2 * (y * pixelWidth + x)] = (color >> 8) & 0xFF;  // High byte (most significant byte)
			buffer[2 * (y * pixelWidth + x)+1] = color & 0xFF;       // Low byte (least significant byte)
			posReal += incReal;
		}
		posImag -= incImag;
	}

	myTFT.drawBitmap16(0, 0,buffer, pixelWidth, pixelHeight);

	elapsedTime = MilliCount() -startTime; // End Timer Calculation
	std::cout << "Time Taken :: " << elapsedTime << " ms" << std::endl;
	delayMilliSecRDL(7000);

}



void EndTests(void)
{
	myTFT.fillScreen(RDLC_BLACK);
	myTFT.PowerDown(); // Power down device
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
