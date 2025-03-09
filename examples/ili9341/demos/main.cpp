/*!
	@file examples/ili9341/demos/main.cpp
	@brief  Various demos see test list
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 401 Mandelbro set demo
		-# test 402 Julia Set demo
		-# test 403 analog clock demo
*/

// Section ::  libraries
#include <ctime>
#include <iostream> // cout cin
#include <limits> // menu limits
#include <csignal> //catch user Ctrl+C
#include "ILI9341_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
ILI9341_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0;

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  24000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device


//  Section ::  Function Headers

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
void drawMandelbrot(void);
void drawJuliaSet(float r, float i, float zoom);
void drawJuliaLoop(void);
void ClockDemo(uint32_t seconds);


std::string UTC_string(void);
void signal_callback_handler(int signum);
void displayMenu(void);
static uint64_t MilliCount(void); // ~millis()
void EndTests(void);

//  Section ::  MAIN loop


int main()
{
	if(SetupHWSPI() != 0) return -1; //Hardware SPI
	int choice;
	signal(SIGINT, signal_callback_handler); //Ctrl+ c
	do {
		displayMenu();
		std::cin >> choice;
		// Check if input is valid
		if (std::cin.fail()) {
			std::cin.clear(); // Clear error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
			std::cout << "Invalid input. Please enter a number between 1 and 4.\n\n";
			continue;
		}

		switch (choice) {
			case 1: drawMandelbrot(); break;
			case 2: drawJuliaLoop(); break;
			case 3: ClockDemo(20); break;
			case 4:
				std::cout << "Exiting menu\n";
				break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
				break;
		}
		std::cout << std::endl;

	} while (choice != 4);

	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	std::cout << "TFT Start Test " << std::endl;

// ** USER OPTION 1 GPIO HW SPI **
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.InitScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.InitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	return 0;
}

void ClockDemo(uint32_t secondsDisplay) 
{
	char buffer[3];
	// Determine the dimensions of the clock
	int radius = 80;
	int centerX = 120;
	int centerY = 160;
	// Draw Clock
	myTFT.fillScreen(myTFT.RDLC_DGREY);
	myTFT.fillCircle(centerX, centerY, radius, myTFT.RDLC_BLACK);
	// Start the clock
	uint16_t xSecond = centerX, ySecond = centerY;
	uint16_t xMinute = centerX, yMinute = centerY;
	uint16_t xHour = centerX, yHour = centerY;
	float angle;
	int lastSecond = -1; // Initialize to a value that is not valid for comparison

	while(secondsDisplay > 1)
	{
		// Get UTC time as string
		std::string utcTime = UTC_string();  // UTC_string() returns a string like "YYYY-MM-DD HH:MM:SS"
		// Parse the time string to extract hour, minute, and second
		int hour = std::stoi(utcTime.substr(11, 2));  // Extract hour
		int minute = std::stoi(utcTime.substr(14, 2)); // Extract minute
		int second = std::stoi(utcTime.substr(17, 2)); // Extract second

		// Check if a second has passed (i.e., the second has changed)
		if (second != lastSecond) 
		{
			// Draw the hour hand
			myTFT.drawLine(centerX, centerY, xHour, yHour, myTFT.RDLC_BLACK);
			angle = 2 * M_PI / 12 * hour;
			xHour = (uint16_t) (centerX + (radius / 2) * sinf(angle));
			yHour = (uint16_t) (centerY - (radius / 2) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xHour, yHour, myTFT.RDLC_RED);
			// Draw the minute hand
			myTFT.drawLine(centerX, centerY, xMinute, yMinute, myTFT.RDLC_BLACK);
			angle = 2 * M_PI / 60 * minute;
			xMinute = (uint16_t) (centerX + (2 * radius / 3) * sinf(angle));
			yMinute = (uint16_t) (centerY - (2 * radius / 3) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xMinute, yMinute, myTFT.RDLC_GREEN);
			// Draw the second hand
			myTFT.drawLine(centerX, centerY, xSecond, ySecond, myTFT.RDLC_BLACK);
			angle = 2 * M_PI / 60 * second;
			xSecond = (uint16_t) (centerX + (radius - 2) * sinf(angle));
			ySecond = (uint16_t) (centerY - (radius - 2) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xSecond, ySecond, myTFT.RDLC_YELLOW);
			lastSecond = second;
			secondsDisplay--;
		}
		delayMilliSecRDL(10);
		myTFT.setCursor(10,10);
		myTFT.setFont(font_mint);
		myTFT.setTextColor(myTFT.RDLC_DGREEN, myTFT.RDLC_DGREY);
		sprintf(buffer, "%02d", secondsDisplay);
		myTFT.print(buffer);
		std::cout<< utcTime << "\r" << std::flush;
	}
	myTFT.setFont(font_default);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void drawMandelbrot(void)
{
	myTFT.setRotation(myTFT.Degrees_90);
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
	std::cout << "End MandelBrot Test"<< std::endl;
	delayMilliSecRDL(5000);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setRotation(myTFT.Degrees_0);
	
}


void drawJuliaSet(float realConstant, float imaginaryConstant, float zoom) {

	// Julia set
	const uint16_t MAX_ITERATION = 300;
	myTFT.setCursor(0, 0);
	float newReal = 0.0, newImaginary = 0.0, oldReal = 0.0, oldImaginary = 0.0;

	/* Loop through each pixel in X direction */
	for (int16_t x = TFT_WIDTH / 2 - 1; x >= 0; x--) { // Rely on inverted symmetry
		/* Loop through each pixel in Y direction */
		for (uint16_t y = 0; y < TFT_HEIGHT; y++) {
			// Convert pixel coordinate to complex plane
			oldReal = 1.5 * (x - TFT_WIDTH / 2) / (0.5 * zoom * TFT_WIDTH);
			oldImaginary = (y - TFT_HEIGHT / 2) / (0.5 * zoom * TFT_HEIGHT);
			uint16_t iteration = 0;
			// Iterate until escape condition is met or maximum iterations are reached
			while ((oldReal * oldReal + oldImaginary * oldImaginary) < 4.0 && iteration < MAX_ITERATION) {
				newReal = oldReal * oldReal - oldImaginary * oldImaginary;
				newImaginary = 2.0 * oldReal * oldImaginary;
				oldReal = newReal + realConstant;
				oldImaginary = newImaginary + imaginaryConstant;
				iteration++;
			}
			/* Display the pixel with color based on iteration count */
			if (iteration < 100) {
				myTFT.drawPixel(x, y, myTFT.Color565(255, 255, rdlib_maths::mapValue(iteration, 0, 100, 255, 0)));
				myTFT.drawPixel(TFT_WIDTH - x - 1, TFT_HEIGHT - y - 1, myTFT.Color565(255, 255, rdlib_maths::mapValue(iteration, 0, 100, 255, 0)));
			}
			if (iteration < 200) {
				myTFT.drawPixel(x, y, myTFT.Color565(255, rdlib_maths::mapValue(iteration, 100, 200, 255, 0), 0));
				myTFT.drawPixel(TFT_WIDTH - x - 1, TFT_HEIGHT - y - 1, myTFT.Color565(255, rdlib_maths::mapValue(iteration, 100, 200, 255, 0), 0));
			}
			else {
				myTFT.drawPixel(x, y, myTFT.Color565(rdlib_maths::mapValue(iteration, 200, 300, 255, 0), 0, 0));
				myTFT.drawPixel(TFT_WIDTH - x - 1, TFT_HEIGHT - y - 1, myTFT.Color565(rdlib_maths::mapValue(iteration, 200, 300, 255, 0), 0, 0));
			}
		}
	}
}

void drawJuliaLoop(void) {
	float zoom = 0.5; 
	for(uint8_t i=0 ;i < 5 ; i ++)
	{
		drawJuliaSet(-0.8, +0.156, zoom);
		myTFT.fillRect(0, 0, 150, 20, myTFT.RDLC_BLACK);
		myTFT.setCursor(0,15);
		myTFT.setTextColor(myTFT.RDLC_WHITE);
		myTFT.print(" Zoom = ");
		myTFT.println(zoom);
		delayMilliSecRDL(2000);
		zoom *= 1.5;
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}


void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.PowerDown(); // Power down device
	std::cout << "End Demos" << std::endl;
}

/*
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

void displayMenu() {
	std::cout << "Demos Menu:\n";
	std::cout << "1. Mandlebrot Set\n";
	std::cout << "2. Julia Set \n";
	std::cout << "3. Clock  \n";
	std::cout << "4. Quit\n";
	std::cout << "Enter your choice: ";
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
