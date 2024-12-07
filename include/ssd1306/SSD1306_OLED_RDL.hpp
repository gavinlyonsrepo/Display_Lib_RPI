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

//  SSD1306 Command Set

// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL    0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME   0xA4
#define SSD1306_DISPLAY_ALL_ON          0xA5
#define SSD1306_NORMAL_DISPLAY          0xA6
#define SSD1306_INVERT_DISPLAY          0xA7
#define SSD1306_DISPLAY_OFF             0xAE
#define SSD1306_DISPLAY_ON              0xAF
#define SSD1306_NOP                     0xE3

// Scrolling Commands
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E
#define SSD1306_ACTIVATE_SCROLL                      0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3

// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN   0x00
#define SSD1306_SET_HIGHER_COLUMN  0x10
#define SSD1306_MEMORY_ADDR_MODE   0x20
#define SSD1306_SET_COLUMN_ADDR    0x21
#define SSD1306_SET_PAGE_ADDR      0x22

// Hardware Configuration Commands
#define SSD1306_SET_START_LINE        0x40
#define SSD1306_SET_SEGMENT_REMAP     0xA0
#define SSD1306_SET_MULTIPLEX_RATIO   0xA8
#define SSD1306_COM_SCAN_DIR_INC      0xC0
#define SSD1306_COM_SCAN_DIR_DEC      0xC8
#define SSD1306_SET_DISPLAY_OFFSET    0xD3
#define SSD1306_SET_COM_PINS          0xDA
#define SSD1306_CHARGE_PUMP           0x8D

// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO  0xD5
#define SSD1306_SET_PRECHARGE_PERIOD         0xD9
#define SSD1306_SET_VCOM_DESELECT            0xDB

// I2C related
#define SSD1306_COMMAND        0x00
#define SSD1306_DATA           0xC0
#define SSD1306_DATA_CONTINUE  0x40
#define SSD1306_ADDR           0x3C  /**< I2C address alt 0x3D */

#define SSD1306_command(Reg)  I2C_Write_Byte(Reg, SSD1306_COMMAND)
#define SSD1306_data(Data)    I2C_Write_Byte(Data, SSD1306_DATA_CONTINUE)

// Delays
#define SSD1306_INITDELAY 100 /**< Initialisation delay in mS */


/*!
	@brief class to control OLED and define buffer
*/
class SSD1306_RDL : public bicolor_graphics  {
  public:
	SSD1306_RDL(int16_t oledwidth, int16_t oledheight);
	~SSD1306_RDL(){};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) override;
	void OLEDupdate(void);
	void OLEDclearBuffer(void);
	void OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data);
	void OLEDFillScreen(uint8_t pixel, uint8_t mircodelay);
	void OLEDFillPage(uint8_t page_num, uint8_t pixels,uint8_t delay);

	void OLEDbegin(bool I2C_debug);
	rpiDisplay_Return_Codes_e OLEDSetBufferPtr(uint8_t width, uint8_t height , uint8_t* pBuffer, uint16_t sizeOfBuffer);
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

	rpiDisplay_Return_Codes_e OLED_I2C_ON(int I2C_device, int I2C_addr , int I2C_flags);
	rpiDisplay_Return_Codes_e OLED_I2C_OFF(void);
	int OLEDCheckConnection(void);
	int OLEDI2CErrorGet(void);
	uint16_t OLEDI2CErrorTimeoutGet(void);
	void OLEDI2CErrorTimeoutSet(uint16_t);
	uint8_t OLEDI2CErrorRetryNumGet(void);
	void OLEDI2CErrorRetryNumSet(uint8_t);
	bool OLEDDebugGet(void);
	void OLEDDebugSet(bool);

  private:

	void I2C_Write_Byte(uint8_t value, uint8_t Cmd);

	int _OLEDI2CAddress = SSD1306_ADDR  ; /**< I2C address for I2C module PCF8574 backpack on OLED*/
	int _OLEDI2CDevice = 1; /**< An I2C device number. */
	int _OLEDI2CFlags =  0;   /**< Flags which modify an I2C open command. None are currently defined. */
	int _OLEDI2CHandle = 0;  /**< A number referencing an object opened by one of lgI2cOpen */
	uint16_t _I2C_ErrorDelay = 100; /**<I2C delay(in between retry attempts) in event of error in mS*/
	uint8_t _I2C_ErrorRetryNum = 3; /**< In event of I2C error number of retry attempts*/
	int _I2C_ErrorFlag = 0; /**< In event of I2C error holds code*/
	bool _I2C_DebugFlag = false; /**<I2C debug flag , print I2C debug messages if true*/

	uint8_t _OLED_WIDTH=128;      /**< Width of OLED Screen in pixels */
	uint8_t _OLED_HEIGHT=64;    /**< Height of OLED Screen in pixels */
	uint8_t _OLED_PAGE_NUM=(_OLED_HEIGHT/8); /**< Number of byte size pages OLED screen is divided into */

	uint8_t* OLEDbuffer = nullptr; /**< pointer to buffer which holds screen data */


};
