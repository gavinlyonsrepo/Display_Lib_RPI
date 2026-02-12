/*!
	@file     ILI9341_TFT_LCD_RDL.hpp
	@author   Gavin Lyons
	@brief    Library header file for ILI9341_TFT_LCD_RDL library.
				to manage hardware interface with ILI9341
*/

#pragma once

#include <lgpio.h>
#include "color16_graphics_RDL.hpp"

/*!
@brief Class to interface with ILI9341 LCD display
*/
class ILI9341_TFT : public color16_graphics
{
public:
	ILI9341_TFT();
	~ILI9341_TFT(){};

	/*!
		@brief  Register addresses for ILI9341 diagnostic read commands.
	 			These constants can be used with readDiagByte() to query
				the controller for internal status and diagnostic information.
				See datasheet section 8.2.3 for more details
	 */
	enum ILI9341_ReadRegister_e : uint8_t {
		ILI9341_RDID        = 0x04, /**< Read Display Identification Information, 3 bytes*/
		ILI9341_RDSTATUS    = 0x09, /**< Read Display Status, 4 bytes */
		ILI9341_RDMODE      = 0x0A, /**< Read Display Power Mode */
		ILI9341_RDMADCTL    = 0x0B, /**< Read MADCTL */
		ILI9341_RDPIXFMT    = 0x0C, /**< Read Display Pixel Format */
		ILI9341_RDIMGFMT    = 0x0D, /**< Read Display Image Mode */
		ILI9341_RDSIGNAL    = 0x0E, /**< Read Display Signal Mode */
		ILI9341_RDSELFDIAG  = 0x0F  /**< Read Display Self-Diagnostic Result */
	};

	//Functions
	virtual void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;

	//Init Setup related
	void SetupGPIO(int8_t RST, int8_t DC, int8_t CS, int8_t SCLK, int8_t MOSI, int8_t MISO = -1); //SW SPI
	void SetupGPIO(int8_t RST, int8_t DC); // HW SPI
	void InitScreenSize(uint16_t w, uint16_t h);
	rdlib::Return_Codes_e InitSPI(uint16_t CommDelay, int gpioDev); // SW SPI
	rdlib::Return_Codes_e InitSPI(int device, int channel, int speed, int flags, int gpioDev); // HW SPI

	// SPI related
	uint16_t HighFreqDelayGet(void);
	void HighFreqDelaySet(uint16_t);
	rdlib::Return_Codes_e PowerDown(void);
	void EnableDisplay(bool enableDisplay);
	void SoftwareReset(void);

	// screen related
	void setRotation(display_rotate_e rotation);
	void invertDisplay(bool i);
	void scrollTo(uint16_t y);
	void setScrollMargins(uint16_t top, uint16_t bottom);
	void NormalMode(void);
	rdlib::Return_Codes_e TFTResetPin(void);
	uint8_t readDiagByte(ILI9341_ReadRegister_e cmd, uint8_t indexs);

protected:

private:

	rdlib::Return_Codes_e TFTDataCommandPin(void);
	rdlib::Return_Codes_e TFTMISOPin(void);
	rdlib::Return_Codes_e TFTClock_Data_ChipSelect_Pins(void);
	rdlib::Return_Codes_e ILI9341Initialize(void);
	void TFTSetupResetPin(int8_t rst);
	void TFTSetupMISOPin(int8_t miso);
	void cmdInit(void);

	// SPI
	bool _resetPinOn = true; /**< reset pin? true:hw rst pin, false:sw rt*/
	bool _MISOPinOn = false; /**< MISO pin used? true:use MISO pin, false: not used*/
	int _spiDev = 0; /**< A SPI device, >= 0. */
	int _spiChan = 0; /**< A SPI channel, >= 0. */
	int _spiBaud = 50000;   /**< The speed of serial communication in bits per second. */
	int _spiFlags = 0; /**<The flags 2 LSB defines SPI mode */ 

	// Screen 
	uint16_t _widthStartTFT = 240;  /**< never change after first init */
	uint16_t _heightStartTFT = 320; /**< never change after first init */

	// Registers & command Set
	static constexpr uint8_t ILI9341_NOP      = 0x00; /**< No-op register */
	static constexpr uint8_t ILI9341_SWRESET  = 0x01; /**< Software reset register */
	static constexpr uint8_t ILI9341_RDDID    = 0x04; /**< Read display identification information */
	static constexpr uint8_t ILI9341_RDDST    = 0x09; /**< Read Display Status */

	static constexpr uint8_t ILI9341_SLPIN   = 0x10; /**< Enter Sleep Mode */
	static constexpr uint8_t ILI9341_SLPOUT  = 0x11; /**< Sleep Out */
	static constexpr uint8_t ILI9341_PTLON   = 0x12; /**< Partial Mode ON */
	static constexpr uint8_t ILI9341_NORON   = 0x13; /**< Normal Display Mode ON */

	static constexpr uint8_t ILI9341_INVOFF     = 0x20; /**< Display Inversion OFF */
	static constexpr uint8_t ILI9341_INVON      = 0x21; /**< Display Inversion ON */
	static constexpr uint8_t ILI9341_GAMMASET   = 0x26; /**< Gamma Set */
	static constexpr uint8_t ILI9341_DISPOFF    = 0x28; /**< Display OFF */
	static constexpr uint8_t ILI9341_DISPON     = 0x29; /**< Display ON */

	static constexpr uint8_t ILI9341_CASET  = 0x2A; /**< Column Address Set */
	static constexpr uint8_t ILI9341_PASET  = 0x2B; /**< Page Address Set */
	static constexpr uint8_t ILI9341_RAMWR  = 0x2C; /**< Memory Write */
	static constexpr uint8_t ILI9341_RAMRD  = 0x2E; /**< Memory Read */

	static constexpr uint8_t ILI9341_PTLAR     = 0x30; /**< Partial Area */
	static constexpr uint8_t ILI9341_VSCRDEF   = 0x33; /**< Vertical Scrolling Definition */
	static constexpr uint8_t ILI9341_MADCTL    = 0x36; /**< Memory Access Control */
	static constexpr uint8_t ILI9341_VSCRSADD  = 0x37; /**< Vertical Scrolling Start Address */
	static constexpr uint8_t ILI9341_PIXFMT    = 0x3A; /**< COLMOD: Pixel Format Set */

	static constexpr uint8_t ILI9341_FRMCTR1  = 0xB1; /**< Frame Rate Ctrl (Normal Mode/Full Colors)  */
	static constexpr uint8_t ILI9341_FRMCTR2  = 0xB2; /**< Frame Rate Ctrl (Idle Mode/8 colors) */
	static constexpr uint8_t ILI9341_FRMCTR3  = 0xB3; /**< Frame Rate control (In Partial Mode/Full Colors) */
	static constexpr uint8_t ILI9341_INVCTR   = 0xB4; /**< Display Inversion Control */
	static constexpr uint8_t ILI9341_DFUNCTR  = 0xB6; /**< Display Function Control */

	static constexpr uint8_t ILI9341_PWCTR1  = 0xC0; /**< Power Control 1 */
	static constexpr uint8_t ILI9341_PWCTR2  = 0xC1; /**< Power Control 2 */
	static constexpr uint8_t ILI9341_PWCTR3  = 0xC2; /**< Power Control 3 */
	static constexpr uint8_t ILI9341_PWCTR4  = 0xC3; /**< Power Control 4 */
	static constexpr uint8_t ILI9341_PWCTR5  = 0xC4; /**< Power Control 5 */

	static constexpr uint8_t ILI9341_VMCTR1  = 0xC5; /**< VCOM Control 1 */
	static constexpr uint8_t ILI9341_VMCTR2  = 0xC7; /**< VCOM Control 2 */

	static constexpr uint8_t ILI9341_RDID1  = 0xDA; /**< Read ID 1 */
	static constexpr uint8_t ILI9341_RDID2  = 0xDB; /**< Read ID 2 */
	static constexpr uint8_t ILI9341_RDID3  = 0xDC; /**< Read ID 3 */
	static constexpr uint8_t ILI9341_RDID4  = 0xDD; /**< Read ID 4 */

	static constexpr uint8_t ILI9341_EN3GAM   = 0xF2; /**< Enable 3 gamma control*/
	static constexpr uint8_t ILI9341_GMCTRP1  = 0xE0; /**< Positive Gamma Correction */
	static constexpr uint8_t ILI9341_GMCTRN1  = 0xE1; /**< Negative Gamma Correction */

	static constexpr uint8_t ILI9341_PWONCS   = 0xED; /**< Power on sequence control */
	static constexpr uint8_t ILI9341_PWCTRA   = 0xCB; /**< Power Control A */
	static constexpr uint8_t ILI9341_PWCTRB   = 0xCF; /**< Power Control B */
	static constexpr uint8_t ILI9341_DTMCTRA  = 0xE8; /**< Driver timing control A*/
	static constexpr uint8_t ILI9341_DTMCTRC  = 0xEA; /**< Driver timing control C*/
	static constexpr uint8_t ILI9341_PURTCTR  = 0xF7; /**< Pump ratio control*/

	static constexpr uint8_t MADCTL_MY   = 0x80; /**< Bottom to top */
	static constexpr uint8_t MADCTL_MX   = 0x40; /**< Right to left */
	static constexpr uint8_t MADCTL_MV   = 0x20; /**< Reverse Mode */
	static constexpr uint8_t MADCTL_ML   = 0x10; /**< LCD refresh Bottom to top */
	static constexpr uint8_t MADCTL_RGB  = 0x00; /**< Red-Green-Blue pixel order */
	static constexpr uint8_t MADCTL_BGR  = 0x08; /**< Blue-Green-Red pixel order */
	static constexpr uint8_t MADCTL_MH   = 0x04; /**< LCD refresh right to left */
};

