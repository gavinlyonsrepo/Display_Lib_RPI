/*!
	@file     ST7735_TFT_LCD_RDL.cpp
	@author   Gavin Lyons
	@brief    Source file for ST7735 TFT LCD library.
		Contains driver methods for ST7735 TFT LCD display
	Project Name: Display_Lib_RPI

*/

#include "../../include/st7735/ST7735_TFT_LCD_RDL.hpp"

/*! @brief Constructor for class ST7735_TFT */
ST7735_TFT :: ST7735_TFT(){}


/*!
	@brief  Init Hardware SPI settings
	@details MSBFIRST, mode 0 , SPI Speed , SPICEX pin
	@note If multiple devices on SPI bus with different settings , 
	can be used to refresh ST7735 settings
*/
void ST7735_TFT::TFTSPIHWSettings(void)
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
void ST7735_TFT ::TFTPowerDown(void)
{
	TFTchangeMode(TFT_Display_off_mode);
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
void ST7735_TFT ::TFTResetPIN() {
	Display_RST_SetDigitalOutput;
	Display_RST_SetHigh;
	TFT_MILLISEC_DELAY(TFT_RESET_DELAY);
	Display_RST_SetLow;
	TFT_MILLISEC_DELAY(TFT_RESET_DELAY);
	Display_RST_SetHigh;
	TFT_MILLISEC_DELAY(TFT_RESET_DELAY);
}

/*!
	@brief  sets up TFT GPIO for Hardware SPi
	@param rst reset GPIO
	@param dc data or command GPIO.
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
void ST7735_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc)
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
void ST7735_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_hardwareSPI = false;
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_Display_RST= rst;
	_Display_DC = dc;
}


/*!
	@brief init sub-routine ST7735R Green Tab
	@return Error if bcm2835_spi_begin has failed
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTGreenTabInitialize() {
	TFTResetPIN();
	Display_DC_SetLow;
	Display_DC_SetDigitalOutput;
if (_hardwareSPI == false){
	Display_CS_SetHigh;
	Display_CS_SetDigitalOutput;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
	Display_SCLK_SetDigitalOutput;
	Display_SDATA_SetDigitalOutput;
}else{
	if (!bcm2835_spi_begin())
		return rpiDisplay_SPIbeginFail;
	TFTSPIHWSettings();
}
	Rcmd1();
	Rcmd2green();
	Rcmd3();
	TFT_PCBtype = TFT_ST7735R_Green;
	return rpiDisplay_Success;
}

/*!
	@brief init sub-routine ST7735R Green Tab
*/
void ST7735_TFT ::Rcmd2green() {
	uint8_t seq1[] {0x00, 0x02, 0x00, (0x7F + 0x02)};
	uint8_t seq2[] {0x00, 0x01, 0x00, (0x9F + 0x01)};
	writeCommand(ST7735_CASET);
	spiWriteDataBuffer(seq1, sizeof(seq1));
	writeCommand(ST7735_RASET);
	spiWriteDataBuffer(seq2, sizeof(seq2));
}


/*!
	@brief ST7735R Red Tab Init Red PCB version
	@return Error if bcm2835_spi_begin has failed
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTRedTabInitialize() {
	TFTResetPIN();
	Display_DC_SetLow;
	Display_DC_SetDigitalOutput;
if (_hardwareSPI == false)
{
	Display_CS_SetHigh;
	Display_CS_SetDigitalOutput;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
	Display_SCLK_SetDigitalOutput;
	Display_SDATA_SetDigitalOutput;
}else{
	if (!bcm2835_spi_begin())
		return rpiDisplay_SPIbeginFail;
	TFTSPIHWSettings();
}
	Rcmd1();
	Rcmd2red();
	Rcmd3();
	TFT_PCBtype = TFT_ST7735R_Red;
	return rpiDisplay_Success;
}

/*!
	@brief Init Routine ST7735R Black Tab (ST7735S)
	@return Error if bcm2835_spi_begin has failed
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTBlackTabInitialize() {
	TFTResetPIN();
	Display_DC_SetLow;
	Display_DC_SetDigitalOutput;
if (_hardwareSPI == false)
{
	Display_CS_SetHigh;
	Display_CS_SetDigitalOutput;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
	Display_SCLK_SetDigitalOutput;
	Display_SDATA_SetDigitalOutput;
}else{
	if (!bcm2835_spi_begin())
		return rpiDisplay_SPIbeginFail;
	TFTSPIHWSettings();
}
	Rcmd1();
	Rcmd2red();
	Rcmd3();
	writeCommand(ST7735_MADCTL);
	writeData(0xC0);
	TFT_PCBtype = TFT_ST7735S_Black ;
	return rpiDisplay_Success;
}

/*!
	@brief init routine for ST7735B controller
	@return Error if bcm2835_spi_begin has failed
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTST7735BInitialize() {
	TFTResetPIN();
	Display_DC_SetLow;
	Display_DC_SetDigitalOutput;
if (_hardwareSPI == false)
{
	Display_CS_SetHigh;
	Display_CS_SetDigitalOutput;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
	Display_SCLK_SetDigitalOutput;
	Display_SDATA_SetDigitalOutput;
}else{
	if (!bcm2835_spi_begin())
		return rpiDisplay_SPIbeginFail;
	TFTSPIHWSettings();
}
	Bcmd();
	TFT_PCBtype = TFT_ST7735B ;
	return rpiDisplay_Success;
}


/*!
	@brief init sub-routine for ST7735B controller
*/
void ST7735_TFT ::Bcmd() {
	uint8_t seq6[] {0x09, 0x16, 0x09, 0x20, 0x21, 0x1B, 0x13, 0x19, 0x17, 0x15, 0x1E, 0x2B, 0x04, 0x05, 0x02, 0x0E};
	uint8_t seq7[] {0x0B, 0x14, 0x08, 0x1E, 0x22, 0x1D, 0x18, 0x1E, 0x1B, 0x1A, 0x24, 0x2B, 0x06, 0x06, 0x02, 0x0F};
	uint8_t seq8[] {0x00, 0x02, 0x08 , 0x81};
	uint8_t seq9[] {0x00, 0x01, 0x08 , 0xA0};
	writeCommand(ST7735_SWRESET);
	TFT_MILLISEC_DELAY(50);
	writeCommand(ST7735_SLPOUT);
	TFT_MILLISEC_DELAY(500);
	writeCommand(ST7735_COLMOD);
	writeData(0x05);
	TFT_MILLISEC_DELAY(10);
	writeCommand(ST7735_FRMCTR1);
	writeData(0x00);
	writeData(0x06);
	writeData(0x03);
	TFT_MILLISEC_DELAY(10);
	writeCommand(ST7735_MADCTL);
	writeData(0x08);
	writeCommand(ST7735_DISSET5);
	writeData(0x15);
	writeData(0x02);
	writeCommand(ST7735_INVCTR);
	writeData(0x00);
	writeCommand(ST7735_PWCTR1);
	writeData(0x02);
	writeData(0x70);
	TFT_MILLISEC_DELAY(10);
	writeCommand(ST7735_PWCTR2);
	writeData(0x05);
	writeCommand(ST7735_PWCTR3);
	writeData(0x01);
	writeData(0x02);
	writeCommand(ST7735_VMCTR1);
	writeData(0x3C);
	writeData(0x38);
	TFT_MILLISEC_DELAY(10);
	writeCommand(ST7735_PWCTR6);
	writeData(0x11);
	writeData(0x15);
	writeCommand(ST7735_GMCTRP1);
	spiWriteDataBuffer(seq6, sizeof(seq6));
	writeCommand(ST7735_GMCTRN1);
	spiWriteDataBuffer(seq7, sizeof(seq7));
	TFT_MILLISEC_DELAY(10);
	writeCommand(ST7735_CASET);
	spiWriteDataBuffer(seq8, sizeof(seq8));
	writeCommand(ST7735_RASET);
	spiWriteDataBuffer(seq9, sizeof(seq9));
	writeCommand(ST7735_NORON);
	TFT_MILLISEC_DELAY(10);
	writeCommand(ST7735_DISPON);
	TFT_MILLISEC_DELAY(500);
}


/*!
	@brief init sub-routine
*/
void ST7735_TFT ::Rcmd1() {

	uint8_t seq1[] { 0x01, 0x2C, 0x2D };
	uint8_t seq3[] { 0xA2, 0x02, 0x84 };
	writeCommand(ST7735_SWRESET);
	TFT_MILLISEC_DELAY(150);
	writeCommand(ST7735_SLPOUT);
	TFT_MILLISEC_DELAY(500);
	writeCommand(ST7735_FRMCTR1);

	spiWriteDataBuffer(seq1, sizeof(seq1));
	writeCommand(ST7735_FRMCTR2);
	spiWriteDataBuffer(seq1, sizeof(seq1));
	writeCommand(ST7735_FRMCTR3);
	spiWriteDataBuffer(seq1, sizeof(seq1));
	spiWriteDataBuffer(seq1, sizeof(seq1));
	writeCommand(ST7735_INVCTR);
	writeData(0x07);
	writeCommand(ST7735_PWCTR1);
	spiWriteDataBuffer(seq3, sizeof(seq3));
	writeCommand(ST7735_PWCTR2);
	writeData(0xC5);
	writeCommand(ST7735_PWCTR3);
	writeData(0x0A);
	writeData(0x00);
	writeCommand(ST7735_PWCTR4);
	writeData(0x8A);
	writeData(0x2A);
	writeCommand(ST7735_PWCTR5);
	writeData(0x8A);
	writeData(0xEE);
	writeCommand(ST7735_VMCTR1);
	writeData(0x0E);
	writeCommand(ST7735_INVOFF);
	writeCommand(ST7735_MADCTL);
	writeData(0xC8);
	writeCommand(ST7735_COLMOD);
	writeData(0x05);
}

/*!
	@brief init sub-routine
*/
void ST7735_TFT ::Rcmd2red() {
	uint8_t seq1[] { 0x00, 0x00, 0x00, 0x7F };
	uint8_t seq2[] { 0x00, 0x00, 0x00, 0x9F };
	writeCommand(ST7735_CASET);
	spiWriteDataBuffer(seq1, sizeof(seq1));
	writeCommand(ST7735_RASET);
	spiWriteDataBuffer(seq2, sizeof(seq2));
}

/*!
	@brief init sub-routine
*/
void ST7735_TFT ::Rcmd3() {
	writeCommand(ST7735_GMCTRP1);
	uint8_t seq4[] {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10};
	spiWriteDataBuffer(seq4, sizeof(seq4));
	writeCommand(ST7735_GMCTRN1);
	uint8_t seq5[] {0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10};
	spiWriteDataBuffer(seq5, sizeof(seq5));
	writeCommand(ST7735_NORON);
	TFT_MILLISEC_DELAY(10);
	writeCommand(ST7735_DISPON);
	TFT_MILLISEC_DELAY(100);
}


/*!
	@brief This method defines the Vertical Scrolling Area of the display where:
	@param top_fix_heightTFT describes the Top Fixed Area,
	@param bottom_fix_heightTFT describes the Bottom Fixed Area and
	@param _scroll_direction is scroll direction (0 for top to bottom and 1 for bottom to top).
*/
void ST7735_TFT ::TFTsetScrollDefinition(uint8_t top_fix_heightTFT, uint8_t bottom_fix_heightTFT, bool _scroll_direction) {
	uint8_t scroll_heightTFT;
	scroll_heightTFT = _height - top_fix_heightTFT - bottom_fix_heightTFT;
	writeCommand(ST7735_VSCRDEF);
	writeData(0x00);
	writeData(top_fix_heightTFT);
	writeData(0x00);
	writeData(scroll_heightTFT);
	writeData(0x00);
	writeData(bottom_fix_heightTFT);
	writeCommand(ST7735_MADCTL);
	if (_scroll_direction) {
		if ((TFT_PCBtype == TFT_ST7735R_Red)  || (TFT_PCBtype == TFT_ST7735R_Green)) {
			writeData(0xD8);
		}
		if (TFT_PCBtype == TFT_ST7735S_Black) {
			writeData(0xD0);
		}
		if (TFT_PCBtype == TFT_ST7735B ) {
			writeData(0x18);
		}
	} else {
		if (((TFT_PCBtype == TFT_ST7735R_Red)  || (TFT_PCBtype == TFT_ST7735R_Green))) {
			writeData(0xC8);
		}
		if (TFT_PCBtype == TFT_ST7735S_Black) {
			writeData(0xC0);
		}
		if (TFT_PCBtype == TFT_ST7735B ) {
			writeData(0x08);
		}
	}
}

/*!
	@brief: This method is used together with the TFTsetScrollDefinition.
*/
void ST7735_TFT ::TFTVerticalScroll(uint8_t _vsp) {
	writeCommand(ST7735_VSCRSADD);
	writeData(0x00);
	writeData(_vsp);
}

/*!
	@brief Toggle the invert mode
	@param invertModeOn true invert on false invert off
*/
void ST7735_TFT ::TFTchangeInvertMode(bool invertModeOn) {
	if(invertModeOn) {
		writeCommand(ST7735_INVON);
	} else {
		writeCommand(ST7735_INVOFF);
	}
}

/*!
	@brief This changes the mode of the display as:
	@param mode enum TFT_mode value
		-# TFT_Normal_mode: Normal mode.
		-# TFT_Partial_mode: Enable partial mode to work in portions of display
		-# TFT_Idle_mode: Idle_mode consume less current and shows less color
		-# TFT_Sleep_mode: Put display driver section in sleep but rest of the logic works.
		-# TFT_Invert_mode: invert the display
		-# TFT_Display_on_mode: Turn on display
		-# TFT_Display_off_mode: Turn off display
*/
void ST7735_TFT ::TFTchangeMode(TFT_modes_e mode) {
	switch (mode) {
		case TFT_Normal_mode:
			if (TFT_mode ==  TFT_Display_off_mode) {//was in off display?
				writeCommand(ST7735_DISPON);
			}
			if (TFT_mode == TFT_Idle_mode) {//was in idle?
				writeCommand(ST7735_TFT_Idle_modeOF);
			}
			if (TFT_mode == TFT_Sleep_mode) {//was in sleep?
				writeCommand(ST7735_SLPOUT);
				TFT_MILLISEC_DELAY(120);
			}
			if (TFT_mode == TFT_Invert_mode) {//was inverted?
				TFT_mode = TFT_Normal_mode;
				writeCommand(ST7735_INVOFF);
			}
			writeCommand(ST7735_NORON);
			TFT_mode = TFT_Normal_mode;
			break;
		case TFT_Partial_mode:
			writeCommand(ST7735_PTLON);
			TFT_mode = TFT_Partial_mode;
			break;
		case TFT_Idle_mode:
			writeCommand(ST7735_TFT_Idle_modeON);
			TFT_mode = TFT_Idle_mode;
			break;
		case TFT_Sleep_mode:
			writeCommand(ST7735_SLPIN);
			TFT_mode = TFT_Sleep_mode;
			TFT_MILLISEC_DELAY(5);
			return;
		case TFT_Invert_mode:
			writeCommand(ST7735_INVON);
			TFT_mode = TFT_Invert_mode;
			break;
		case TFT_Display_on_mode:
			writeCommand(ST7735_DISPON);
			TFT_mode = TFT_Display_on_mode;
			break;
		case TFT_Display_off_mode:
			writeCommand(ST7735_DISPOFF);
			TFT_mode =  TFT_Display_off_mode;
			break;
	}//switch
}


/*!
	@brief: change rotation of display.
	@param mode TFT_rotate_e enum
	0 = Normal
	1=  90 rotate
	2 = 180 rotate
	3 =  270 rotate
	@note if on your display colors are wrong after rotate change
	you may have chosen wrong display pcb type.
*/
void ST7735_TFT ::TFTsetRotation(TFT_rotate_e mode) {
	uint8_t madctl = 0;

	switch (mode) {
		case TFT_Degrees_0 :
			if (TFT_PCBtype == TFT_ST7735S_Black ){
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR;
			}
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_XStart = _colstart;
			_YStart = _rowstart;
			break;
		case TFT_Degrees_90:
			if (TFT_PCBtype == TFT_ST7735S_Black )
			{
				madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
			}
			_YStart = _colstart;
			_XStart = _rowstart;
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			break;
		case TFT_Degrees_180:
			if (TFT_PCBtype == TFT_ST7735S_Black)
			{
				madctl = ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_BGR;
			}
			_XStart = _colstart;
			_YStart = _rowstart;
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case TFT_Degrees_270:
			if (TFT_PCBtype == TFT_ST7735S_Black){
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
			}
			_YStart = _colstart;
			_XStart = _rowstart;
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			break;
	}
	TFT_rotate = mode;
	writeCommand(ST7735_MADCTL);
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
void ST7735_TFT  :: TFTInitScreenSize(uint8_t colOffset, uint8_t rowOffset, uint16_t width_TFT, uint16_t height_TFT)
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
	@brief intialise PCBtype and SPI,  Hardware SPI
	@param pcbType 4 choices 0-3
	@param hertz  SPI Clock frequency in Hz, HW SPI only MAX 125 Mhz , MIN 30Khz(RPI3). typical/tested 8000000
	@param SPICE_Pin which SPI CE/CS pin to use 0 = SPICE0 GPIO08 RPI3, 1 = SPICE1 GPIO07 RPI3
	@return 
		-# rpiDisplay_Success = success
		-# rpiDisplay_SPICEXPin SPI pin  incorrect . 
		-# rpiDisplay_WrongInputPCBType see enum choices.
		-# rpiDisplay_SPIbeginFail bcm2835_spi_begin has failed
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
rpiDisplay_Return_Codes_e ST7735_TFT::TFTInitPCBType(TFT_PCBtype_e pcbType, uint32_t hertz, uint8_t SPICE_Pin)
{
	if (SPICE_Pin >= 2)
	{
		std::cout << "Error:TFTInitPCBType 2: SPICE_PIN value incorrect :" << SPICE_Pin <<std::endl;
		return  rpiDisplay_SPICEXPin;
	}
	
	_SPICEX_pin = SPICE_Pin;
	_hertz = hertz;
	bool initErrorFlag = false;
	
	switch(pcbType)
	{
		case TFT_ST7735R_Red :
			if(TFTRedTabInitialize() != rpiDisplay_Success)
				initErrorFlag = true;
		break;
		case TFT_ST7735R_Green:
			if(TFTGreenTabInitialize() != rpiDisplay_Success)
				initErrorFlag = true;
		break;
		case TFT_ST7735S_Black:
			if(TFTBlackTabInitialize() != rpiDisplay_Success)
				initErrorFlag = true;
		break;
		case TFT_ST7735B :
			if(TFTST7735BInitialize() != rpiDisplay_Success)
				initErrorFlag = true;
		break;
		default:
			std::cout << "Error:TFTInitPCBType 3: Wrong input pcb type:" << pcbType<<std::endl;
			return rpiDisplay_WrongInputPCBType;
		break;
	}
	
	if(initErrorFlag)
	{
		std::cout << "Error:TFTInitPCBType 4:  bcm2835_spi_begin :Cannot start spi, Running as root?" << std::endl;
		return rpiDisplay_SPIbeginFail;
	}
	
	return rpiDisplay_Success;
}

/*!
	@brief intialise PCBtype and SPI, SOftware SPI
	@param pcbType 4 choices 0-3
	@param CommDelay uS GPIO delay used in software SPI
		-# rpiDisplay_Success = success
		-# rpiDisplay_WrongInputPCBType see enum choices.
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
rpiDisplay_Return_Codes_e ST7735_TFT::TFTInitPCBType(TFT_PCBtype_e pcbType, uint16_t CommDelay)
{
	HighFreqDelaySet(CommDelay);
	switch(pcbType)
	{
		case TFT_ST7735R_Red : TFTRedTabInitialize(); break;
		case TFT_ST7735R_Green: TFTGreenTabInitialize(); break;
		case TFT_ST7735S_Black: TFTBlackTabInitialize(); break;
		case TFT_ST7735B : TFTST7735BInitialize(); break;
		default:
			std::cout << "Error:TFTInitPCBType: Wrong input pcb type:" << pcbType<<std::endl;
			return rpiDisplay_WrongInputPCBType;
		break;
	}
	return rpiDisplay_Success;
}


/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ST7735_TFT::HighFreqDelayGet(void){return _HighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ST7735_TFT::HighFreqDelaySet(uint16_t CommDelay){_HighFreqDelay = CommDelay;}

/*!
	@brief SPI displays set an address window rectangle for blitting pixels
	@param  x0 Top left corner x coordinate
	@param  y0  Top left corner y coordinate
	@param  x1  Width of window
	@param  y1  Height of window
	@note virtual function overloads graphics library
 */
void ST7735_TFT::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	uint8_t x0Lower = x0 & 0xFF;
	uint8_t y0Lower = y0 & 0xFF;
	uint8_t x1Lower = x1 & 0xFF;
	uint8_t y1Lower = y1 & 0xFF;
	writeCommand(ST7735_CASET);
	writeData(0);
	writeData(x0Lower + _XStart);
	writeData(0);
	writeData(x1Lower + _XStart);
	writeCommand(ST7735_RASET);
	writeData(0);
	writeData(y0Lower +_YStart);
	writeData(0);
	writeData(y1Lower +_YStart);
	writeCommand(ST7735_RAMWR); // Write to RAM
}

//**************** EOF *****************
