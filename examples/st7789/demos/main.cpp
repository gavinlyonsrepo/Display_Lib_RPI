/*!
	@file examples/st7789/demos/main.cpp
	@brief  Various demos see test list
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 430 Round Gauge random value
		-# test 431 Vertical Gauge Based on sin(x), cos(x), & sin(x)*cos(x). Updates over time to create a dynamic effect.
*/

// Section ::  libraries
#include <ctime>
#include <random> // gauge
#include <iostream> // cout cin
#include <limits> // menu limits
#include <csignal> //catch user Ctrl+C
#include "ST7789_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
ST7789_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0;

uint8_t OFFSET_COL = 0;  // These offsets can be adjusted for any issues
uint8_t OFFSET_ROW = 0; // with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  24000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
std::string UTC_string(void);
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
const int GAUGE_HEIGHT = 140;
const int GAUGE_X_START = 20;
const int GAUGE_Y_START = 40;
const int GAUGE_SPACING = 70;
void updateGauges(float t);
void drawGauge(int x, int y, uint16_t color, float value);
void drawGaugeDemoTwo(uint16_t  countLimit = 50);

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
			case 1: gaugeDemo(40); break;
			case 2: drawGaugeDemoTwo(100); break;
			case 3:
				std::cout << "Exiting menu\n";
				break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
				break;
		}
		std::cout << std::endl;

	} while (choice != 3);

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
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************

// ** USER OPTION 2 Screen SetupHWSPI **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	if(myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	return 0;
}



void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "End Demos" << std::endl;
}

void displayMenu() {
	std::cout << "Demos Menu:\n";
	std::cout << "1. Round Gauge\n";
	std::cout << "2. Vertical Gauges  \n";
	std::cout << "3. Quit\n";
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

// Demo 1

void gaugeDemo(uint16_t countLimit)
{
	std::cout << "Gauge Demo ends at : " << countLimit << std::endl;
	int16_t currentValue = 0;
	int16_t oldValue = 0;
	myTFT.setFont(font_mega);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x =100;
	uint8_t y = 100;
	uint8_t radius = 63;
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
		myTFT.setCursor(0,180);
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

// Demo 2

void drawGaugeDemoTwo(uint16_t countLimit)
{
	std::cout << "Gauge Demo 2  ends at : " << countLimit << std::endl;
	myTFT.setFont(font_mega);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	float t = 0;
	while (countLimit-- > 1 ) 
	{
		updateGauges(t);
		t += 0.1;
		std::cout<< countLimit << "\r" << std::flush;
		delayMilliSecRDL(100);
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

void updateGauges(float t) {
	// Calculate values
	float val1 = (std::sin(t) + 1) / 2;
	float val2 = (std::cos(t) + 1) / 2;
	float val3 = ((std::sin(t) * std::cos(t)) + 1) / 2;

	drawGauge(GAUGE_X_START, GAUGE_Y_START, myTFT.RDLC_RED, val1);
	drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, myTFT.RDLC_GREEN, val2);
	drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, myTFT.RDLC_BLUE, val3);

	char buffer[6]; // To store formatted text
	sprintf(buffer, "%.2f", val1);
	myTFT.fillRect(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10, 40, 10, myTFT.RDLC_BLACK); // Clear previous text
	myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10);
	myTFT.print(buffer);
	sprintf(buffer, "%.2f", val2);
	myTFT.fillRect(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 40, 10, myTFT.RDLC_BLACK);
	myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
	myTFT.print(buffer);
	sprintf(buffer, "%.2f", val3);
	myTFT.fillRect(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 40, 10,  myTFT.RDLC_BLACK);
	myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
	myTFT.print(buffer);
}
/// @endcond
