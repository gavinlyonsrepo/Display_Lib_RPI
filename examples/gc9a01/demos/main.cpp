/*!
	@file examples/gc9a01/demos/main.cpp
	@brief  Various demos see test list
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 440 Round Gauge
		-# test 441 Arc Meter
		-# test 442 Rotary Menu Dial (Keyboard Input)
		-# test 444 analog Clock Demo
		-# test 445 volume Knob Demo (Keyboard Input)
*/

// Section ::  libraries
#include <ctime>
#include <random> // gauge
#include <iostream> // cout cin
#include <algorithm> // For std::clamp
#include <limits> // menu limits
#include <csignal> //catch user Ctrl+C
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include "GC9A01_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
GC9A01_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0;
uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 240; // Screen height in pixels
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

// === Demo 1 & 2 ===
void gaugeDemo(uint16_t  countLimit = 50);  // demo1
void arcGauge(uint16_t count=100); // demo 2
void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color);
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor, uint16_t color);
void drawPointer(int16_t &val, int16_t &oldVal , uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor);
// === Demo 3 ===
void rotaryMenuDemo(void);
int getchar_timeout(int timeout_ms);
// === Demo 4 ===
void analogClockDemo(uint16_t countLimit = 50);
// === Demo 5 ===
void volumeKnobDemo(void);

// Section :: MAIN loop
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
			std::cout << "Invalid input. Please enter a number between 1 and 5.\n\n";
			continue;
		}
		switch (choice) {
			case 1: gaugeDemo(100); break;
			case 2: arcGauge(100); break;
			case 3: rotaryMenuDemo() ; break;
			case 4: analogClockDemo(50); break;
			case 5: volumeKnobDemo(); break;
			case 6:
				std::cout << "Exiting menu\n";
				break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
				break;
		}
		std::cout << std::endl;
	} while (choice != 6);

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
	myTFT.TFTInitScreenSize(TFT_WIDTH , TFT_HEIGHT);
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
	std::cout << "1. Round Gauge Demo\n";
	std::cout << "2. Arc Gauge Demo\n";
	std::cout << "3. Rotary Menu Demo\n";
	std::cout << "4. Analog Clock Demo\n";
	std::cout << "5. Volume Knob Demo\n";
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

// Terminate program on ctrl + C
void signal_callback_handler(int signum)
{
	EndTests();
	exit(signum);
}

// === Demo 1 & 2 ===

void gaugeDemo(uint16_t countLimit) // Demo 1
{
	std::cout << "Gauge Demo 1 ends at seconds: " << countLimit << std::endl;
	int16_t currentValue = 120;
	int16_t oldValue = 120;
	myTFT.setFont(font_mega);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x =120;
	uint8_t y = 120;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	uint8_t radius = 100;
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
		myTFT.setCursor(95,180);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
			oldValue = currentValue;
		}
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Gauge Demo Over " << std::endl;
}

void arcGauge(uint16_t countLimit) // demo 2
{
	myTFT.setTextCharPixelOrBuffer(true);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	std::cout << "Arc Demo 2 ends at seconds: " <<  countLimit << std::endl;
	int16_t currentValue = 150;
	int16_t oldValue = 150;
	myTFT.setFont(font_default);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint16_t x = 120;
	uint16_t y =120;
	uint16_t radius = 90;
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
	while(count++ < countLimit)
	{
		int step = stepDist(gen);
		currentValue += step;
		currentValue = std::clamp(currentValue, minValue, maxValue);
		myTFT.setCursor(60,160);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue,  x, y, radius, myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
			oldValue = currentValue;
		}
		myTFT.print(" Count :: ");
		myTFT.print(count);
		myTFT.setCursor(60,170);
		myTFT.print(" Value :: ");
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
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

// Demo 3
/*!
 * @brief Reads a single character from standard input with a timeout.
 * This function switches the terminal to non-canonical mode (no line buffering)
 * and disables echoing. It waits for a single character input for up to the
 * specified number of milliseconds. If a character is entered within the timeout,
 * it is returned; otherwise, the function returns -1.
 * @param timeout_ms Timeout duration in milliseconds to wait for input.
 * @return The character read from input, or -1 if the timeout expired without input.
 * @note This function is designed for use on POSIX systems (e.g., Linux).
 *       It temporarily modifies terminal attributes and restores them afterward.
 */
int getchar_timeout(int timeout_ms) {
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set);
	FD_SET(STDIN_FILENO, &set);
	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_usec = (timeout_ms % 1000) * 1000;
	int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
	int ch = -1;
	if (rv > 0) {
		ch = getchar();
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

void rotaryMenuDemo()
{
	std::cout << "Demo 3 Start: press a or d to move pointer , q to quit" << std::endl;
	myTFT.setFont(font_default);
	constexpr uint8_t centerX = 120;
	constexpr uint8_t centerY = 120;
	constexpr uint8_t radius = 60;
	constexpr uint8_t itemCount = 6;
	constexpr uint8_t nodeRadius = 16;
	struct MenuItem { const char* label; };
	MenuItem items[itemCount] = {
		{"Volts"}, {"Power"}, {"Amps"},
		{"Watts"}, {"System"}, {"Diode"}
	};
	// Lambda to draw each node
	auto drawItem = [&](uint8_t index, bool selected) {
		float angle = 2.0f * std::numbers::pi * index / itemCount;
		int x = centerX + static_cast<int>(radius * std::cos(angle));
		int y = centerY + static_cast<int>(radius * std::sin(angle));
		uint16_t color = selected ? myTFT.RDLC_RED : myTFT.RDLC_WHITE;
		myTFT.fillCircle(x, y, nodeRadius, color);
		myTFT.setTextColor(myTFT.RDLC_BLACK, color);
		char c[2] = { items[index].label[0], '\0' };
		myTFT.writeCharString(x - 3, y - 4, c);
	};
	// Lambda to draw pointer from center to selected node
	auto drawPointer = [&](uint8_t index, uint16_t color) {
		float angle = 2.0f * std::numbers::pi * index / itemCount;
		constexpr uint8_t pointerLength = radius - 10;
		int x1 = centerX + static_cast<int>(pointerLength * std::cos(angle));
		int y1 = centerY + static_cast<int>(pointerLength * std::sin(angle));
		myTFT.drawLine(centerX, centerY, x1, y1, color);
		myTFT.fillCircle(centerX, centerY, 2, myTFT.RDLC_GREEN);
	};
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.drawArc(centerX, centerY, radius + 40, 10, 135.0f, 45.0f, myTFT.RDLC_GREEN);
	// Draw all menu nodes, initially selecting the first
	for (uint8_t i = 0; i < itemCount; ++i){
		drawItem(i, i == 0);}
	uint8_t selected = 0;
	int8_t oldSelected = -1;
	// Draw pointer to initial selection
	drawPointer(selected, myTFT.RDLC_YELLOW);
	// inline label drawing
	myTFT.setFont(font_mega);
	myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
	myTFT.fillRectangle(centerX - 30, centerY + radius + 35, 80, 16, myTFT.RDLC_YELLOW);
	char buf[20];
	std::strncpy(buf, items[selected].label, sizeof(buf));
	buf[sizeof(buf) - 1] = '\0';
	myTFT.writeCharString(centerX - 50, centerY + radius + 35, buf);
	// --- Main input loop ---
	while (true) {
		int ch = getchar_timeout(1000);
		if (ch == 'q' || ch == 'Q') break;
		oldSelected = selected;
		if (ch == 'a' || ch == 'A')
			selected = (selected + itemCount - 1) % itemCount;
		if (ch == 'd' || ch == 'D')
			selected = (selected + 1) % itemCount;
		if (selected != oldSelected)
		{
			// Erase old pointer and node
			drawPointer(oldSelected, myTFT.RDLC_BLACK);
			drawItem(oldSelected, false);
			// Draw new pointer and node
			drawItem(selected, true);
			drawointer(selected, myTFT.RDLC_YELLOW);
			// Draw updated label below the new selection
			myTFT.setFont(font_mega);
			myTFT.setTextColor(myTFT.RDLC_RED, myTFT.RDLC_YELLOW);
			myTFT.fillRectangle(centerX - 30, centerY + radius + 35, 80, 16, myTFT.RDLC_YELLOW);
			std::strncpy(buf, items[selected].label, sizeof(buf));
			buf[sizeof(buf) - 1] = '\0';
			myTFT.writeCharString(centerX - 50, centerY + radius + 35, buf);
			myTFT.setFont(font_default);
		}
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Demo 3 Over" << std::endl;
}

// Demo 4
void analogClockDemo(uint16_t countLimit)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Analog Clock Demo 5 Start, ends in seconds : "<< countLimit << std::endl;
	// Determine the dimensions of the clock
	int radius = (TFT_HEIGHT - 10) / 2;
	int centerX = TFT_WIDTH / 2;
	int centerY = (TFT_HEIGHT / 2);
	// Draw Clock
	drawGaugeMarkers(centerX, centerY , radius, 0.0f, 360.0f, 1.3f, myTFT.RDLC_WHITE);
	drawGaugeMarkers(centerX, centerY , radius, 15.0f, 360.0f, 1.1f, myTFT.RDLC_TAN);
	myTFT.drawArc(centerX, centerY , radius, 5,  0.0f, 360.0f, myTFT.RDLC_BROWN);
	myTFT.fillCircle(centerX, centerY, 5, myTFT.RDLC_CYAN);
	// Start the clock
	uint16_t xSecond = centerX, ySecond = centerY;
	uint16_t xMinute = centerX, yMinute = centerY;
	uint16_t xHour = centerX, yHour = centerY;
	float angle;
	int lastSecond = -1; // Initialize to a value that is not valid for comparison
	while(countLimit >1)
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
			angle = 2 * std::numbers::pi/ 12 * hour;
			xHour = (uint16_t) (centerX + (radius / 2) * sinf(angle));
			yHour = (uint16_t) (centerY - (radius / 2) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xHour, yHour, myTFT.RDLC_RED);
			// Draw the minute hand
			myTFT.drawLine(centerX, centerY, xMinute, yMinute, myTFT.RDLC_BLACK);
			angle = 2 * std::numbers::pi/ 60 * minute;
			xMinute = (uint16_t) (centerX + (2 * radius / 3) * sinf(angle));
			yMinute = (uint16_t) (centerY - (2 * radius / 3) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xMinute, yMinute, myTFT.RDLC_GREEN);
			// Draw the second hand
			myTFT.drawLine(centerX, centerY, xSecond, ySecond, myTFT.RDLC_BLACK);
			angle = 2 * std::numbers::pi/ 60 * second;
			xSecond = (uint16_t) (centerX + (radius - 30) * sinf(angle));
			ySecond = (uint16_t) (centerY - (radius - 30) * cosf(angle));
			myTFT.drawLine(centerX, centerY, xSecond, ySecond, myTFT.RDLC_YELLOW);
			lastSecond = second;
			countLimit--;
		}
		delayMilliSecRDL(10);
		std::cout<< utcTime << "\r" << std::flush;
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Clock Demo over : " <<std::endl;
}

// Demo 5
void volumeKnobDemo(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	std::cout << "Volume Knob Demo Start. Use 'a' / 'd' to change, 'q' to quit.\n";
	const uint16_t centerX = TFT_WIDTH / 2;
	const uint16_t centerY = TFT_HEIGHT / 2;
	const uint16_t radius = (TFT_HEIGHT - 30) / 2;
	const uint16_t arcThickness = 10;
	uint8_t value = 1;
	int8_t lastValue = -1;
	bool running = true;
	while (running)
	{
		char c = getchar_timeout(1000);
		switch (c)
		{
			case 'a':
			case 'A':
				if (value > 0) value--;
				break;
			case 'd':
			case 'D':
				if (value < 99) value++;
				break;
			case 'q':
			case 'Q':
				running = false;
				break;
			default:
				break;
		}
		if (value != lastValue)
		{
		float startAngle = 135.0f;
		float oldEndAngle = startAngle + (270.0f * lastValue / 99.0f);
		float newEndAngle = startAngle + (270.0f * value / 99.0f);
		if (value > lastValue)
		{// Draw only the added arc segment
			uint8_t colorIndex = static_cast<uint8_t>(value * 127 / 99);
			uint16_t arcColor = rdlib_maths::generateColor(colorIndex);
			myTFT.drawArc(centerX, centerY, radius, arcThickness, oldEndAngle, newEndAngle, arcColor);
		}
		else if (value < lastValue)
		{// Clear only the excess arc segment
			myTFT.drawArc(centerX, centerY, radius, arcThickness, newEndAngle, oldEndAngle, myTFT.RDLC_BLACK);
		}
			char buf[4];
			snprintf(buf, sizeof(buf), "%02d", value);
			myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
			myTFT.setFont(font_sevenSeg);
			myTFT.setCursor(centerX - 24, centerY - 16);
			myTFT.print(buf);
			std::cout << "\rValue: " << (int)value << "   " << std::flush;
			lastValue = value;
		}
	}
	std::cout << "\nVolume Knob Demo End\n";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

/// @endcond
