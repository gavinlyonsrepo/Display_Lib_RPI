/*!
	@file     ILI9341_TFT_LCD_RDL.cpp
	@author   Gavin Lyons
	@brief   Source file for ILI9341_TFT_LCD_RDL library.
				to manage hardware interface with ILI9341
*/

#include "../../include/ili9341/ILI9341_TFT_LCD_RDL.hpp"

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
	@param rst reset GPIO (pass -1 to use software reset if applicable to your display)
	@param dc data or command GPIO.
	@param cs chip select GPIO
	@param sclk Data clock GPIO
	@param mosi Data to TFT GPIO
	@param miso Data from TFT GPIO read diagnostics (pass -1 to disable: default)
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ILI9341_TFT::SetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t mosi, int8_t miso)
{
	_hardwareSPI = false;
	TFTSetupResetPin(rst);
	TFTSetupMISOPin(miso);
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
	@brief sets up TFT GPIO MISO pin
	@param miso master in slave out SPI GPIO
	@details only needed for read diagnostic function & software spi
*/
void ILI9341_TFT::TFTSetupMISOPin(int8_t miso)
{
	if (miso != -1)
	{
		_Display_MISO = miso;
		_MISOPinOn = true;
	}else{
		_MISOPinOn = false;
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
	@param device A SPI device, >= 0. 
	@param channel A SPI channel, >= 0. 
	@param speed The speed of serial communication in bits per second. 
	@param flags The flags may be used to modify the default behaviour. Set to 0(mode 0) for this device.
	@param gpioDev The device number of a gpiochip.  
	@return
		-# rdlib::Success = success
		-# Upstream error code from ILI9341Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rdlib::Return_Codes_e ILI9341_TFT::InitSPI(int device, int channel, int speed, int flags, int gpioDev)
{
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;
	return ILI9341Initialize();
}

/*!
	@brief intialise PCBtype and SPI, Software SPI
	@param CommDelay uS GPIO delay used in software SPI
	@param gpioDev The device number of a gpiochip.  
	@return Upstream error code from ILI9341Initialize()
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
rdlib::Return_Codes_e ILI9341_TFT::InitSPI(uint16_t CommDelay , int gpioDev)
{
	HighFreqDelaySet(CommDelay);
	_DeviceNumGpioChip = gpioDev;
	return ILI9341Initialize();
}

/*!
	@brief init routine for ILI9341_TFTcontroller
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinClaim
		-# rdlib::SPIOpenFailure;
*/
rdlib::Return_Codes_e ILI9341_TFT::ILI9341Initialize()
{
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
	// miso routine GPIO pin
	if (TFTMISOPin() != rdlib::Success){return rdlib::GpioPinClaim;}
	
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
	cmdInit();
	return rdlib::Success;
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
	@brief Call when powering down TFT
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioChipDevice
		-# rdlib::GpioPinFree
		-# rdlib::SPIOpenClose
	@note Turns off Display Sets GPIO low and turns off SPI
*/
rdlib::Return_Codes_e  ILI9341_TFT::PowerDown(void)
{
	EnableDisplay(false);
	uint8_t ErrorFlag = 0; // Becomes >0 in event of error

	// 1. free reset & DC GPIO lines
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
	int GpioDCErrorStatus = 0;
	Display_DC_SetLow;
	GpioDCErrorStatus  =  Display_GPIO_FREE_DC;
	if (GpioDCErrorStatus  < 0 )
	{
		fprintf(stderr,"Error: Can't Free CD GPIO (%s)\n", lguErrorText(GpioDCErrorStatus));
		ErrorFlag = 2;
	}

	if (_hardwareSPI == false)
	{
		// 2A Software SPI free other 3 GPIO LINES
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
			ErrorFlag =2;
		}
	}else{
		// 2B hardware SPi Closes a SPI device 
		int spiErrorStatus = 0;
		spiErrorStatus =  Display_CLOSE_SPI;
		if (spiErrorStatus <0) 
		{
			fprintf(stderr, "Error: Cannot Close SPI device :(%s)\n", lguErrorText(spiErrorStatus));
			return rdlib::SPICloseFailure;
		}
	}
	// 3 Closes the opened gpiochip device.
	int GpioCloseStatus = 0;
	GpioCloseStatus =Display_CLOSE_GPIO_HANDLE; // close gpiochip
	if ( GpioCloseStatus < 0)
	{
		fprintf(stderr,"Error: Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rdlib::GpioChipDevice;
	}

	// 4 Check error flag ( we don't want to return early just for one failure)
	switch (ErrorFlag)
	{
		case 0:return rdlib::Success;break;
		case 2:return rdlib::GpioPinFree;break;
		case 3:return rdlib::SPICloseFailure;break;
		case 4:return rdlib::GpioChipDevice;break;
		default:fprintf(stderr, "Warning:Unknown error flag value in SPI-PowerDown"); break;
	}

	return rdlib::Success;
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
void ILI9341_TFT::setRotation(display_rotate_e rotation) {
	uint8_t madctlData = 0;
	switch (rotation)
	{
	case Degrees_0 :
		madctlData = (MADCTL_MX | MADCTL_BGR);
		_width = _widthStartTFT ;
		_height = _heightStartTFT ;
	break;
	case Degrees_90:
		madctlData = (MADCTL_MV | MADCTL_BGR);
		_width = _heightStartTFT ;
		_height = _widthStartTFT ;
	break;
	case Degrees_180:
		madctlData = (MADCTL_MY | MADCTL_BGR);
		_width = _widthStartTFT ;
		_height = _heightStartTFT ;
	break;
	case Degrees_270:
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

/*!
	@brief: Method for Hardware Reset pin control
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e ILI9341_TFT ::TFTResetPin() 
{
	if (_resetPinOn == true)
	{
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
		delayMilliSecRDL(20);
		Display_RST_SetHigh;
		delayMilliSecRDL(120);
	}
	return rdlib::Success;
}

/*!
	@brief Method for Hardware MISO  pin control set up as input if used
	@details if miso not used returns success by default
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e ILI9341_TFT ::TFTMISOPin() 
{
	if (_MISOPinOn == true)
	{
		// Claim GPIO as input for MISO line
		int GpioMISOErrorStatus = 0;
		GpioMISOErrorStatus= Display_MISO_SetDigitalInput;
		if (GpioMISOErrorStatus < 0 )
		{
			fprintf(stderr,"Error : Can't claim MISO GPIO for input (%s)\n", lguErrorText(GpioMISOErrorStatus));
			return rdlib::GpioPinClaim;
		}
	}
	return rdlib::Success;
}

/*!
	@brief: Method for Data or Command pin setup
	@return a rdlib::Return_Codes_e  code
		-# rdlib::Success
		-# rdlib::GpioPinClaim
*/
rdlib::Return_Codes_e ILI9341_TFT::TFTDataCommandPin(void) {
	
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
rdlib::Return_Codes_e ILI9341_TFT::TFTClock_Data_ChipSelect_Pins(void)
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
	@brief  Read a single diagnostic byte from the ILI9341 controller.
			This function sends a diagnostic read command and retrieves
			the corresponding status or mode byte from the display controller.
	@param reg The diagnostic register to read
	@param index Optional index offset (usually zero).
	@return The diagnostic byte read from the specified register. Will
			return early in software SPI if MISO pin not set and if user tries
			to run hardware SPI
	@note This is primarily intended for debugging and testing.
			Only works for Software SPI, as the library is currently configured,
			complete software control of chip select line required.  
 */
uint8_t ILI9341_TFT::readDiagByte(ILI9341_ReadRegister_e reg, uint8_t index) {
	uint8_t result = 0;
	if (_hardwareSPI) { 
			fprintf(stderr, "Warning: readDiagByte not configured for Hardware SPI, exiting \n");
			return 0xFF;
	} else { // software SPI 
		if (!_MISOPinOn) 	// check if MISO pin set 
		{
			fprintf(stderr, "Warning: MISO not set for SW SPI, exiting\n");
			return 0xFF;
		}
		Display_CS_SetLow;
		Display_DC_SetLow; spiWrite(0xD9);
		Display_DC_SetHigh; spiWrite(0x10 + index);
		Display_DC_SetLow; spiWrite(static_cast<uint8_t>(reg));
		Display_DC_SetHigh; result = spiRead();
		Display_CS_SetHigh;
	}
	return result;
}

/*!
	@brief software reset
	@details When the Software Reset command is written,
		it causes a software reset. It resets the commands and parameters to their
		S/W Reset default values.
*/
void ILI9341_TFT::SoftwareReset(void)
{
	writeCommand(ILI9341_SWRESET);
	delayMilliSecRDL(120);
}
