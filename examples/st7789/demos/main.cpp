/*!
	@file examples/st7789/demos/main.cpp
	@brief  Various demos see test list
	@note See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 431 Round Gauge random value
		-# test 432 Vertical Gauge Based on sin(x), cos(x), & sin(x)*cos(x). Updates over time to create a dynamic effect.
		-# test 433 Menu Demo
		-# test 434 Slider and buttons demo
		-# test 435 Demo blobs animation 
*/

// Section ::  libraries
#include <ctime>
#include <random> // gauge
#include <iostream> // cout cin
#include <algorithm> // For std::clamp
#include <limits> // menu limits
#include <csignal> //catch user Ctrl+C
#include <termios.h> //key press
#include <fcntl.h> //key press
#include <numbers> // for std::numbers::pi_v
#include "ST7789_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
ST7789_TFT myTFT;
int8_t RST_TFT  = 25;
int8_t DC_TFT   = 24;
int  GPIO_CHIP_DEVICE = 0;

uint8_t OFFSET_COL = 0;  // These offsets can be adjusted for any issues
uint8_t OFFSET_ROW = 0; // with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 240; // Screen width in pixels
uint16_t TFT_HEIGHT = 280; // Screen height in pixels

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

// === Demo 1 ===
void gaugeDemo(uint16_t  countLimit = 50);
void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color);
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor);
void drawPointer(int16_t &val, int16_t &oldVal , uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor);

// === Demo 2 ===
// Gauge properties
const int GAUGE_WIDTH = 20;
const int GAUGE_HEIGHT = 140;
const int GAUGE_X_START = 20;
const int GAUGE_Y_START = 40;
const int GAUGE_SPACING = 70;
// Store previous values to prevent redundant redraws
float prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
void updateGauges(float phase);
void drawGauge(int x, int y, uint16_t color, float value);
void drawGaugeDemoTwo(uint16_t  countLimit = 50);
void drawGaugeBorder(int x, int y);

// === Demo 3 ===
constexpr uint16_t SCREEN_WIDTH = 240;
constexpr uint16_t SCREEN_HEIGHT = 280;
constexpr uint8_t FONT_HEIGHT = 20;
constexpr uint16_t TEXT_COLOR = ST7789_TFT::RDLC_WHITE;
constexpr uint16_t BG_COLOR = ST7789_TFT::RDLC_BLACK;
constexpr uint16_t HIGHLIGHT_COLOR = ST7789_TFT::RDLC_YELLOW;
constexpr size_t VISIBLE_ITEMS = SCREEN_HEIGHT / FONT_HEIGHT;

void menuDemo(void);
char getKeyPress(); // also used by demo 4
void drawMenu(
	const std::array<std::string, 6>& items,
	size_t selectedIndex,
	size_t topIndex);
void showSelection(const std::string& item);

// === Demo 4 ===
constexpr uint16_t BUTTON_COLOR = ST7789_TFT::RDLC_CYAN;
constexpr uint16_t SLIDER_BG_COLOR = ST7789_TFT::RDLC_DGREY;
constexpr int SLIDER_MIN = 5;
constexpr int SLIDER_MAX = 100;
constexpr int SLIDER_STEP = 5;

void slidersAndButtonsDemo(void);
void drawUI(int sliderValue);
void drawButtons();
void drawSlider(int sliderValue);
void drawValueLabel(int value);

// === Demo 5 ===
// Constants
// Total number of 3D points ("dots") in the animation
constexpr int NDOTS = 256;
// Lookup table size for sine/cosine waveforms
constexpr int SCALE = 4096;
// Variables
// Precomputed sine wave lookup table; cosi points to sine phase-shifted by π/2
int16_t sine[SCALE + (SCALE / 4)];
int16_t* cosi = &sine[SCALE / 4];
// 3D coordinates of each dot: xyz[axis][dotIndex]
int16_t xyz[3][NDOTS];
// Per-dot color values (16-bit RGB565 or library-dependent)
uint16_t col[NDOTS];
// Function Declarations
void runBlobDemo(uint16_t count);
void matrix(int16_t xyz[3][NDOTS], uint16_t col[NDOTS]);
void rotate(int16_t xyz[3][NDOTS], uint16_t , uint16_t , uint16_t );
void draw(int16_t xyz[3][NDOTS], uint16_t col[NDOTS]);


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
			case 1: gaugeDemo(60); break;
			case 2: drawGaugeDemoTwo(100); break;
			case 3: menuDemo(); break;
			case 4: slidersAndButtonsDemo(); break;
			case 5: runBlobDemo(300); break;
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
	std::cout << "3. Menu Graphic input\n";
	std::cout << "4. Sliders and Buttons\n";
	std::cout << "5. Blob Demo\n";
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

// === Demo 1 ===

void gaugeDemo(uint16_t countLimit)
{
	std::cout << "Gauge Demo ends at : " << countLimit << std::endl;
	int16_t currentValue = 100;
	int16_t oldValue = 100;
	myTFT.setFont(font_mega);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x = 100;
	uint8_t y = 100;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	uint8_t radius = 63;
	myTFT.drawCircle(x, y, radius, myTFT.RDLC_RED);
	drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
	drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
	// Random number setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes

	while(count++ < countLimit)
	{
		int step = stepDist(gen);
		currentValue += step;
		currentValue = std::clamp(currentValue, minValue, maxValue);
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

// === Demo 2 ===

void drawGaugeBorder(int x, int y) { // Draw border once 
	myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.RDLC_WHITE);
}

void drawGaugeDemoTwo(uint16_t countLimit)
{
	std::cout << "Gauge Demo 2  ends at : " << countLimit << std::endl;
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
	// Draw borders once
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	drawGaugeBorder(GAUGE_X_START, GAUGE_Y_START);
	drawGaugeBorder(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START);
	drawGaugeBorder(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START);
	float phase = 0;
	while (countLimit-- > 1 ) 
	{
		updateGauges(phase);
		phase += 0.1;
		std::cout<< countLimit << "\r" << std::flush;
		delayMilliSecRDL(250);
	}
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
	std::cout << "Gauge Demo 2 Over " << std::endl;
}

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

	myTFT.setTextColor(myTFT.RDLC_YELLOW, myTFT.RDLC_BLACK);
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

// === Demo 3 ===
void menuDemo(void)
{
const std::array<std::string, 6> menuItems = {
		"Start Measurement",
		"Settings",
		"View Logs",
		"About",
		"Shutdown",
		"Diagnostics"
	};

	size_t selectedIndex = 0;
	size_t topIndex = 0;
	drawMenu(menuItems, selectedIndex, topIndex);
	std::cout << "Menu demo 3. W up, S Down, enter select, Q quit\n";
	while (true) {
		char key = getKeyPress();
		if (key == 'w') {
			if (selectedIndex > 0) {
				--selectedIndex;
				if (selectedIndex < topIndex) {
					--topIndex;
				}
				drawMenu(menuItems, selectedIndex, topIndex);
			}
		} else if (key == 's') {
			if (selectedIndex + 1 < menuItems.size()) {
				++selectedIndex;
				if (selectedIndex >= topIndex + VISIBLE_ITEMS) {
					++topIndex;
				}
				drawMenu(menuItems, selectedIndex, topIndex);
			}
		} else if (key == '\n' || key == '\r') {
			showSelection(menuItems[selectedIndex]);
			drawMenu(menuItems, selectedIndex, topIndex);
		} else if (key == 'q' || key == 'e'){
			break;
		}
		usleep(200000); // debounce
	}

	std::cout << "Exiting menu demo 3.\n";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}
//  (non-blocking, instant key capture)
char getKeyPress() {
	struct termios oldt{}, newt{};
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	char ch = 0;
	if (read(STDIN_FILENO, &ch, 1) < 0) {
		ch = 0;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, 0);
	return ch;
}

void drawMenu(
	const std::array<std::string, 6>& items,
	size_t selectedIndex,
	size_t topIndex
) {
	myTFT.fillScreen(BG_COLOR);
	myTFT.setTextColor(TEXT_COLOR, BG_COLOR);
	myTFT.setTextWrap(false);
	myTFT.setFont(font_retro);
	myTFT.setCursor(10, 0);
	myTFT.print("Main Menu");

	for (size_t i = 0; i < VISIBLE_ITEMS; ++i) {
		size_t itemIdx = topIndex + i;
		if (itemIdx >= items.size()) break;

		uint16_t y = (i + 1) * FONT_HEIGHT;
		bool isSelected = (itemIdx == selectedIndex);
		uint16_t color = isSelected ? HIGHLIGHT_COLOR : TEXT_COLOR;

		myTFT.setCursor(10, y);
		myTFT.setTextColor(color, BG_COLOR);
		myTFT.print((isSelected ? "> " : "  ") + items[itemIdx]);
	}
}

void showSelection(const std::string& item) {
	myTFT.fillScreen(BG_COLOR);
	myTFT.setCursor(10, 100);
	myTFT.setTextColor(TEXT_COLOR, BG_COLOR);
	myTFT.setTextWrap(true);
	myTFT.setFont(font_retro);
	myTFT.print("Selected:\n" + item);
	sleep(1);
}

// === Demo 4 ===

void slidersAndButtonsDemo (void)
{
	std::cout << "Slider demo 4. a - , d +, q quit\n";
	int sliderValue = 50;
	drawUI(sliderValue);
	while (true)
	{
		char key = getKeyPress();
		if (key == 'a') {
			if (sliderValue > SLIDER_MIN) {
				sliderValue -= SLIDER_STEP;
				if (sliderValue < SLIDER_MIN) sliderValue = SLIDER_MIN;
				drawSlider(sliderValue);
				drawValueLabel(sliderValue);
			}
		} else if (key == 'd') {
			if (sliderValue < SLIDER_MAX) {
				sliderValue += SLIDER_STEP;
				if (sliderValue > SLIDER_MAX) sliderValue = SLIDER_MAX;
				drawSlider(sliderValue);
				drawValueLabel(sliderValue);
			}
		} else if (key == '\n' || key == '\r') {
			myTFT.setCursor(40, 250);
			myTFT.setTextColor(ST7789_TFT::RDLC_YELLOW, BG_COLOR);
			myTFT.print("Pressed ENTER");
		} else if (key == 'q' || key == 'e') {
			break;
		}
	usleep(200000); // debounce
	}
	std::cout << "Exiting menu demo 4.\n";
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}

void drawButtons() {
	myTFT.fillRoundRect(30, 200, 45, 45, 10, BUTTON_COLOR);	// "-"
	myTFT.fillRoundRect(170, 200, 45, 45,10, BUTTON_COLOR);	// "+"
	myTFT.setTextColor(ST7789_TFT::RDLC_BLACK, BUTTON_COLOR);
	myTFT.setFont(font_arialRound);
	myTFT.setCursor(45, 210);
	myTFT.print("-");
	myTFT.setCursor(185, 210);
	myTFT.print("+");
}

void drawSlider (int sliderValue) {
	const int x = 30;
	const int y = 150;
	const int w = 180;
	const int h = 20;
	uint8_t mappedVal1 = rdlib_maths::mapValue(sliderValue, 0, 100, 1, 127);
	uint16_t color1 = rdlib_maths::generateColor(mappedVal1);
	// Clear slider area
	myTFT.fillRectangle(x, y, w, h, SLIDER_BG_COLOR);
	// Only draw fill if value is above minimum
	if (sliderValue > SLIDER_MIN) {
		int fillWidth = (sliderValue - SLIDER_MIN) * w / (SLIDER_MAX - SLIDER_MIN);
		if (fillWidth < 1) fillWidth = 1; // Ensure minimum width

		myTFT.fillRectangle(x, y, fillWidth, h, color1);
	}
	// Always draw border
	myTFT.drawRectWH(x, y, w, h, ST7789_TFT::RDLC_WHITE);
}

void drawValueLabel(int value) {
	myTFT.fillRect(116, 100, 60, 20, BG_COLOR);
	myTFT.setCursor(116, 100);
	myTFT.setTextColor(TEXT_COLOR, BG_COLOR);
	myTFT.setFont(font_mega);
	myTFT.print(std::to_string(value));
}

void drawUI(int sliderValue) {
	myTFT.fillScreen(BG_COLOR);
	myTFT.setTextColor(TEXT_COLOR, BG_COLOR);
	myTFT.setFont(font_mega);
	myTFT.setCursor(10, 10);
	myTFT.print("Control Panel");
	myTFT.setCursor(20, 100);
	myTFT.print("Value:");
	drawSlider(sliderValue);
	drawValueLabel(sliderValue);
	drawButtons();
}

// === Demo 5 ===

/**
 * @brief Runs the 3D blob demo animation.
 *
 * Generates a rotating, sine-wave-based 3D "blob" pattern projected onto
 * the TFT display. The function animates color, depth, and rotation over time.
 *
 * @param countLimit Number of frames (iterations) to render before stopping.
 */
void runBlobDemo(uint16_t countLimit)
{
	std::cout << "Blob demo Start\n";
	std::cout << "Count limit: " << countLimit << std::endl;
	uint16_t count = 0;
	// Rotation angles and speeds for X, Y, Z axes.
	int16_t angleX = 0, angleY = 0, angleZ = 0;
	int16_t speedX = 0, speedY = 0, speedZ = 0;
	// 2π constant (used for sine table generation)
	const double TWO_PI = std::numbers::pi_v<double> * 2.0;
	// Precompute sine lookup table for faster animation updates.
	for (uint16_t i = 0; i < SCALE + (SCALE / 4); i++){
		sine[i] = static_cast<int16_t>(sinf(TWO_PI * i / SCALE) * SCALE);
	}
	// Main animation loop
	while (count++ < countLimit)
	{
		// Display progress in console
		std::cout << count << "\r" << std::flush;
		// Step 1: Generate 3D point cloud and color map for this frame
		matrix(xyz, col);
		// Step 2: Apply 3D rotation transformations
		rotate(xyz, angleX, angleY, angleZ);
		// Step 3: Render updated points to display
		draw(xyz, col);
		// Step 4: Gradually enable rotation on each axis
		if (count > 75)	speedY = 1;
		if (count > 150)	speedX = 1;
		if (count > 250)	speedZ = 1;
		// Step 5: Increment rotation angles
		angleX += speedX;
		angleY += speedY;
		angleZ += speedZ;
		// Step 6: Wrap angles to stay within valid range [0, SCALE)
		auto wrapAngle = [](int16_t &angle) {
			if (angle >= SCALE) angle -= SCALE;
			else if (angle < 0) angle += SCALE;
		};
		wrapAngle(angleX);
		wrapAngle(angleY);
		wrapAngle(angleZ);
	}
	std::cout << "Blob demo End\n";

	myTFT.fillScreen(myTFT.RDLC_BLACK);
}


/*!
 * @brief Generate a dynamic 3D sine-wave surface ("blob") and color map.
 * This function computes the 3D positions (xyz) and corresponding
 * RGB565 color values (col) for each point in the NDOTS grid.
 * It uses time-based sine modulation to animate the surface over time.
 * @param xyz  Output array of 3D coordinates [3][NDOTS]
 * @param col  Output color buffer (16-bit RGB565 values)
 */
void matrix(int16_t xyz[3][NDOTS], uint16_t col[NDOTS])
{
	// Color component resolution (used to map sine values to color channels)
	constexpr int RED_COLORS   = 32;
	constexpr int GREEN_COLORS = 64;
	constexpr int BLUE_COLORS  = 32;
	// Step increment between points in the virtual XY grid
	constexpr int INCREMENT = 512;
	// Time accumulator for animation progression
	static uint32_t timeAccumulator = 0;
	int16_t x = -SCALE;
	int16_t y = -SCALE;

	for (uint16_t i = 0; i < NDOTS; i++)
	{
		// Assign current X,Y coordinates to point i
		xyz[0][i] = x;
		xyz[1][i] = y;
		// Compute radial distance from origin (for circular wave propagation)
		uint16_t d = static_cast<uint16_t>(sqrt(x * x + y * y));
		// Phase shift based on time accumulator
		uint16_t s = sine[(timeAccumulator * 30) % SCALE] + SCALE;
		// Compute Z coordinate using time- and distance-modulated sine waves
		xyz[2][i] = (sine[(d + s) % SCALE] * sine[(timeAccumulator * 10) % SCALE]) / (SCALE * 2);
		// Compute RGB color based on Z position (phase-shifted cosine functions)
		uint8_t red =  (cosi[xyz[2][i] + SCALE / 2] % SCALE + SCALE) 
		               * (RED_COLORS - 1) / SCALE / 2;
		uint8_t grn =  (cosi[(xyz[2][i] + SCALE / 2 + 2 * SCALE / 3) % SCALE] + SCALE)
		               * (GREEN_COLORS - 1) / SCALE / 2;
		uint8_t blu =  (cosi[(xyz[2][i] + SCALE / 2 + SCALE / 3) % SCALE] + SCALE)
		               * (BLUE_COLORS - 1) / SCALE / 2;
		// Pack into 16-bit RGB565 format
		col[i] = static_cast<uint16_t>((red << 11) | (grn << 5) | blu);
		// Advance to next grid position
		x += INCREMENT;
		if (x >= SCALE)
		{
			x = -SCALE;
			y += INCREMENT;
		}
	}
	timeAccumulator++;
}


/*!
 * @brief Apply 3D rotation to a set of points around the X, Y, and Z axes.
 * Each point in the xyz array is rotated in-place using precomputed
 * sine and cosine lookup tables.
 * @param xyz     3D coordinates of all points (axis-major layout: xyz[3][NDOTS])
 * @param angleX  Rotation angle index around X-axis (0–SCALE range)
 * @param angleY  Rotation angle index around Y-axis (0–SCALE range)
 * @param angleZ  Rotation angle index around Z-axis (0–SCALE range)
 */
void rotate(int16_t xyz[3][NDOTS], uint16_t angleX, uint16_t angleY, uint16_t angleZ)
{
	// Precompute sine/cosine values for each axis from lookup tables
	const int16_t sinx = sine[angleX];
	const int16_t cosx = cosi[angleX];
	const int16_t siny = sine[angleY];
	const int16_t cosy = cosi[angleY];
	const int16_t sinz = sine[angleZ];
	const int16_t cosz = cosi[angleZ];
	// Temporary storage for intermediate values during rotation
	int16_t tmpX, tmpY;
	for (uint16_t i = 0; i < NDOTS; i++)
	{
		// X-axis 
		tmpX      = (xyz[0][i] * cosx - xyz[2][i] * sinx) / SCALE;
		xyz[2][i] = (xyz[0][i] * sinx + xyz[2][i] * cosx) / SCALE;
		xyz[0][i] = tmpX;
		// Y-axis 
		tmpY      = (xyz[1][i] * cosy - xyz[2][i] * siny) / SCALE;
		xyz[2][i] = (xyz[1][i] * siny + xyz[2][i] * cosy) / SCALE;
		xyz[1][i] = tmpY;
		// Z-axis 
		tmpX      = (xyz[0][i] * cosz - xyz[1][i] * sinz) / SCALE;
		xyz[1][i] = (xyz[0][i] * sinz + xyz[1][i] * cosz) / SCALE;
		xyz[0][i] = tmpX;
	}
}

/*!
 * @brief Render a frame of the rotating blobs demo.
 * This function projects 3D coordinates (xyz) into 2D screen space,
 * draws each point as a small circle, and erases its previous position.
 * A simple perspective division is used based on the Z coordinate.
 * @param xyz  3D coordinates of all points (axis-major layout: xyz[3][NDOTS])
 * @param col  Per-dot color values (RGB565 or driver-specific format)
 */
void draw(int16_t xyz[3][NDOTS], uint16_t col[NDOTS])
{
	// Screen dimensions for projection (should match your display area)
	constexpr uint16_t WIDTH  = 140;
	constexpr uint16_t HEIGHT = 140;
	// Persistent arrays store previous positions and sizes for erasing old dots
	static uint16_t oldProjX[NDOTS] = {0};
	static uint16_t oldProjY[NDOTS] = {0};
	static uint8_t  oldDotSize[NDOTS] = {0};
	for (uint16_t i = 0; i < NDOTS; i++)
	{
		// Perspective scaling based on Z-depth (simple camera model)
		uint16_t projZ = SCALE - (xyz[2][i] + SCALE) / 4;
		// Project 3D coordinates into 2D screen space
		uint16_t projX = WIDTH  / 2 + (xyz[0][i] * projZ / SCALE) / 25;
		uint16_t projY = HEIGHT / 2 + (xyz[1][i] * projZ / SCALE) / 25;
		// Compute apparent dot size (smaller when farther away)
		uint8_t dotSize = 3 - (xyz[2][i] + SCALE) * 2 / SCALE;
		// Erase previous dot position (draw in black)
		myTFT.drawCircle(oldProjX[i], oldProjY[i], oldDotSize[i], myTFT.RDLC_BLACK);
		// Only draw if fully within visible bounds
		if (projX > dotSize && projY > dotSize && projX < WIDTH - dotSize && projY < HEIGHT - dotSize)
		{
			myTFT.drawCircle(projX, projY, dotSize, col[i]);
			oldProjX[i]  = projX;
			oldProjY[i]  = projY;
			oldDotSize[i] = dotSize;
		}
	}
}

/// @endcond
