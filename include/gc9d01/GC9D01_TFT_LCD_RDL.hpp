/*!
	@file     GC9D01_TFT_LCD_RDL.hpp
	@author   Gavin Lyons
	@brief    Library header file for GC9D01_TFT_RPI library.
*/

#pragma once

// Section Libraries
#include <cstdint>
#include <cstdbool>
#include <cstring>
#include <lgpio.h>
#include "color16_graphics_RDL.hpp"

/*!
	@brief Class to control GC9D01 TFT basic functionality.
*/
class GC9D01_TFT : public color16_graphics
{

public:

	GC9D01_TFT();
	~GC9D01_TFT(){};

	//  Enums
	/*!
	 * @brief GC9D01 power states based on the power control flow chart.
	 * These states combine the modes: Normal/Partial, Idle ON/OFF, and Sleep IN/OUT.
	 * They are listed from highest to lowest power consumption.
	 * 	Power states are based on the power control flow chart in the datasheet.
		FIG 89 5.10.2. Power Flow Chart
	 */
	enum class PowerState_e : uint8_t{
		NormalIdleOff,  	/**< SLPOUT + NORON + IDMOFF */
		NormalIdleOn,		/**< SLPOUT + NORON + IDMON */
		PartialIdleOff,		/**< SLPOUT + PLTON + IDMOFF */
		PartialIdleOn,		/**< SLPOUT + PLTON + IDMON */
		SleepNormalIdleOff,	/**< SLPIN + NORON + IDMOFF */
		SleepNormalIdleOn,	/**< SLPIN + NORON + IDMON */
		SleepPartialIdleOff,/**< SLPIN + PLTON + IDMOFF */
		SleepPartialIdleOn	/**< SLPIN + PLTON + IDMON */
	};

	/*!
	 * @brief GC9D01 panel resolution and gate configuration
	 */
	enum class Resolution_e : uint8_t
	{
		RGB160x160_DualGate,	 /**< 160RGB × 160, S1–S240, Dual gate (default) */
		RGB120x160_DualGate,	 /**< 120RGB × 160, S31–S210, Dual gate */
		RGB80x160_SingleGate,	 /**< 80RGB × 160, S1–S240, Single gate */
		RGB40x160_SingleGate	 /**< 40RGB × 160, S61–S180, Single gate */
	};

	display_rotate_e displayRotate = Degrees_0; /**< Enum to hold rotation */

	// Functions
	// From graphics virtual
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;
	//Setup related
	void TFTSetupGPIO(int8_t, int8_t, int8_t, int8_t, int8_t); //SW SPI
	void TFTSetupGPIO(int8_t, int8_t); // HW SPI
	void TFTInitScreenSize(uint16_t w = 160, uint16_t h = 160, 
		Resolution_e r = Resolution_e::RGB160x160_DualGate,
		uint16_t Xstart= 0, uint16_t Ystart=0);
	rdlib::Return_Codes_e TFTInitSPI(uint16_t CommDelay, int gpioDev ); // SW SPI
	rdlib::Return_Codes_e TFTInitSPI(int device, int channel, int speed, int flags, int gpioDev); // HW SPI
	rdlib::Return_Codes_e  TFTPowerDown(void);
	uint16_t HighFreqDelayGet(void);
	void HighFreqDelaySet(uint16_t);
	rdlib::Return_Codes_e TFTResetPin(void);

	// Screen related
	void TFTsetRotation(display_rotate_e r);
	void TFTchangeInvertMode(bool m);
	void TFTsetPowerMode(PowerState_e m);
	void TFTenableDisplay(bool m);
	void TFTsetBrightness(uint8_t level);
	void TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed);
	void TFTsetScrollStart(uint16_t vsp);
	void TFTScrollModeLeave(void);

private:

	rdlib::Return_Codes_e TFTDataCommandPin(void);
	rdlib::Return_Codes_e TFTClock_Data_ChipSelect_Pins(void);
	void TFTSetupResetPin(int8_t rst);
	rdlib::Return_Codes_e TFTGC9D01Initialize(void);
	void DualGatecmdInitSequence(void);
	void SingleGatecmdInitSequence(void);

	// Display
	PowerState_e _currentPowerState = PowerState_e::NormalIdleOff; /**< Enum to hold display mode */
	Resolution_e  _currentResolution = Resolution_e::RGB160x160_DualGate ; /**< enum to hold display resolution */
	bool _displayOn = false; /**< Enum to hold display on/off status */
	const uint16_t _sleepDelay = 120; /**< Sleep delay in ms ? */
	// Screen
	uint16_t _widthStartTFT =  160; /**<  never change after first init */
	uint16_t _heightStartTFT = 160; /**< never change after first init */
	uint16_t  _GC9D01_X_Offset_Start = 0; /**< column offset, never change after first init */
	uint16_t  _GC9D01_Y_Offset_Start = 0; /**< row offset, never change after first init */
	uint16_t _GC9D01_X_Offset = 0; /**< Column offset based on rotation, resolution and display type */
	uint16_t _GC9D01_Y_Offset = 0; /**< Row offset based on rotation, resolution and display type */
	// SPI related
	int _spiDev = 0;       /**< A SPI device, >= 0. */
	int _spiChan = 0;     /**< A SPI channel, >= 0. */
	int _spiBaud = 50000; /**< The speed of serial communication in bits per second. */
	int _spiFlags = 0;    /**<The flags 2 LSB defines SPI mode */
	bool _resetPinOn = true; /**< reset pin? true:hw rst pin, false:sw rt*/

	// GC9D01 registers + Commands
	/*!
	 * @brief MADCTL bit flags for register GC9D01_MADCTL (0x36).
	 */
	struct MADCTL_FLAGS_t
	{
		static constexpr uint8_t MY = 0x80; /**< Row Address Order: bottom to top */
		static constexpr uint8_t MX = 0x40; /**< Column Address Order: right to left */
		static constexpr uint8_t MV = 0x20; /**< Row/Column Exchange (Transpose) */
		static constexpr uint8_t ML = 0x10; /**< Vertical Refresh Order: bottom to top */
		static constexpr uint8_t RGB = 0x00; /**< RGB color order */
		static constexpr uint8_t BGR = 0x08; /**< BGR color order */
		static constexpr uint8_t MH = 0x04; /**< Horizontal Refresh Order: right to left */
	};

	// GC9D01 registers + Commands
	static constexpr uint8_t GC9D01_SWRESET = 0x01; /**< Software Reset undocumented ???*/
	static constexpr uint8_t GC9D01_RDDID = 0x04; /**< Read display identification information */
	static constexpr uint8_t GC9D01_RDDST = 0x09; /**< Read Display Status */

	static constexpr uint8_t GC9D01_SLPIN = 0x10; /**< Enter Sleep Mode */
	static constexpr uint8_t GC9D01_SLPOUT = 0x11; /**< Sleep Out */
	static constexpr uint8_t GC9D01_PTLON = 0x12; /**< Partial Mode ON */
	static constexpr uint8_t GC9D01_NORON = 0x13; /**< Normal Display Mode ON */
	static constexpr uint8_t GC9D01_DISPOFF = 0x28; /**< Display OFF */
	static constexpr uint8_t GC9D01_DISPON = 0x29; /**< Display ON */
	static constexpr uint8_t GC9D01_IDLEOFF = 0x38; /**< Idle mode OFF */
	static constexpr uint8_t GC9D01_IDLEON = 0x39; /**< Idle mode ON */

	static constexpr uint8_t GC9D01_INVOFF = 0x20; /**< Display Inversion OFF */
	static constexpr uint8_t GC9D01_INVON = 0x21; /**< Display Inversion ON */
	static constexpr uint8_t GC9D01_CASET = 0x2A; /**< Column Address Set */
	static constexpr uint8_t GC9D01_RASET = 0x2B; /**< Row/page Address Set */
	static constexpr uint8_t GC9D01_RAMWR = 0x2C; /**< Memory Write */
	static constexpr uint8_t GC9D01_PTLAR = 0x30; /**< Partial Area */
	static constexpr uint8_t GC9D01_VSCRDEF = 0x33; /**< Vertical Scrolling Definition */
	static constexpr uint8_t GC9D01_TEOFF = 0x34; /**< Tearing Effect Line OFF */
	static constexpr uint8_t GC9D01_TEON = 0x35; /**< Tearing Effect Line ON */
	static constexpr uint8_t GC9D01_MADCTL = 0x36; /**< Memory Access Control */
	static constexpr uint8_t GC9D01_VSCRSADD = 0x37; /**< Vertical Scrolling Start Address */
	static constexpr uint8_t GC9D01_COLMOD = 0x3A; /**< Pixel Format Set */
	static constexpr uint8_t GC9D01_CONTINUE = 0x3C; /**< Write Memory Continue */
	static constexpr uint8_t GC9D01_TEARSET = 0x44; /**< Set Tear Scanline */
	static constexpr uint8_t GC9D01_GETLINE = 0x45; /**< Get Scanline */
	static constexpr uint8_t GC9D01_SETBRIGHT = 0x51; /**< Write Display Brightness */
	static constexpr uint8_t GC9D01_SETCTRL = 0x53; /**< Write CTRL Display */

	static constexpr uint8_t GC9D01_RGB_CTRL = 0xB0; /**< RGB Interface Signal Control*/
	static constexpr uint8_t GC9D01_TEWC = 0xB4; /**< Tearing effect width control */
	static constexpr uint8_t GC9D01_BLANK_PORCH_CTRL = 0xB5; /**< Blanking Porch Control */
	static constexpr uint8_t GC9D01_FUNCTION_CTRL = 0xB6; /**< Display Function Control */
	static constexpr uint8_t GC9D01_DUAL_SINGLE = 0xBF; /**< Dual-Single gate select */

	static constexpr uint8_t GC9D01_POWER_CTRL  = 0xC1; /**< Power Criterion Control */
	static constexpr uint8_t GC9D01_VREG1A_CTRL = 0xC3; /**< Vreg1a voltage Control  */
	static constexpr uint8_t GC9D01_VREG1B_CTRL = 0xC4; /**< Vreg1b voltage Control  */
	static constexpr uint8_t GC9D01_VREG2A_CTRL = 0xC9; /**< Vreg2a voltage Control  */

	static constexpr uint8_t GC9D01_RDID1 = 0xDA; /**< Read ID 1 */
	static constexpr uint8_t GC9D01_RDID2 = 0xDB; /**< Read ID 2 */
	static constexpr uint8_t GC9D01_RDID3 = 0xDC; /**< Read ID 3 */
	
	static constexpr uint8_t GC9D01_GAMMA1 = 0xF0; /**< Set gamma 1 */
	static constexpr uint8_t GC9D01_GAMMA2 = 0xF1; /**< Set gamma 2 */
	static constexpr uint8_t GC9D01_GAMMA3 = 0xF2; /**< Set gamma 3 */
	static constexpr uint8_t GC9D01_GAMMA4 = 0xF3; /**< Set gamma 4 */

	static constexpr uint8_t GC9D01_SPI2DATA    = 0xB1; /**< SPI 2DATA control */
	static constexpr uint8_t GC9D01_INVERSION   = 0xEC; /**< Inversion */
	static constexpr uint8_t GC9D01_IFACE  = 0xF6; /**< Interface control */
	static constexpr uint8_t GC9D01_INREGEN1 = 0xFE; /**< Inter register enable 1 */
	static constexpr uint8_t GC9D01_INREGEN2    = 0xEF; /**< Inter register enable 2 */

}; //end of GC9D01_TFT  class

// ********************** EOF *********************
