/*!
	@file     ILI9341_TFT_LCD_RDL.cpp
	@author   Gavin Lyons
	@brief   Source file for ILI9341_TFT_LCD_RDL library.
				to manage hardware interface with ILI9341
*/

#include "../../include/ili9341/ILI9341_TFT_LCD_RDL.hpp"

#define ILI9341_NOP     0x00 /**< No-op register */
#define ILI9341_SWRESET 0x01 /**< Software reset register */
#define ILI9341_RDDID   0x04 /**< Read display identification information */
#define ILI9341_RDDST   0x09 /**< Read Display Status */

#define ILI9341_SLPIN  0x10 /**< Enter Sleep Mode */
#define ILI9341_SLPOUT 0x11 /**< Sleep Out */
#define ILI9341_PTLON  0x12 /**< Partial Mode ON */
#define ILI9341_NORON  0x13 /**< Normal Display Mode ON */

#define ILI9341_INVOFF    0x20 /**< Display Inversion OFF */
#define ILI9341_INVON     0x21 /**< Display Inversion ON */
#define ILI9341_GAMMASET  0x26 /**< Gamma Set */
#define ILI9341_DISPOFF   0x28 /**< Display OFF */
#define ILI9341_DISPON    0x29 /**< Display ON */

#define ILI9341_CASET 0x2A /**< Column Address Set */
#define ILI9341_PASET 0x2B /**< Page Address Set */
#define ILI9341_RAMWR 0x2C /**< Memory Write */
#define ILI9341_RAMRD 0x2E /**< Memory Read */

#define ILI9341_PTLAR    0x30  /**< Partial Area */
#define ILI9341_VSCRDEF  0x33  /**< Vertical Scrolling Definition */
#define ILI9341_MADCTL   0x36  /**< Memory Access Control */
#define ILI9341_VSCRSADD 0x37  /**< Vertical Scrolling Start Address */
#define ILI9341_PIXFMT   0x3A  /**< COLMOD: Pixel Format Set */

#define ILI9341_FRMCTR1 0xB1 /**< Frame Rate Ctrl (Normal Mode/Full Colors)  */
#define ILI9341_FRMCTR2 0xB2 /**< Frame Rate Ctrl (Idle Mode/8 colors) */
#define ILI9341_FRMCTR3 0xB3 /**< Frame Rate control (In Partial Mode/Full Colors) */
#define ILI9341_INVCTR  0xB4 /**< Display Inversion Control */
#define ILI9341_DFUNCTR 0xB6 /**< Display Function Control */

#define ILI9341_PWCTR1 0xC0 /**< Power Control 1 */
#define ILI9341_PWCTR2 0xC1 /**< Power Control 2 */
#define ILI9341_PWCTR3 0xC2 /**< Power Control 3 */
#define ILI9341_PWCTR4 0xC3 /**< Power Control 4 */
#define ILI9341_PWCTR5 0xC4 /**< Power Control 5 */

#define ILI9341_VMCTR1 0xC5 /**< VCOM Control 1 */
#define ILI9341_VMCTR2 0xC7 /**< VCOM Control 2 */

#define ILI9341_RDID1 0xDA /**< Read ID 1 */
#define ILI9341_RDID2 0xDB /**< Read ID 2 */
#define ILI9341_RDID3 0xDC /**< Read ID 3 */
#define ILI9341_RDID4 0xDD /**< Read ID 4 */

#define ILI9341_EN3GAM  0xF2 /**< Enable 3 gamma control*/
#define ILI9341_GMCTRP1 0xE0 /**< Positive Gamma Correction */
#define ILI9341_GMCTRN1 0xE1 /**< Negative Gamma Correction */

#define ILI9341_PWONCS  0xED /**< Power on sequence control */
#define ILI9341_PWCTRA  0xCB /**< Power Control A */
#define ILI9341_PWCTRB  0xCF /**< Power Control B */
#define ILI9341_DTMCTRA 0xE8 /**< Driver timing control A*/
#define ILI9341_DTMCTRC 0xEA /**< Driver timing control C*/
#define ILI9341_PURTCTR 0xF7 /**< Pump ratio control*/

#define MADCTL_MY  0x80  /**< Bottom to top */
#define MADCTL_MX  0x40  /**< Right to left */
#define MADCTL_MV  0x20  /**< Reverse Mode */
#define MADCTL_ML  0x10  /**< LCD refresh Bottom to top */
#define MADCTL_RGB 0x00  /**< Red-Green-Blue pixel order */
#define MADCTL_BGR 0x08  /**< Blue-Green-Red pixel order */
#define MADCTL_MH  0x04  /**< LCD refresh right to left */

/*! @brief Constructor for class ILI9341_TFT */
ILI9341_TFT::ILI9341_TFT(){}

/*!
	@brief  sets up TFT GPIO for Hardware SPi
	@param rst reset GPIO
	@param dc data or command GPIO.
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ILI9341_TFT::SetupGPIO(int8_t rst, int8_t dc)
{
	_hardwareSPI = true;
	TFTSetupResetPin(rst);
	_Display_DC = dc;
}

/*!
	@brief  sets up TFT GPIO for software SPI
	@param rst reset GPIO
	@param dc data or command GPIO.
	@param cs chip select GPIO
	@param sclk Data clock GPIO
	@param mosi Data to TFT GPIO
	@param miso Data from TFT GPIO read diagnostics
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ILI9341_TFT::SetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t mosi, int8_t miso)
{
	_hardwareSPI = false;
	TFTSetupResetPin(rst);
	_Display_MISO = miso;
	_Display_DC = dc;
	_Display_CS = cs;
	_Display_SCLK = sclk;
	_Display_SDATA = mosi;
}

/*!
	@brief sets up TFT GPIO reset pin
	@param rst reset GPIO
	@details if rst value is -1 use software reset, else use Hardware reset.
*/
void ILI9341_TFT::TFTSetupResetPin(int8_t rst)
{
	if(rst != -1)
	{
		_Display_RST= rst;
		_resetPinOn = true;
	}else{
		_resetPinOn  = false;
	}
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_TFT width in pixels
	@param height_TFT height in pixels
*/
void ILI9341_TFT:: InitScreenSize(uint16_t width_TFT, uint16_t height_TFT)
{
	_width = width_TFT;
	_height = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
}

/*!
	@brief intialise  SPI, Hardware SPI
	@param hertz  SPI Clock frequency in Hz, HW SPI only MAX 125 Mhz , MIN 30Khz(RPI3). typical/tested 8000000
	@param SPICE_Pin which SPI CE/CS pin to use 0 = SPICE0 GPIO08 RPI3, 1 = SPICE1 GPIO07 RPI3
	@return
		-# rpiDisplay_Success = success
		-# rpiDisplay_SPICEXPin SPI pin  incorrect value (upstream)
		-# rpiDisplay_SPIbeginFail bcm2835_spi_begin has failed (upstream)
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rpiDisplay_Return_Codes_e ILI9341_TFT::InitSPI(uint32_t hertz, uint8_t SPICE_Pin)
{
	if (SPICE_Pin >= 2)
	{
		std::cout << "Error:InitSPI 2: SPICE_PIN value incorrect must be 0 or 1:" << SPICE_Pin <<std::endl;
		return  rpiDisplay_SPICEXPin;
	}

	_SPICEX_pin = SPICE_Pin;
	_SPIhertz = hertz;

	return ILI9341Initialize();
}

/*!
	@brief intialise PCBtype and SPI, Software SPI
	@param CommDelay uS GPIO delay used in software SPI
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ILI9341_TFT::InitSPI(uint16_t CommDelay)
{
	HighFreqDelaySet(CommDelay);
	ILI9341Initialize();
}

/*!
	@brief init routine for ILI9341_TFTcontroller
	@return Error if bcm2835_spi_begin has failed
*/
rpiDisplay_Return_Codes_e ILI9341_TFT::ILI9341Initialize()
{
	ResetPIN();
	Display_DC_SetDigitalOutput;
	Display_DC_SetLow;
if (_hardwareSPI == false)
{
	Display_SCLK_SetDigitalOutput;
	Display_SDATA_SetDigitalOutput;
	Display_CS_SetDigitalOutput;
	Display_CS_SetHigh;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
}else{
	if (!bcm2835_spi_begin())
	{
		std::cout << "Error:ILI9341Initialize:  bcm2835_spi_begin :Cannot start spi, Running as root?" << std::endl;
		return rpiDisplay_SPIbeginFail;
	}
	TFTSPIHWSettings();
}
	cmdInit();
	return rpiDisplay_Success;
}

// SPI related

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ILI9341_TFT::HighFreqDelayGet(void){return _HighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ILI9341_TFT::HighFreqDelaySet(uint16_t CommDelay){_HighFreqDelay = CommDelay;}

/*!
	@brief  Init Hardware SPI settings
	@details MSBFIRST, mode 0 , SPI Speed , SPICEX pin
	@note If multiple devices on SPI bus with different settings,
	can be used to refresh ST7789 settings
*/
void ILI9341_TFT::TFTSPIHWSettings(void)
{
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

	if (_SPIhertz > 0)
		bcm2835_spi_setClockDivider(bcm2835_aux_spi_CalcClockDivider(_SPIhertz));
	else //SPI_CLOCK_DIVIDER_32 = 7.8125MHz on Rpi2, 12.5MHz on RPI3
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);

	if (_SPICEX_pin == 0)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	}else if (_SPICEX_pin == 1)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
	}
}

/*!
	@brief Call when powering down TFT
	@note Turns off Display Sets GPIO low and turns off SPI
	End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK),
	P1-24 (CE0) and P1-26 (CE1) are returned to their default INPUT behavior.
*/
void ILI9341_TFT::PowerDown(void)
{
	EnableDisplay(false);
	Display_DC_SetLow;
	if (_resetPinOn == true){Display_RST_SetLow;}

	if (_hardwareSPI == false)
	{
		Display_SCLK_SetLow;
		Display_SDATA_SetLow;
		Display_CS_SetLow;
	}else{
		bcm2835_spi_end();
	}
}

/*!
	@brief: Method for Hardware & software Reset pin control
*/
void ILI9341_TFT::ResetPIN() {

	if (_resetPinOn == true)
	{
		Display_RST_SetDigitalOutput;
		Display_RST_SetHigh;
		delayMilliSecRDL(5);
		Display_RST_SetLow;
		delayMilliSecRDL(20);
		Display_RST_SetHigh;
		delayMilliSecRDL(120);
	}else{
		writeCommand(ILI9341_SWRESET); // no hw reset pin, software reset.
		delayMilliSecRDL(120);
	}


}

/*!
	@brief Command Initialization sequence for ILI9341 LCD TFT display
*/
void ILI9341_TFT::cmdInit(void)
{
	uint8_t seqPCA[]    {0x39,0x2C,0x00,0x34, 0x02};
	uint8_t seqPCB[]    {0x00,0xC1,0x30};
	uint8_t seqDTMCA[]  {0x85,0x00,0x78};
	uint8_t seqPWONCS[] {0x64,0x03,0x12,0x81};

	uint8_t seqDFUNCTR[] {0x08,0x82,0x27};
	uint8_t seqGammaP[] {0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00};
	uint8_t seqGammaN[] {0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F};

	// Section 0
	writeCommand(ILI9341_DISPOFF); // Display off
	delayMilliSecRDL(5);
	// Section 1
	writeCommand(ILI9341_PWCTRA); // Power control A
	spiWriteDataBuffer(seqPCA, sizeof(seqPCA));
	writeCommand(ILI9341_PWCTRB); // Power control B 
	spiWriteDataBuffer(seqPCB, sizeof(seqPCB));
	writeCommand(ILI9341_DTMCTRA); // Driver timing control A
	spiWriteDataBuffer(seqDTMCA, sizeof(seqDTMCA));
	writeCommand(ILI9341_DTMCTRC); // Driver timing control C
	writeData(0x00);
	writeData(0x00);
	writeCommand(ILI9341_PWONCS); // Power on sequence control
	spiWriteDataBuffer(seqPWONCS, sizeof(seqPWONCS));
	writeCommand(ILI9341_PURTCTR); // Pump ratio control (This has a subtle effect on colors/saturation.)
	writeData(0x20); // Try 0x00 or 0x30 if color's saturated or dim 
	// Section 2
	writeCommand(ILI9341_PWCTR1); // Power control 1
	writeData(0x23);
	writeCommand(ILI9341_PWCTR2); // Power control 2
	writeData(0x10);
	writeCommand(ILI9341_VMCTR1); // VCM control
	writeData(0x3E);
	writeData(0x28);
	writeCommand(ILI9341_VMCTR2); // VCM control2
	writeData(0x86);
	writeCommand(ILI9341_MADCTL); //  Memory Access Control
	writeData(0x48);
	writeCommand(ILI9341_VSCRSADD); // // Vertical scroll zero
	writeData(0x00);
	writeCommand(ILI9341_PIXFMT);// COLMOD: Pixel Format Set
	writeData(0x55);
	writeCommand(ILI9341_FRMCTR1); // Frame Rate Ctrl (Normal Mode/Full Colors)
	writeData(0x00);
	writeData(0x18);
	writeCommand(ILI9341_DFUNCTR); // Display Function Control
	spiWriteDataBuffer(seqDFUNCTR, sizeof(seqDFUNCTR));
	writeCommand(ILI9341_EN3GAM); // 3 Gamma Function Disable
	writeData(0);
	// Section 3
	writeCommand(ILI9341_GAMMASET); // Gamma curve selected
	writeData(0x01);
	writeCommand(ILI9341_GMCTRP1); // Set Gamma P1
	spiWriteDataBuffer(seqGammaP, sizeof(seqGammaP));
	writeCommand(ILI9341_GMCTRN1); // Set Gamma N1
	spiWriteDataBuffer(seqGammaN, sizeof(seqGammaN));
	// Section 4
	writeCommand(ILI9341_SLPOUT); // Exit Sleep
	delayMilliSecRDL(150);
	writeCommand(ILI9341_DISPON); // Display on
	delayMilliSecRDL(150);
}

/*!
	@brief   Set origin of (0,0) and orientation of TFT display
	@param   rotation  The index for rotation, from 0-3 inclusive, enum
*/
void ILI9341_TFT::setRotation(TFT_rotate_e rotation) {
	uint8_t madctlData = 0;
	switch (rotation)
	{
	case TFT_Degrees_0 :
		madctlData = (MADCTL_MX | MADCTL_BGR);
		_width = _widthStartTFT ;
		_height = _heightStartTFT ;
	break;
	case TFT_Degrees_90:
		madctlData = (MADCTL_MV | MADCTL_BGR);
		_width = _heightStartTFT ;
		_height = _widthStartTFT ;
	break;
	case TFT_Degrees_180:
		madctlData = (MADCTL_MY | MADCTL_BGR);
		_width = _widthStartTFT ;
		_height = _heightStartTFT ;
	break;
	case TFT_Degrees_270:
		madctlData = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
		_width = _heightStartTFT ;
		_height = _widthStartTFT ;
	break;
}
	writeCommand(ILI9341_MADCTL);
	writeData(madctlData);
}

/*!
	@brief   Enable/Disable display color inversion
	@param   invert True to invert, False to have normal color
*/
void ILI9341_TFT::invertDisplay(bool invert) {
	if(invert) {
		writeCommand(ILI9341_INVON);
	} else {
		writeCommand(ILI9341_INVOFF);
	}
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on, false disable
*/
void ILI9341_TFT::EnableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(ILI9341_SLPOUT);
		delayMilliSecRDL(120);
		writeCommand(ILI9341_DISPON);
	} else {
		writeCommand(ILI9341_DISPOFF);
		writeCommand(ILI9341_SLPIN);
		delayMilliSecRDL(120);
	}
}

/*!
	@brief Scroll display memory
	@param pixelY How many pixels to scroll display by.
*/

void ILI9341_TFT::scrollTo(uint16_t pixelY) {
	writeCommand(ILI9341_VSCRSADD);
	writeData(pixelY >> 8);
	writeData(pixelY & 0xff);
}

/*!
	@brief To Return Display to normal mode
	@note used after scroll set for example
*/
void ILI9341_TFT::NormalMode(void){writeCommand(ILI9341_NORON);}

/*!
	@brief   Set the height of the Top and Bottom Scroll Margins
	@param   top The height of the Top scroll margin
	@param   bottom The height of the Bottom scroll margin
 */

void ILI9341_TFT::setScrollMargins(uint16_t top, uint16_t bottom) {
	// TFA+VSA+BFA must equal 320
	if (top + bottom <= _heightStartTFT ) 
	{
		uint16_t middle = _heightStartTFT  - (top + bottom);
		uint8_t dataBuffer[6];
		dataBuffer[0] = top >> 8;
		dataBuffer[1] = top & 0xff;
		dataBuffer[2] = middle >> 8;
		dataBuffer[3] = middle & 0xff;
		dataBuffer[4] = bottom >> 8;
		dataBuffer[5] = bottom & 0xff;
		writeCommand(ILI9341_VSCRDEF);
		spiWriteDataBuffer(dataBuffer, sizeof(dataBuffer));
	}
}

/*!
	@brief Set the address window - the rectangle we will write to RAM with
			the next chunk of SPI data writes. The ILI9341 will automatically wrap
			the data as each row is filled
	@param x0  TFT memory 'x' origin
	@param y0  TFT memory 'y' origin
	@param x1  Width of rectangle
	@param y1  Height of rectangle
*/
void ILI9341_TFT::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

	uint8_t x0Higher = x0 >> 8;
	uint8_t x0Lower  = x0 &  0xFF;
	uint8_t y0Higher = y0 >> 8;
	uint8_t y0Lower  = y0 &  0xFF;
	uint8_t x1Higher = x1 >> 8;
	uint8_t x1Lower  = x1 &  0xFF;
	uint8_t y1Higher = y1 >> 8;
	uint8_t y1Lower  = y1 &  0xFF;
	uint8_t seqCASET[]    {x0Higher,x0Lower,x1Higher,x1Lower};
	uint8_t seqPASET[]    {y0Higher,y0Lower,y1Higher,y1Lower};
	writeCommand(ILI9341_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(ILI9341_PASET); //Row address set
	spiWriteDataBuffer(seqPASET, sizeof(seqPASET));
	writeCommand(ILI9341_RAMWR); // Write to RAM*/
}
