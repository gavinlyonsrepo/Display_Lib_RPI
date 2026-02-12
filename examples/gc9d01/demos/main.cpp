/*!
	@file examples/gc9d01/demos/main.cpp
	@brief  Various demos see test list
	@details Uses advanced screen buffer mode. Hardware SPI
			Setup for 160X160 dual gate display
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 440 Round Gauge
		-# test 441 Arc Meter
*/

// Section ::  libraries
#include <random> // gauge
#include <iostream> // cout cin
#include <algorithm> // For std::clamp
#include <limits> // menu limits

#include <atomic>  // Ctrl + C exit
#include <csignal> // Ctrl + C exit
#include <thread>  // Ctrl + C exit

#include "GC9D01_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
std::atomic<bool> stopRequested{false};

GC9D01_TFT myTFT;

//  Section ::  Function Headers
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
uint8_t SetupBufferMode(void); // setup the buffer

void displayMenu(void);
void EndTests(void);
// // for CtrL +C
void handleSignal(int){
	stopRequested = true;
}

// === Demo 1 & 2 ===
void gaugeDemo(uint16_t  countLimit = 50);  // demo1
void arcGauge(uint16_t count=100); // demo 2
void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color);
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor, uint16_t color);
void drawPointer(int16_t &val, int16_t &oldVal , uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor);

// Section :: MAIN loop
int main()
{
	//rdlib_config::loadConfig(); // optional
	if(SetupHWSPI() != 0) return -1; // Hardware SPI setup
	std::signal(SIGINT, handleSignal); // for user press Ctrl+C
	std::signal(SIGTERM, handleSignal);// for kill command
	int choice;
	do {
		if (stopRequested) break;
		displayMenu();
		std::cin >> choice;
		// Check if input is valid
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Please enter a number between 1 and 3.\n\n";
			continue;
		}
		switch (choice) {
			case 1: gaugeDemo(100); break;
			case 2: arcGauge(100); break;
			case 3:
				std::cout << "Exiting menu\n";
				break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
				break;
		}
		std::cout << std::endl;
	} while (choice != 3);

	if (stopRequested)
		std::cout << "Stop Signal Sent" << std::endl;
	EndTests();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	// GPIO
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int  GPIO_CHIP_DEVICE = 0;
	// SPI
	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  24000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	// Screen
	uint16_t TFT_WIDTH = 160;// Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	uint16_t OFFSET_X   =   0; // Screen X offset in pixels
	uint16_t OFFSET_Y   =   0; // Screen Y offset in pixels
	GC9D01_TFT::Resolution_e DisplayType = GC9D01_TFT::Resolution_e::RGB160x160_DualGate;
	
	std::cout << "TFT Start Demos: Ctrl + C quits" << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.TFTInitScreenSize(TFT_WIDTH , TFT_HEIGHT, DisplayType, OFFSET_X, OFFSET_Y);
// ***********************************
// ** USER OPTION 3 SPI **
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	return SetupBufferMode();
}

// Set up advanced screen buffer mode
uint8_t SetupBufferMode(void)
{
	// 1. Turn it on!
	myTFT.setAdvancedScreenBuffer_e(myTFT.AdvancedScreenBuffer_e::On);
	// 2. Check its on, optional!
	if (myTFT.getAdvancedScreenBuffer_e() == myTFT.AdvancedScreenBuffer_e::Off)
	{
		std::cout << "Error: Setup: Wrong Mode. This example is for Advanced Screen Buffer Mode" << std::endl;
		return 4;
	}
	// 3. Set up buffer
	if (myTFT.setBuffer() != rdlib::Success) 
		return 5;
	// 4. Turn on Text pixel mode(instead of local buffer mode:default)
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text

	// 5. clear buffer and screen
	myTFT.fillScreen(myTFT.RDLC_BLACK);	
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.writeBuffer();
	return 0;
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "End Demos" << std::endl;
}

void displayMenu() {
	std::cout << "Demos Menu:\n";
	std::cout << "1. Round Gauge Demo\n";
	std::cout << "2. Arc Gauge Demo\n";
	std::cout << "3. Quit\n";
	std::cout << "Enter your choice: ";
}


// === Demo 1 & 2 ===

void gaugeDemo(uint16_t countLimit) // Demo 1
{
	std::cout << "Gauge Demo 1 ends at : " << countLimit << std::endl;
	int16_t currentValue = 120;
	int16_t oldValue = 120;
	myTFT.setFont(font_mega);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x =80;
	uint8_t y = 80;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	uint8_t radius = 60;
	myTFT.drawCircle(x, y, radius, myTFT.RDLC_RED);
	myTFT.drawArc(x,y, radius+11, 10 ,0.0f, 360.0f, myTFT.RDLC_RED);
	drawGaugeMarkers(x, y, radius, 150, 390, 1.3, myTFT.RDLC_WHITE);
	drawGaugeMarkers(x, y, radius, 165, 375, 1.1, myTFT.RDLC_TAN);
	// Random number setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes

	while(count++ < countLimit)
	{
		int step = stepDist(gen);
		currentValue += step;
		currentValue = std::clamp(currentValue, minValue, maxValue);
		myTFT.setCursor(60,120);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
			oldValue = currentValue;
		}
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
		myTFT.writeBuffer();
		if (stopRequested) break; // for Ctrl + C pressed?
	}
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.writeBuffer();
	std::cout << "Gauge Demo Over " << std::endl;
}

void arcGauge(uint16_t countLimit) // demo 2
{
	myTFT.setTextCharPixelOrBuffer(true);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	std::cout << "Arc Demo 2 ends at count= " <<  countLimit << std::endl;
	int16_t currentValue = 120;
	int16_t oldValue = 120;
	myTFT.setFont(font_default);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint16_t x = 80;
	uint16_t y =80;
	uint16_t radius = 70;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	//myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.RDLC_BLUE);
	myTFT.drawArc(x, y, radius, 11,  150.0f, 180.0f, myTFT.RDLC_RED);
	myTFT.drawArc(x, y, radius, 11,  181.0f, 250.0f, myTFT.RDLC_ORANGE);
	myTFT.drawArc(x, y, radius, 11,  251.0f, 320.0f, myTFT.RDLC_YELLOW);
	myTFT.drawArc(x, y, radius, 11,  321.0f, 30.0f, myTFT.RDLC_GREEN);
	drawGaugeMarkers(x, y, radius, 150.0f, 390.0f, 1.3f, myTFT.RDLC_WHITE);
	drawGaugeMarkers(x, y, radius, 165.0f, 375.0f, 1.1f, myTFT.RDLC_TAN);
	// Random number setup
	// Initialize random generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes
	while(count++ < countLimit )
	{
		int step = stepDist(gen);
		currentValue += step;
		currentValue = std::clamp(currentValue, minValue, maxValue);
		myTFT.setCursor(40,125);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
			oldValue = currentValue;
		}
		myTFT.print(" Count: ");
		myTFT.print(count);
		myTFT.setCursor(40,140);
		myTFT.print(" Value: ");
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
		myTFT.writeBuffer();
		if (stopRequested) break; // for Ctrl + C pressed?
	}
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.writeBuffer();
	std::cout <<  "Arc Gauge Demo Over" << std::endl;
}

void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor, uint16_t color)
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
					   color);
	}
}

void drawPointer(int16_t &currentValue, int16_t &oldValue, uint8_t x, uint8_t y, uint8_t r, uint16_t colour, uint16_t bcolour)
{
	uint16_t i;
	// If the current value is increasing
	if (currentValue > oldValue)
	{
		// Incrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i <= currentValue; i++)
		{
			drawPointerHelper(i - 1, x, y, r, bcolour);
			drawPointerHelper(i, x, y, r, colour);
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
			drawPointerHelper(i + 1, x, y, r, bcolour);
			drawPointerHelper(i, x, y, r, colour);
			if ((oldValue - currentValue) >= 128) {
				delayMilliSecRDL(20);
			} else {
				delayMilliSecRDL(30); // Slower update for small changes
			}
		}
	}
}

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


/// @endcond
