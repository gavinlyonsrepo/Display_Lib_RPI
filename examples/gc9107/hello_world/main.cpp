/*!
	@file   examples/gc9107/hello_world/main.cpp
	@brief  Hello World: hardware or software SPI test
	@author Gavin Lyons.
	@details
		See USER OPTIONS in SETUP function.
		To switch between Hardware and software SPI
		set ''HardwareSPI'' in ''Globals''.
		Prints Hello, at 0 degrees,
		Prints World! at 90 degrees.
	@test
		-# 101 Hello World Hardware SPI
		-# 102 Hello World Software SPI
*/

// Section ::  libraries
#include <iostream>
#include "GC9107_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Globals
bool HardwareSPI = true; // Hardware SPI = true, software SPI = false
GC9107_TFT myTFT;

//  Section ::  Function Headers
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI
uint8_t SetupSWSPI(void); // setup + user options for software SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop
int main()
{
	std::cout << "GC9107 library version : " << rdlib::LibraryVersion() << std::endl;
	std::cout << "Lgpio library version :" << lguVersion() << std::endl;
	if (HardwareSPI == true)
	{ // Hardware SPI
		if (SetupHWSPI() != 0)
		{
			return -1;
		}
	}
	else
	{ // Software SPI
		if (SetupSWSPI() != 0)
		{
			return -1;
		}
	}
	HelloWorld();
	EndTests();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space
// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	std::cout << "TFT Start Test 101 HWSPI" << std::endl;
	int8_t RST_TFT = 25;
	int8_t DC_TFT = 24;
	int GPIO_CHIP_DEVICE = 0;  // GPIO chip device number usually 0
	int HWSPI_DEVICE = 0;	   // A SPI device, >= 0. which SPI interface to use
	int HWSPI_CHANNEL = 0;	   // A SPI channel, >= 0. Which Chip enable pin to use
	int HWSPI_SPEED = 8000000; // The speed of serial communication in bits per second.
	int HWSPI_FLAGS = 0;	   // last 2 LSB bits define SPI mode, see readme, mode 0 for this device
	// Size of Display (note 60x160 display has VRAM of 128x160)
	uint16_t TFT_WIDTH = 60;   // width  size in pixels
	uint16_t TFT_HEIGHT = 160; // height size in pixels
	uint8_t X_Offset = 34;     // X offset
	uint8_t Y_Offset = 0;      // Y offset
	// GRAM memory base mapping resolution, see GC9107_LTSM::GM_memory_base_e enum
	GC9107_TFT::GM_memory_base_e TFTmemoryBase = GC9107_TFT::GM_memory_base_e::MEMORY_BASE_GM_128x160;

	// ** USER OPTION 0 GPIO **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);

	// *** USER OPTION 1 Screen Setup ***
	myTFT.TFTInitScreen(TFT_WIDTH, TFT_HEIGHT, TFTmemoryBase, GC9107_TFT::MADCTL_FLAGS_t::RGB);

	// *** USER OPTION 2 Screen offsets***
	myTFT.TFTsetPanelOffset(X_Offset, Y_Offset);

	// ** USER OPTION 3 SPI settings **
	if (myTFT.TFTInitSPI(HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}

	delayMilliSecRDL(100);
	return 0;
}

// Software SPI setup
uint8_t SetupSWSPI(void)
{
	std::cout << "Start Test 102 SWSPI" << std::endl;
	int8_t RST_TFT = 25;
	int8_t DC_TFT = 24;
	int8_t SCLK_TFT = 20;
	int8_t SDIN_TFT = 16;
	int8_t CS_TFT = 21;
	int GPIO_CHIP_DEVICE = 0;
	uint16_t SWSPI_CommDelay = 0; // uS GPIO SW SPI delay
	// Size of Display (note 60x160 display has VRAM of 128x160)
	uint16_t TFT_WIDTH = 60;   // width  size in pixels
	uint16_t TFT_HEIGHT = 160; // height size in pixels
	uint8_t X_Offset = 34;     // X offset
	uint8_t Y_Offset = 0;      // Y offset
	// GRAM memory base mapping resolution, see GC9107_LTSM::GM_memory_base_e enum
	GC9107_TFT::GM_memory_base_e TFTmemoryBase = GC9107_TFT::GM_memory_base_e::MEMORY_BASE_GM_128x160;

	// ** USER OPTION 0 GPIO **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);

	// *** USER OPTION 1 Screen Setup ***
	myTFT.TFTInitScreen(TFT_WIDTH, TFT_HEIGHT, TFTmemoryBase, GC9107_TFT::MADCTL_FLAGS_t::RGB);

	// *** USER OPTION 2 Screen offset ***
	myTFT.TFTsetPanelOffset(X_Offset, Y_Offset);

	// ** USER OPTION 3 SPI settings **
	if (myTFT.TFTInitSPI(SWSPI_CommDelay, GPIO_CHIP_DEVICE) != rdlib::Success)
	{
		return 3;
	}
	std::cout << "SWSPI Comm GPIO Delay set to : " << myTFT.HighFreqDelayGet() << "uS" << std::endl;

	delayMilliSecRDL(100);
	return 0;
}

void HelloWorld(void)
{
	std::cout << "Hello, World!" << std::endl;
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.setCursor(0, 0);
	myTFT.setTextColor(myTFT.RDLC_GREEN, myTFT.RDLC_BLACK);
	myTFT.setFont(font_default);

	myTFT.print("Hello, ");
	delayMilliSecRDL(5000);

	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.fillScreen(myTFT.RDLC_BLACK);

	myTFT.print("World!");
	delayMilliSecRDL(5000);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	delayMilliSecRDL(500);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "End" << std::endl;
}

/// @endcond
