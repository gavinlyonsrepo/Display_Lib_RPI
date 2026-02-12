/*!
	@file examples/ili9341/demos/main.cpp
	@brief  Various demos see test list
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 401 Mandelbro set demo
		-# test 402 Julia Set demo
		-# test 403 analog clock demo
		-# test 404 Unit circle demo
		-# test 405 Control panel demo
	@todo Improve test 402 Julia Set demo speed performance
*/

// Section ::  libraries
#include <ctime>
#include <random> // gauge
#include <algorithm> // For std::clamp
#include <iostream> // cout cin
#include <limits> // menu limits

#include <atomic>  // Ctrl + C exit
#include <csignal> // Ctrl + C exit
#include <thread>  // Ctrl + C exit

#include "ILI9341_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
ILI9341_TFT myTFT;
uint16_t SCREEN_WIDTH = 240;// Screen width in pixels
uint16_t SCREEN_HEIGHT = 320; // Screen height in pixels
std::atomic<bool> stopRequested{false}; // Stop signal , Ctrl + c etc

//  Section ::  Function Headers

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
void displayMenu(void);
static uint64_t MilliCount(void); // ~millis()
void EndTests(void);
void handleSignal(int){
	stopRequested = true; // for CtrL +C
}

// Demo 1
void drawMandelbrot(void);
// Demo 2
void drawJuliaSet(float r, float i, float zoom);
void drawJuliaLoop(void);
// Demo 3
void ClockDemo(uint32_t seconds);
std::string UTC_string(void);
// Demo 4
void unitCircleDemo(void);
// Demo 5
const uint16_t QUAD_WIDTH = SCREEN_WIDTH / 2;  // 120px
const uint16_t QUAD_HEIGHT = SCREEN_HEIGHT / 2; // 160px
void ControlPanel(uint16_t countLimit = 100);
// Demo 5-1
void gaugeDemo(uint16_t count = 0);
void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color);
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor);
void drawPointer(int16_t &val, int16_t &oldVal , uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor);
// Demo 5-2
void arcGauge(uint16_t count = 0);
// Demo 5-3
// Gauge parameters  (Q3)
const int GAUGE_WIDTH = 18;
const int GAUGE_HEIGHT = 70;
const int GAUGE_X_START = 7;
const int GAUGE_Y_START = 220;
const int GAUGE_SPACING = 38;
// Store previous values to prevent redundant redraws
float prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
void updateGauges(float phase);
void drawGauge(int x, int y, uint16_t color, float value);
void drawGaugeDemo(uint16_t  count = 0);
void drawGaugeBorder(int x, int y);
// Demo 5-4
// VU Meter parameters (Q4)
const uint16_t VU_X = QUAD_WIDTH + 10;
const uint16_t VU_Y = QUAD_HEIGHT + 60;
const uint16_t VU_BAR_WIDTH = 12;
const uint16_t VU_BAR_HEIGHT = 70;
const uint8_t VU_BAR_COUNT = 5;
const uint16_t VU_BAR_SPACING = 5;
float vuValues[VU_BAR_COUNT] = {0};
void vuMeter(uint16_t count = 0);

//  Section ::  MAIN loop
int main()
{
	if(SetupHWSPI() != 0) return -1; //Hardware SPI
	int choice;
	std::signal(SIGINT, handleSignal); // for user press Ctrl+C
	std::signal(SIGTERM, handleSignal);// for kill command

	do {
		if (stopRequested) break;
		displayMenu();
		std::cin >> choice;
		// Check if input is valid
		if (std::cin.fail()) {
			std::cin.clear(); // Clear error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
			std::cout << "Invalid input. Please enter a number between 1 and 5.\n\n";
			continue;
		}

		switch (choice) {
			case 1: drawMandelbrot(); break;
			case 2: drawJuliaLoop(); break;
			case 3: ClockDemo(20); break;
			case 4: unitCircleDemo();break;
			case 5: ControlPanel(100); break;
			case 6:
				std::cout << "Exiting menu\n";
				break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
				break;
		}
		std::cout << std::endl;

	} while (choice != 6);

	if (stopRequested)
		std::cout << "Exit signal received" << std::endl;
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int  GPIO_CHIP_DEVICE = 0;

	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  24000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	std::cout << "TFT Start Test " << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.SetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.InitScreenSize(SCREEN_WIDTH , SCREEN_HEIGHT);
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


// --- Utilities + Misc functions ---
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
	std::cout << "2. Julia Set\n";
	std::cout << "3. Clock\n";
	std::cout << "4. Unit Circle Demo\n";
	std::cout << "5. Control Panel\n";
	std::cout << "6. Quit\n";
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

// Demo 1
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

// Demo 2
void drawJuliaSet(float realConstant, float imaginaryConstant, float zoom) {

	uint32_t startTime,elapsedTime;
	startTime = MilliCount(); // Start Timer Calculation
	// Julia set
	const uint16_t MAX_ITERATION = 300;
	myTFT.setCursor(0, 0);
	float newReal = 0.0, newImaginary = 0.0, oldReal = 0.0, oldImaginary = 0.0;

	/* Loop through each pixel in X direction */
	for (int16_t x = SCREEN_WIDTH / 2 - 1; x >= 0; x--) { // Rely on inverted symmetry
		/* Loop through each pixel in Y direction */
		for (uint16_t y = 0; y < SCREEN_HEIGHT; y++) {
			// Convert pixel coordinate to complex plane
			oldReal = 1.5 * (x - SCREEN_WIDTH / 2) / (0.5 * zoom * SCREEN_WIDTH);
			oldImaginary = (y - SCREEN_HEIGHT / 2) / (0.5 * zoom * SCREEN_HEIGHT);
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
				myTFT.drawPixel(SCREEN_WIDTH - x - 1, SCREEN_HEIGHT - y - 1, myTFT.Color565(255, 255, rdlib_maths::mapValue(iteration, 0, 100, 255, 0)));
			}
			if (iteration < 200) {
				myTFT.drawPixel(x, y, myTFT.Color565(255, rdlib_maths::mapValue(iteration, 100, 200, 255, 0), 0));
				myTFT.drawPixel(SCREEN_WIDTH - x - 1, SCREEN_HEIGHT - y - 1, myTFT.Color565(255, rdlib_maths::mapValue(iteration, 100, 200, 255, 0), 0));
			}
			else {
				myTFT.drawPixel(x, y, myTFT.Color565(rdlib_maths::mapValue(iteration, 200, 300, 255, 0), 0, 0));
				myTFT.drawPixel(SCREEN_WIDTH - x - 1, SCREEN_HEIGHT - y - 1, myTFT.Color565(rdlib_maths::mapValue(iteration, 200, 300, 255, 0), 0, 0));
			}
		}
		if (stopRequested) break; // for Ctrl + C pressed?
	}
	elapsedTime = MilliCount() -startTime; // End Timer Calculation
	std::cout << "Time Taken :: " << elapsedTime << " ms" << std::endl;
}

void drawJuliaLoop(void) {
	float zoom = 0.5;
	uint8_t NumOfiteration =1;
	std::cout << "Start Julia Set Test" << std::endl;
	for(uint8_t i=0 ;i < NumOfiteration; i ++)
	{
		drawJuliaSet(-0.8, +0.156, zoom);
		myTFT.fillRect(0, 0, 150, 20, myTFT.RDLC_BLACK);
		myTFT.setCursor(0,15);
		myTFT.setTextColor(myTFT.RDLC_WHITE);
		myTFT.print(" Zoom = ");
		myTFT.println(zoom);
		delayMilliSecRDL(3000);
		zoom *= 1.5;
		if (stopRequested) break; // for Ctrl + C pressed?
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setRotation(myTFT.Degrees_0);
	std::cout << "End Julia Set Test"<< std::endl;
}

// Demo 3
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
	// Draw tick marks
	for (int i = 0; i < 60; ++i) {
		float angle = i * 2 * M_PI / 60.0;
		int outerX = centerX + (int)((radius - 1) * sinf(angle));
		int outerY = centerY - (int)((radius - 1) * cosf(angle));
		int innerX = centerX + (int)((radius - (i % 5 == 0 ? 8 : 3)) * sinf(angle));
		int innerY = centerY - (int)((radius - (i % 5 == 0 ? 8 : 3)) * cosf(angle));
		uint16_t color = (i % 5 == 0) ? myTFT.RDLC_WHITE : myTFT.RDLC_DGREY;
		myTFT.drawLine(innerX, innerY, outerX, outerY, color);
	}
	// Draw center pivot
	myTFT.fillCircle(centerX, centerY, 3, myTFT.RDLC_WHITE);
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
		int hour = std::stoi(utcTime.substr(11, 2));
		int minute = std::stoi(utcTime.substr(14, 2));
		int second = std::stoi(utcTime.substr(17, 2));

		// Check if a second has passed (i.e., the second has changed)
		if (second != lastSecond)
		{
			// Draw the hour hand
			myTFT.drawLine(centerX, centerY, xHour, yHour, myTFT.RDLC_BLACK);
			float hourAngle = 2 * M_PI / 12 * (hour % 12 + minute / 60.0f);
			xHour = (uint16_t)(centerX + (radius / 2) * sinf(hourAngle));
			yHour = (uint16_t)(centerY - (radius / 2) * cosf(hourAngle));
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
			xSecond = (uint16_t) (centerX + (radius - 10) * sinf(angle));
			ySecond = (uint16_t) (centerY - (radius - 10) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xSecond, ySecond, myTFT.RDLC_YELLOW);
			lastSecond = second;
			secondsDisplay--;
			if (stopRequested) break; // for Ctrl + C pressed?
		}
		delayMilliSecRDL(10);
		myTFT.setCursor(10,10);
		myTFT.setFont(font_mint);
		myTFT.setTextColor(myTFT.RDLC_DGREEN, myTFT.RDLC_DGREY);
		sprintf(buffer, "%02d", secondsDisplay);
		myTFT.print(buffer);
		std::string timeStr = utcTime.substr(11, 8); // "HH:MM:SS"
		myTFT.setCursor(centerX - 90, centerY + radius + 10);
		myTFT.setTextColor(myTFT.RDLC_CYAN, myTFT.RDLC_DGREY);
		myTFT.setFont(font_mint);
		myTFT.print(timeStr.c_str());
		std::cout<< utcTime << "\r" << std::flush;
	}
	myTFT.setFont(font_default);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}


// Demo 4
void unitCircleDemo(void)
{
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Unit Circle & Sine & Cos Wave" << std::endl;

	int16_t CenterCircleX = 24;
	int16_t CenterCircleY = 64;
	int16_t Radius = 23;
	int16_t waveStartX = 48; // Start position for sine wave
	int16_t waveBaseY = CenterCircleY; // Align sine wave with center
	char buffer[4];

	myTFT.setFont(font_mega);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.drawCircle(CenterCircleX, CenterCircleY, Radius+2, myTFT.RDLC_WHITE);
	myTFT.drawCircle(CenterCircleX, CenterCircleY, Radius+1, myTFT.RDLC_WHITE);
	myTFT.setCursor(5, 130);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.print("Degrees = ");
	myTFT.setCursor(5, 150);
	myTFT.print("Radians = ");
	myTFT.setCursor(5, 170);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_BLACK);
	myTFT.print("    Sin = ");
	myTFT.setTextColor(myTFT.RDLC_TAN, myTFT.RDLC_BLACK);
	myTFT.setCursor(5, 190);
	myTFT.print("    Cos = ");
	// Initialize previous needle position at 0 degrees
	int16_t prevX1 = CenterCircleX + (int16_t)(Radius * cos(0));
	int16_t prevY1 = CenterCircleY - (int16_t)(Radius * sin(0));
	int16_t sineX = waveStartX;
	int16_t cosX = waveStartX;
	for (int j = 0 ; j < 2 ; j++)
	{
		for (int angle = 0; angle <= 360; angle++)
		{
			// Convert angle to radians
			float radians = angle * (std::numbers::pi / 180.0f);
			int16_t x1 = CenterCircleX + (int16_t)(Radius * cos(radians));
			int16_t y1 = CenterCircleY - (int16_t)(Radius * sin(radians)); // Inverted Y for display
			// Erase previous needle
			myTFT.drawLine(CenterCircleX, CenterCircleY, prevX1, prevY1, myTFT.RDLC_BLACK);
			// Draw new needle (counterclockwise)
			myTFT.drawLine(CenterCircleX, CenterCircleY, x1, y1, myTFT.RDLC_GREEN);
			// Update previous needle position
			prevX1 = x1;
			prevY1 = y1;
			// Draw x and y axes
			myTFT.drawFastHLine(0, CenterCircleY, 320, myTFT.RDLC_CYAN);
			myTFT.drawFastVLine(CenterCircleX, 0, 120, myTFT.RDLC_CYAN);
			// Draw sine wave
			int16_t sineY = waveBaseY - (int16_t)(Radius * sin(radians));
			myTFT.drawPixel(sineX, sineY, myTFT.RDLC_RED);
			if ( angle % 2) sineX++;
			// Draw cos wave
			int16_t cosY = waveBaseY - (int16_t)(Radius * cos(radians));
			myTFT.drawPixel(cosX, cosY, myTFT.RDLC_TAN);
			if (angle % 2) cosX++;
			myTFT.setCursor(150, 130);
			sprintf(buffer, "%03d", angle);
			myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
			myTFT.print(buffer);
			myTFT.setCursor(150, 150);
			myTFT.print(radians, 4);
			myTFT.setCursor(150, 170);
			myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_BLACK);
			myTFT.print(sin(radians), 4);
			myTFT.setCursor(150, 190);
			myTFT.setTextColor(myTFT.RDLC_TAN, myTFT.RDLC_BLACK);
			myTFT.print(cos(radians), 4);
			delayMilliSecRDL(50);
			if (stopRequested) break; // for Ctrl + C pressed?
		}
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setRotation(myTFT.Degrees_0);
	std::cout << "End Demo 4"<< std::endl;
}

// Demo 5

void ControlPanel(uint16_t countLimit)
{
	std::cout << "Demo 5 ends at : " << countLimit << std::endl;
	// setup
	myTFT.setRotation(myTFT.Degrees_0);
	uint16_t count = 0;
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	// Draw divider lines
	myTFT.drawLine(QUAD_WIDTH, 0, QUAD_WIDTH, SCREEN_HEIGHT, myTFT.RDLC_GREY); // Vertical
	myTFT.drawLine(0, QUAD_HEIGHT, SCREEN_WIDTH, QUAD_HEIGHT, myTFT.RDLC_GREY); // Horizontal
	myTFT.drawRectWH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, myTFT.RDLC_GREY); // Border

	while (count < countLimit) {
		gaugeDemo(count);      // Q1: Top-Left
		arcGauge(count);       // Q2: Top-Right
		drawGaugeDemo(count);  // Q3: Bottom-Left
		vuMeter(count);        // Q4: Bottom-Right
		count++;
		if (stopRequested) break; // for Ctrl + C pressed?
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Demo 5 Over " << std::endl;
}

// Demo 5-1
void gaugeDemo(uint16_t count)
{
	static int16_t currentValue = 150;
	static int16_t oldValue = 150;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	uint8_t x = 50;
	uint8_t y = 50;
	uint8_t radius = 40;
	char buffer[10];
	// Draw Gauge on first pass
	if(count == 0 )
	{
		myTFT.setCursor(10,100);
		myTFT.print("RPM  :");
		myTFT.drawCircle(x, y, radius, myTFT.RDLC_RED);
		drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
		drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
	}
	// Random number setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes

	int step = stepDist(gen);
	currentValue += step;
	currentValue = std::clamp(currentValue, minValue, maxValue);
	if (oldValue != currentValue) {
		drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
		oldValue = currentValue;
	}
	myTFT.setCursor(75,100);
	sprintf(buffer, "%03d", currentValue);
	myTFT.print(buffer);
}

// Demo 5-1 & Demo 5-2
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor)
{
	float angleRad, innerX, innerY, outerX, outerY;
	int angle;
	// Loop through the specified angle range, drawing ticks every 30 degrees
	for (angle = startAngle; angle <= endAngle; angle += 30)
	{
		// Convert degrees to radians
		angleRad = angle * (std::numbers::pi / 180);
		// inner marker position
		innerX = cos(angleRad) * (radius / scaleFactor);
		innerY = sin(angleRad) * (radius / scaleFactor);
		// outer marker position
		outerX = cos(angleRad) * radius;
		outerY = sin(angleRad) * radius;
		// Draw marker line from inner to outer radius
		myTFT.drawLine(1 + centerX + innerX, 1 + centerY + innerY,
						 1 + centerX + outerX, 1 + centerY + outerY,
						 myTFT.RDLC_WHITE);
	}
}

// Demo 5-1 & Demo 5-2
void drawPointer(int16_t &currentValue, int16_t &oldValue, uint8_t x, uint8_t y, uint8_t r, uint16_t colour, uint16_t bcolour)
{
	uint16_t i;
	// If the current value is increasing
	if (currentValue > oldValue)
	{
		// Incrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i <= currentValue; i++)
		{
			// Erase the previous pointer position
			drawPointerHelper(i - 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Introduce a small delay for smooth animation
			if ((currentValue - oldValue) < 128) {
				delayMilliSecRDL(20);
			}else{
				delayMilliSecRDL(30); // Slower update for small changes
			}
		}
	}
	else // If the current value is decreasing
	{
		// Decrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i >= currentValue; i--)
		{
			// Erase the previous pointer position
			drawPointerHelper(i + 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Adjust delay based on the difference to control animation speed
			if ((oldValue - currentValue) >= 128) {
				delayMilliSecRDL(20); // Faster update for large changes
			}else{
				delayMilliSecRDL(30); // Slower update for small changes
			}
		}
	}
}

//  Demo 5-1 & Demo 5-2
void drawPointerHelper(int16_t value, uint8_t centerX, uint8_t centerY, uint8_t radius, uint16_t pointerColor)
{
	float angleRad, pointerX, pointerY;
	int16_t minValue = 0;
	int16_t maxValue = 255;
	int startAngle = 150; // Gauge starting angle in degrees
	int endAngle = 240;   // Gauge ending angle in degrees
	// Ensure the value stays within the valid range
	if (value > maxValue) value = maxValue;
	if (value < minValue) value = minValue;
	// Map value to an angle in radians based on the gauge range
	angleRad = (((float)(uint16_t)(value - minValue) / (float)(uint16_t)(maxValue - minValue) * endAngle)
				 + startAngle) * (std::numbers::pi / 180);
	// Calculate pointer tip coordinates
	pointerX = cos(angleRad) * (radius / 1.35);
	pointerY = sin(angleRad) * (radius / 1.35);
	// Draw the pointer line from the center to the calculated tip position
	myTFT.drawLine(centerX, centerY, 1 + centerX + pointerX, 1 + centerY + pointerY, pointerColor);
	// Draw a small circle to represent the pointer base
	myTFT.fillCircle(centerX, centerY, 2, pointerColor);
}

// Demo 5-2
void arcGauge(uint16_t count)
{
	static int16_t currentValue = 150;
	static int16_t oldValue = 150;
	char buffer[10];
	// Draw Gauge
	uint8_t x = 170;
	uint8_t y = 60;
	uint8_t radius = 40;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	if (count == 0){
		myTFT.drawArc(x, y, radius, 10,  150.0f, 180.0f, myTFT.RDLC_RED);
		myTFT.drawArc(x, y, radius, 10,  181.0f, 250.0f, myTFT.RDLC_ORANGE);
		myTFT.drawArc(x, y, radius, 10,  251.0f, 320.0f, myTFT.RDLC_YELLOW);
		myTFT.drawArc(x, y, radius, 10,  321.0f, 35.0f, myTFT.RDLC_GREEN);
		drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
		drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
		myTFT.setCursor(130,100);
		myTFT.print("TEMP :");
	}
	// Random number setup
	// Initialize random generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes
	int step = stepDist(gen);
	currentValue += step;
	currentValue = std::clamp(currentValue, minValue, maxValue);
	myTFT.setCursor(0,100);
	if (oldValue != currentValue) {
		drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
		oldValue = currentValue;
	}
	sprintf(buffer, "%03d", currentValue);
	myTFT.setCursor(200,100);
	myTFT.print(buffer);

}

// Demo 5-3
void drawGaugeBorder(int x, int y) { // Draw border once
	myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.RDLC_WHITE);
}

void drawGaugeDemo(uint16_t count)
{
	// Draw borders once
	if (count == 0){

		myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START - GAUGE_HEIGHT + 45);
		myTFT.print("VOL");
		drawGaugeBorder(GAUGE_X_START, GAUGE_Y_START);

		myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START - GAUGE_HEIGHT + 45);
		myTFT.print("LVL");
		drawGaugeBorder(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START);

		myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START - GAUGE_HEIGHT + 45);
		myTFT.print("PWR");
		drawGaugeBorder(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START);

		prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
	}
	static float phase = 0;
	updateGauges(phase);
	phase += 0.1;
}

// Gauge update
void drawGauge(int x, int y, float value, float prevVal) {
	int fillHeight = static_cast<int>(GAUGE_HEIGHT * value);
	int prevFillHeight = (prevVal < 0) ? 0 : static_cast<int>(GAUGE_HEIGHT * prevVal);

	if (fillHeight == prevFillHeight) return;  // nothing to update

	// If shrinking, clear the difference
	if (fillHeight < prevFillHeight) {
	int clearHeight = prevFillHeight - fillHeight;
	myTFT.fillRectangle(
		x,
		y + (GAUGE_HEIGHT - prevFillHeight),
		GAUGE_WIDTH,
		clearHeight,
		myTFT.RDLC_BLACK);
	}
	// If growing, draw gradient for the new part
	if (fillHeight > prevFillHeight) {
	int growHeight = fillHeight - prevFillHeight;
	for (int i = 0; i < growHeight; i++) {
		uint8_t val = rdlib_maths::mapValue(static_cast<int>(prevFillHeight + i), 0, GAUGE_HEIGHT - 1, 1, 127);
		uint16_t color = rdlib_maths::generateColor(val);
		myTFT.fillRectangle(
		x,
		y + (GAUGE_HEIGHT - fillHeight) + (growHeight - 1 - i),
		GAUGE_WIDTH,
		1,
		color);
	}
	}
}

// Update all gauges + numeric readouts
void updateGauges(float phase) {
	// Calculate values (0 → 1)
	float val1 = (std::sin(phase) + 1) / 2;
	float val2 = (std::cos(phase) + 1) / 2;
	float val3 = ((std::sin(phase) * std::cos(phase)) + 1) / 2;

	char buffer[6];  // formatted text
	// --- Gauge 1 ---
	if (val1 != prevVal1) {
		drawGauge(GAUGE_X_START, GAUGE_Y_START, val1, prevVal1);
		sprintf(buffer, "%.2f", val1);
		myTFT.fillRectangle(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.RDLC_BLACK);
		myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal1 = val1;
	}
	// --- Gauge 2 ---
	if (val2 != prevVal2) {
		drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, val2, prevVal2);
		sprintf(buffer, "%.2f", val2);
		myTFT.fillRectangle(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.RDLC_BLACK);
		myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal2 = val2;
	}
	// --- Gauge 3 ---
	if (val3 != prevVal3) {
		drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, val3, prevVal3);
		sprintf(buffer, "%.2f", val3);
		myTFT.fillRectangle(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.RDLC_BLACK);
		myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal3 = val3;
	}
}

// Demo 5-4
// Q4: VU Meter (Bottom-Right)
void vuMeter(uint16_t count) {
	if (count == 0) { // First pass: Draw static elements
		// Label
		myTFT.setFont(font_default);
		myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
		myTFT.setCursor(QUAD_WIDTH + 10, QUAD_HEIGHT + 140);
		myTFT.print("SIG:");

		// Bar outlines
		for (int i = 0; i < VU_BAR_COUNT; i++) {
			int16_t x = VU_X + i * (VU_BAR_WIDTH + VU_BAR_SPACING);
			myTFT.drawRectWH(x, VU_Y, VU_BAR_WIDTH, VU_BAR_HEIGHT, myTFT.RDLC_WHITE);
		}
	}

	// Update fake data
	float t = count * 0.1f;
	float sum = 0.0f;
	for (int i = 0; i < VU_BAR_COUNT; i++) {
		vuValues[i] = 50.0f + 30.0f * sin(t * (0.8f + i * 0.1f)) + (rand() % 20 - 10); // 20-80
		vuValues[i] = (vuValues[i] < 0) ? 0 : (vuValues[i] > 100) ? 100 : vuValues[i];
		sum += vuValues[i];
	}
	int avgValue = (int)(sum / VU_BAR_COUNT); // Average for display

	// Clear and update average display
	myTFT.fillRectangle(QUAD_WIDTH + 46, QUAD_HEIGHT + 140, 20, 10, myTFT.RDLC_BLACK); // Clear 2-digit area
	myTFT.setCursor(QUAD_WIDTH + 46, QUAD_HEIGHT + 140);
	myTFT.print(avgValue);

	// Draw bars (clear and fill only dynamic areas)
	for (int i = 0; i < VU_BAR_COUNT; i++) {
		int16_t x = VU_X + i * (VU_BAR_WIDTH + VU_BAR_SPACING);
		// Clear previous bar fill (2px inset to preserve all outlines)
		myTFT.fillRectangle(x + 2, VU_Y + 2, VU_BAR_WIDTH - 4, VU_BAR_HEIGHT - 4, myTFT.RDLC_BLACK);
		// Draw new bar fill with gradient color
		int16_t barHeight = (int16_t)(VU_BAR_HEIGHT * (vuValues[i] / 100.0f));
		uint8_t colorVal = rdlib_maths::mapValue((int)vuValues[i], 0, 100, 1, 127); // Map 0-100 to 1-127
		uint16_t barColor = rdlib_maths::generateColor(colorVal);
		myTFT.fillRectangle(x + 2, VU_Y + (VU_BAR_HEIGHT - barHeight), VU_BAR_WIDTH - 4, barHeight - 2, barColor);
	}
}

/// @endcond
