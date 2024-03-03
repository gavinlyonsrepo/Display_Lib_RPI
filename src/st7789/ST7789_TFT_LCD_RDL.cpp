/*!
	@file ST7789_TFT_LCD_RDL.cpp
	@author   Gavin Lyons
	@brief    Source file for ST7789 TFT LCD library. Contains driver methods.
	Project Name: Display_Lib_RPI
*/

#include "../../include/st7789/ST7789_TFT_LCD_RDL.hpp"

// Section:  Defines

// ST7789 registers + Commands

// ST7789 general purpose
#define ST7789_NOP     0x00 /**< Non operation */
#define ST7789_SWRESET 0x01 /**< Soft Reset */

// ST7789 Modes
#define ST7789_SLPIN    0x10 /**< Sleep ON */
#define ST7789_SLPOUT   0x11 /**< Sleep OFF */
#define ST7789_PTLON    0x12 /**< Partial mode */
#define ST7789_NORON    0x13 /**< Normal Display */
#define ST7789_INVOFF   0x20 /**< Display invert off */
#define ST7789_INVON    0x21 /**< Display Invert on */
#define ST7789_DISPOFF  0x28 /**< Display off */
#define ST7789_DISPON   0x29 /**< Display on */
#define ST7789_IDLE_ON  0x39 /**< Idle Mode ON */
#define ST7789_IDLE_OFF 0x38 /**< Idle Mode OFF */

// ST7789 Addressing
#define ST7789_CASET    0x2A /**< Column address set */
#define ST7789_RASET    0x2B /**<  Page address set */
#define ST7789_RAMWR    0x2C /**< Memory write */
#define ST7789_RAMRD    0x2E /**< Memory read */
#define ST7789_PTLAR    0x30 /**< Partial Area */
#define ST7789_VSCRDEF  0x33 /**< Vertical scroll def */
#define ST7789_SRLBTT   0x28 /**< Scroll direction bottom to top */
#define ST7789_SRLTTB   0x30 /**< Scroll direction top to bottom */
#define ST7789_COLMOD   0x3A /**< Interface Pixel Format */
#define ST7789_MADCTL   0x36 /**< Memory Access Control */
#define ST7789_VSCRSADD 0x37 /**< Vertical Access Control */

// Frame Rate Control
#define ST7789_FRMCTR1 0xB1 /**< Normal */
#define ST7789_FRMCTR2 0xB2 /**< idle */
#define ST7789_FRMCTR3 0xB3 /**< Partial */

#define ST7789_INVCTR  0xB4 /**< Display Inversion control */
#define ST7789_DISSET5 0xB6 /**< Display Function set */

#define ST7789_RDID1   0xDA /**< read ID1 */
#define ST7789_RDID2   0xDB /**< read ID2 */
#define ST7789_RDID3   0xDC /**< read ID3 */
#define ST7789_RDID4   0xDD /**< read ID4 */

// ST7789 color control
#define ST7789_GMCTRP1 0xE0 /**< Positive Gamma Correction Setting */
#define ST7789_GMCTRN1 0xE1 /**< Negative Gamma Correction Setting */

// Memory Access Data Control  Register
#define ST7789_MADCTL_MY  0x80 /**< Row Address Order */
#define ST7789_MADCTL_MX  0x40 /**< Column Address Order */
#define ST7789_MADCTL_MV  0x20 /**< Row/Column Order (MV) */
#define ST7789_MADCTL_ML  0x10 /**< Vertical Refresh Order */
#define ST7789_MADCTL_RGB 0x00 /**< RGB order */
#define ST7789_MADCTL_BGR 0x08 /**< BGR order */
#define ST7789_MADCTL_MH  0x04  /**< Horizontal Refresh Order */

/*! @brief Constructor for class ST7789_TFT */
ST7789_TFT :: ST7789_TFT(){}

/*!
	@brief  Init Hardware SPI settings
	@details MSBFIRST, mode 0 , SPI Speed , SPICEX pin
	@note If multiple devices on SPI bus with different settings,
	can be used to refresh ST7789 settings
*/
void ST7789_TFT::TFTSPIHWSettings(void)
{
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

	if (_hertz > 0)
		bcm2835_spi_setClockDivider(bcm2835_aux_spi_CalcClockDivider(_hertz));
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
void ST7789_TFT ::TFTPowerDown(void)
{
	TFTenableDisplay(false);
	Display_DC_SetLow;
	Display_RST_SetLow;

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
	@brief: Method for Hardware Reset pin control
*/
void ST7789_TFT ::TFTResetPIN() {
	Display_RST_SetDigitalOutput;
	Display_RST_SetHigh;
	const uint8_t TFT_RESET_DELAY = 10; /**< Reset delay in mS*/
	delayMilliSecRDL(TFT_RESET_DELAY);
	Display_RST_SetLow;
	delayMilliSecRDL(TFT_RESET_DELAY);
	Display_RST_SetHigh;
	delayMilliSecRDL(TFT_RESET_DELAY);
}

/*!
	@brief  sets up TFT GPIO for Hardware SPi
	@param rst reset GPIO
	@param dc data or command GPIO.
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
void ST7789_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc)
{
	_hardwareSPI = true;
	_Display_RST= rst;
	_Display_DC = dc;
}

/*!
	@brief  sets up TFT GPIO for software SPI
	@param rst reset GPIO
	@param dc data or command GPIO.
	@param cs chip select GPIO
	@param sclk Data clock GPIO
	@param din Data to TFT GPIO
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
void ST7789_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_hardwareSPI = false;
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_Display_RST= rst;
	_Display_DC = dc;
}


/*!
	@brief init routine for ST7789B controller
	@return Error if bcm2835_spi_begin has failed
*/
rpiDisplay_Return_Codes_e ST7789_TFT::TFTST7789Initialize() {
	TFTResetPIN();
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
		std::cout << "Error:TFTST7789Initialize1:  bcm2835_spi_begin :Cannot start spi, Running as root?" << std::endl;
		return rpiDisplay_SPIbeginFail;
	}
	TFTSPIHWSettings();
}
	cmd89();
	AdjustWidthHeight();
	TFTsetRotation(TFT_Degrees_0);
	return rpiDisplay_Success;
}


/*!
	@brief This method defines the Vertical Scrolling Area of the display where:
	@param top_fix_heightTFT describes the Top Fixed Area,
	@param bottom_fix_heightTFT describes the Bottom Fixed Area and
	@param _scroll_direction is scroll direction (0 for top to bottom and 1 for bottom to top).
*/
void ST7789_TFT::TFTsetScrollDefinition(uint16_t top_fix_heightTFT, uint16_t bottom_fix_heightTFT, bool _scroll_direction) {
	uint16_t scroll_heightTFT;
	scroll_heightTFT = 320- top_fix_heightTFT - bottom_fix_heightTFT; // ST7789 320x240 VRAM
	writeCommand(ST7789_VSCRDEF);
	writeData(top_fix_heightTFT >> 8);
	writeData(top_fix_heightTFT & 0xFF);
	writeData(scroll_heightTFT >> 8);
	writeData(scroll_heightTFT & 0xFF);
	writeData(bottom_fix_heightTFT >> 8);
	writeData(bottom_fix_heightTFT & 0xFF);
	writeCommand(ST7789_MADCTL);

	if (_scroll_direction) {
		writeData(ST7789_SRLBTT); //bottom to top
	} else {
		writeData(ST7789_SRLTTB); //top to bottom
	}
}

/*!
	@brief: This method is used together with the TFTsetScrollDefinition.
*/
void ST7789_TFT ::TFTVerticalScroll(uint16_t _vsp) {
	writeCommand(ST7789_VSCRSADD);
	writeData(_vsp >> 8);
	writeData(_vsp & 0xFF);
}

/*!
	@brief Toggle the invert mode
	@param invert true invert off , false invert on
*/
void ST7789_TFT ::TFTchangeInvertMode(bool invert) {
	if(invert) {
		writeCommand(ST7789_INVOFF);
	} else {
		writeCommand(ST7789_INVON);
	}
}

/*!
	@brief Toggle the partial display mode
	@param partialDisplay true  on false  off
*/
void ST7789_TFT ::TFTpartialDisplay(bool partialDisplay){
	if(partialDisplay) {
		writeCommand(ST7789_PTLON);
	} else {
		writeCommand(ST7789_NORON);
	}
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on false disable
*/
void ST7789_TFT ::TFTenableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(ST7789_DISPON);
	} else {
		writeCommand(ST7789_DISPOFF);
	}
}

/*!
	@brief Toggle the idle display mode
	@param idleMode true enable on false disable
*/
void ST7789_TFT ::TFTidleDisplay(bool idleMode){
	if( idleMode) {
		writeCommand(ST7789_IDLE_ON);
	} else {
		writeCommand(ST7789_IDLE_OFF);
	}
}

/*!
	@brief Toggle the sleep mode
	@param sleepMode true sleep on false sleep off
*/
void ST7789_TFT ::TFTsleepDisplay(bool sleepMode){
	if(sleepMode) {
		writeCommand(ST7789_SLPIN);
		delayMilliSecRDL(5);
	} else {
		writeCommand(ST7789_SLPOUT);
		delayMilliSecRDL(120);
	}
}

/*!
	@brief: change rotation of display.
	@param mode TFT_rotate_e enum
	0 = Normal
	1=  90 rotate
	2 = 180 rotate
	3 =  270 rotate
*/
void ST7789_TFT ::TFTsetRotation(TFT_rotate_e mode) {
	uint8_t madctl = 0;

	switch (mode) {
		case TFT_Degrees_0 :
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_XStart = _colstart;
			_YStart = _rowstart;
			break;
		case TFT_Degrees_90:
			madctl = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			_YStart = _colstart2;
			_XStart = _rowstart;
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			break;
		case TFT_Degrees_180:
			madctl = ST7789_MADCTL_RGB;
			_XStart = _colstart2;
			_YStart = _rowstart2;
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case TFT_Degrees_270:
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			_YStart = _colstart;
			_XStart = _rowstart2;
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			break;
	}
	TFT_rotate = mode;
	writeCommand(ST7789_MADCTL);
	writeData(madctl);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param colOffset Column offset
	@param rowOffset row offset
	@param width_TFT width in pixels
	@param height_TFT height in pixels
	@note  The offsets can be adjusted for any issues with manufacture tolerance/defects
*/
void ST7789_TFT  :: TFTInitScreenSize(uint8_t colOffset, uint8_t rowOffset, uint16_t width_TFT, uint16_t height_TFT)
{
	_colstart = colOffset;
	_rowstart = rowOffset;
	_XStart = colOffset;
	_YStart = rowOffset;

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
		-# rpiDisplay_SPICEXPin SPI pin  incorrect value
		-# rpiDisplay_SPIbeginFail bcm2835_spi_begin has failed (upstream)
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
rpiDisplay_Return_Codes_e ST7789_TFT::TFTInitSPI(uint32_t hertz, uint8_t SPICE_Pin)
{
	if (SPICE_Pin >= 2)
	{
		std::cout << "Error:TFTInitSPI 2: SPICE_PIN value incorrect :" << SPICE_Pin <<std::endl;
		return  rpiDisplay_SPICEXPin;
	}

	_SPICEX_pin = SPICE_Pin;
	_hertz = hertz;

	return TFTST7789Initialize();
}

/*!
	@brief return Display to normal mode
	@note used after scroll set for example
*/
void ST7789_TFT::TFTNormalMode(void){writeCommand(ST7789_NORON);}

/*!
	@brief intialise PCBtype and SPI, Software SPI
	@param CommDelay uS GPIO delay used in software SPI
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
void ST7789_TFT::TFTInitSPI(uint16_t CommDelay)
{
	HighFreqDelaySet(CommDelay);
	TFTST7789Initialize();
}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ST7789_TFT::HighFreqDelayGet(void){return _HighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ST7789_TFT::HighFreqDelaySet(uint16_t CommDelay){_HighFreqDelay = CommDelay;}

/*!
	@brief SPI displays set an address window rectangle for blitting pixels
	@param  x0 Top left corner x coordinate
	@param  y0  Top left corner y coordinate
	@param  x1  Width of window
	@param  y1  Height of window
	@note virtual function overloads graphics library
 */
void ST7789_TFT::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	uint8_t x0Higher = (x0 >> 8) + _XStart;
	uint8_t x0Lower  = (x0 &  0xFF) + _XStart;
	uint8_t y0Higher = (y0 >> 8) + _YStart;
	uint8_t y0Lower  = (y0 &  0xFF) + _YStart;
	uint8_t x1Higher = (x1 >> 8) + _XStart;
	uint8_t x1Lower  = (x1 &  0xFF) + _XStart;
	uint8_t y1Higher = (y1 >> 8) + _YStart;
	uint8_t y1Lower  = (y1 &  0xFF) + _YStart;
	uint8_t seqCASET[]    {x0Higher ,x0Lower,x1Higher,x1Lower};
	uint8_t seqRASET[]    {y0Higher,y0Lower,y1Higher,y1Lower};
	writeCommand(ST7789_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(ST7789_RASET); //Row address set
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(ST7789_RAMWR); // Write to RAM*/
}


/*!
	@brief Command Initialization sequence for ST7789 display
*/
void ST7789_TFT::cmd89(void)
{
	uint8_t CASETsequence[] {0x00, 0x00, uint8_t(_widthStartTFT  >> 8),  uint8_t(_widthStartTFT  & 0xFF)};
	uint8_t RASETsequence[] {0x00, 0x00, uint8_t(_heightStartTFT >> 8) , uint8_t(_heightStartTFT & 0XFF)};
	
	writeCommand(ST7789_SWRESET);
	delayMilliSecRDL(150);
	writeCommand(ST7789_SLPOUT);
	delayMilliSecRDL(500);
	writeCommand(ST7789_COLMOD); //Set color mode
	writeData(0x55); // 16 bit color
	delayMilliSecRDL(10);

	writeCommand(ST7789_MADCTL); // Mem access ctrl (directions)
	writeData(0x08); // Row/col address, top-bottom refresh

	writeCommand(ST7789_CASET);  //Column address set
	spiWriteDataBuffer(CASETsequence, sizeof(CASETsequence));
	writeCommand(ST7789_RASET);  //Row address set
	spiWriteDataBuffer(RASETsequence, sizeof(RASETsequence));

	writeCommand(ST7789_INVON);
	delayMilliSecRDL(10);
	writeCommand(ST7789_NORON);
	delayMilliSecRDL(10);
	writeCommand(ST7789_DISPON);
	delayMilliSecRDL(10);
}

/*!
	@brief Initialization  width and height code common to all ST7789 displays
	@note ST7789 display require an offset calculation which differs for different size displays
	only tested on 1.69 240 X 280 display
*/
void ST7789_TFT::AdjustWidthHeight() {
	if (_width == 240 && _height == 240) {
	// 1.3", 1.54" displays (right justified)
		_rowstart = (320 - _height);
		_rowstart2 = 0;
		_colstart = _colstart2 = (240 - _width);
	} else if (_width == 135 && _height == 240) {
	// 1.14" display (centered, with odd size)
		_rowstart = _rowstart2 = (int)((320 - _height) / 2);
		_colstart = (int)((240 - _width + 1) / 2);
		_colstart2 = (int)((240 - _width) / 2);
	} else {
	// 1.47", 1.69, 1.9", 2.0" displays (centered)
		_rowstart = _rowstart2 = (int)((320 - _height) / 2);
		_colstart = _colstart2 = (int)((240 - _width) / 2);
	}
}

//**************** EOF *****************
