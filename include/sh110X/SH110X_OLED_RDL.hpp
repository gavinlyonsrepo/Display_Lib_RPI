/*!
* @file SH110X_OLED_RDL.hpp
* @brief   OLED driven by SH110X_RDL controller. Sh1106 and Sh1107
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
class SH110X_RDL : public bicolor_graphics  {
  public:
	SH110X_RDL(int16_t oledwidth, int16_t oledheight);
	~SH110X_RDL(){};
	
	const uint8_t SH110X_RDL_ADDR = 0x3C; /**< I2C address */

	/*! type of OLED  controller */
	enum OLED_IC_type_e: uint8_t
	{
		SH1106_IC = 0, /**< Sh1106 controller */
		SH1107_IC  =1  /**< Sh1107 controller */
	};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) override;

	rdlib::Return_Codes_e OLEDSetBufferPtr(uint8_t width, uint8_t height , std::span<uint8_t> buffer);
	rdlib::Return_Codes_e OLEDupdate(void);
	rdlib::Return_Codes_e OLEDclearBuffer(void);
	void OLEDBufferScreen(uint8_t w, uint8_t h, std::span<uint8_t> buffer);


	void OLEDFillScreen(uint8_t pixel, uint8_t mircodelay);
	void OLEDFillPage(uint8_t page_num, uint8_t pixels,uint8_t delay);

	rdlib::Return_Codes_e OLEDbegin(OLED_IC_type_e = SH1106_IC, int8_t ResetPin = -1, int gpioDev = 0);
	void OLEDinit(void);
	void OLEDPowerDown(void);
	void OLEDReset(void);

	void OLEDEnable(uint8_t on);
	void OLEDContrast(uint8_t OLEDcontrast);
	void OLEDInvert(bool on);

	rdlib::Return_Codes_e OLED_I2C_ON(int I2C_device, int I2C_addr , int I2C_flags);
	rdlib::Return_Codes_e OLED_I2C_OFF(void);
	int OLEDCheckConnection(void);
	uint8_t OLEDI2CErrorGet(void);
	uint16_t OLEDI2CErrorTimeoutGet(void);
	void OLEDI2CErrorTimeoutSet(uint16_t);
	uint8_t OLEDI2CErrorRetryNumGet(void);
	void OLEDI2CErrorRetryNumSet(uint8_t);

  protected:
	
	 uint8_t pageStartOffset = 0; /**< the SH1106 display  requires a small offset */

  private:

	void I2CWriteByte(uint8_t value = 0x00, uint8_t DataOrCmd = SH110X_COMMAND_BYTE);
	void SH1106_begin(void);
	void SH1107_begin(void);

	int _OLEDI2CAddress = SH110X_RDL_ADDR; /**< I2C address for I2C module PCF8574 backpack on OLED*/
	int _OLEDI2CDevice = 1; /**< An I2C device number. */
	int _OLEDI2CFlags =  0;   /**< Flags which modify an I2C open command. None are currently defined. */
	int _OLEDI2CHandle = 0;  /**< A number referencing an object opened by one of lgI2cOpen */
	uint16_t _I2C_ErrorDelay = 100; /**<I2C delay(in between retry attempts) in event of error in mS*/
	uint8_t _I2C_ErrorRetryNum = 3; /**< In event of I2C error number of retry attempts*/
	int _I2C_ErrorFlag = 0; /**< In event of I2C error holds lgpio error code*/

	uint8_t _OLED_WIDTH=128;      /**< Width of OLED Screen in pixels */
	uint8_t _OLED_HEIGHT=64;    /**< Height of OLED Screen in pixels */
	uint8_t _OLED_PAGE_NUM=(_OLED_HEIGHT/8); /**< Number of byte size pages OLED screen is divided into */

	std::span<uint8_t> _OLEDbuffer ; /**< buffer which holds screen data */

	OLED_IC_type_e _OLED_IC_type = SH1106_IC;  /**< Enum to hold TFT type  */

	// If reset pin is present on device
	int8_t _Display_RST = -1; /**< Reset pin only needed of reset pin is present on display*/
	int _DeviceNumGpioChip = 0; /**< The device number of a gpiochip  ls /dev/gpio */
	int _GpioHandle = 0; /**< This returns a handle to a  device. */

	//  SH110X register command Set
	static constexpr uint8_t SH110X_MEMORYMODE  = 0x20 ; /**< Memory mode control register */
	static constexpr uint8_t SH110X_COLUMNADDR  = 0x21 ; /**< Column address control register */
	static constexpr uint8_t SH110X_PAGEADDR    = 0x22 ; /**< Page address control register */
	static constexpr uint8_t SH110X_CHARGEPUMP  = 0x8D ; /**< Charge pump control register */
	static constexpr uint8_t SH110X_SETCONTRAST = 0x81 ; /**<Command to set contrast, 256 contrast steps*/
	static constexpr uint8_t SH110X_SEGREMAP    = 0xA0 ; /**<Set segment remap, left or right */

	static constexpr uint8_t SH110X_DISPLAYALLON_RESUME = 0xA4 ; /**<Set entire Display OFF */

	static constexpr uint8_t SH110X_DISPLAYALLON  = 0xA5 ; /**<Set entire Display ON */
	static constexpr uint8_t SH110X_NORMALDISPLAY = 0xA6 ; /**<Normal Display, no ram rewrite*/
	static constexpr uint8_t SH110X_INVERTDISPLAY = 0xA7 ; /**<Inverts Display, no rma rewrite */
	static constexpr uint8_t SH110X_SETMULTIPLEX  = 0xA8 ; /**<Set multiplex ration */
	static constexpr uint8_t SH110X_DCDC          = 0xAD ; /**<Controls DC-DC voltage convertor*/
	static constexpr uint8_t SH110X_DISPLAYOFF    = 0xAE ; /**<Turns off OLED panel*/
	static constexpr uint8_t SH110X_DISPLAYON     = 0xAF ; /**<Turns on OLED panel */
	static constexpr uint8_t SH110X_SETPAGEADDR   = 0xB0 ; /**<Specify page address to load display RAM data to page address */

	static constexpr uint8_t SH110X_COMSCANINC         = 0xC0 ; /**<Sets the scan direction of common output*/
	static constexpr uint8_t SH110X_COMSCANDEC         = 0xC8 ; /**<Scan from COMN to COM0*/
	static constexpr uint8_t SH110X_SETDISPLAYOFFSET   = 0xD3 ; /**<Mapping of start line to COM lines*/
	static constexpr uint8_t SH110X_SETDISPLAYCLOCKDIV = 0xD5 ; /**<Set freq of internal clock*/
	static constexpr uint8_t SH110X_SETPRECHARGE       = 0xD9 ; /**<Duration of precharge period */
	static constexpr uint8_t SH110X_SETCOMPINS         = 0xDA ; /**<Common signals pad configuration*/
	static constexpr uint8_t SH110X_SETVCOMDETECT      = 0xDB ; /**<Common pad output voltage level deselect stage*/
	static constexpr uint8_t SH110X_SETDISPSTARTLINE   = 0xDC ; /**<Specify Column address to determine the initial display line or COM0.*/

	static constexpr uint8_t SH110X_SETLOWCOLUMN  = 0x00  ; /**<Set lower column address*/
	static constexpr uint8_t SH110X_SETHIGHCOLUMN = 0x10  ; /**<Set higher column address*/
	static constexpr uint8_t SH110X_SETSTARTLINE  = 0x40  ; /**<Specifies line address to determine the initial display*/

	static constexpr uint8_t SH110X_COMMAND_BYTE    =  0x00 ; /**< Command byte command */
	static constexpr uint8_t SH110X_DATA_BYTE       =  0x40 ; /**< Data byte command */

};
