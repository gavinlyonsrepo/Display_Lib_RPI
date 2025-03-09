/*!
	@file examples/st7735/demos/main.cpp
	@brief  Various demos see test list
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 440 Round Gauge random value
		-# test 441 Vertical Gauge Based on sin(x), cos(x), & sin(x)*cos(x). Updates over time to create a dynamic effect.
		-# test 442 Radar, draw a line at angle function
		-# test 443 arc Gauge
*/

// Section ::  libraries
#include <ctime>
#include <random> // gauge
#include <algorithm> // For std::clamp
#include <iostream> // cout cin
#include <limits> // menu limits
#include <csignal> //catch user Ctrl+C
#include "ST7735_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
ST7735_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEV = 0;

uint8_t OFFSET_COL = 0;  // These offsets can be adjusted for any issues
uint8_t OFFSET_ROW = 0; // with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 128;// Screen width in pixels
uint16_t TFT_HEIGHT = 128; // Screen height in pixels

int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  8000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers
int8_t Setup(void); // setup + user options for hardware SPI
void signal_callback_handler(int signum);
void displayMenu(void);
void EndTests(void);

// Demo 1
void gaugeDemo(uint16_t  countLimit = 50);
void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color);
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor);
void drawPointer(int16_t &val, int16_t &oldVal , uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor);
// Demo 2
// Gauge properties
const int GAUGE_WIDTH = 20;
const int GAUGE_HEIGHT = 70;
const int GAUGE_X_START = 5;
const int GAUGE_Y_START = 10;
const int GAUGE_SPACING = 40;
// Store previous values to prevent redundant redraws
float prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
void updateGauges(float phase);
void drawGauge(int x, int y, uint16_t color, float value);
void drawGaugeDemoTwo(uint16_t  countLimit = 50);
// Demo 3
void demoRadar(uint16_t sweeps=7);
// Demo 4
void arcGauge(uint16_t count=100);

//  Section ::  MAIN loop

int main()
{
	if(Setup() != 0) return -1; //Hardware SPI
	myTFT.fillScreen(myTFT.RDLC_BLACK);
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
			case 1: gaugeDemo(20); break;
			case 2: drawGaugeDemoTwo(75); break;
			case 3: demoRadar(7); break;
			case 4: arcGauge(100); break;
			case 5:
				std::cout << "Exiting menu\n";
				break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
				break;
		}
		std::cout << std::endl;
	} while (choice != 5);
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

int8_t Setup(void)
{
	std::cout << "Demos Start" << std::endl;
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 PCB_TYPE + SPI settings**
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SPI_DEV, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	return 0;
}

void EndTests(void)
{
	char teststr1[] = "Demos over";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setFont(font_retro);
	myTFT.writeCharString(5, 5, teststr1);
	delayMilliSecRDL(1500);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "Demos Over" << std::endl;
}

void displayMenu() {
	std::cout << "Demos Menu:\n";
	std::cout << "1. Round Gauge\n";
	std::cout << "2. Vertical Gauges \n";
	std::cout << "3. Radar \n";
	std::cout << "4. Arc Gauge \n";
	std::cout << "5. Quit\n";
	std::cout << "Enter your choice: ";
}


// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	EndTests();
	exit(signum);
}

// Demo 1

void gaugeDemo(uint16_t countLimit)
{
	std::cout << "Gauge Demo ends at : " << countLimit << std::endl;
	myTFT.setCursor(0,100);
	myTFT.println(" Count :: ");
	myTFT.print(" Value :: ");
	int16_t currentValue = 0;
	int16_t oldValue = 0;
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x = 50;
	uint8_t y = 50;
	uint8_t radius = 40;
	myTFT.drawCircle(x, y, radius, myTFT.RDLC_RED);
	drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
	drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
	// Random number setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(1, 254);

	while(count++ < countLimit)
	{
		currentValue = dist(gen);
		myTFT.setCursor(0,100);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
			oldValue = currentValue;
		}
		myTFT.setCursor(75,100);
		myTFT.print(count);
		myTFT.setCursor(75,110);
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Gauge Demo Over " << std::endl;
}


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

void drawPointer(int16_t &currentValue, int16_t &oldValue, uint8_t x, uint8_t y, uint8_t r, uint16_t colour, uint16_t bcolour) 
{
	uint8_t i;
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

// Demo 2

void drawGaugeDemoTwo(uint16_t countLimit)
{
	std::cout << "Gauge Demo 2  ends at : " << countLimit << std::endl;
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	float phase = 0;
	while (countLimit-- > 1 ) 
	{
		updateGauges(phase);
		phase += 0.1;
		std::cout<< countLimit << "\r" << std::flush;
		delayMilliSecRDL(250);
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Gauge Demo 2 Over " << std::endl;
}

void drawGauge(int x, int y, uint16_t color, float value) {
	// Draw border
	myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.RDLC_WHITE);
	// Clear inside
	myTFT.fillRectangle(x, y, GAUGE_WIDTH, GAUGE_HEIGHT, myTFT.RDLC_BLACK);
	// Draw fill
	int fillHeight = static_cast<int>(GAUGE_HEIGHT * value);
	if (fillHeight == 0) fillHeight =1;
	myTFT.fillRectangle(x, y + (GAUGE_HEIGHT - fillHeight), GAUGE_WIDTH, fillHeight, color);
}

void updateGauges(float phase) {
	// Calculate values
	float val1 = (std::sin(phase) + 1) / 2;
	float val2 = (std::cos(phase) + 1) / 2;
	float val3 = ((std::sin(phase) * std::cos(phase)) + 1) / 2;
	// Display values only if they change
	char buffer[6]; // To store formatted text
	// Draw gauges only if values changed
	
	if (val1 != prevVal1) {
		drawGauge(GAUGE_X_START, GAUGE_Y_START, myTFT.RDLC_RED, val1);
		sprintf(buffer, "%.2f", val1);
		myTFT.fillRectangle(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.RDLC_BLACK); // Clear previous text
		myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal1 = val1;
	}
	if (val2 != prevVal2) {
		drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, myTFT.RDLC_GREEN, val2);
		sprintf(buffer, "%.2f", val2);
		myTFT.fillRectangle(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.RDLC_BLACK);
		myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal2 = val2;
	}
	if (val3 != prevVal3) {
		drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, myTFT.RDLC_BLUE, val3);
		sprintf(buffer, "%.2f", val3);
		myTFT.fillRectangle(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.RDLC_BLACK);
		myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal3 = val3;
	}
}

// demo3 
void demoRadar(uint16_t sweeps)
{
	std::cout << "Demo Radar :Draw line at angle function ends at : " << sweeps << std::endl;
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.RDLC_BLACK, myTFT.RDLC_TAN);
	myTFT.fillScreen(myTFT.RDLC_TAN);
	float radians = 0.0F;
	myTFT.fillCircle(64, 64, 50, myTFT.RDLC_BLACK);
	myTFT.drawCircle(64, 64, 50, myTFT.RDLC_GREEN);
	myTFT.drawCircle(64, 64, 49, myTFT.RDLC_GREEN);
	for (uint8_t j =0 ; j< sweeps ; j++)
	{
		for (int angle = 0; angle <= 361; angle++)
		{
			//drawLineAngle(x, y, angle, start,length, offset,  color);
			myTFT.drawLineAngle(64, 64, angle-1, 0 , 48, -90, myTFT.RDLC_BLACK);//erase last needle
			myTFT.drawLineAngle(64, 64, angle, 0, 48, -90, myTFT.RDLC_GREEN);//draw needle
			myTFT.fillRectangle(20 ,119, 90 , 10 , myTFT.RDLC_TAN);
			myTFT.setCursor(20,119);
			myTFT.print(angle);
			myTFT.print(" ");
			// Convert angles to radians
			radians  = angle * (std::numbers::pi / 180);
			myTFT.print(radians);
			delayMilliSecRDL(15);
		}
		std::cout<< +j << "\r" << std::flush;
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}


//demo 4
void arcGauge(uint16_t countLimit)
{
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	std::cout << "Arc Demo ends at : " << countLimit << std::endl;
	int16_t currentValue = 150;
	int16_t oldValue = 150;
	myTFT.setFont(font_default);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x = 50;
	uint8_t y = 50;
	uint8_t radius = 40;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	//myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.RDLC_BLUE);
	myTFT.drawArc(x, y, radius, 10,  150.0f, 180.0f, myTFT.RDLC_RED);
	myTFT.drawArc(x, y, radius, 10,  181.0f, 250.0f, myTFT.RDLC_ORANGE);
	myTFT.drawArc(x, y, radius, 10,  251.0f, 320.0f, myTFT.RDLC_YELLOW);
	myTFT.drawArc(x, y, radius, 10,  321.0f, 35.0f, myTFT.RDLC_GREEN);
	drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
	drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
	// Random number setup
	// Initialize random generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes
	while(count++ < countLimit)
	{
		int step = stepDist(gen);
		currentValue += step;
		currentValue = std::clamp(currentValue, minValue, maxValue);
		myTFT.setCursor(0,100);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
			oldValue = currentValue;
		}
		myTFT.print(" Count :: ");
		myTFT.println(count);
		myTFT.print(" Value :: ");
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Arc Gauge Demo Over " << std::endl;
}
/// @endcond
