/*!
* @file NOKIA_5110_LCD_RDL.cpp
* @brief   Source file for NOKIA 5110 library to communicate with LCD
* @author Gavin Lyons.
*/

#include "../../include/nokia5110/NOKIA_5110_LCD_RDL.hpp"


/*!
	@brief Constructor of the class object from left to right pin 1-5(LCD)
	@param lcdwidth width of lcd in pixels
	@param lcdheight height of lcd in pixels
	@param LCD_RST reset GPIO
	@param LCD_DC data or command GPIO
	@param LCD_CE chip enable GPIO
	@param LCD_DIN data in GPIO
	@param LCD_CLK clock GPIO
	@note Software SPI default , RST pin 1, DC pin 2,CE pin 3, DIN pin 4, CLK pin 5 , overloaded
*/
NOKIA_5110_RPI::NOKIA_5110_RPI(int16_t lcdwidth, int16_t lcdheight, uint8_t LCD_RST, uint8_t LCD_DC, uint8_t LCD_CE, int8_t LCD_DIN, int8_t LCD_CLK) :bicolor_graphics(lcdwidth, lcdheight)
{
	_Display_RST = LCD_RST;
	_Display_DC  = LCD_DC;
	_Display_CS = LCD_CE;
	_Display_SDATA  = LCD_DIN;
	_Display_SCLK  = LCD_CLK;
	_LCD_WIDTH = lcdwidth;
	_LCD_HEIGHT = lcdheight;
	_LCDHardwareSPI = false;
}


/*!
	@brief Constructor of the class object from left to right pin 1-2(LCD)
	@param lcdwidth width of lcd in pixels
	@param lcdheight height of lcd in pixels
	@param LCD_RST reset GPIO
	@param LCD_DC data or command GPIO
	@note Hardware SPI default , RST pin 1, DC pin 2 , overloaded
*/
NOKIA_5110_RPI::NOKIA_5110_RPI(int16_t lcdwidth, int16_t lcdheight, uint8_t LCD_RST, uint8_t LCD_DC) : bicolor_graphics(lcdwidth, lcdheight)

{
	_Display_RST = LCD_RST;
	_Display_DC  = LCD_DC;
	_LCD_WIDTH = lcdwidth;
	_LCD_HEIGHT = lcdheight;
	_LCDHardwareSPI = true;
}


/*!
	@brief This sends the commands to the PCD8544 to init LCD
	@param Inverse false normal mode true display inverted
	@param Contrast Set LCD VOP contrast range 0xB1-BF
	@param Bias LCD Bias mode 1:48 0x12 to 0x14
	@param device A SPI device, >= 0. 
	@param channel A SPI channel, >= 0. 
	@param speed The speed of serial communication in bits per second. 
	@param flags The flags may be used to modify the default behaviour. Set to 0(mode 0) for this device.
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@note overloaded this one is for  Hardware SPI
	@return rpiDisplay_Return_Codes_e
		-# rpiDisplay_Success
		-# rpiDisplay_WrongModeChosen
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinClaim
		-# rpiDisplay_SPIOpenFailure
*/
rpiDisplay_Return_Codes_e NOKIA_5110_RPI::LCDBegin(bool Inverse, uint8_t Contrast,uint8_t Bias, int device, int channel, int speed, int flags, int gpioDev)
{
	_inverse = Inverse;
	_bias = Bias;
	_contrast = Contrast;
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;

	// 1. check communication mode being called, if user called wrong one.
	if(isHardwareSPI() == false)
	{
		printf("Wrong SPI mode chosen this method is for Hardware SPI \n");
		return rpiDisplay_WrongModeChosen;
	}

	// 2. setup gpioDev
	_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Error : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rpiDisplay_GpioChipDevice;
	}

	// 3. Claim 2 GPIO as outputs for RST and CD lines
	int GpioResetErrorStatus = 0;
	int GpioDCErrorStatus = 0;
	GpioResetErrorStatus= Display_RST_SetDigitalOutput;
	GpioDCErrorStatus= Display_DC_SetDigitalOutput;
	if (GpioResetErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}
	if (GpioDCErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim DC GPIO for output (%s)\n", lguErrorText(GpioDCErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}

	// 4. set up spi open
	 _spiHandle  = Display_OPEN_SPI;
	if ( _spiHandle  < 0)
	{
		fprintf(stderr, "Error : Cannot open SPI :(%s)\n", lguErrorText( _spiHandle ));
		return rpiDisplay_SPIOpenFailure;
	}

	LCDInit();
	return rpiDisplay_Success;
}

/*!
	@brief This sends the commands to the PCD8544 to init LCD
	@param Inverse false normal mode true display inverted
	@param Contrast Set LCD VOP contrast range 0xB1-BF
	@param Bias LCD Bias mode 1:48 0x12 to 0x14
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@note overloaded this one is for Software SPI
	@return rpiDisplay_Return_Codes_e
		-# rpiDisplay_Success
		-# rpiDisplay_WrongModeChosen
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinClaim
*/
rpiDisplay_Return_Codes_e NOKIA_5110_RPI::LCDBegin(bool Inverse, uint8_t Contrast,uint8_t Bias, int gpioDev)
{
	_inverse = Inverse;
	_bias = Bias;
	_contrast = Contrast;
	_DeviceNumGpioChip = gpioDev;
	bool ErrorFlag = true;

	// 1. check communication mode being called, if user called wrong one.
	if(isHardwareSPI() == true )
	{
		printf("Wrong SPI mode chosen this method is for Software SPI.\n");
		return rpiDisplay_WrongModeChosen;
	}

	// 2. setup gpioDev
	_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Error : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rpiDisplay_GpioChipDevice;
	}

	// 3. Claim 5 GPIO as outputs
	int GpioResetErrorStatus = 0;
	int GpioDCErrorStatus = 0;
	int GpioCEErrorStatus = 0;
	int GpioCLKErrorStatus = 0;
	int GpioDINErrorStatus = 0;

	GpioResetErrorStatus= Display_RST_SetDigitalOutput;
	GpioDCErrorStatus= Display_DC_SetDigitalOutput;
	GpioCEErrorStatus= Display_CS_SetDigitalOutput;
	GpioCLKErrorStatus= Display_SCLK_SetDigitalOutput;
	GpioDINErrorStatus= Display_SDATA_SetDigitalOutput;

	if (GpioResetErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorStatus));
	} else if (GpioDCErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim DC GPIO for output (%s)\n", lguErrorText(GpioDCErrorStatus));
	} else if (GpioCEErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim CE GPIO for output (%s)\n", lguErrorText(GpioCEErrorStatus));
	} else if (GpioCLKErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim SCLK GPIO for output (%s)\n", lguErrorText(GpioCLKErrorStatus));
	} else if (GpioDINErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim DIN GPIO for output (%s)\n", lguErrorText(GpioDINErrorStatus));
	} else { ErrorFlag = false;}

	if (ErrorFlag == true ) {return rpiDisplay_GpioPinClaim;}

	Display_CS_SetHigh;
	LCDInit();
	return rpiDisplay_Success;
}

/*! 
	@brief Init the LCD command sequence, called from begin 
*/
void  NOKIA_5110_RPI::LCDInit(void)
{
	delayMilliSecRDL(100);
	Display_RST_SetHigh;
	Display_RST_SetLow;
	delayMilliSecRDL(50);
	Display_RST_SetHigh;
	// get into the EXTENDED mode
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand(_bias);
	LCDWriteCommand(LCD_SETTEMP);
	LCDWriteCommand(_contrast);
	LCDWriteCommand(LCD_FUNCTIONSET); //We must send 0x20 before modifying the display control mode
	// set LCDdisplayUpdate to normal mode or inverted
	if (_inverse  == false)
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
	else
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
}

/*!
	@brief End SPI operations. 
	@return
		-#  rpiDisplay_Success
		-#  rpiDisplay_GpioPinFree
		-#  rpiDisplay_SPICloseFailure
		-#  rpiDisplay_GpioChipDevice
*/
rpiDisplay_Return_Codes_e  NOKIA_5110_RPI::LCDSPIoff(void)
{
	uint8_t ErrorFlag = 0; // Becomes >0 in event of error
	
	// 1. free rst & DC GPIO lines
	int GpioResetErrorStatus = 0;
	int GpioDCErrorStatus = 0;
	Display_RST_SetLow;
	Display_DC_SetLow;
	GpioResetErrorStatus = Display_GPIO_FREE_RST;
	GpioDCErrorStatus  =  Display_GPIO_FREE_DC;

	if (GpioResetErrorStatus < 0 ){
		fprintf(stderr,"Error: Can't Free RST GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
	}else if (GpioDCErrorStatus  < 0 ){
		fprintf(stderr,"Error: Can't Free DC GPIO (%s)\n", lguErrorText(GpioDCErrorStatus));
	}
	
	// 2A Software SPI other 3 GPIO LINES
	if (isHardwareSPI()  == false) // software spi
	{
		int GpioCEErrorStatus = 0;
		int GpioCLKErrorStatus = 0;
		int GpioDINErrorStatus = 0;
		Display_CS_SetLow;
		Display_SCLK_SetLow;
		Display_SDATA_SetLow;
		GpioCEErrorStatus = Display_GPIO_FREE_CS;
		GpioCLKErrorStatus =  Display_GPIO_FREE_CLK;
		GpioDINErrorStatus =   Display_GPIO_FREE_SDATA;
		if (GpioCEErrorStatus < 0 ){
			fprintf(stderr,"Error: Can't Free CE GPIO (%s)\n", lguErrorText(GpioCEErrorStatus ));
			ErrorFlag = 2;
		}else if (GpioCLKErrorStatus< 0 ){
			fprintf(stderr,"Error: Can't Free CLK GPIO t (%s)\n", lguErrorText(GpioCLKErrorStatus));
			ErrorFlag = 2;
		}else if (GpioDINErrorStatus< 0 ){
			fprintf(stderr, "Error: Can't free DATA GPIO (%s)\n", lguErrorText(GpioDINErrorStatus));
			ErrorFlag = 2;
		}
	 // 2B hardware SPi Closes a SPI device 
	}else
	{
		int spiErrorStatus = 0;
		spiErrorStatus =  Display_CLOSE_SPI;
		if (spiErrorStatus <0) 
		{
			fprintf(stderr, "Error : Cannot Close SPI device :(%s)\n", lguErrorText(spiErrorStatus));
			ErrorFlag = 3;
		}
	}
	// 3 Closes the opened gpiochip device.
	int GpioCloseStatus = 0;
	GpioCloseStatus =Display_CLOSE_GPIO_HANDLE; // close gpiochip
	if ( GpioCloseStatus < 0)
	{
		fprintf(stderr,"Error: Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		ErrorFlag = 4;
	}
	// 4 Check error flag ( we don't want to return early just for one failure)
	switch (ErrorFlag)
	{
		case 0:return rpiDisplay_Success;break;
		case 2:return rpiDisplay_GpioPinFree;break;
		case 3:return rpiDisplay_SPICloseFailure;break;
		case 4:return rpiDisplay_GpioChipDevice;break;
		default:printf("Warning:Unknown error flag value in LCDSPIoff"); break;
	}
	return rpiDisplay_Success;
}


/*!
	@brief Call when powering down LCD
*/
void NOKIA_5110_RPI::LCDPowerDown(void)
{
	Display_DC_SetLow;
	Display_RST_SetLow;

	if(isHardwareSPI() == false)
	{
		Display_CS_SetLow;
		Display_SCLK_SetLow;
		Display_SDATA_SetLow;
	}
}

/*!
	@brief  Writes a byte to the PCD8544
	@param dataByte byte will be sent as command or data depending on status of DC line
*/
void NOKIA_5110_RPI::LCDWriteData(uint8_t dataByte)
{
	if (isHardwareSPI() == false)
	{
		uint8_t bit_n;
		for (bit_n = 0x80; bit_n; bit_n >>= 1)
		{
			Display_SCLK_SetLow;
			delayMicroSecRDL(_LCDHighFreqDelay);
			if (dataByte & bit_n)
				Display_SDATA_SetHigh;
			else
				Display_SDATA_SetLow;
			Display_SCLK_SetHigh;
			delayMicroSecRDL(_LCDHighFreqDelay);
		}
	}else{
			int spiErrorStatus = 0;
			char TransmitBuffer[1];
			TransmitBuffer[0] = dataByte;
			spiErrorStatus = Display_SPI_WRITE( _spiHandle, static_cast<const char*>(TransmitBuffer), sizeof(TransmitBuffer));
			if (spiErrorStatus <0) 
			{
				fprintf(stderr, "Error : Failure to Write  SPI :(%s)\n", lguErrorText(spiErrorStatus));
			}
		}
}

/*!
	@brief  Writes a command byte to the PCD8544
	@param command The command byte to send
*/
void NOKIA_5110_RPI::LCDWriteCommand(uint8_t command) {
	Display_DC_SetLow;
	if (isHardwareSPI() == false)Display_CS_SetLow;
	LCDWriteData(command);
	if (isHardwareSPI() == false)Display_CS_SetHigh;
}

/*!
	@brief Function to set contrast passed a byte
	@param contrast Set LCD VOP Contrast B0 to BF
*/
void NOKIA_5110_RPI::LCDSetContrast(uint8_t contrast) {
	_contrast = contrast;
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand(_contrast);
	LCDWriteCommand(LCD_FUNCTIONSET);
}

/*!
	@brief Writes the buffer to the LCD
*/
void NOKIA_5110_RPI::LCDdisplayUpdate(void) {
	uint16_t i;
	LCDWriteCommand(LCD_SETYADDR);  // set y = 0
	LCDWriteCommand(LCD_SETXADDR);  // set x = 0

	Display_DC_SetHigh; //Data send
	if (isHardwareSPI() == false) Display_CS_SetLow;

	for(i = 0; i < _LCD_Display_size; i++)  
	LCDWriteData( LCDDisplayBuffer[i] );

	if (isHardwareSPI() == false)Display_CS_SetHigh;
}


/*!
	@brief Set a single pixel in the buffer
	@param x  x coordinate
	@param y  y coordinate
	@param color Color of pixel
*/
void NOKIA_5110_RPI::drawPixel(int16_t x, int16_t y, uint8_t color) {

	if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
	return;
	}
	
	uint8_t rotation = getRotation();
	switch(rotation) 
	{
		case 1:
			LCD_Rotate_swap_int16_t(x, y);
			y =  HEIGHT - 1 - y;
			break;
		case 2:
			x = WIDTH - 1 - x;
			y = HEIGHT - 1 - y;
			break;
		case 3:
			LCD_Rotate_swap_int16_t(x, y);
			x = WIDTH - 1 - x;
	}

	switch (color)
	{
		case RDL_WHITE:   LCDDisplayBuffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7)); break;
		case RDL_BLACK:   LCDDisplayBuffer[x + (y / 8) * WIDTH] |= (1 << (y & 7)); break;
		case RDL_INVERSE: LCDDisplayBuffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7)); break;
	}
}


/*!
	@brief Writes the buffer (full of zeros) to the LCD
*/
void NOKIA_5110_RPI::LCDdisplayClear(void) {
	uint16_t i;
	for (i = 0; i < _LCD_Display_size ; i++)
		LCDDisplayBuffer[i] = 0x00;
}

/*!
	@brief Writes the buffer (full of ones(0xFF)) to the LCD
*/
void NOKIA_5110_RPI::LCDfillScreen() {
	uint16_t i;
	for (i = 0; i < _LCD_Display_size; i++)
		LCDDisplayBuffer[i] = 0xFF;
}

/*!
	@brief Writes the buffer (with pattern) to the LCD
	@param Pattern The pattern to send 0x00 to 0xFF
*/
void NOKIA_5110_RPI::LCDfillScreenPattern(uint8_t Pattern) {
	uint16_t i;
	for (i = 0; i < _LCD_Display_size; i++)
		LCDDisplayBuffer[i] = Pattern;
}


/*!
	@brief inverts color on display
	@param invert True = Inverted mode , False = Display control mode normal
*/
void NOKIA_5110_RPI::LCDinvertDisplay(bool invert) {
	if (invert == false){
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
		_inverse = false;
	}
	else{
		_inverse = true;
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
	}
}

/*!
	@brief Turn on sleep mode
*/
void NOKIA_5110_RPI::LCDenableSleep(void)
{
	_sleep = true;
	LCDWriteCommand(LCD_FUNCTIONSET |  LCD_POWERDOWN);
}

/*!
	@brief Turn off sleep mode
*/
void NOKIA_5110_RPI::LCDdisableSleep(void)
{
	_sleep = false;
	// get into the EXTENDED mode
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand(_bias);
	LCDWriteCommand(LCD_SETTEMP);
	LCDWriteCommand(_contrast);
	LCDWriteCommand(LCD_FUNCTIONSET);   // normal mode
	// set LCDdisplayUpdate to normal mode
	if (_inverse  == false)
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
	else
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
}

/*!
	@brief LCDisSleeping
	@return  value of _sleep, if true LCD is in sleep mode.
*/
bool NOKIA_5110_RPI::LCDIsSleeping() { return _sleep;}

/*!
	@brief Checks which SPI mode is on
	@return true 1 if hardware SPI on , false 0 for software SPI
*/
bool NOKIA_5110_RPI::isHardwareSPI(){return _LCDHardwareSPI;}


/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t NOKIA_5110_RPI::LCDHighFreqDelayGet(void){return _LCDHighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  NOKIA_5110_RPI::LCDHighFreqDelaySet(uint16_t CommDelay){_LCDHighFreqDelay = CommDelay;}


/* ------------- EOF ------------------ */
