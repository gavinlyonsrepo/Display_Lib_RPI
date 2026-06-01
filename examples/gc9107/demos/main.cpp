/*!
	@file   examples/gc9107/demos/main.cpp
	@brief  Demo: Fake System Monitor Status Bar
	@author Gavin Lyons.
	@details
			Demonstrates the GC9107 60x160 portrait display as a system monitor
			status bar using fake/simulated data. Showcases the narrow portrait
			form factor with a vertical stack of labelled metric panels.
			Hardware SPI, Advanced Screen Buffer Mode.
	@test
			-# 401 System Monitor Demo
*/

// Section :: libraries
#include <iostream>
#include <cstdio>   // snprintf
#include <cstring>  // strlen

#include <atomic>  // Ctrl + C exit
#include <csignal> // Ctrl + C exit
#include <thread>  // Ctrl + C exit

#include "GC9107_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
GC9107_TFT myTFT;
std::atomic<bool> stopRequested{false}; // Stop signal , Ctrl + c etc

// Section :: Demo Config
#define DEMO_CYCLES      10     // how many refresh cycles to run
#define CYCLE_DELAY_MS   2000   // ms between each refresh

// Section :: Layout constants (portrait 60 x 160)
// Display is divided into 5 equal panels, each 32px tall with a 1px divider.
// Total = 5 * 32 = 160px
#define PANEL_H          32     // height of each metric panel in pixels
#define PANEL_W          60     // full display width
#define BAR_X            1      // progress bar left margin
#define BAR_Y_OFFSET     18     // y offset of bar within panel
#define BAR_H            8      // progress bar height
#define BAR_MAX_W        52     // max usable bar width (leaving 7px for border/margin)
#define LABEL_X          1      // x for panel label text
#define VALUE_X          38     // x for value text (right-aligned area)

// Section :: Function Headers
uint8_t SetupHWSPI(void);
uint8_t SetupBufferMode(void);
void    Demo(void);
void    EndDemo(void);
void handleSignal(int){
	stopRequested = true; // for CtrL +C
}

// Drawing helpers
void DrawHeader(void);
void DrawPanel(uint8_t panelIndex, const char* label, uint8_t percent,
               uint16_t barColor, const char* valueStr);
void DrawProgressBar(int16_t x, int16_t y, uint8_t percent, uint16_t color);

// Fake data helpers
uint8_t  FakeCPU(uint8_t step);
uint8_t  FakeRAM(uint8_t step);
uint8_t  FakeTemp(uint8_t step);
uint8_t  FakeDisk(uint8_t step);
uint32_t FakeUptime(uint8_t step);

// Color picker for thresholded metrics
uint16_t PanelColor(uint8_t percent);

//  Section ::  MAIN loop
int main()
{
	if (SetupHWSPI() != 0)
		return -1;
	std::signal(SIGINT, handleSignal); // for user press Ctrl+C
	std::signal(SIGTERM, handleSignal);// for kill command
	Demo();
	if (stopRequested)
		std::cout << "Ctrl+C pressed" << std::endl;
	EndDemo();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space

uint8_t SetupHWSPI(void)
{
	std::cout << "GC9107 System Monitor Demo(FAKE DATA)" << std::endl;

	int8_t  RST_TFT        = 25;
	int8_t  DC_TFT         = 24;
	int     GPIO_CHIP_DEVICE = 0;
	int     HWSPI_DEVICE   = 0;
	int     HWSPI_CHANNEL  = 0;
	int     HWSPI_SPEED    = 8000000;
	int     HWSPI_FLAGS    = 0;

	uint16_t TFT_WIDTH     = 60;
	uint16_t TFT_HEIGHT    = 160;
	uint8_t  X_Offset      = 34;
	uint8_t  Y_Offset      = 0;

	GC9107_TFT::GM_memory_base_e TFTmemoryBase =
		GC9107_TFT::GM_memory_base_e::MEMORY_BASE_GM_128x160;

	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
	myTFT.TFTInitScreen(TFT_WIDTH, TFT_HEIGHT, TFTmemoryBase,
	                    GC9107_TFT::MADCTL_FLAGS_t::RGB);
	myTFT.TFTsetPanelOffset(X_Offset, Y_Offset);

	if (myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED,
	                     HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}

	delayMilliSecRDL(100);
	return SetupBufferMode();
}

uint8_t SetupBufferMode(void)
{
	myTFT.setAdvancedScreenBuffer_e(myTFT.AdvancedScreenBuffer_e::On);
	if (myTFT.getAdvancedScreenBuffer_e() == myTFT.AdvancedScreenBuffer_e::Off)
	{
		std::cout << "Error: Advanced Screen Buffer Mode not set" << std::endl;
		return 4;
	}
	if (myTFT.setBuffer() != rdlib::Success)
		return 5;
	myTFT.setTextCharPixelOrBuffer(true);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.writeBuffer();
	return 0;
}

//  DEMO  – cycles through DEMO_CYCLES refreshes of fake data
void Demo(void)
{
	std::cout << "Demo 401: System Monitor" << std::endl;

	for (uint8_t step = 0; step < DEMO_CYCLES; step++)
	{
		if (stopRequested) break;
		// --- Gather fake metrics ---
		uint8_t  cpu    = FakeCPU(step);
		uint8_t  ram    = FakeRAM(step);
		uint8_t  temp   = FakeTemp(step);      // 0-100 scaled from 30-90°C
		uint8_t  disk   = FakeDisk(step);
		// Build display strings
		char cpuStr[8],  ramStr[8], tempStr[8], diskStr[8];
		snprintf(cpuStr,  sizeof(cpuStr),  "%2u",  cpu);
		snprintf(ramStr,  sizeof(ramStr),  "%2u",  ram);
		// temp: 0-100 maps to 30-90°C
		snprintf(tempStr, sizeof(tempStr), "%2u",   (uint8_t)(30 + (temp * 60) / 100));
		snprintf(diskStr, sizeof(diskStr), "%2u",  disk);
		// --- Render ---
		myTFT.clearBuffer(myTFT.RDLC_BLACK);
		DrawHeader();
		// Panel 0 – CPU  (y starts at 16 below header)
		DrawPanel(0, "CPU",  cpu,  PanelColor(cpu),  cpuStr);
		// Panel 1 – RAM
		DrawPanel(1, "RAM",  ram,  PanelColor(ram),  ramStr);
		// Panel 2 – TEMP
		DrawPanel(2, "TEMP", temp, PanelColor(temp), tempStr);
		// Panel 3 – DISK
		DrawPanel(3, "DISK", disk, PanelColor(disk), diskStr);
		myTFT.writeBuffer();

		std::cout << "Step " << (int)step
		          << "  CPU=" << (int)cpu  << "%"
		          << "  RAM=" << (int)ram  << "%"
		          << "  TEMP=" << tempStr
		          << "  DISK=" << (int)disk << "%"
		          << std::endl;
		delayMilliSecRDL(CYCLE_DELAY_MS);
	}
}

void EndDemo(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.clearBuffer(myTFT.RDLC_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown();
	std::cout << "Demo End" << std::endl;
}

// === Drawing helpers ===

// Helper: map 0-100 percent to generateColor's 1-127 range
uint16_t PanelColor(uint8_t percent)
{
	uint8_t val = (uint8_t)(1 + ((uint16_t)percent * 126) / 100);
	return rdlib_maths::generateColor(val);
}

void DrawHeader(void)
{
	// Thin top border line
	myTFT.drawRectWH(0, 0, PANEL_W, 1, myTFT.RDLC_WHITE);

	myTFT.setFont(font_pico);
	myTFT.setTextColor(myTFT.RDLC_CYAN, myTFT.RDLC_BLACK);
	myTFT.setCursor(2, 3);
	myTFT.print("SYS MON");

	// Separator line below header
	myTFT.drawRectWH(0, 14, PANEL_W, 1, myTFT.RDLC_WHITE);
}

/*!
	@brief  Draw a single metric panel.
	@param  panelIndex  0-4, controls vertical position (below 16px header)
	@param  label       Short metric name e.g. "CPU"
	@param  percent     0-100 fill level for progress bar
	@param  barColor    Colour of the progress bar
	@param  valueStr    Formatted value string e.g. "72%"
*/
void DrawPanel(uint8_t panelIndex, const char* label,
               uint8_t percent, uint16_t barColor, const char* valueStr)
{
	// Top y of this panel (16px header + panelIndex * PANEL_H)
	int16_t py = 16 + (int16_t)panelIndex * PANEL_H;
	// Thin divider at bottom of panel
	myTFT.drawRectWH(0, py + PANEL_H - 1, PANEL_W, 1, myTFT.RDLC_WHITE);
	// Label (left side, pico font)
	myTFT.setFont(font_pico);
	myTFT.setTextColor(myTFT.RDLC_WHITE, myTFT.RDLC_BLACK);
	myTFT.setCursor(LABEL_X, py + 3);
	myTFT.print(label);
	// Value (right side)
	myTFT.setTextColor(barColor, myTFT.RDLC_BLACK);
	myTFT.setCursor(VALUE_X, py + 3);
	myTFT.print(valueStr);
	// Progress bar
	DrawProgressBar(BAR_X, py + BAR_Y_OFFSET, percent, barColor);
}

/*!
	@brief  Draw a filled progress bar with an outline.
	@param  x        Left edge of bar
	@param  y        Top edge of bar
	@param  percent  0-100 fill
	@param  color    Fill colour
*/
void DrawProgressBar(int16_t x, int16_t y, uint8_t percent, uint16_t color)
{
	// Outer outline
	myTFT.drawRectWH(x, y, BAR_MAX_W + 2, BAR_H, myTFT.RDLC_WHITE);
	// Inner fill
	if (percent > 0)
	{
		uint8_t fillW = (uint8_t)((uint16_t)percent * BAR_MAX_W / 100);
		if (fillW > BAR_MAX_W) fillW = BAR_MAX_W;
		myTFT.fillRect(x + 1, y + 1, fillW, BAR_H - 2, color);
	}
}


//  === Fake data generators ===
// CPU: oscillates 20-90% with a spike pattern
uint8_t FakeCPU(uint8_t step)
{
	static const uint8_t pattern[] = {22, 35, 48, 71, 89, 99, 76, 49, 30, 25};
	return pattern[step % 10];
}

// RAM: slowly climbs then levels
uint8_t FakeRAM(uint8_t step)
{
	static const uint8_t pattern[] = {45, 47, 50, 53, 56, 58, 60, 61, 62, 63};
	return pattern[step % 10];
}

// Temperature: scaled 0-100 (maps to 30-90°C in display string)
uint8_t FakeTemp(uint8_t step)
{
	static const uint8_t pattern[] = {28, 35, 45, 58, 72, 82, 78, 65, 50, 40};
	return pattern[step % 10];
}

// Disk: mostly stable, small creep
uint8_t FakeDisk(uint8_t step)
{
	static const uint8_t pattern[] = {68, 68, 69, 69, 70, 70, 71, 71, 71, 72};
	return pattern[step % 10];
}

/// @endcond