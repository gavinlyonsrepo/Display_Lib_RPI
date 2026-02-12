/*!
	@file ST7789_TFT_LCD_RDL.cpp
	@author   Gavin Lyons
	@brief    Source file for ST7789 TFT LCD library. Contains driver methods.
	Project Name: Display_Lib_RPI
*/

#include "../../include/st7789/ST7789_TFT_LCD_RDL.hpp"


/*! @brief Constructor for class ST7789_TFT */
ST7789_TFT :: ST7789_TFT(){}


/*!
	@brief Call when powering down TFT
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinFree
		-# rdlib::SPIOpenClose
	@note Turns off Display Sets GPIO low and turns off SPI
*/
rdlib::Return_Codes_e  ST7789_TFT ::TFTPowerDown(void)
{
	TFTenableDisplay(false);
	uint8_t ErrorFlag = 0; // Becomes > 0 in event of error

	// 1A. free rst GPIO line
	if (_resetPinOn == true)
	{
		int GpioResetErrorStatus = 0;
		Display_RST_SetLow;
		GpioResetErrorStatus = Display_GPIO_FREE_RST;
		if (GpioResetErrorStatus < 0 )
		{
			fprintf(stderr,"Error: Can't Free RST GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
			ErrorFlag = 2;
		}
	}
	// 1B. free DC GPIO line
	int GpioDCErrorStatus = 0;
	Display_DC_SetLow;
	GpioDCErrorStatus  =  Display_GPIO_FREE_DC;
	if (GpioDCErrorStatus  < 0 ){
		fprintf(stderr,"Error:Can't Free CD GPIO (%s)\n", lguErrorText(GpioDCErrorStatus));
		ErrorFlag = 2;
	}

if (_hardwareSPI == false)
{
	// 2A Software SPI only , free other 3 GPIO CLK DATA & CS
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
		fprintf(stderr,"Error:Can't Free CS GPIO (%s)\n", lguErrorText(GpioCSErrorStatus ));
		ErrorFlag = 2;
	}else if (GpioCLKErrorStatus< 0 ){
		fprintf(stderr,"Error:Can't Free CLK GPIO t (%s)\n", lguErrorText(GpioCLKErrorStatus));
		ErrorFlag = 2;
	}else if (GpioSDATAErrorStatus< 0 ){
		fprintf(stderr, "Error:Can't free DATA GPIO (%s)\n", lguErrorText(GpioSDATAErrorStatus));
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
		// open error
		fprintf(stderr,"Error:Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		ErrorFlag = 4;
	}

	// 4 Check error flag (we don't want to return early for any failure)
	switch (ErrorFlag)
	{
		case 0:return rdlib::Success;break;
		case 2:return rdlib::GpioPinFree;break;
		case 3:return rdlib::SPICloseFailure;;break;
		case 4:return rdlib::GpioChipDevice;;break;
		default:fprintf(stderr, "Warning::Unknown error flag value in SPI-PowerDown"); break;
	}
	return rdlib::Success;
}

/*!
	@brief Method for Hardware Reset pin setup and control
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e ST7789_TFT ::TFTResetPin() {
	if (_resetPinOn == true){
		// Claim GPIO as outputs for RST line
		int GpioResetErrorStatus = 0;
		GpioResetErrorStatus= Display_RST_SetDigitalOutput;
		if (GpioResetErrorStatus < 0 )
		{
			fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorStatus));
			return rdlib::GpioPinClaim;
		}
		Display_RST_SetHigh;
		const uint8_t TFT_RESET_DELAY = 10; /**< Reset delay in mS*/
		delayMilliSecRDL(TFT_RESET_DELAY);
		Display_RST_SetLow;
		delayMilliSecRDL(TFT_RESET_DELAY);
		Display_RST_SetHigh;
		delayMilliSecRDL(TFT_RESET_DELAY);
	}
	return rdlib::Success;
}

/*!
	@brief: Method for Data or Command pin setup
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e ST7789_TFT ::TFTDataCommandPin(void) {
	
	// Claim GPIO as outputs for DC line
	int GpioDCErrorStatus = 0;
	GpioDCErrorStatus= Display_DC_SetDigitalOutput;
	if (GpioDCErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim DC GPIO for output (%s)\n", lguErrorText(GpioDCErrorStatus));
		return rdlib::GpioPinClaim;
	}
	Display_DC_SetLow;
	return rdlib::Success;
}

/*!
	@brief: Method for Clock, data and chip select  pin setup routine for software SPI.
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e ST7789_TFT::TFTClock_Data_ChipSelect_Pins(void)
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
		return rdlib::GpioPinClaim;
	}
	if (GpioClockErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorStatus));
		return rdlib::GpioPinClaim;
	}
	if (GpioSDATAErrorStatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioSDATAErrorStatus));
		return rdlib::GpioPinClaim;
	}

	Display_CS_SetHigh;
	Display_SCLK_SetLow;
	Display_SDATA_SetLow;
	return rdlib::Success;
}

/*!
	@brief  sets up TFT GPIO for Hardware SPi
	@param rst reset GPIO
	@param dc data or command GPIO.
	@details for software reset pass -1 to rst
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ST7789_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc)
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
	@param din Data to TFT GPIO
	@details for software reset pass -1 to rst
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ST7789_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_hardwareSPI = false;
	TFTSetupResetPin(rst);
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_Display_DC = dc;
}

/*!
	@brief init routine for ST7789 controller
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinClaim
		-# rdlib::SPIOpenFailure;
*/
rdlib::Return_Codes_e ST7789_TFT::TFTST7789Initialize() {
	
	//  gpio Device open?
	_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Error : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rdlib::GpioChipDevice;
	}
	// reset routine GPIO pin
	if (TFTResetPin() != rdlib::Success){return rdlib::GpioPinClaim;}
	// Data or command routine  GPIO pin 
	if (TFTDataCommandPin() != rdlib::Success){return rdlib::GpioPinClaim;}

	if (_hardwareSPI == false)
	{
		// Setup Software SPI for the 3 other GPIO : SCLK, Data & CS
		if (TFTClock_Data_ChipSelect_Pins() != rdlib::Success){return rdlib::GpioPinClaim;}
	}else{
		_spiHandle = Display_OPEN_SPI;
		if ( _spiHandle  < 0)
		{
			fprintf(stderr, "Error : Cannot open SPI :(%s)\n", lguErrorText( _spiHandle ));
			return rdlib::SPIOpenFailure;
		}
	}

	cmd89();
	AdjustWidthHeight();
	TFTsetRotation(Degrees_0);
	return rdlib::Success;
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

	if (_scroll_direction) {
		writeData(ST7789_SRLBTT); //bottom to top
	} else {
		writeData(ST7789_SRLTTB); //top to bottom
	}
}

/*!
	@brief: This method is used together with the TFTsetScrollDefinition.
	@param vsp scrolling mode
*/
void ST7789_TFT ::TFTVerticalScroll(uint16_t vsp) {
	writeCommand(ST7789_VSCRSADD);
	writeData(vsp >> 8);
	writeData(vsp & 0xFF);
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
void ST7789_TFT ::TFTsetRotation(display_rotate_e mode) {
	uint8_t madctl = 0;

	switch (mode) {
		case Degrees_0 :
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MY;
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_XStart = _colstart;
			_YStart = _rowstart;
			break;
		case Degrees_90:
			madctl = ST7789_MADCTL_MY | ST7789_MADCTL_MV;
			_YStart = _colstart2;
			_XStart = _rowstart;
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			break;
		case Degrees_180:
			madctl = ST7789_MADCTL_RGB;
			_XStart = _colstart2;
			_YStart = _rowstart2;
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case Degrees_270:
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV;
			_YStart = _colstart;
			_XStart = _rowstart2;
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			break;
	}
	displayRotate = mode;
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
	@param device A SPI device, >= 0. 
	@param channel A SPI channel, >= 0. 
	@param speed The speed of serial communication in bits per second. 
	@param flags The flags may be used to modify the default behaviour. Set to 0(mode 0) for this device.
	@param gpioDev The device number of a gpiochip.  
	@return
		-# rdlib::Success = success
		-# upstream failure from TFTST7789Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
rdlib::Return_Codes_e ST7789_TFT::TFTInitSPI(int device, int channel, int speed, int flags, int gpioDev)
{
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;
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
	@param gpioDev The device number of a gpiochip.  
	@return
		-# rdlib::Success = success
		-# upstream failure from TFTST7789Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI 
*/
rdlib::Return_Codes_e ST7789_TFT::TFTInitSPI(uint16_t CommDelay, int gpioDev)
{
	HighFreqDelaySet(CommDelay);
	_DeviceNumGpioChip = gpioDev;
	return TFTST7789Initialize();
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
	uint16_t x0_ = x0 + _XStart;
	uint16_t x1_ = x1 + _XStart;
	uint16_t y0_ = y0 + _YStart;
	uint16_t y1_ = y1 + _YStart;
	uint8_t seqCASET[] = {
		(uint8_t)(x0_ >> 8), (uint8_t)(x0_ & 0xFF),
		(uint8_t)(x1_ >> 8), (uint8_t)(x1_ & 0xFF)
	};
	uint8_t seqRASET[] = {
		(uint8_t)(y0_ >> 8), (uint8_t)(y0_ & 0xFF),
		(uint8_t)(y1_ >> 8), (uint8_t)(y1_ & 0xFF)
	};

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
	uint8_t CASETsequence[] {0x00, 0x00, 0,  240};
	uint8_t RASETsequence[] {0x00, 0x00, 320 >> 8 , 320 & 0xFF};

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

/*!
	@brief sets up TFT GPIO reset pin
	@param rst reset GPIO
	@details if rst value is -1 use software reset, else use Hardware reset.
*/
void ST7789_TFT::TFTSetupResetPin(int8_t rst)
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
	@brief software reset
	@details When the Software Reset command is written,
		it causes a software reset. It resets the commands and parameters to their
		S/W Reset default values.
*/
void ST7789_TFT::TFTsoftwareReset(void)
{
	writeCommand(ST7789_SWRESET);
	delayMilliSecRDL(120);
}
//**************** EOF *****************
