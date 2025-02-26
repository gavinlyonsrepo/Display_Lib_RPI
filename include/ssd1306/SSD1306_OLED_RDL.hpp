/*!
* @file SSD1306_OLED_RDL.hpp
* @brief   OLED driven by SSD1306 controller.
* @author Gavin Lyons.
* @details Project Name: Display_Lib_RPI
	URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
*/

#pragma once

// Library includes

#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include <lgpio.h>
#include "bicolor_graphics_RDL.hpp"


/*!
	@brief class to control OLED and define buffer
*/
class SSD1306_RDL : public bicolor_graphics  {
  public:
	SSD1306_RDL(int16_t oledwidth, int16_t oledheight);
	~SSD1306_RDL(){};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) override;

	rdlib::Return_Codes_e OLEDSetBufferPtr(uint8_t width, uint8_t height , std::span<uint8_t> buffer);
	rdlib::Return_Codes_e OLEDupdate(void);
	rdlib::Return_Codes_e OLEDclearBuffer(void);
	void OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h, std::span<uint8_t> data);

	void OLEDFillScreen(uint8_t pixel, uint8_t mircodelay);
	void OLEDFillPage(uint8_t page_num, uint8_t pixels,uint8_t delay);

	void OLEDbegin();
	void OLEDinit(void);
	void OLEDPowerDown(void);

	void OLEDEnable(uint8_t on);
	void OLEDContrast(uint8_t OLEDcontrast);
	void OLEDInvert(bool on);

	void OLEDStartScrollRight(uint8_t start, uint8_t stop);
	void OLEDStartScrollLeft(uint8_t start, uint8_t stop) ;
	void OLEDStartScrollDiagRight(uint8_t start, uint8_t stop) ;
	void OLEDStartScrollDiagLeft(uint8_t start, uint8_t stop) ;
	void OLEDStopScroll(void) ;

	rdlib::Return_Codes_e OLED_I2C_ON(int I2C_device, int I2C_addr , int I2C_flags);
	rdlib::Return_Codes_e OLED_I2C_OFF(void);
	int OLEDCheckConnection(void);
	int OLEDI2CErrorGet(void);
	uint16_t OLEDI2CErrorTimeoutGet(void);
	void OLEDI2CErrorTimeoutSet(uint16_t);
	uint8_t OLEDI2CErrorRetryNumGet(void);
	void OLEDI2CErrorRetryNumSet(uint8_t);

	const uint8_t SSD1306_ADDR  = 0x3C;  /**< I2C address, alt 0x3D */  

  private:

	void I2CWriteByte(uint8_t value = 0x00, uint8_t DataOrCmd =  SSD1306_COMMAND);
	//  === SSD1306 Command Set  ===
	// Fundamental Commands
	static constexpr uint8_t SSD1306_SET_CONTRAST_CONTROL = 0x81;
	static constexpr uint8_t SSD1306_DISPLAY_ALL_ON_RESUME = 0xA4;
	static constexpr uint8_t SSD1306_DISPLAY_ALL_ON = 0xA5;
	static constexpr uint8_t SSD1306_NORMAL_DISPLAY = 0xA6;
	static constexpr uint8_t SSD1306_INVERT_DISPLAY = 0xA7;
	static constexpr uint8_t SSD1306_DISPLAY_OFF = 0xAE;
	static constexpr uint8_t SSD1306_DISPLAY_ON = 0xAF;
	static constexpr uint8_t SSD1306_NOP = 0xE3;

	// Scrolling Commands
	static constexpr uint8_t SSD1306_RIGHT_HORIZONTAL_SCROLL = 0x26;
	static constexpr uint8_t SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27;
	static constexpr uint8_t SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;
	static constexpr uint8_t SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;
	static constexpr uint8_t SSD1306_DEACTIVATE_SCROLL = 0x2E;
	static constexpr uint8_t SSD1306_ACTIVATE_SCROLL = 0x2F;
	static constexpr uint8_t SSD1306_SET_VERTICAL_SCROLL_AREA = 0xA3;

	// Addressing Setting Commands
	static constexpr uint8_t SSD1306_SET_LOWER_COLUMN = 0x00;
	static constexpr uint8_t SSD1306_SET_HIGHER_COLUMN = 0x10;
	static constexpr uint8_t SSD1306_MEMORY_ADDR_MODE = 0x20;
	static constexpr uint8_t SSD1306_SET_COLUMN_ADDR = 0x21;
	static constexpr uint8_t SSD1306_SET_PAGE_ADDR = 0x22;

	// Hardware Configuration Commands
	static constexpr uint8_t SSD1306_SET_START_LINE = 0x40;
	static constexpr uint8_t SSD1306_SET_SEGMENT_REMAP = 0xA0;
	static constexpr uint8_t SSD1306_SET_MULTIPLEX_RATIO = 0xA8;
	static constexpr uint8_t SSD1306_COM_SCAN_DIR_INC = 0xC0;
	static constexpr uint8_t SSD1306_COM_SCAN_DIR_DEC = 0xC8;
	static constexpr uint8_t SSD1306_SET_DISPLAY_OFFSET = 0xD3;
	static constexpr uint8_t SSD1306_SET_COM_PINS = 0xDA;
	static constexpr uint8_t SSD1306_CHARGE_PUMP = 0x8D;

	// Timing & Driving Scheme Setting Commands
	static constexpr uint8_t SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO = 0xD5;
	static constexpr uint8_t SSD1306_SET_PRECHARGE_PERIOD = 0xD9;
	static constexpr uint8_t SSD1306_SET_VCOM_DESELECT = 0xDB;

	// I2C related
	static constexpr uint8_t SSD1306_COMMAND        = 0x00;
	static constexpr uint8_t SSD1306_DATA           = 0xC0;
	static constexpr uint8_t SSD1306_DATA_CONTINUE  = 0x40;
	//  === SSD1306 Command Set END ===

	int _OLEDI2CAddress = SSD1306_ADDR  ; /**< I2C address for I2C module PCF8574 backpack on OLED*/
	int _OLEDI2CDevice = 1; /**< An I2C device number. */
	int _OLEDI2CFlags =  0;   /**< Flags which modify an I2C open command. None are currently defined. */
	int _OLEDI2CHandle = 0;  /**< A number referencing an object opened by one of lgI2cOpen */
	uint16_t _I2C_ErrorDelay = 100; /**<I2C delay(in between retry attempts) in event of error in mS*/
	uint8_t _I2C_ErrorRetryNum = 3; /**< In event of I2C error number of retry attempts*/
	int _I2C_ErrorFlag = 0; /**< In event of I2C error holds code*/

	uint8_t _OLED_WIDTH=128;      /**< Width of OLED Screen in pixels */
	uint8_t _OLED_HEIGHT=64;    /**< Height of OLED Screen in pixels */
	uint8_t _OLED_PAGE_NUM=(_OLED_HEIGHT/8); /**< Number of byte size pages OLED screen is divided into */

	std::span<uint8_t> _OLEDbuffer; /**< span to buffer which holds screen data */


};
