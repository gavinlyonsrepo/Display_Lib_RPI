/*!
	@file SSD1331_OLED_RDL.cpp
	@author   Gavin Lyons
	@brief    Source file for SSD1331 OLED library. Contains driver methods.
*/

#include "../../include/ssd1331/SSD1331_OLED_RDL.hpp"


/*! @brief Constructor for class SSD1331_OLED
	@param colororder The enum value from color_order_e RGB or BGR color mode
	@param contrast Struct containing 3 color values for normal mode contrast
	@param dimContrast Struct containing 3 color values for dim mode contrast
 * */
SSD1331_OLED::SSD1331_OLED(color_order_e colororder, const Constrast_values_t& contrast, const Dim_Constrast_values_t& dimContrast)
    : _colorOrder(colororder), ContrastValues(contrast), DimContrastValues(dimContrast)
{
}

/*!
	@brief Call when powering down OLED
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinFree
		-# rdlib::SPIOpenClose
	@details Turns off Display, sets GPIO low and frees them,closes GPIO device, closes SPI handle.
*/
rdlib::Return_Codes_e  SSD1331_OLED ::OLEDPowerDown(void)
{
	OLEDsleepDisplay();

	uint8_t ErrorFlag = 0; // Becomes > 0 in event of error

	// 1. free rst & DC GPIO lines
	int GpioResetErrorStatus = 0;
	int GpioDCErrorStatus = 0;
	Display_RST_SetLow;
	Display_DC_SetLow;
	GpioResetErrorStatus = Display_GPIO_FREE_RST;
	GpioDCErrorStatus  =  Display_GPIO_FREE_DC;

	if (GpioResetErrorStatus < 0 ){
		fprintf(stderr,"Error:Can't Free RST GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
		ErrorFlag = 2;
	}else if (GpioDCErrorStatus  < 0 ){
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
	// 3 Closes the opened gpiochip device.
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
		default:fprintf(stderr, "Warning::Unknown error flag value in OLEDPowerDown"); break;
	}
	return rdlib::Success;
}

/*!
	@brief Method for Hardware Reset pin control
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e SSD1331_OLED ::OLEDResetPin() {

	const uint8_t OLEDResetDelay = 10; /**< Reset delay in mS*/
	int GpioResetErrorStatus = 0;
	GpioResetErrorStatus= Display_RST_SetDigitalOutput;
	if (GpioResetErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorStatus));
		return rdlib::GpioPinClaim;
	}
	Display_RST_SetHigh;
	delayMilliSecRDL(OLEDResetDelay);
	Display_RST_SetLow;
	delayMilliSecRDL(OLEDResetDelay);
	Display_RST_SetHigh;
	delayMilliSecRDL(OLEDResetDelay);
	return rdlib::Success;
}

/*!
	@brief: Method for Data or Command pin setup
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e SSD1331_OLED ::OLEDDataCommandPin(void) {

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
rdlib::Return_Codes_e SSD1331_OLED::OLEDClock_Data_ChipSelect_Pins(void)
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
	@brief  sets up OLED GPIO for Hardware SPi
	@param rst reset GPIO
	@param dc data or command GPIO.
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void SSD1331_OLED ::OLEDSetupGPIO(int8_t rst, int8_t dc)
{
	_hardwareSPI = true;
	_Display_RST= rst;
	_Display_DC = dc;
}

/*!
	@brief  sets up OLED GPIO for software SPI
	@param rst reset GPIO
	@param dc data or command GPIO.
	@param cs chip select GPIO
	@param sclk Data clock GPIO
	@param din Data to OLED GPIO
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void SSD1331_OLED ::OLEDSetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_hardwareSPI = false;
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_Display_RST= rst;
	_Display_DC = dc;
}

/*!
	@brief init routine for SSD1331 controller
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinClaim
		-# rdlib::SPIOpenFailure;
*/
rdlib::Return_Codes_e SSD1331_OLED::OLEDSSD1331Initialize() {

	//  gpio Device open?
	_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Error : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rdlib::GpioChipDevice;
	}
	// reset routine GPIO pin
	if (OLEDResetPin() != rdlib::Success){return rdlib::GpioPinClaim;}
	// Data or command routine  GPIO pin
	if (OLEDDataCommandPin() != rdlib::Success){return rdlib::GpioPinClaim;}

	if (_hardwareSPI == false)
	{
		// Setup Software SPI for the 3 other GPIO : SCLK, Data & CS
		if (OLEDClock_Data_ChipSelect_Pins() != rdlib::Success){return rdlib::GpioPinClaim;}
	}else{
		_spiHandle = Display_OPEN_SPI;
		if ( _spiHandle  < 0)
		{
			fprintf(stderr, "Error : Cannot open SPI :(%s)\n", lguErrorText( _spiHandle ));
			return rdlib::SPIOpenFailure;
		}
	}

	cmdBegin();
	OLEDsetRotation(Degrees_0);
	return rdlib::Success;
}


/*!
	@brief This method setups up horizontal and vertical scrolling register _CMD_CONT_SCROLL_SET
	@param horOffset Set num of col as h scroll Range: 0-95 ( no h scroll = 0)
	@param startRowAddr Define start row address 0-63
	@param numOfRows Set number of rows to be horizontal scrolled (startRowAddr + numOfRows) <= 64 
	@param verOffset Set num of col as v scroll Range: 0-63 ( no v scroll = 0)
	@param scrollTimeInterval Set time interval between each scroll, enum
*/
void SSD1331_OLED::OLEDScrollSetup(uint8_t horOffset,uint8_t startRowAddr, uint8_t numOfRows, uint8_t verOffset, scroll_time_interval_e scrollTimeInterval) {
	// Check user input
	if (horOffset > 95) horOffset = 94;
	if (startRowAddr > 63) startRowAddr= 62;
	if (startRowAddr + numOfRows > 64) {
		fprintf(stderr, " startRowAddr + numOfRows > 64, setting numOfRows = 1 ,  %u + %u  <= 64 \n", startRowAddr, numOfRows);
		numOfRows = 1;
	}
	if (verOffset > 63) verOffset= 62;
	// Send scroll command list
	writeCommand(_CMD_CONT_SCROLL_SET);
	writeCommand(horOffset);
	writeCommand(startRowAddr);
	writeCommand(numOfRows);
	writeCommand(verOffset);
	writeCommand(scrollTimeInterval);
}

/*!
	@brief: This method is used to turn scroll on or off
	@param scrollActivate Deactive or active scroll called after OLEDScrollSetup()
*/
void SSD1331_OLED::OLEDScroll(scroll_control_e scrollActivate)  {
	
	switch (scrollActivate)
	{
		case SCROLL_ACTIVATE : writeCommand(_CMD_ACTIVE_SCROLL); break;
		case SCROLL_DEACTIVE : writeCommand(_CMD_DEACTIVE_SCROLL); break;
	}
}

/*!
	@brief change the display mode
	@param mode enum containing the display modes available
*/
void SSD1331_OLED ::OLEDchangeDisplayMode(display_mode_e mode) {
	switch (mode)
	{
		case DISPLAY_INVERSE: writeCommand(_CMD_INVERTDISPLAY); break;
		case DISPLAY_NORMAL:  writeCommand(_CMD_NORMALDISPLAY); break;
		case DISPLAY_ALL_ON:  writeCommand(_CMD_DISPLAYALLON);  break;
		case DISPLAY_ALL_OFF: writeCommand(_CMD_DISPLAYALLOFF); break;
	};
}

/*! @brief enable dim mode */
void SSD1331_OLED ::OLEDDimDisplay(){
	writeCommand(_CMD_DIM_MODE_ON);
}

/*! @brief enable display mode */
void SSD1331_OLED ::OLEDenableDisplay(){
	writeCommand(_CMD_DISPLAYON);
}

/*!@brief enable sleep mode*/
void SSD1331_OLED ::OLEDsleepDisplay(){
	writeCommand(_CMD_DISPLAYOFF);
}

/*!
	@brief: change rotation of display.
	@param mode OLED_rotate_e enum
		0 = Normal
		1=  90 rotate
		2 = 180 rotate
		3 =  270 rotate
*/
void SSD1331_OLED ::OLEDsetRotation(display_rotate_e mode) {

	_displayRotate = mode;

	uint8_t rotateValue = 0;
	uint8_t DegreeZero = 0x72;
	uint8_t Degree90   = 0x71;
	uint8_t Degree180  = 0x60;
	uint8_t Degree270  = 0x63;
	// We must preserve setting A[2] in _CMD_SETREMAP setting register
	// This is set in constructor passing a by color_order_e. 
	// the pixel color order RGB mapping 
	uint8_t BitMask_ReMapCmdA2 = 0x04; 

	writeCommand(_CMD_SETREMAP);
	switch (mode) {
		case Degrees_0 :
			 // RGB 0x72 BGR 0x76 A[2]
			rotateValue  = (DegreeZero | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width =_widthStartOLED;
			_height = _heightStartOLED;
			break;
		case Degrees_90:
			rotateValue  = (Degree90 | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width  =_heightStartOLED;
			_height = _widthStartOLED;
			break;
		case Degrees_180:
			// RGB 0x60 BGR 0x64 A[2]
			rotateValue  = (Degree180 | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width =_widthStartOLED;
			_height = _heightStartOLED;
			break;
		case Degrees_270:
			rotateValue  = (Degree270 | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width =_heightStartOLED;
			_height = _widthStartOLED;
			break;
	}
	writeCommand(rotateValue);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_OLED width in pixels
	@param height_OLED height in pixels
*/
void SSD1331_OLED  :: OLEDInitScreenSize(uint16_t width_OLED, uint16_t height_OLED)
{
	_width = width_OLED;
	_height = height_OLED;
	_widthStartOLED = width_OLED;
	_heightStartOLED = height_OLED;
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
		-# upstream failure from OLEDSSD1331Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rdlib::Return_Codes_e SSD1331_OLED::OLEDInitSPI(int device, int channel, int speed, int flags, int gpioDev)
{
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;
	return OLEDSSD1331Initialize();
}


/*!
	@brief intialise PCBtype and SPI, Software SPI
	@param CommDelay uS GPIO delay used in software SPI
	@param gpioDev The device number of a gpiochip.
	@return
		-# rdlib::Success = success
		-# upstream failure from OLEDSSD1331Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rdlib::Return_Codes_e SSD1331_OLED::OLEDInitSPI(uint16_t CommDelay, int gpioDev)
{
	HighFreqDelaySet(CommDelay);
	_DeviceNumGpioChip = gpioDev;
	return OLEDSSD1331Initialize();
}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t SSD1331_OLED::HighFreqDelayGet(void){return _HighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  SSD1331_OLED::HighFreqDelaySet(uint16_t CommDelay){_HighFreqDelay = CommDelay;}

/*!
	@brief SPI displays set an address window rectangle for blitting pixels
	@param  x Top left corner x coordinate
	@param  y  Top left corner y coordinate
	@param  w  Width of window
	@param  h  Height of window
	@note virtual function overloads graphics library
*/
void SSD1331_OLED::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	if (_displayRotate & 1) {  // 90° or 270° rotation
		writeCommand(_CMD_SETROW);
		writeCommand(x);
		writeCommand(w);

		writeCommand(_CMD_SETCOLUMN);
		writeCommand(y);
		writeCommand(h);
		} else {  // 0° or 180° rotation
		writeCommand(_CMD_SETCOLUMN);
		writeCommand(x);
		writeCommand(w);

		writeCommand(_CMD_SETROW);
		writeCommand(y);
		writeCommand(h);
	}
}

/*!
	@brief Command Initialization sequence for SSD1331 display
*/
void SSD1331_OLED::cmdBegin(void)
{
	writeCommand(_CMD_DISPLAYOFF);
	OLEDConfigueContrast();
	writeCommand(_CMD_MASTERCURRENT);
	writeCommand(0x06);
	writeCommand(_CMD_PRECHARGEA);
	writeCommand(0x64);
	writeCommand(_CMD_PRECHARGEB);
	writeCommand(0x78);
	writeCommand(_CMD_PRECHARGEC);
	writeCommand(0x64);
	OLEDConfigueDimMode();

	writeCommand(_CMD_SETREMAP);
	switch (_colorOrder)
	{
		case COLORORDER_RGB: writeCommand(COLORORDER_RGB); break;
		case COLORORDER_BGR: writeCommand(COLORORDER_BGR); break;
		default: writeCommand(COLORORDER_RGB); break;
	};

	writeCommand(_CMD_STARTLINE);
	writeCommand(0x0);
	writeCommand(_CMD_DISPLAYOFFSET);
	writeCommand(0x0);
	writeCommand(_CMD_NORMALDISPLAY);
	writeCommand(_CMD_SETMULTIPLEX);
	writeCommand(0x3F);
	writeCommand(_CMD_SETMASTER);
	writeCommand(0x8E);
	writeCommand(_CMD_POWERMODE);
	writeCommand(0x0B); // disable power save mode
	writeCommand(_CMD_PRECHARGE);
	writeCommand(0x31);
	writeCommand(_CMD_CLOCKDIV);
	writeCommand(0xF0); // 7:4 = Oscillator Freq, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
	writeCommand(_CMD_PRECHARGE_VOLT);
	writeCommand(0x3A);
	writeCommand(_CMD_V_VOLTAGE );
	writeCommand(0x3E);

	writeCommand(_CMD_DISPLAYON);
}

/*!
	@brief Command sequence for configure Dim mode contrast
*/
void SSD1331_OLED::OLEDConfigueDimMode(void)
{
	writeCommand(_CMD_DIM_MODE_SET);
	writeCommand(0x00);
	writeCommand(DimContrastValues.Dim_ContrastColorA);
	writeCommand(DimContrastValues.Dim_ContrastColorB);
	writeCommand(DimContrastValues.Dim_ContrastColorC);
	writeCommand(0x0F);
}


/*!
	@brief Command sequence for configure  contrast
*/
void SSD1331_OLED::OLEDConfigueContrast(void)
{
	writeCommand(_CMD_CONTRASTA);
	writeCommand(ContrastValues.ContrastColorA);
	writeCommand(_CMD_CONTRASTB);
	writeCommand(ContrastValues.ContrastColorB);
	writeCommand(_CMD_CONTRASTC);
	writeCommand(ContrastValues.ContrastColorC);
}

/*!
	@brief Clears a specific rectangular window on the OLED display.
	@param column1 Starting column (X coordinate) of the window (Range: 0-95).
	@param row1 Starting row (Y coordinate) of the window (Range: 0-63).
	@param column2 Ending column (X coordinate) of the window (Range: 0-95).
	@param row2 Ending row (Y coordinate) of the window (Range: 0-63).
	@note The coordinates must be within the display bounds (96x64 pixels).
	@note Uses the SSD1331 `_CMD_CLEAR_WINDOW` command.
	@note A small delay is added (`delayMicroSecRDL(500)`) to allow processing time.
 */
void SSD1331_OLED::OLEDClearWindowCmd(uint8_t column1 , uint8_t row1 , uint8_t column2 , uint8_t row2)
{
	writeCommand(_CMD_CLEAR_WINDOW);
	writeCommand(column1);
	writeCommand(row1);
	writeCommand(column2);
	writeCommand(row2);
	delayMicroSecRDL(500);
}

/*!
	@brief Copies a specific rectangular window to a new position on the OLED display.
	@param column1 Starting column (X coordinate) of the source window (Range: 0-95).
	@param row1 Starting row (Y coordinate) of the source window (Range: 0-63).
	@param column2 Ending column (X coordinate) of the source window (Range: 0-95).
	@param row2 Ending row (Y coordinate) of the source window (Range: 0-63).
	@param column3 Destination column (X coordinate) where the copied window will be placed (Range: 0-95).
	@param row3 Destination row (Y coordinate) where the copied window will be placed (Range: 0-63).
	@note The coordinates must be within the display bounds (96x64 pixels).
	@note Uses the SSD1331 `_CMD_COPY_WINDOW` command.
	@note A small delay is added (`delayMicroSecRDL(500)`) to allow processing time.
 */
void SSD1331_OLED::OLEDCopyWindowCmd(uint8_t column1 , uint8_t row1 , uint8_t column2 , uint8_t row2, uint8_t column3 , uint8_t row3)
{
	writeCommand(_CMD_COPY_WINDOW);
	writeCommand(column1);
	writeCommand(row1);
	writeCommand(column2);
	writeCommand(row2);
	writeCommand(column3);
	writeCommand(row3);
	delayMicroSecRDL(500);
}


/*!
	@brief Dims a specific rectangular window on the OLED display.
	@param column1 Starting column (X coordinate) of the window to be dimmed (Range: 0-95).
	@param row1 Starting row (Y coordinate) of the window to be dimmed (Range: 0-63).
	@param column2 Ending column (X coordinate) of the window to be dimmed (Range: 0-95).
	@param row2 Ending row (Y coordinate) of the window to be dimmed (Range: 0-63).
	@note The coordinates must be within the display bounds (96x64 pixels).
	@note Uses the SSD1331 `_CMD_DIM_WINDOW` command.
	@note A small delay is added (`delayMicroSecRDL(500)`) to allow processing time.
 */
void SSD1331_OLED::OLEDDimWindowCmd(uint8_t column1 , uint8_t row1 , uint8_t column2 , uint8_t row2)
{
	writeCommand(_CMD_DIM_WINDOW);
	writeCommand(column1);
	writeCommand(row1);
	writeCommand(column2);
	writeCommand(row2);
	delayMicroSecRDL(500);
}

/*!
	@brief Draws a line between two points on the OLED display.
	@param c1 Starting column (X coordinate) of the line (Range: 0-95).
	@param r1 Starting row (Y coordinate) of the line (Range: 0-63).
	@param c2 Ending column (X coordinate) of the line (Range: 0-95).
	@param r2 Ending row (Y coordinate) of the line (Range: 0-63).
	@param color 16-bit RGB565 color value for the line.
	@note this uses built-in OLED command NOT the graphics library.
 */
void SSD1331_OLED::OLEDDrawLineCmd(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint16_t color)
{
	if(_displayRotate  % 2 == 1)
	{
		uint8_t temp = c1;
		c1 = r1;
		r1 = temp;
		temp = c2;
		c2 = r2;
		r2 = temp;
	}
	writeCommand(_CMD_DRAWLINE);
	writeCommand(c1);
	writeCommand(r1);
	writeCommand(c2);
	writeCommand(r2);
	writeCommand(color >> 10 & 0x3E);
	writeCommand(color >> 5 & 0x3F);
	writeCommand(color << 1 & 0x3E);
	delayMicroSecRDL(100);
}

/*!
	@brief Draws a rectangle on the OLED display, with optional filling.
		This function issues a command to draw a rectangle with the specified 
		coordinates and color. The rectangle can be either outlined or filled.
		The function also accounts for screen rotation when determining coordinates.
	@param c1 Starting column (X coordinate) of the rectangle.
	@param r1 Starting row (Y coordinate) of the rectangle.
	@param c2 Ending column (X coordinate) of the rectangle.
	@param r2 Ending row (Y coordinate) of the rectangle.
	@param color The 16-bit RGB565 color value for the rectangle.
	@param fill If true, the rectangle will be filled with the specified color.
            If false, only the border will be drawn.
    @note this uses built-in OLED command NOT the graphics library.
 */
void SSD1331_OLED::OLEDDrawRectCmd(uint8_t c1, uint8_t r1, uint8_t  c2, uint8_t r2, uint16_t color, bool fill) {

	if(_displayRotate  % 2 == 1)
	{
		uint8_t temp = c1;
		c1 = r1;
		r1 = temp;
		temp = c2;
		c2 = r2;
		r2 = temp;
	}
	//turn fill on or off
	if (fill)
	{
		writeCommand(_CMD_FILL);
		writeCommand(_CMD_ENABLE_FILL);
	}else{
		writeCommand(_CMD_FILL);
		writeCommand(_CMD_DISABLE_FILL);
	}
	// Rect coord
	writeCommand(_CMD_DRAWRECT);
	writeCommand(c1);
	writeCommand(r1);
	writeCommand(c2);
	writeCommand(r2);
	// Rect color
	writeCommand(color >> 10 & 0x3E);
	writeCommand(color >> 5 & 0x3F);
	writeCommand(color << 1 & 0x3E);
	if (fill){
		writeCommand(color >> 10 & 0x3E);
		writeCommand(color >> 5 & 0x3F);
		writeCommand(color << 1 & 0x3E);
	}else{
		writeCommand(0);
		writeCommand(0);
		writeCommand(0);
	}
	delayMicroSecRDL(500);
}


//**************** EOF *****************
