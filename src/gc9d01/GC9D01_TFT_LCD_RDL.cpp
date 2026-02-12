/*!
	@file   GC9D01_TFT_LCD_RDL.cpp
	@author Gavin Lyons
	@brief  Source file for GC9D01 TFT LCD library. Contains driver methods.
	Project Name: Display_Lib_RPI
*/

#include "../../include/gc9d01/GC9D01_TFT_LCD_RDL.hpp"


/*! @brief Constructor for class GC9D01_TFT */
GC9D01_TFT :: GC9D01_TFT(){}


/*!
	@brief Call when powering down TFT
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinFree
		-# rdlib::SPIOpenClose
	@note Turns off Display Sets GPIO low and turns off SPI
*/
rdlib::Return_Codes_e  GC9D01_TFT ::TFTPowerDown(void)
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
	@brief Method for Hardware Reset pin control
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e GC9D01_TFT ::TFTResetPin() {
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
		delayMilliSecRDL(5);
		Display_RST_SetLow;
		delayMilliSecRDL(50);
		Display_RST_SetHigh;
		delayMilliSecRDL(150);
	}
	return rdlib::Success;
}

/*!
	@brief: Method for Data or Command pin setup
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e GC9D01_TFT ::TFTDataCommandPin(void) {

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
rdlib::Return_Codes_e GC9D01_TFT::TFTClock_Data_ChipSelect_Pins(void)
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
void GC9D01_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc)
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
void GC9D01_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_hardwareSPI = false;
	TFTSetupResetPin(rst);
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_Display_DC = dc;
}

/*!
	@brief init routine for GC9D01 controller
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinClaim
		-# rdlib::SPIOpenFailure;
*/
rdlib::Return_Codes_e GC9D01_TFT::TFTGC9D01Initialize() {

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

	if(_currentResolution == Resolution_e::RGB160x160_DualGate ||
			_currentResolution == Resolution_e::RGB120x160_DualGate) {
			DualGatecmdInitSequence();
	}else if(_currentResolution == Resolution_e::RGB80x160_SingleGate ||
			_currentResolution == Resolution_e::RGB40x160_SingleGate) {
			SingleGatecmdInitSequence();
	}
	TFTsetRotation(Degrees_0);
	return rdlib::Success;
}

/*!
	@brief This method defines the Vertical Scrolling Area of the display where:
	@param topFixed describes the Top Fixed Area.
	@param scrollArea describes the Scrolling Area.
	@param bottomFixed describes the Bottom Fixed Area.
*/
void GC9D01_TFT::TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed) {

	writeCommand(GC9D01_VSCRDEF);
	writeData(topFixed >> 8);
	writeData(topFixed & 0xFF);
	writeData(scrollArea >> 8);
	writeData(scrollArea  & 0xFF);
	writeData(bottomFixed >> 8);
	writeData(bottomFixed & 0xFF);
}

/*!
	@brief This method is used together with the setScrollDefinition.
	@param vsp scrolling mode
*/
void GC9D01_TFT::TFTsetScrollStart(uint16_t vsp) {
	writeCommand(GC9D01_VSCRSADD);
	writeData(vsp >> 8);
	writeData(vsp & 0xFF);
}

/*! @brief Scroll Mode can be left ,by the Normal Display Mode ON cmd*/
void GC9D01_TFT::TFTScrollModeLeave(void) {writeCommand(GC9D01_NORON);}

/*!
	@brief Toggle the invert mode
	@param invert true = invert off , false = invert on
*/
void GC9D01_TFT ::TFTchangeInvertMode(bool invert) {
	if(invert) {
		writeCommand(GC9D01_INVOFF);
	} else {
		writeCommand(GC9D01_INVON);
	}
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on false disable
	@note Temporarily blank the screen.
	Use Case: Screen blanking, brief off periods without resetting or reinitializing the display.
*/
void GC9D01_TFT::TFTenableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(GC9D01_DISPON);
		_displayOn = true;
	} else {
		writeCommand(GC9D01_DISPOFF);
		_displayOn = false;
	}
}

/*!
	@brief Set the power mode of the display
	@param mode The power state to set
	@details
		Power states are based on the power control flow chart in the datasheet.
		FIG 89 5.10.2. Power Flow Chart
*/
void GC9D01_TFT::TFTsetPowerMode(PowerState_e mode) {
	// If already in the desired state or off , skip
	if (_currentPowerState == mode || _displayOn != true)
	{
		printf("Warning: TFTsetPowerMode: Display already in this state or off, nothing done\n");
		return;
	}
	// Always return to a known base state
	writeCommand(GC9D01_SLPOUT);
	delayMilliSecRDL(_sleepDelay);
	switch (mode) {
		case PowerState_e::NormalIdleOff:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEOFF);
			break;
		case PowerState_e::NormalIdleOn:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEON);
			break;
		case PowerState_e::PartialIdleOff:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEOFF);
			break;
		case PowerState_e::PartialIdleOn:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEON);
			break;
		case PowerState_e::SleepNormalIdleOff:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEOFF);
			writeCommand(GC9D01_SLPIN);
			delayMilliSecRDL(_sleepDelay);
			break;
		case PowerState_e::SleepNormalIdleOn:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEON);
			writeCommand(GC9D01_SLPIN);
			delayMilliSecRDL(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOff:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEOFF);
			writeCommand(GC9D01_SLPIN);
			delayMilliSecRDL(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOn:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEON);
			writeCommand(GC9D01_SLPIN);
			delayMilliSecRDL(_sleepDelay);
			break;
	}
	_currentPowerState = mode;
}

/*!
	@brief: change rotation of display.
	@param mode display_rotate_e enum
	@details 
			0 = Normal
			1=  90 rotate
			2 = 180 rotate
			3 =  270 rotate
			Offsets passed in by user in setup() via TFTInitScreenSize
			offsets 0,0 for   160x160 resolution
			offsets -60,60 for 40x160 resolution in 90 and 270 rotations
*/
void GC9D01_TFT::TFTsetRotation(display_rotate_e mode) {
	uint8_t madctl =0;
	switch (mode) {
		case Degrees_0 : // 0x00
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_GC9D01_X_Offset = 0;
			_GC9D01_Y_Offset = 0;
			break;
		case Degrees_90:
			madctl |= (MADCTL_FLAGS_t::MV | MADCTL_FLAGS_t::ML);
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			_GC9D01_X_Offset = _GC9D01_X_Offset_Start;
			_GC9D01_Y_Offset = _GC9D01_Y_Offset_Start;
			break;
		case Degrees_180:  
			madctl |= (MADCTL_FLAGS_t::MY | MADCTL_FLAGS_t::MX );
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_GC9D01_X_Offset = 0;
			_GC9D01_Y_Offset = 0;
			break;
		case Degrees_270:  
			madctl |= (MADCTL_FLAGS_t::MV |MADCTL_FLAGS_t::MX |MADCTL_FLAGS_t::MY |MADCTL_FLAGS_t::ML );
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			_GC9D01_X_Offset = _GC9D01_X_Offset_Start;
			_GC9D01_Y_Offset = _GC9D01_Y_Offset_Start;
			break;
	}
	writeCommand(GC9D01_MADCTL);
	writeData(madctl);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_TFT width in pixels
	@param height_TFT height in pixels
	@param resolution Current Resolution see enum gc9d01_resolution_e for options
	@param Xstart Column start offset based on resolution and display type
	@param Ystart Row start offset based on resolution and display type
*/
void GC9D01_TFT  :: TFTInitScreenSize(uint16_t width_TFT, uint16_t height_TFT, Resolution_e resolution,
	 uint16_t Xstart, uint16_t Ystart)
{
	_width = width_TFT;
	_height = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
	_currentResolution = resolution;
	_GC9D01_X_Offset_Start = Xstart;
	_GC9D01_Y_Offset_Start = Ystart;
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
		-# upstream failure from TFTGC9D01Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rdlib::Return_Codes_e GC9D01_TFT::TFTInitSPI(int device, int channel, int speed, int flags, int gpioDev)
{
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;
	return TFTGC9D01Initialize();
}

/*!
	@brief intialise PCBtype and SPI, Software SPI
	@param CommDelay uS GPIO delay used in software SPI
	@param gpioDev The device number of a gpiochip.
	@return
		-# rdlib::Success = success
		-# upstream failure from TFTGC9D01Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rdlib::Return_Codes_e GC9D01_TFT::TFTInitSPI(uint16_t CommDelay, int gpioDev)
{
	HighFreqDelaySet(CommDelay);
	_DeviceNumGpioChip = gpioDev;
	return TFTGC9D01Initialize();
}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t GC9D01_TFT::HighFreqDelayGet(void){return _HighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  GC9D01_TFT::HighFreqDelaySet(uint16_t CommDelay){_HighFreqDelay = CommDelay;}

/*!
  @brief SPI displays set an address window rectangle for blitting pixels
  @param  x1 Top left corner x coordinate
  @param  y1  Top left corner y coordinate
  @param  w  Width of window
  @param  h  Height of window
  @note https://en.wikipedia.org/wiki/Bit_blit
 */
void GC9D01_TFT::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
	//if drawing a single pixel we need do this to avoid a blank pixel for this device
	const bool singlePixel =
		((w == x1 + 1) && (h == y1 + 1));
	if (singlePixel) {
		w = x1;
		h = y1;
	}
	// Apply offsets: not needed for normal 160x160 dual gate resolution
	if (_currentResolution != Resolution_e::RGB160x160_DualGate) {
		x1 += _GC9D01_X_Offset;
		y1 += _GC9D01_Y_Offset;
		w += _GC9D01_X_Offset;
		h += _GC9D01_Y_Offset;
	}

	uint8_t x1Higher = (x1 >> 8) ;
	uint8_t x1Lower  = (x1 & 0xFF);
	uint8_t x2Higher = (w >> 8);
	uint8_t x2Lower  = (w &  0xFF);
	uint8_t seqCASET[4]  =  {x1Higher ,x1Lower,x2Higher,x2Lower};
	uint8_t y1Higher = (y1 >> 8); 
	uint8_t y1Lower  = (y1 &  0xFF);
	uint8_t y2Higher = (h >> 8);
	uint8_t y2Lower  = (h &  0xFF);
	uint8_t seqRASET[4]  =  {y1Higher,y1Lower,y2Higher,y2Lower};
	writeCommand(GC9D01_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(GC9D01_RASET); //Row address set
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(GC9D01_RAMWR); //Write to RAM
}

/*!
	@brief GC9D01 init command sequence for Dual Gate variant
		Dual Gate variants include 160x160 and 120x160 resolutions
*/
void GC9D01_TFT::DualGatecmdInitSequence(void)
{

	writeCommand(GC9D01_INREGEN1);
	writeCommand(GC9D01_INREGEN2); 

	// Enabling the internal register: Undocumented in datasheet registers
	constexpr uint8_t startCmd = 0x80;
	constexpr uint8_t endCmd   = 0x8F;
	for (uint8_t cmd = startCmd; cmd <= endCmd; ++cmd)
	{
		writeCommand(cmd);
		writeData(0xFF);
	}

	writeCommand(GC9D01_INVERSION);
	writeData(0x70); 
	
	// Change frame rate: Undocumented in datasheet registers
	writeCommand(0x74); 
	uint8_t seqReg1[] = {0x02, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
	spiWriteDataBuffer(seqReg1, sizeof(seqReg1));

	// Internal voltage regulation: Undocumented in datasheet registers
	writeCommand(0x98); 
	writeData(0x3E);
	writeCommand(0x99);
	writeData(0x3E);

	// Blanking Porch Control
	writeCommand(GC9D01_BLANK_PORCH_CTRL);
	uint8_t segBlankCTR[] = {0x0D, 0x0D, 0x00};
	spiWriteDataBuffer(segBlankCTR, sizeof(segBlankCTR));
	
	 // Timing Gap: Undocumented in datasheet registers
	writeCommand(0x60);
	uint8_t seqReg2[] = {0x38, 0x0F, 0x79, 0x67};
	spiWriteDataBuffer(seqReg2, sizeof(seqReg2));
	writeCommand(0x61);
	uint8_t seqReg3[] = {0x38, 0x11, 0x79, 0x67};
	spiWriteDataBuffer(seqReg3, sizeof(seqReg3));
	writeCommand(0x64);
	uint8_t seqReg4[] = {0x38, 0x17, 0x71, 0x5F, 0x79, 0x67};
	spiWriteDataBuffer(seqReg4, sizeof(seqReg4));
	writeCommand(0x65);
	uint8_t seqReg5[] = {0x38, 0x13, 0x71, 0x5B, 0x79, 0x67};
	spiWriteDataBuffer(seqReg5, sizeof(seqReg5));
	writeCommand(0x6A);
	writeData(0x00);
	writeData(0x00);
	writeCommand(0x6C);
	uint8_t segReg6[] = {0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50};
	spiWriteDataBuffer(segReg6, sizeof(segReg6));
	writeCommand(0x6E);
	uint8_t segReg7[] = {0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x0F, 0x0F, 0x0D, 0x0D,
						 0x0B, 0x0B, 0x09, 0x09, 0x00, 0x00, 0x00,
						 0x00, 0x0A, 0x0A, 0x0C, 0x0C, 0x0E, 0x0E,
						 0x10, 0x10, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04};
	spiWriteDataBuffer(segReg7, sizeof(segReg7));

	writeCommand(GC9D01_DUAL_SINGLE);
	writeData(0x01);// Dual-Single Gate Select (BFh) 0=>Single gate

	 // Adjustments related to SOU : Undocumented in datasheet registers
	writeCommand(0xF9);
	writeData(0x40);
	//Voltage regulation vreg : Undocumented in datasheet registers
	writeCommand(0x9b);
	writeData(0x3b);
	writeCommand(0x93);
	uint8_t segReg8[] = {0x33, 0x7F, 0x00};
	spiWriteDataBuffer(segReg8, sizeof(segReg8));

	// Undocumented in datasheet registers
	writeCommand(0x7E);
	writeData(0x30);
	// VGH/VGL CLK adjustment 70，71h : Undocumented in datasheet registers
	writeCommand(0x70);
	uint8_t segReg9[] = {0x0D, 0x02, 0x08, 0x0D, 0x02, 0x08};
	spiWriteDataBuffer(segReg9, sizeof(segReg9));
	writeCommand(0x71);
	uint8_t segReg10[] = {0x0D, 0x02, 0x08};
	spiWriteDataBuffer(segReg10, sizeof(segReg10));
	// Internal voltage regulation : Undocumented in datasheet registers
	writeCommand(0x91);
	writeData(0x0E);
	writeData(0x09);

	writeCommand(GC9D01_VREG1A_CTRL);
	writeData(0x18);
	writeCommand(GC9D01_VREG1B_CTRL);
	writeData(0x18);
	writeCommand(GC9D01_VREG2A_CTRL);
	writeData(0x3c);

	writeCommand(GC9D01_GAMMA1); // 0xF0
	uint8_t seqGamma1[] = {0x13, 0x15, 0x04, 0x05, 0x01, 0x38};
	spiWriteDataBuffer(seqGamma1, sizeof(seqGamma1));
	writeCommand(GC9D01_GAMMA3); // 0xF2
	uint8_t seqGamma3[] = {0x13, 0x15, 0x04, 0x05, 0x01, 0x34};
	spiWriteDataBuffer(seqGamma3, sizeof(seqGamma3));
	writeCommand(GC9D01_GAMMA2); // 0xF1
	uint8_t seqGamma2[] = {0x4B, 0xB8, 0x7B, 0x34, 0x35, 0xEF};
	spiWriteDataBuffer(seqGamma2, sizeof(seqGamma2));
	writeCommand(GC9D01_GAMMA4); // 0xF3
	uint8_t seqGamma4[] = {0x47, 0xB4, 0x72, 0x34, 0x35, 0xDA};
	spiWriteDataBuffer(seqGamma4, sizeof(seqGamma4));

	writeCommand(GC9D01_COLMOD);
	writeData(0x55); // 16bit/pixel control
	writeCommand(GC9D01_FUNCTION_CTRL);
	writeData(0x00);
	writeData(0x00);
	writeCommand(GC9D01_IFACE);
	writeData(0xc0);
	writeCommand(GC9D01_SPI2DATA);
	writeData(0x00);
	
	writeCommand(GC9D01_MADCTL);
	writeData(0x00); 
	writeCommand(GC9D01_SLPOUT);
	delayMilliSecRDL(200); // wait at least 120ms after sending Sleep Out cmd(4.2.4.)
	writeCommand(GC9D01_DISPON);
	writeCommand(GC9D01_CONTINUE);
}

/*!
	@brief GC9D01 init command sequence for Single Gate variant
		Single Gate variants include 80x160 and 40x160 resolutions
*/
void GC9D01_TFT::SingleGatecmdInitSequence(void)
{
	writeCommand(0xFE);
	writeCommand(0xEF);

	// Internal registers 80h–8Fh (all 0xFF) — loop is already optimal
	for (uint8_t cmd = 0x80; cmd <= 0x8F; ++cmd) {
		writeCommand(cmd);
		writeData(0xFF);
	}

	writeCommand(GC9D01_COLMOD);
	writeData(0x05);
	writeCommand(0xEC);
	writeData(0x11);

	writeCommand(0x7E);
	writeData(0x7A);
	// Frame rate (0x74) — 7 bytes
	writeCommand(0x74);
	static const uint8_t FrameRatedata[] = {0x02, 0x0E, 0x00, 0x00, 0x28, 0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(FrameRatedata), sizeof(FrameRatedata));

	writeCommand(0x98);
	writeData(0x3E);
	writeCommand(0x99);
	writeData(0x3E);

	// Porch B5h
	writeCommand(0xB5);
	static const uint8_t PorchData[] = {0x0E, 0x0E};
	spiWriteDataBuffer(const_cast<uint8_t*>(PorchData), sizeof(PorchData));

	// GIP / timing registers — grouped where multiple bytes
	writeCommand(0x60);
	static const uint8_t gap1data[]  = {0x38, 0x09, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap1data), sizeof(gap1data));
	writeCommand(0x63);
	static const uint8_t gap2data[] = {0x38, 0xAD, 0x6D, 0x67, 0x05};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap2data), sizeof(gap2data));
	writeCommand(0x64);
	static const uint8_t gap3data[]  = {0x38, 0x0B, 0x70, 0xAB, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap3data), sizeof(gap3data));
	writeCommand(0x66);
	static const uint8_t gap4data[]  = {0x38, 0x0F, 0x70, 0xAF, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap4data), sizeof(gap4data));
	writeCommand(0x6A);
	static const uint8_t gap5data[]  = {0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap5data), sizeof(gap5data));
	writeCommand(0x68);
	static const uint8_t gap6data[] = {0x3B, 0x08, 0x04, 0x00, 0x04, 0x64, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap6data), sizeof(gap6data));
	writeCommand(0x6C);
	static const uint8_t gap7data[] = {0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap7data), sizeof(gap7data));
	// Long 0x6E sequence — 32 bytes
	writeCommand(0x6E);
	{
		static const uint8_t gap8data[] = {
			0x00, 0x00, 0x00, 0x00, 0x07, 0x01, 0x13, 0x11,
			0x0B, 0x09, 0x16, 0x15, 0x1D, 0x1E, 0x00, 0x00,
			0x00, 0x00, 0x1E, 0x1D, 0x15, 0x16, 0x0A, 0x0C,
			0x12, 0x14, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00
		};
		spiWriteDataBuffer(const_cast<uint8_t*>(gap8data), sizeof(gap8data));
	}

	// Voltage / adjustment block
	writeCommand(0xA9); writeData(0x1B);
	writeCommand(0xA8); writeData(0x6B);
	writeCommand(0xA8); writeData(0x6D);
	writeCommand(0xA7); writeData(0x40);
	writeCommand(0xAD); writeData(0x47);
	writeCommand(0xAF); writeData(0x73);
	writeCommand(0xAF); writeData(0x73);
	writeCommand(0xAC); writeData(0x44);
	writeCommand(0xA3); writeData(0x6C);
	writeCommand(0xCB); writeData(0x00);
	writeCommand(0xCD); writeData(0x22);
	writeCommand(0xC2); writeData(0x10);
	writeCommand(0xC5); writeData(0x00);
	writeCommand(0xC6); writeData(0x0E);
	writeCommand(0xC7); writeData(0x1F);
	writeCommand(0xC8); writeData(0x0E);

	// Gate select
	writeCommand(0xBF);
	writeData(0x00);

	writeCommand(0xF9);
	writeData(0x20);
	writeCommand(0x9B);
	writeData(0x3B);
	writeCommand(0x93);
	static const uint8_t data1[]  = {0x33, 0x7F, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(data1), sizeof(data1));
	writeCommand(0x70);
	static const uint8_t data2[] = {0x0E, 0x0F, 0x03, 0x0E, 0x0F, 0x03};
	spiWriteDataBuffer(const_cast<uint8_t*>(data2), sizeof(data2));
	writeCommand(0x71);
	static const uint8_t data3[]  = {0x0E, 0x16, 0x03};
	spiWriteDataBuffer(const_cast<uint8_t*>(data3), sizeof(data3));
	writeCommand(0x91);
	static const uint8_t data4[]  = {0x0E, 0x09};
	spiWriteDataBuffer(const_cast<uint8_t*>(data4), sizeof(data4));

	writeCommand(0xC3);
	writeData(0x2C);
	writeCommand(0xC4);
	writeData(0x1A);

	// Gamma curves — each 6 bytes
	writeCommand(0xF0);
	static const uint8_t gamma1[] = {0x51, 0x13, 0x0C, 0x06, 0x00, 0x2F};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma1), sizeof(gamma1));
	writeCommand(0xF2);
	static const uint8_t gamma2[] = {0x51, 0x13, 0x0C, 0x06, 0x00, 0x33};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma2), sizeof(gamma2));
	writeCommand(0xF1);
	static const uint8_t gamma3[] = {0x3C, 0x94, 0x4F, 0x33, 0x34, 0xCF};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma3), sizeof(gamma3));
	writeCommand(0xF3);
	static const uint8_t gamma4[]  = {0x4D, 0x94, 0x4F, 0x33, 0x34, 0xCF};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma4), sizeof(gamma4));

	// MADCTL
	writeCommand(GC9D01_MADCTL);
	writeData(0x00);
	// Sleep out + display on
	writeCommand(GC9D01_SLPOUT);
	delayMilliSecRDL(200); 
	writeCommand(GC9D01_DISPON);
	delayMilliSecRDL(50);
	writeCommand(GC9D01_CONTINUE); 
}


/*!
	@brief sets up TFT GPIO reset pin
	@param rst reset GPIO
	@details if rst value is -1 use software reset, else use Hardware reset.
*/
void GC9D01_TFT::TFTSetupResetPin(int8_t rst)
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
  @brief Set display brightness (0–255).
  @param level Brightness level, 0 = darkest, 255 = brightest
  @note This is a software brightness control, not hardware PWM, may not work on all displays.
*/
void GC9D01_TFT::TFTsetBrightness(uint8_t level)
{
	writeCommand(GC9D01_SETCTRL); // CTRL Display
	writeData(0x2C);    // Brightness registers are active, Display Dimming is on Backlight On
	writeCommand(GC9D01_SETBRIGHT);
	writeData(level);
}
//**************** EOF *****************
