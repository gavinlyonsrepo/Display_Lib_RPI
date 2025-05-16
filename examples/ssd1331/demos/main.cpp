/*! 
	@file examples/ssd1331/demos/main.cpp
	@brief demos for ssd1331 OLED
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function and in myOLED constructor
	@test 
		-# Test 408 Analog clock demo
		-# Test 409 Digital Clock demo
		-# Test 410 Simulated VU meter demo 1
		-# Test 411 Simulated VU meter demo 2
		-# Test 412 Signal Generator 
		-# Test 414 Radar
*/

// Section ::  libraries
#include <iostream>
#include <ctime>
#include <csignal> //catch user Ctrl+C
#include <random>
#include <algorithm> // For std::clamp

#include "Bitmap_test_data.hpp" // Data for tests 
#include "SSD1331_OLED_RDL.hpp"

/// @cond

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);
uint16_t OLED_WIDTH = 96;// Screen width in pixels
uint16_t OLED_HEIGHT = 64; // Screen height in pixels

//  Section ::  Function Headers 
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
void displayMenu(void);
std::string UTC_string(void);
void signal_callback_handler(int signum);
void EndTests(void);

//demo 1
void ClockDemo(uint16_t secondsDisplay);

//demo 2
void updateLevels(void);
uint16_t getColorForLevel(int level);
void drawVuMeter();
void VUmeter(uint16_t secondsDisplay);
// Simulated VU levels
struct VuBar_t{
	int level;    // Current level (0-100%)
	int peak;     // Peak hold position
} ;
#define _NUM_BARS      10  // Number of VU bars
std::vector<VuBar_t> bars(_NUM_BARS);
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(10, 99);

//demo 3 
void VUmeterGradient(uint16_t secondsDisplay);
void drawVerticalVU(uint8_t x, uint8_t y, uint8_t w, uint8_t val, uint16_t color);
#define _BARS       7
#define _BARWIDTH   10
#define _BARSPACE    2

//demo 4 
void DigitalClock(uint16_t count);

//demo 5
void SignalGen(uint16_t secondsDisplay);
#define _WAVE_AMPLITUDE  20  // Max height of the waveform
#define _WAVE_FREQUENCY  3   // Number of cycles per screen width
#define _SWEEP_SPEED     4   // Pixels per update
int sweepPos = 0;

// Demo 6
void demoRadar(uint16_t sweeps=7);

//  Section ::  MAIN loop
int main() 
{
	if(SetupHWSPI() != 0) return -1; 
	myOLED.fillScreen(myOLED.RDLC_BLACK);
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
			case 1: ClockDemo(30); break;
			case 2: DigitalClock(2000); break;
			case 3: VUmeter(75); break;
			case 4: VUmeterGradient(100); break;
			case 5: SignalGen(100);break;
			case 6: demoRadar(5); break;
			case 7: std::cout << "Exiting menu\n"; break;
			default:
				std::cout << "Invalid choice. Please try again.\n";
				break;
		}
		std::cout << std::endl;
	} while (choice != 7);
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	std::cout << "Demos, Ctrl +c to quit" << std::endl;
	int8_t RST_OLED  = 25; // GPIO reset pin
	int8_t DC_OLED   = 24; // GPIO DC pin
	int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0
	int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED =  12000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
// ** USER OPTION 1 GPIO  **
	myOLED.OLEDSetupGPIO(RST_OLED, DC_OLED);
//*******************************************
// ** USER OPTION 2 Screen Setup**
	myOLED.OLEDInitScreenSize(OLED_WIDTH , OLED_HEIGHT);
// ***********************************
// ** USER OPTION 3 SPI settings **
	if(myOLED.OLEDInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(100);
	return 0;
}


// demo 1 clock
void ClockDemo(uint16_t secondsDisplay) 
{
	std::cout << "Clock Demo ends in : " << secondsDisplay << std::endl;
	// Determine the dimensions of the clock
	int radius = (OLED_HEIGHT - 10) / 2;
	int centerX = OLED_WIDTH / 2;
	int centerY = (OLED_HEIGHT / 2);
	// Draw Clock
	myOLED.fillScreen(myOLED.RDLC_DGREY);
	myOLED.fillCircle(centerX, centerY, radius, myOLED.RDLC_BLACK);
	// Start the clock
	uint16_t xSecond = centerX, ySecond = centerY;
	uint16_t xMinute = centerX, yMinute = centerY;
	uint16_t xHour = centerX, yHour = centerY;
	float angle;
	int lastSecond = -1; // Initialize to a value that is not valid for comparison

	while(secondsDisplay >1) 
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
			myOLED.drawLine(centerX, centerY, xHour, yHour, myOLED.RDLC_BLACK);
			angle = 2 * M_PI / 12 * hour;
			xHour = (uint16_t) (centerX + (radius / 2) * sinf(angle));
			yHour = (uint16_t) (centerY - (radius / 2) * cosf(angle));
			myOLED.drawLine(centerX, centerY, xHour, yHour, myOLED.RDLC_RED);
			// Draw the minute hand
			myOLED.drawLine(centerX, centerY, xMinute, yMinute, myOLED.RDLC_BLACK);
			angle = 2 * M_PI / 60 * minute;
			xMinute = (uint16_t) (centerX + (2 * radius / 3) * sinf(angle));
			yMinute = (uint16_t) (centerY - (2 * radius / 3) * cosf(angle));
			myOLED.drawLine(centerX, centerY, xMinute, yMinute, myOLED.RDLC_GREEN);
			// Draw the second hand
			myOLED.drawLine(centerX, centerY, xSecond, ySecond, myOLED.RDLC_BLACK);
			angle = 2 * M_PI / 60 * second;
			xSecond = (uint16_t) (centerX + (radius - 2) * sinf(angle));
			ySecond = (uint16_t) (centerY - (radius - 2) * cosf(angle));
			myOLED.drawLine(centerX, centerY, xSecond, ySecond, myOLED.RDLC_YELLOW);
			lastSecond = second;
			secondsDisplay--;
		}
		delayMilliSecRDL(10);
		std::cout<< utcTime << "\r" << std::flush;
	}
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	std::cout << "Clock Demo over : " <<std::endl;
}

// demo 2 

void VUmeter(uint16_t secondsDisplay)
{
	std::cout << "VU meter Demo start, ends in : " << secondsDisplay << std::endl;
	while (secondsDisplay >1)
	{
		updateLevels();
		drawVuMeter();
		delayMilliSecRDL(500); // Smooth animation
		secondsDisplay--;
		std::cout<< secondsDisplay << "\r" << std::flush;
	}; 
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	std::cout << "VU meter Demo over : " << std::endl;
}

void updateLevels() {
	for (auto &bar : bars) 
	{
		// Simulate random smooth changes
		int change = (dist(gen) % 5) - 2;
		bar.level = std::clamp(bar.level + change, 10, 100);
		bar.peak = std::max(bar.peak - 1, bar.level); // Peak hold decay
	}
}

uint16_t getColorForLevel(int level) {
	return rdlib_maths::generateColor(rdlib_maths::mapValue(level, 0, 100, 0, 127));
}

void drawVuMeter() {
	const int barWidth = OLED_WIDTH / _NUM_BARS;
	const int maxHeight = OLED_HEIGHT - 5;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	for (int i = 0; i < _NUM_BARS; i++) 
	{
		int height = (bars[i].level * maxHeight) / 100;
		int peakY = OLED_HEIGHT - ((bars[i].peak * maxHeight) / 100);
		uint16_t color = getColorForLevel(bars[i].level);
		myOLED.fillRectangle(i * barWidth, OLED_HEIGHT - height, barWidth - 2, height, color);
		myOLED.drawPixel(i * barWidth + barWidth / 2, peakY, myOLED.RDLC_WHITE); // Peak indicator
	}
}

// demo 3 
void VUmeterGradient(uint16_t secondsDisplay){
	std::cout << "VU meter Demo 2 start, ends in : " << secondsDisplay << std::endl;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(1, 254);
	while (secondsDisplay-- > 1)
		{
		for (uint8_t i=0;i<_BARS;i++)
		{
			drawVerticalVU(_BARWIDTH*i+_BARSPACE*i,_BARS,_BARWIDTH, dist(gen),0);
		}
		delayMilliSecRDL(250);
		std::cout<< secondsDisplay << "\r" << std::flush;
	}
	std::cout << "VU meter Demo 2 over : " << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
}

// For each bar, the random value value is used to map a color using generateColor.
// This will create a gradient color between blue and red based on the value.
// The color is then applied to each bar, giving the VU meter a dynamic 
// visual effect with varying colors that smoothly transition from blue (low values) to red (high values).
void drawVerticalVU(uint8_t x, uint8_t y, uint8_t width, uint8_t value, uint16_t color) {
	uint8_t height = rdlib_maths::mapValue(value, 0, 255, 63- y, 1);
	if (color < 1) {
		color = rdlib_maths::generateColor(rdlib_maths::mapValue(value, 0, 255, 1, 127));
	}
	myOLED.fillRectangle(x, 1, width, height, myOLED.RDLC_GREY);
	// Draw colored VU meter segment
	if (value > 4) {
			myOLED.fillRect(x, height + 1, width, 63 - (height + y + 2), color); // Draw colored bar
		
	}
}

//Demo 4
void DigitalClock(uint16_t count)
{
	std::cout << "Digital Clock demo , icons, small bitmap" << std::endl;
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	
	// Draw top bar of clock
	myOLED.drawIcon(0, 0, 16, myOLED.RDLC_BLACK, myOLED.RDLC_WHITE, SignalIconVa);
	myOLED.drawIcon(16, 0, 16, myOLED.RDLC_BLACK, myOLED.RDLC_WHITE, MsgIconVa);
	myOLED.drawIcon(32, 0, 8, myOLED.RDLC_BLACK, myOLED.RDLC_WHITE, AlarmIconVa);
	myOLED.drawIcon(42, 0, 16, myOLED.RDLC_BLACK, myOLED.RDLC_WHITE, BatIconVa);

	myOLED.drawIcon(60, 0, 12, myOLED.RDLC_GREEN , myOLED.RDLC_BLACK, PowerIcon);
	myOLED.drawIcon(80, 0, 12, myOLED.RDLC_RED, myOLED.RDLC_YELLOW, SpeedIcon);
	myOLED.setFont(font_default);
	myOLED.setCursor(0,20);
	myOLED.print(rdlib::LibraryVersion());

	//print sun
	if (myOLED.drawBitmap(50, 20, 40 , 16, myOLED.RDLC_CYAN , myOLED.RDLC_BLACK, SunText) != rdlib::Success)
	{
		std::cout << "Warning an Error occurred in drawBitmap." << std::endl;
		return ;
	}
	// Lines
	myOLED.drawFastHLine(0,18,95,myOLED.RDLC_WHITE);
	myOLED.drawFastHLine(0,40,95,myOLED.RDLC_WHITE);
	// draw clock
	while(count > 1)
	{
		std::string TimeString = UTC_string();
		std::cout<< TimeString << "\r" << std::flush;
		auto DateInfo = TimeString.substr(0, 10);
		auto TimeInfo = TimeString.substr(11,8);
		
		myOLED.setFont(font_gll);
		//print time
		myOLED.setTextColor(myOLED.RDLC_RED, myOLED.RDLC_BLACK);
		myOLED.setCursor(5,41);
		myOLED.print(TimeInfo);
		myOLED.setFont(font_gll);
		//print date
		myOLED.setTextColor(myOLED.RDLC_GREEN, myOLED.RDLC_BLACK);
		myOLED.setCursor(5,50);
		myOLED.print(DateInfo);
		myOLED.setFont(font_default);
		// print count
		myOLED.setTextColor(myOLED.RDLC_YELLOW, myOLED.RDLC_BLACK);
		myOLED.fillRectangle(0,28, 50,8, myOLED.RDLC_BLACK); //blank text box
		myOLED.setCursor(0,28);
		myOLED.print(count);
		count--;
	}
	delayMilliSecRDL(2000);
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	std::cout << "Clock demo over" << std::endl;
}

//demo 5
void SignalGen(uint16_t secondsDisplay) {
	std::cout << "oscilloscope Start" << std::endl;
	myOLED.setTextColor(myOLED.RDLC_WHITE, myOLED.RDLC_BLUE);
	static float phaseShift = 0.0f; // Continuous phase shift
	uint16_t sectionTime = secondsDisplay / 4; // Each wave gets 1/4 of the time
	const int waveAreaHeight = 48; // Waveform display area (0-48)
	const int labelHeight = 16; // Label display area (48-64)
	const int waveYOffset = 0; // Waveform starts from pixel 0
	// Draw the label section
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	myOLED.fillRect(0, 48, OLED_WIDTH, labelHeight, myOLED.RDLC_BLUE);

	while (secondsDisplay-- > 1) {
		delayMilliSecRDL(500); // Adjust for smoother/faster sweep
		std::cout << secondsDisplay << "\r" << std::flush;
		int waveType = (secondsDisplay / sectionTime) % 4;
		std::vector<std::string> stringWaveVec = {"Sine    ", "Square  ", "Triangle", "Sawtooth"};
		myOLED.setCursor(10, 52);
		myOLED.print(stringWaveVec[waveType]);
		myOLED.fillRect(0, 0, OLED_WIDTH, waveAreaHeight, myOLED.RDLC_BLACK);
		int prevY = waveYOffset + waveAreaHeight / 2; // Store previous Y for vertical lines
		// Draw the waveform
		for (int x = 0; x < OLED_WIDTH; x += 2) {
			float angle = ((x + phaseShift) * 2.0f * std::numbers::pi) / (OLED_WIDTH / _WAVE_FREQUENCY);
			int y = waveYOffset + (waveAreaHeight / 2);
			switch (waveType) {
				case 0: // **Sine Wave**
					y += _WAVE_AMPLITUDE * std::sin(angle);
					break;
				case 1: // **Square Wave**
					y += _WAVE_AMPLITUDE * (std::sin(angle) > 0 ? 1 : -1);
					if (x > 0 && (std::sin(angle) > 0) != (std::sin(angle - 2.0f * std::numbers::pi / _WAVE_FREQUENCY) > 0)) {
						myOLED.drawFastVLine(x, std::min(prevY, y), std::abs(prevY - y), myOLED.RDLC_GREEN);
					}
					break;
				case 2: // **Triangle Wave**
					y += _WAVE_AMPLITUDE * (2.0f / std::numbers::pi) * std::asin(std::sin(angle));
					break;
				case 3: // **Sawtooth Wave**
					y += _WAVE_AMPLITUDE * (2.0f / std::numbers::pi) * std::atan(std::tan(angle / 2.0f));
					if (x > 0 && std::atan(std::tan(angle / 2.0f)) < std::atan(std::tan((angle - 2.0f * std::numbers::pi / _WAVE_FREQUENCY) / 2.0f))) {
						myOLED.drawFastVLine(x, std::min(prevY, y), std::abs(prevY - y), myOLED.RDLC_GREEN);
					}
					break;
			}
			myOLED.drawPixel(x, y, myOLED.RDLC_GREEN);
			prevY = y;
		}
		// Smooth phase shift
		phaseShift += (_SWEEP_SPEED * 2.0f * std::numbers::pi) / (OLED_WIDTH / _WAVE_FREQUENCY);
		if (phaseShift >= OLED_WIDTH / _WAVE_FREQUENCY) phaseShift -= OLED_WIDTH / _WAVE_FREQUENCY;
	}

	myOLED.fillScreen(myOLED.RDLC_BLACK);
	std::cout << "oscilloscope demo over" << std::endl;
}

// demo 6
void demoRadar(uint16_t sweeps)
{
	std::cout << "Demo Radar: Draw line at angle function, ends at: " << sweeps << std::endl;
	myOLED.setFont(font_default);
	myOLED.setTextColor(myOLED.RDLC_BLACK, myOLED.RDLC_TAN);
	myOLED.fillScreen(myOLED.RDLC_TAN);
	const int centerX = 48;  // Center of radar (Adjusted for 96x64)
	const int centerY = 32;
	const int radarRadius = 30; // Smaller radius for 96x64 display
	const int lineLength = 28; // Line length slightly shorter than radius
	// Draw radar circles
	myOLED.fillCircle(centerX, centerY, radarRadius, myOLED.RDLC_BLACK);
	myOLED.drawCircle(centerX, centerY, radarRadius, myOLED.RDLC_GREEN);
	myOLED.drawCircle(centerX, centerY, radarRadius - 1, myOLED.RDLC_GREEN);
	// Sweep loop
	for (uint8_t j = 0; j < sweeps; j++)
	{
		for (int angle = 0; angle <= 360; angle++)
		{
			// Erase previous line
			myOLED.drawLineAngle(centerX, centerY, angle - 1, 0, lineLength, -90, myOLED.RDLC_BLACK);
			// Draw new radar line
			myOLED.drawLineAngle(centerX, centerY, angle, 0, lineLength, -90, myOLED.RDLC_GREEN);
			// Clear and update angle display
			myOLED.fillRectangle(68, 5, 28, 10, myOLED.RDLC_TAN);
			myOLED.setCursor(68, 5);
			myOLED.print(angle);
			delayMilliSecRDL(15);
		}
		std::cout << +j << "\r" << std::flush;
	}
	myOLED.fillScreen(myOLED.RDLC_BLACK);
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

void EndTests(void)
{
	myOLED.fillScreen(myOLED.RDLC_BLACK);
	delayMilliSecRDL(1000);
	myOLED.OLEDPowerDown();
	std::cout << "Demos Over" << std::endl;
}

void displayMenu() {
	std::cout << "Demos Menu:\n";
	std::cout << "1. Clock Analog\n";
	std::cout << "2. Clock Digital \n";
	std::cout << "3. VU meter 1\n";
	std::cout << "4. VU meter 2\n";
	std::cout << "5. Signal Generator \n";
	std::cout << "6. Radar \n";
	std::cout << "7. Quit\n";
	std::cout << "Enter your choice: ";
}

// *************** EOF ****************

/// @endcond
