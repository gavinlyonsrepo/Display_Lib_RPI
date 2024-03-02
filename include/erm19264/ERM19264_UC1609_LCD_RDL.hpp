/*!
	@file ERM19264_UC1609_LCD_RDL.hpp
	@brief ERM19264 LCD driven by UC1609C controller, header file.
	@author Gavin Lyons.
*/

#pragma once

// ** INCLUDES **
#include <bcm2835.h>
#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include "bicolor_graphics_RDL.hpp"

/*!
    @brief Display Pixel colours  definition
    @details
        (1): white on blue,  FG = white, BG = blue
        ERM19264SBS-4 LCD Display White on Blue
        (2): black on white, FG = black, BG = white
        ERM19264FS-4 LCD Display Black on White
        (3):white on black,  FG = white, BG = black
        ERM19264DNS-4LCD Display White on Black
*/

// UC1909 Write registers
#define UC1609_SYSTEM_RESET 0xE2 /**< System Reset */

#define UC1609_POWER_CONTROL 0x28 /**< Power control Address */
#define UC1609_PC_SET 0x06 /**< PC[2:0] 110, Internal V LCD (7x charge pump) + 10b: 1.4mA */

#define UC1609_ADDRESS_CONTROL 0x88 /**< set RAM address control */
#define UC1609_ADDRESS_SET 0x02 /**< Set AC [2:0] Program registers  for RAM address control.*/

#define UC1609_SET_PAGEADD 0xB0 /**< Page address Set PA[3:0]  */
#define UC1609_SET_COLADD_LSB 0x00 /**< Column Address Set CA [3:0] */
#define UC1609_SET_COLADD_MSB 0x10 /**< Column Address Set CA [7:4] */

#define UC1609_TEMP_COMP_REG 0x27 /**< Temperature Compensation Register */
#define UC1609_TEMP_COMP_SET 0x00 /**< TC[1:0] = 00b= -0.00%/ C */

#define UC1609_FRAMERATE_REG 0xA0 /**< Frame rate register */
#define UC1609_FRAMERATE_SET 0x01  /**< Set Frame Rate LC [4:3] 01b: 95 fps */

#define UC1609_BIAS_RATIO 0xE8 /**< Bias Ratio. The ratio between V-LCD and V-D . */
#define UC1609_BIAS_RATIO_SET 0x03 /**<  Set BR[1:0] = 11 (set to 9 default, 11b = 9) */

#define UC1609_GN_PM 0x81 /**< Set V BIAS Potentiometer to fine tune V-D and V-LCD  (double-byte command) */
#define UC1609_DEFAULT_GN_PM 0x49 /**< default only used if user does not specify Vbias */

#define UC1609_LCD_CONTROL 0xC0 /**< Rotate map control */
#define UC1609_DISPLAY_ON 0xAE /**< enables display */
#define UC1609_ALL_PIXEL_ON 0xA4 /**< sets on all Pixels on */
#define UC1609_INVERSE_DISPLAY 0xA6 /**< inverts display */
#define UC1609_SCROLL 0x40 /**< scrolls , Set the scroll line number. 0-64 */

// Rotate
#define UC1609_ROTATION_FLIP_TWO 0x06/**< mirror image Rotation LCD command*/
#define UC1609_ROTATION_NORMAL 0x04  /**< Normal Rotation LCD command*/
#define UC1609_ROTATION_FLIP_ONE 0x02  /**< 180 degree  Rotation LCD command*/
#define UC1609_ROTATION_FLIP_THREE 0x00 /**< mirror image Rotation LCD command*/

// Delays
#define UC1609_RESET_DELAY 3 /**<  ms Rest Delay ,datasheet >3uS */
#define UC1609_RESET_DELAY2   0 /**< mS reset delay datasheet says > 5mS, does not work */
#define UC1609_INIT_DELAY 100   /**<  mS init delay ,after init  */
#define UC1609_INIT_DELAY2 3 /**<  mS init delay,  before reset called datasheet <3mS */

// GPIO abstraction
#define UC1609_CS_SetHigh bcm2835_gpio_write(_LCD_CS, HIGH)
#define UC1609_CS_SetLow bcm2835_gpio_write(_LCD_CS, LOW)
#define UC1609_CD_SetHigh bcm2835_gpio_write(_LCD_CD, HIGH)
#define UC1609_CD_SetLow bcm2835_gpio_write(_LCD_CD, LOW)
#define UC1609_RST_SetHigh bcm2835_gpio_write(_LCD_RST, HIGH)
#define UC1609_RST_SetLow bcm2835_gpio_write(_LCD_RST, LOW)
#define UC1609_SCLK_SetHigh bcm2835_gpio_write(_LCD_SCLK, HIGH)
#define  UC1609_SCLK_SetLow bcm2835_gpio_write(_LCD_SCLK, LOW)
#define  UC1609_SDA_SetHigh bcm2835_gpio_write(_LCD_DIN, HIGH)
#define  UC1609_SDA_SetLow bcm2835_gpio_write(_LCD_DIN, LOW)

#define UC1609_RST_SetDigitalOutput bcm2835_gpio_fsel(_LCD_RST, BCM2835_GPIO_FSEL_OUTP);
#define UC1609_CD_SetDigitalOutput bcm2835_gpio_fsel(_LCD_CD, BCM2835_GPIO_FSEL_OUTP);
#define UC1609_CS_SetDigitalOutput bcm2835_gpio_fsel( _LCD_CS, BCM2835_GPIO_FSEL_OUTP); // SW SPI only last 3 lines
#define UC1609_SCLK_SetDigitalOutput bcm2835_gpio_fsel(_LCD_SCLK, BCM2835_GPIO_FSEL_OUTP);
#define UC1609_DIN_SetDigitalOutput bcm2835_gpio_fsel(_LCD_DIN, BCM2835_GPIO_FSEL_OUTP);

/*!
	@brief class to drive the ERM19264_UC1609 LCD
*/
class ERM19264_UC1609 : public bicolor_graphics {
public:
	 // Contructor 1 Software SPI with explicit SCLK and SDIN
	ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd, int8_t cs,  int8_t sclk, int8_t din);
	// Constructor 2 Hardware SPI
	ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd);

	~ERM19264_UC1609(){};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t colour) override;
	void LCDupdate(void);
	void LCDclearBuffer(void);
	void LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data);
	rpiDisplay_Return_Codes_e LCDSetBufferPtr(uint8_t width, uint8_t height , uint8_t* pBuffer, uint16_t sizeOfBuffer);
	rpiDisplay_Return_Codes_e LCDbegin(uint8_t AddressSet = UC1609_ADDRESS_SET, uint8_t VbiasPot = UC1609_DEFAULT_GN_PM, uint32_t _SPICLK_DIVIDER = 0, uint8_t _SPICE_Pin = 0  );
	void LCDinit(void);
	void LCDEnable(uint8_t on);
	void LCDFillScreen(uint8_t pixel);
	void LCDFillPage(uint8_t page_num, uint8_t pixels);
	void LCDrotate(uint8_t rotatevalue);
	void LCDinvert(uint8_t on);
	void LCDallpixelsOn(uint8_t bits);
	void LCDscroll(uint8_t bits);
	void LCDReset(void);
	void LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data);
	void LCDPowerDown(void);
	void LCDSPIHWSettings(void);
	void LCDSPIoff(void);
	bool LCDIssleeping(void);

	uint16_t LCD_HighFreqDelayGet(void);
	void LCD_HighFreqDelaySet(uint16_t);

  private:

	void send_data(uint8_t data);
	void send_command(uint8_t command, uint8_t value);
	int8_t GetCommMode(void);
	void CustomshiftOut(uint8_t val);

	int8_t _LCD_CS;    /**< GPIO Chip select  line */
	int8_t _LCD_CD;   /**< GPIO Data or command line */
	int8_t _LCD_RST;  /**< GPIO Reset line */
	int8_t _LCD_SCLK; /**< GPIO Clock Line Software SPI only*/
	int8_t _LCD_DIN;  /**< GPIO MOSI Line Software SPI only*/

	uint8_t _VbiasPOT= 0x49;    /**< Contrast default 0x49 datasheet 00-FE */
	uint8_t _AddressCtrl= 0x02; /**< Set AC [2:0] Program registers  for RAM address control. 0x00 to 0x07*/

	// Display Size
	int16_t _LCD_WIDTH = 192;                 /**< Width of screen in pixels */
	int16_t _LCD_HEIGHT = 64;                 /**< Height of screen in pixels */
	int8_t _LCD_PAGE_NUM = (_LCD_HEIGHT/8); /**< Number of 8 bit screen pages*/

	int8_t _LCD_mode = 2; /**< 2 = HW SPI 3 = SW SPI, other numbers reserved for future use*/
	bool _sleep = true;   /**< False = awake/ON , true = sleep/OFF*/

	uint32_t _SPICLK_DIVIDER = 0; /**<Spi clock divider , bcm2835SPIClockDivider enum bcm2835*/
	uint8_t _SPICE_PIN = 0;       /**< which SPI_CE pin to use , 0 or 1*/

	uint16_t  _LCD_HighFreqDelay = 0; /**< uS used in software SPI , option.*/

	uint8_t* LCDbufferScreen = nullptr;/**< pointer to the screen buffer data array*/
};
