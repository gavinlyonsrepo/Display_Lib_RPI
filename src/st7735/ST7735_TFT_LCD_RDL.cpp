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
	@brief Call when powering down TFT
	@return 
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinFree
		-# rpiDisplay_SPICloseFailure
		-# rpiDisplay_GpioChipDevice
	@note Turns off Display Sets GPIO low and turns off SPI
*/
rpiDisplay_Return_Codes_e  ST7735_TFT ::TFTPowerDown(void)
{
	TFTchangeMode(TFT_Display_off_mode);
	uint8_t ErrorFlag = 0; // Becomes >0 in event of error

	// 1. Free Reset & DC GPIO lines
	int GpioResetErrorStatus = 0;
	int GpioDCErrorStatus = 0;
	Display_RST_SetLow;
	Display_DC_SetLow;
	GpioResetErrorStatus = Display_GPIO_FREE_RST;
	GpioDCErrorStatus  =  Display_GPIO_FREE_DC;

	if (GpioResetErrorStatus < 0 ){

		fprintf(stderr,"Error: Can't Free RST GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
		ErrorFlag = 2;
	}else if (GpioDCErrorStatus  < 0 ){
		fprintf(stderr,"Error: Can't Free CD GPIO (%s)\n", lguErrorText(GpioDCErrorStatus));
		ErrorFlag = 2;
	}

if (_hardwareSPI == false)
{
	// 2A Software SPI only , free other 3 GPIO, CLK DATA & CS.
	int GpioCSErrorStatus = 0;
	int GpioCLKErrorStatus = 0;
	int GpioSDATAErrorStatus = 0;
	Display_CS_SetLow;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
	GpioCSErrorStatus = Display_GPIO_FREE_CS;
	GpioCLKErrorStatus =  Display_GPIO_FREE_CLK;
	GpioSDATAErrorStatus =   Display_GPIO_FREE_SDATA;
	if (GpioCSErrorStatus < 0 ){
		fprintf(stderr,"Error: Can't Free CS GPIO (%s)\n", lguErrorText(GpioCSErrorStatus ));
		ErrorFlag = 2;
	}else if (GpioCLKErrorStatus< 0 ){
		fprintf(stderr,"Error: Can't Free CLK GPIO t (%s)\n", lguErrorText(GpioCLKErrorStatus));
		ErrorFlag = 2;
	}else if (GpioSDATAErrorStatus< 0 ){
		fprintf(stderr, "Error: Can't free DATA GPIO (%s)\n", lguErrorText(GpioSDATAErrorStatus));
		ErrorFlag = 2;
	}
}else{
	// 2B hardware SPI only, Closes the open SPI device
	int spiErrorStatus = 0;
	spiErrorStatus =  Display_CLOSE_SPI;
	if (spiErrorStatus <0)
	{
		fprintf(stderr, "Error: Cannot Close SPI device :(%s)\n", lguErrorText(spiErrorStatus));
		ErrorFlag = 3;
	}
}
	// 3A Closes the opened gpiochip device.
	int GpioCloseStatus = 0;
	GpioCloseStatus =Display_CLOSE_GPIO_HANDLE; // close gpiochip
	if ( GpioCloseStatus < 0)
	{
		fprintf(stderr,"Error: Failed to close lgGpioChipclose %d , error :  (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		ErrorFlag = 4;
	}

	// 4 Check error flag ( we don't want to return early just for one failure)
	switch (ErrorFlag)
	{
		case 0:return rpiDisplay_Success;break;
		case 2:return rpiDisplay_GpioPinFree;break;
		case 3:return rpiDisplay_SPICloseFailure;break;
		case 4:return rpiDisplay_GpioChipDevice;break;
		default:printf("Warning:Unknown error flag value in SPI-PowerDown"); break;
	}
	return rpiDisplay_Success;
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
void ST7735_TFT::TFTSetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_hardwareSPI = false;
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_Display_RST= rst;
	_Display_DC = dc;
}

/*! 
	@brief init the SPI
	@return 
		rpiDisplay_Success
		rpiDisplay_GpioPinClaim
		rpiDisplay_SPIOpenFailure
		rpiDisplay_GpioChipDevice;
*/
rpiDisplay_Return_Codes_e ST7735_TFT::TFTSPIInit(void)
{
	//  1 open gpio Device open
	_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Error : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rpiDisplay_GpioChipDevice;
	}
	// 2 reset routine GPIO pin
	if (TFTResetPin() != rpiDisplay_Success){return rpiDisplay_GpioPinClaim;}
	// 3 Data or command routine GPIO pin 
	if (TFTDataCommandPin() != rpiDisplay_Success){return rpiDisplay_GpioPinClaim;}

	if (_hardwareSPI == false)
	{
		// 4A Setup Software SPI for the 3 other GPIO : SCLK, Data & CS
		if (TFTClock_Data_ChipSelect_Pins() != rpiDisplay_Success){return rpiDisplay_GpioPinClaim;}
	}else{
		// 4B Open Hardware SPI
		_spiHandle = Display_OPEN_SPI;
		if ( _spiHandle  < 0)
		{
			fprintf(stderr, "Error : Cannot open SPI :(%s)\n", lguErrorText( _spiHandle ));
			return rpiDisplay_SPIOpenFailure;
		}
	}
	return rpiDisplay_Success;
}

/*!
	@brief Method for Hardware Reset pin control
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinClaim
*/
rpiDisplay_Return_Codes_e ST7735_TFT::TFTResetPin() {
	// Claim GPIO as outputs for RST line
	int GpioResetErrorStatus = 0;
	GpioResetErrorStatus= Display_RST_SetDigitalOutput;
	if (GpioResetErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}
	Display_RST_SetHigh;
	delayMilliSecRDL(TFT_RESET_DELAY);
	Display_RST_SetLow;
	delayMilliSecRDL(TFT_RESET_DELAY);
	Display_RST_SetHigh;
	delayMilliSecRDL(TFT_RESET_DELAY);
	return rpiDisplay_Success;
}

/*!
	@brief Method for Data or Command pin setup
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinClaim
*/
rpiDisplay_Return_Codes_e ST7735_TFT::TFTDataCommandPin(void) {
	
	// Claim GPIO as outputs for DC line
	int GpioDCErrorStatus = 0;
	GpioDCErrorStatus= Display_DC_SetDigitalOutput;
	if (GpioDCErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim DC GPIO for output (%s)\n", lguErrorText(GpioDCErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}
	Display_DC_SetLow;
	return rpiDisplay_Success;
}

/*!
	@brief Method for Clock, data and chip select  pin setup routine for software SPI.
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinClaim
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTClock_Data_ChipSelect_Pins(void)
{
	// Claim 3 GPIO as outputs
	int GpioCSErrorStatus = 0;
	int GpioClockErrorStatus = 0;
	int GpioSDATAErrorStatus = 0;
	GpioCSErrorStatus= Display_CS_SetDigitalOutput;
	GpioClockErrorStatus= Display_SCLK_SetDigitalOutput;
	GpioSDATAErrorStatus= Display_SDATA_SetDigitalOutput;

	if (GpioCSErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CS GPIO for output (%s)\n", lguErrorText(GpioCSErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}
	if (GpioClockErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}
	if (GpioSDATAErrorStatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioSDATAErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}

	Display_CS_SetHigh;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
	return rpiDisplay_Success;
}

/*!
	@brief init sub-routine ST7735R Green Tab
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# upstream error code from TFTSPIinit()
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTGreenTabInitialize() {
	rpiDisplay_Return_Codes_e DrawCharReturnCode;
	DrawCharReturnCode = TFTSPIInit();
	if(DrawCharReturnCode  != rpiDisplay_Success) return DrawCharReturnCode;
	Rcmd1();
	Rcmd2green();
	Rcmd3();
	TFT_PCBtype = TFT_ST7735R_Green;
	return rpiDisplay_Success;
}


/*!
	@brief ST7735R Red Tab Init Red PCB version
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# upstream error code from TFTSPIinit()
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTRedTabInitialize() {
	rpiDisplay_Return_Codes_e DrawCharReturnCode;
	DrawCharReturnCode = TFTSPIInit();
	if(DrawCharReturnCode  != rpiDisplay_Success) return DrawCharReturnCode;
	Rcmd1();
	Rcmd2red();
	Rcmd3();
	TFT_PCBtype = TFT_ST7735R_Red;
	return rpiDisplay_Success;
}

/*!
	@brief Init Routine ST7735R Black Tab (ST7735S)
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# upstream error code from TFTSPIinit()
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTBlackTabInitialize() {

	rpiDisplay_Return_Codes_e DrawCharReturnCode;
	DrawCharReturnCode = TFTSPIInit();
	if(DrawCharReturnCode  != rpiDisplay_Success) return DrawCharReturnCode;
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
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# upstream error code from TFTSPIinit()
*/
rpiDisplay_Return_Codes_e ST7735_TFT ::TFTST7735BInitialize() {
	rpiDisplay_Return_Codes_e DrawCharReturnCode;
	DrawCharReturnCode = TFTSPIInit();
	if(DrawCharReturnCode  != rpiDisplay_Success) return DrawCharReturnCode;
	Bcmd();
	TFT_PCBtype = TFT_ST7735B ;
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
	@brief init sub-routine for ST7735B controller
*/
void ST7735_TFT ::Bcmd() {
	uint8_t seq6[] {0x09, 0x16, 0x09, 0x20, 0x21, 0x1B, 0x13, 0x19, 0x17, 0x15, 0x1E, 0x2B, 0x04, 0x05, 0x02, 0x0E};
	uint8_t seq7[] {0x0B, 0x14, 0x08, 0x1E, 0x22, 0x1D, 0x18, 0x1E, 0x1B, 0x1A, 0x24, 0x2B, 0x06, 0x06, 0x02, 0x0F};
	uint8_t seq8[] {0x00, 0x02, 0x08 , 0x81};
	uint8_t seq9[] {0x00, 0x01, 0x08 , 0xA0};
	writeCommand(ST7735_SWRESET);
	delayMilliSecRDL(50);
	writeCommand(ST7735_SLPOUT);
	delayMilliSecRDL(500);
	writeCommand(ST7735_COLMOD);
	writeData(0x05);
	delayMilliSecRDL(10);
	writeCommand(ST7735_FRMCTR1);
	writeData(0x00);
	writeData(0x06);
	writeData(0x03);
	delayMilliSecRDL(10);
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
	delayMilliSecRDL(10);
	writeCommand(ST7735_PWCTR2);
	writeData(0x05);
	writeCommand(ST7735_PWCTR3);
	writeData(0x01);
	writeData(0x02);
	writeCommand(ST7735_VMCTR1);
	writeData(0x3C);
	writeData(0x38);
	delayMilliSecRDL(10);
	writeCommand(ST7735_PWCTR6);
	writeData(0x11);
	writeData(0x15);
	writeCommand(ST7735_GMCTRP1);
	spiWriteDataBuffer(seq6, sizeof(seq6));
	writeCommand(ST7735_GMCTRN1);
	spiWriteDataBuffer(seq7, sizeof(seq7));
	delayMilliSecRDL(10);
	writeCommand(ST7735_CASET);
	spiWriteDataBuffer(seq8, sizeof(seq8));
	writeCommand(ST7735_RASET);
	spiWriteDataBuffer(seq9, sizeof(seq9));
	writeCommand(ST7735_NORON);
	delayMilliSecRDL(10);
	writeCommand(ST7735_DISPON);
	delayMilliSecRDL(500);
}


/*!
	@brief init sub-routine
*/
void ST7735_TFT ::Rcmd1() {

	uint8_t seq1[] { 0x01, 0x2C, 0x2D };
	uint8_t seq3[] { 0xA2, 0x02, 0x84 };
	writeCommand(ST7735_SWRESET);
	delayMilliSecRDL(150);
	writeCommand(ST7735_SLPOUT);
	delayMilliSecRDL(500);
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
	delayMilliSecRDL(10);
	writeCommand(ST7735_DISPON);
	delayMilliSecRDL(100);
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
				delayMilliSecRDL(120);
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
			delayMilliSecRDL(5);
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
void ST7735_TFT::TFTInitScreenSize(uint8_t colOffset, uint8_t rowOffset, uint16_t width_TFT, uint16_t height_TFT)
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
	@param device A SPI device, >= 0. 
	@param channel A SPI channel, >= 0. 
	@param speed The speed of serial communication in bits per second. 
	@param flags The flags may be used to modify the default behaviour. Set to 0(mode 0) for this device.
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@return
		-# rpiDisplay_Success = success
		-# rpiDisplay_WrongInputPCBType see enum choices.
		-# Various other failures modes from upstream functions TFT*Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rpiDisplay_Return_Codes_e ST7735_TFT::TFTInitPCBType(TFT_PCBtype_e pcbType, int device, int channel, int speed, int flags, int gpioDev)
{
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;
	rpiDisplay_Return_Codes_e DrawCharReturnCode = rpiDisplay_Success;
	switch(pcbType)
	{
		case TFT_ST7735R_Red : DrawCharReturnCode = TFTRedTabInitialize();break;
		case TFT_ST7735R_Green : DrawCharReturnCode = TFTGreenTabInitialize();break;
		case TFT_ST7735S_Black: DrawCharReturnCode = TFTBlackTabInitialize() ;break;
		case TFT_ST7735B : DrawCharReturnCode = TFTST7735BInitialize() ;break;
		default:
			printf( "Error:TFTInitPCBType 3: Wrong input pcb type\n");
			DrawCharReturnCode = rpiDisplay_WrongInputPCBType;
		break;
	}
	return DrawCharReturnCode;
}

/*!
	@brief intialise PCBtype and SPI, Software SPI
	@param pcbType 4 choices 0-3
	@param CommDelay uS GPIO delay used in software SPI
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@return
		-# rpiDisplay_Success = success
		-# rpiDisplay_WrongInputPCBType see enum choices.
		-# Various other failures modes from upstream functions TFT*Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rpiDisplay_Return_Codes_e ST7735_TFT::TFTInitPCBType(TFT_PCBtype_e pcbType, uint16_t CommDelay, int gpioDev)
{
	HighFreqDelaySet(CommDelay);
	_DeviceNumGpioChip = gpioDev;
	rpiDisplay_Return_Codes_e DrawCharReturnCode = rpiDisplay_Success;
	switch(pcbType)
	{
		case TFT_ST7735R_Red : DrawCharReturnCode = TFTRedTabInitialize();break;
		case TFT_ST7735R_Green : DrawCharReturnCode = TFTGreenTabInitialize();break;
		case TFT_ST7735S_Black: DrawCharReturnCode = TFTBlackTabInitialize() ;break;
		case TFT_ST7735B : DrawCharReturnCode = TFTST7735BInitialize() ;break;
		default:
			printf("Error:TFTInitPCBType 4: Wrong input pcb type:\n");
			DrawCharReturnCode = rpiDisplay_WrongInputPCBType;
		break;
	}
	return DrawCharReturnCode;
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
	uint8_t x0Lower = (x0 & 0xFF) + _XStart;
	uint8_t y0Lower = (y0 & 0xFF) +_YStart;
	uint8_t x1Lower = (x1 & 0xFF) + _XStart;
	uint8_t y1Lower = (y1 & 0xFF) +_YStart;
	uint8_t seqCASET[]   {0x00 ,x0Lower,0x00, x1Lower};
	uint8_t seqRASET[]   {0x00,y0Lower,0x00, y1Lower};
	writeCommand(ST7735_CASET);
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(ST7735_RASET);
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(ST7735_RAMWR); // Write to RAM
}

//**************** EOF *****************
