 /*!
	@file ERM1_CH1115_OLED_RDL.hpp
	@brief ER_OLEDM1 OLED driven by CH1115 controller header file
	@details Project Name: Display_Lib_RPI
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@author  Gavin Lyons
*/

#pragma once

// ** INCLUDES **
#include <lgpio.h>
#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include <cstring>
#include "bicolor_graphics_RDL.hpp"

// ** DEFINES **

/* CH1115 Command Set*/
// Fundamental Commands
#define ERMCH1115_ENTIRE_DISPLAY_ON 0xA4 /**< Forcibly turns the entire display on regardless of the contents of the display data RAM */
#define ERMCH1115_ENTIRE_DISPLAY_OFF 0xA5 /**< Forcibly turns the entire display on regardless of the contents of the display data RAM */
#define ERMCH1115_DISPLAY_ON 0xAF /**< Turns on Display */
#define ERMCH1115_DISPLAY_OFF 0xAE /**< Turns off Display */
#define ERMCH1115_DISPLAY_NORMAL 0xA6  /**< Display  Inversion off*/
#define ERMCH1115_DISPLAY_INVERT 0xA7  /**< Display  Inversion on*/
#define ERMCH1115_CONTRAST_CONTROL 0x81 /**<  Contrast Control Mode Set */
#define ERMCH1115_CONTRAST_DATA_DEFAULT 0x80  /**<  Contrast Data Reg Set 0x00 to 0xFF */

// Addressing Setting Commands
#define ERMCH1115_SET_PAGEADD 0xB0 /**< Set Page Address: (B0H - B7H) */
#define ERMCH1115_SET_COLADD_LSB 0x00 /**< Set Lower Column Address: (00H - 0FH) */
#define ERMCH1115_SET_COLADD_MSB 0x10 /**< Set Higher Column Address: (10H – 1FH) */
#define ERMCH1115_SET_DISPLAY_START_LINE 0x40 /**< Determine the initial display line */

// Hardware Configuration Commands
#define ERMCH1115_SET_PUMP_REG 0x30 /**< Output voltage (V PP ) internal charger pump. */
#define ERMCH1115_SET_PUMP_SET   0x01 /**<  A1:A0[] 01 = 7.4v  */
#define ERMCH1115_IREF_REG 0x82 /**< I REF Resistor Set  */
#define ERMCH1115_IREF_SET 0x00 /**< I REF Resistor adjust */
#define ERMCH1115_SEG_SET_REMAP  0xA0 /**< Set Segment Re-map: (A0H - A1H) */
#define ERMCH1115_SEG_SET_PADS 0xA2 /**<Set SEG pads hw config: (A2H – A3H) */
#define ERMCH1115_MULTIPLEX_MODE_SET 0xA8 /**<Multiplex Ration Mode Set */
#define ERMCH1115_MULTIPLEX_DATA_SET  0x3F /**<Multiplex Ration Data Set: (00H - 3FH) */

#define ERMCH1115_DC_MODE_SET 0xAD /**< DC-DC Control Mode Set */
#define ERMCH1115_DC_ONOFF_SET 0x8B /**< DC-DC ON/OFF Mode Set (8AH - 8BH) (a=off; b=on) */
#define ERMCH1115_COMMON_SCAN_DIR 0xC0 /**< Set Common Op Scan Dir:C0H - C8H Flip vertical */
#define ERMCH1115_OFFSET_MODE_SET 0xD3 /**<Display Offset Mode Set: */
#define ERMCH1115_OFFSET_DATA_SET 0x00 /**<Display Offset Data Set: (00H~3FH) */
#define ERMCH1115_BREATHEFFECT_SET 0x23 /**< Set Breathing Display Effect: */
#define ERMCH1115_BREATHEFFECT_DATA 0x81 /**< Display Effect ON/OFF and Time Interval: */

// Timing & Driving Scheme Setting Commands
#define ERMCH1115_OSC_FREQ_MODE_SET 0xD5 /**<Divide Ratio/Oscillator Frequency Mode Set: (D5H) */
#define ERMCH1115_OSC_FREQ_DATA_SET 0xA0  /**<Divide Ratio/Oscillator Frequency Data Set: */
#define ERMCH1115_PRECHARGE_MODE_SET 0xD9 /**<Pre-charge Period Mode Set: */
#define ERMCH1115_PRECHARGE_DATA_SET 0x22 /**<Discharge/Pre-charge Period Data Set: (00H - FFH) */
#define ERMCH1115_COM_LEVEL_MODE_SET 0xDB /**<COM Deselect Level Mode Set: (DBH) */
#define ERMCH1115_COM_LEVEL_DATA_SET 0x40 /**<VCOM Deselect Level Data Set: (00H - FFH) */


// HORIZONTAL Scrolling Commands
#define ERMCH1115_HORIZONTAL_A_SCROLL_SETUP 0x24 /**<  Additional Horizontal Scroll Setup Mode Set: (24H) */
#define ERMCH1115_HORIZONTAL_A_SCROLL_SET_SCOL 0x00  /**< Start Column Position Set: (A7 – A0) */
#define ERMCH1115_HORIZONTAL_A_SCROLL_SET_ECOL 0x7F /**< End Column Position Set: (B7 – B0) */
#define ERMCH1115_SCROLL_SETUP  0x26 /**<Horizontal Scroll Setup: 26 right 27 left (A2 – A0)*/
#define ERMCH1115_SPAGE_ADR_SET 0x00  /**< Start Page Address Set 0x00 = page 0 */
#define ERMCH1115_TIME_SET 0x00 /**<Time Interval Set: (B2 – B0) 0x00 = 6 frames */
#define ERMCH1115_EPAGE_ADR_SET  0x07 /**<End Page Address Set: (C2 – C0)  */
#define ERMCH1115_SET_SCROLL_MODE 0x28 /**< Set Scroll Mode: (28H – 2BH) 0x28 = continuous */
#define ERMCH1115_DEACTIVATE_SCROLL  0x2E /**< De-active Scroll mode */
#define ERMCH1115_ACTIVATE_SCROLL   0x2F /**<active Scroll mode */


// Delays
#define ERMCH1115_INITDELAY 100 /**< Init delay mS */
#define ERMCH1115_HIGHFREQ_DELAY 0 /**< uS GPIO SPI SW Delay, default 0uS */
#define ERMCH1115_RST_DELAY1 10 /**< Reset Delay in mS*/
#define ERMCH1115_RST_DELAY2 100 /**< Reset Delay 2 in mS */


// CLASS SECTION 

/*! @brief class to drive the ERMCh1115 OLED */
class ERMCH1115 : public bicolor_graphics   {

  public:

	 // Contructor 1 Software SPI with explicit SCLK and SDIN and DC/cd
	ERMCH1115(int16_t oledwidth, int16_t oledheight , int8_t rst, int8_t dc, int8_t cs,  int8_t sclk, int8_t din);
	// Constructor 2 Hardware SPI
	ERMCH1115(int16_t oledwidth, int16_t oledheight , int8_t rst, int8_t dc);
	~ERMCH1115(){};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t colour) override;
	void OLEDupdate(void);
	void OLEDclearBuffer(void);
	void OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data);

	// No buffer functions
	void OLEDFillScreen(uint8_t pixel);
	void OLEDFillPage(uint8_t page_num, uint8_t pixels);
	void OLEDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data);

	rpiDisplay_Return_Codes_e OLEDbegin(uint8_t contrast,  int gpioDev);
	rpiDisplay_Return_Codes_e OLEDbegin(uint8_t contrast, int device, int channel, int speed, int flags, int gpioDev );
	void OLEDinit(void);
	void OLEDReset(void);

	void OLEDEnable(uint8_t on);
	void OLEDInvert(uint8_t on);
	void OLEDFlip(uint8_t on);
	void OLEDContrast(uint8_t contrast = ERMCH1115_CONTRAST_DATA_DEFAULT);
	void OLEDscroll(uint8_t bits) ;
	void OLEDscrollSetup(uint8_t Timeinterval= ERMCH1115_TIME_SET, uint8_t Direction=ERMCH1115_SCROLL_SETUP, uint8_t mode=ERMCH1115_SET_SCROLL_MODE) ;
	void OLEDfadeEffect(uint8_t bits = ERMCH1115_BREATHEFFECT_DATA);
	bool OLEDIsSleeping(void);

	rpiDisplay_Return_Codes_e  OLEDSPIoff(void);
	void OLEDPowerDown(void);

	uint16_t OLEDHighFreqDelayGet(void);
	void OLEDHighFreqDelaySet(uint16_t);
	
	rpiDisplay_Return_Codes_e OLEDSetBufferPtr(uint8_t width, uint8_t height , uint8_t* pBuffer, uint16_t sizeOfBuffer);

  private:

	void send_data(uint8_t data);
	void send_command(uint8_t command, uint8_t value);
	int8_t GetCommMode(void);
	void SoftwareSPIShiftOut(uint8_t val);

	// GPIO related
	int8_t _Display_CS;   /**< GPIO Chip select line, Software SPI only*/
	int8_t _Display_DC;   /**< GPIO Data or command line */
	int8_t _Display_RST;  /**< GPIO Reset line */
	int8_t _Display_SCLK; /**< GPIO Clock Line, Software SPI only*/
	int8_t _Display_SDATA;   /**< GPIO MOSI Line, Software SPI only*/
	int _DeviceNumGpioChip = 0; /**< SWSPI The device number of a gpiochip 4=rpi5 0=rpi4,3 /dev/gpio */
	int _GpioHandle = 0; /**< This holds a handle to a gpiochip device opened by lgGpiochipOpen  */

	// SPI related 
	int _spiHandle = 0; /**< Hold a handle for the SPI device on the channel.*/
	int _spiDev = 0; /**< A SPI device, >= 0. */
	int _spiChan = 0; /**< A SPI channel, >= 0. */
	int _spiBaud = 50000;   /**< The speed of serial communication in bits per second. */
	int _spiFlags = 0; /**<The flags 2 LSB defines SPI mode */ 
	uint16_t _OLEDHighFreqDelay = ERMCH1115_HIGHFREQ_DELAY; /**< uS GPIO Communications delay, SW SPI ONLY */
	int8_t _OLED_mode = 2; /**< Mode, hardware or software SPI , 2=HWSPI 3=SWSPI */

	// Display
	uint8_t _OLED_WIDTH = 128; /**< Width in pxiels  of the OLED */
	uint8_t _OLED_HEIGHT = 64; /**< Height in pixels  of the OLED */
	uint8_t _OLED_PAGE_NUM = (_OLED_HEIGHT/8); /**< Number of pages in OLED based on height, PageNum = (h/8) */
	uint8_t* OLEDbuffer = nullptr; /**< pointer to buffer which holds screen data */
	bool _sleep = true; /**< False awake , true sleep */
	uint8_t _OLEDcontrast = ERMCH1115_CONTRAST_DATA_DEFAULT; /**< Contrast , default 0x80 ,datasheet 00-FF */

}; // end of class



