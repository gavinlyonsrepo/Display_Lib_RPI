/*!
* @file SH110X_OLED_RDL.cpp
* @brief   OLED driven by  controller. Sh1106 and Sh1107, Source file
* @details Project Name: Display_Lib_RPI
* 	URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
*/

#include "../../include/sh110X/SH110X_OLED_RDL.hpp"

/*!
	@brief init the screen object
	@param oledwidth width of OLED in pixels
	@param oledheight height of OLED in pixels
*/
SH110X_RDL::SH110X_RDL(int16_t oledwidth, int16_t oledheight) :bicolor_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_PAGE_NUM = (_OLED_HEIGHT/8);
}

/*!
	@brief  begin Method initialise OLED
	@param OLEDtype enum type of display sh1106 or sh1107
	@param resetPin Used only if reset pin present on device, iF not = set to -1
	@param gpioDev device num gpiochip 4-RPI5, 0=RPI3 Used only rst pin on device
	@return
		-#  rdlib::Success everything worked
		-#  rdlib::GpioPinClaim Cannot claim the reset pin (if used)
		-#  rdlib::GpioChipDevice Cannot open the gpio handle for reset pin (if used)
*/
rdlib::Return_Codes_e SH110X_RDL::OLEDbegin(OLED_IC_type_e OLEDtype, int8_t resetPin, int gpioDev)
{
	_OLED_IC_type = OLEDtype;
	_Display_RST = resetPin;
	_DeviceNumGpioChip = gpioDev;
	// If reset pin is used , open gpio device and claim as output
	if(_Display_RST >= 0)
	{
		int GpioResetErrorstatus = 0;
		_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
		if ( _GpioHandle < 0)	// open error
		{
			fprintf(stderr,"Errror : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
			return rdlib::GpioChipDevice;
		}
		// Clain GPIO as outputs
		GpioResetErrorstatus = Display_RST_SetDigitalOutput;
		if (GpioResetErrorstatus < 0 )
		{
			fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorstatus));
			return rdlib::GpioPinClaim;
		}
	}
	OLEDinit();
	return rdlib::Success;
}

/*!
	@brief sets the buffer pointer to the users screen data buffer
	@param width width of buffer in pixels
	@param height height of buffer in pixels
	@param buffer the buffer span
	@return Will return rdlib::Return_Codes_e enum
		-# Success rdlib::Success
		-# Error 1 rdlib::BufferSize
		-# Error 2 rdlib::BufferEmpty
*/
rdlib::Return_Codes_e SH110X_RDL::OLEDSetBufferPtr(uint8_t width, uint8_t height , std::span<uint8_t> buffer)
{
	if(buffer.size() != static_cast<size_t>(width * (height / 8)))
	{
		fprintf(stderr, "Error 1: OLEDSetBufferPtr: buffer size does not equal : width * (height/8))\n");
		return rdlib::BufferSize;
	}
	if(buffer.empty())
	{
		fprintf(stderr, "Error 2: OLEDSetBufferPtr: Problem assigning buffer pointer, empty object\r\n");
		return rdlib::BufferEmpty;
	}
	_OLEDbuffer = buffer;
	return rdlib::Success;
}

/*!
	@brief  Start I2C operations.
	@return
		-#  rdlib::Success everything worked
		-#  rdlib::I2CbeginFail Cannot open I2C device
*/
rdlib::Return_Codes_e SH110X_RDL::OLED_I2C_ON(int I2C_device, int I2C_addr , int I2C_flags)
{
	_OLEDI2CDevice = I2C_device;
	_OLEDI2CAddress = I2C_addr;
	_OLEDI2CFlags = I2C_flags;

	int I2COpenHandle = 0;

	I2COpenHandle = Display_RDL_I2C_OPEN(_OLEDI2CDevice, _OLEDI2CAddress, _OLEDI2CFlags);
	if (I2COpenHandle < 0 )
	{
		fprintf(stderr, "Error OLED_I2C_ON :: Can't open I2C %s \n", lguErrorText(I2COpenHandle));
		return rdlib::I2CbeginFail;
	}
	else
	{
		_OLEDI2CHandle = I2COpenHandle;
		return rdlib::Success;
	}
}


/*!
	@brief  End I2C operations. This closes the I2C device.
	@return in event of any error
		-#  rdlib::Success everything worked
		-#  rdlib::GpioPinClaim Cannot free the reset pin (if used)
		-#  rdlib::GpioChipDevice Cannot close the gpio handle for reset pin (if used)
		-#  rdlib::I2CcloseFail cannot close I2c bus device
*/
rdlib::Return_Codes_e  SH110X_RDL::OLED_I2C_OFF(void)
{
	uint8_t ErrorFlag = 0; // Becomes >0 in event of error
	
	// 1 Is reset pin being used?
	if(_Display_RST >= 0) 
	{
		int GpioResetErrorStatus = 0;
		int GpioCloseStatus = 0;
		Display_RST_SetLow ;
		GpioResetErrorStatus =  Display_GPIO_FREE_RST; // Free reset pin
		if (GpioResetErrorStatus < 0 )
		{
			fprintf(stderr,"Error:Can't Free Reset GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
			ErrorFlag = 2;
		}
		// 2 close gpiochip
		GpioCloseStatus = Display_CLOSE_GPIO_HANDLE; 
		if ( GpioCloseStatus < 0)
		{
			fprintf(stderr,"Error:Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
			ErrorFlag = 3;
		}
	}

	// 3 close I2C device handle
	int I2COpenHandleStatus = 0;
	I2COpenHandleStatus = Display_RDL_I2C_CLOSE (_OLEDI2CHandle);
	if (I2COpenHandleStatus < 0 )
	{
		fprintf(stderr, "Error: OLED_I2C_OFF : Can't Close I2C %s \n", lguErrorText(I2COpenHandleStatus));
		ErrorFlag = 4;
	}
	
		// 4 Check error flag ( we don't want to return early just for one failure)
	switch (ErrorFlag)
	{
		case 0:return rdlib::Success;break;
		case 2:return rdlib::GpioPinFree;break;
		case 3:return rdlib::GpioChipDevice;break;
		case 4:return rdlib::I2CcloseFail;break;
		default:fprintf(stderr, "Warning:Unknown error flag value in OLED_I2C_OFFn"); break;
	}
	return rdlib::Success;
}

/*!
	@brief Disables  OLED Call when powering down
*/
void SH110X_RDL::OLEDPowerDown(void)
{
	OLEDEnable(0);
	delayMilliSecRDL(100);
}

/*!
	@brief Called from OLEDbegin carries out Power on sequence and register init
*/
void SH110X_RDL::OLEDinit()
{
	switch(_OLED_IC_type)
	{
		case SH1106_IC:
			SH1106_begin();
		break;
		case SH1107_IC:
			SH1107_begin();
		break;
		default:
			fprintf(stderr, "Warning: OLEDinit: Unknown OLED type, Init Sh1106 by default\n");
			SH1106_begin();
		break;
	}
}

/*!
	@brief Handles OLED reset, if reset pin present on device.
*/
void SH110X_RDL::OLEDReset(void)
{
	const uint8_t resetDelay = 10; /**< reset delay in miliseconds*/

	Display_RST_SetDigitalOutput;
	Display_RST_SetHigh;
	delayMilliSecRDL(resetDelay);
	Display_RST_SetLow;
	delayMilliSecRDL(resetDelay);
	Display_RST_SetHigh;
}

/*!
 @brief Init routine for SH1106 OLED
*/
void SH110X_RDL::SH1106_begin(void)
{
	const uint16_t _INITDELAY = 100; /**< Initialisation delay in mS */

	if(_Display_RST >= 0)
	{
		OLEDReset();
	}
	pageStartOffset = 2; // the SH1106 display  requires a small offset 

	I2CWriteByte(SH110X_DISPLAYOFF);
	I2CWriteByte(SH110X_SETDISPLAYCLOCKDIV);
	I2CWriteByte(0x80);

	I2CWriteByte(SH110X_SETMULTIPLEX);
	I2CWriteByte(0x3F);

	I2CWriteByte(SH110X_SETDISPLAYOFFSET);
	I2CWriteByte(0x00);
	I2CWriteByte(SH110X_SETSTARTLINE);

	I2CWriteByte(SH110X_DCDC);
	I2CWriteByte(0x8B);

	I2CWriteByte(SH110X_SEGREMAP + 1); // Left rotation +1
	I2CWriteByte(SH110X_COMSCANDEC);
	I2CWriteByte(SH110X_SETCOMPINS);
	I2CWriteByte(0x12);  // Alternative (POR)

	I2CWriteByte(SH110X_SETCONTRAST);
	I2CWriteByte(0xFF);

	I2CWriteByte(SH110X_SETPRECHARGE);
	I2CWriteByte(0x1F);

	I2CWriteByte(SH110X_SETVCOMDETECT);
	I2CWriteByte(0x40);
	I2CWriteByte(0x33);

	I2CWriteByte(SH110X_NORMALDISPLAY);
	I2CWriteByte(SH110X_MEMORYMODE);
	I2CWriteByte(0x10);
	I2CWriteByte(SH110X_DISPLAYALLON_RESUME);

	delayMilliSecRDL(_INITDELAY);
	I2CWriteByte(SH110X_DISPLAYON);
}

/*!
 @brief Init routine for SH1107 OLED
*/
void SH110X_RDL::SH1107_begin(void)
{
	const uint16_t _INITDELAY = 100; /**< Initialisation delay in mS */

	if(_Display_RST >= 0)
	{
		OLEDReset();
	}

	I2CWriteByte(SH110X_DISPLAYOFF);
	I2CWriteByte(SH110X_SETDISPLAYCLOCKDIV);
	I2CWriteByte(0x51);
	I2CWriteByte(SH110X_MEMORYMODE);
	I2CWriteByte(SH110X_SETCONTRAST);
	I2CWriteByte(0x4F);
	I2CWriteByte(SH110X_DCDC);
	I2CWriteByte(0x8A);
	I2CWriteByte(SH110X_SEGREMAP);
	I2CWriteByte(SH110X_COMSCANINC);
	I2CWriteByte(SH110X_SETDISPSTARTLINE);
	I2CWriteByte(0x0);
	I2CWriteByte(SH110X_SETDISPLAYOFFSET);
	I2CWriteByte(0x60);
	I2CWriteByte(SH110X_SETPRECHARGE);
	I2CWriteByte(0x22);
	I2CWriteByte(SH110X_SETVCOMDETECT);
	I2CWriteByte(0x35);
	I2CWriteByte(SH110X_SETMULTIPLEX);
	I2CWriteByte(0x3F);
	I2CWriteByte(SH110X_DISPLAYALLON_RESUME);
	I2CWriteByte(SH110X_NORMALDISPLAY);

	if (_width == 128 && _height == 128)
	{
		I2CWriteByte(SH110X_SETDISPLAYOFFSET);
		I2CWriteByte(0x00);
		I2CWriteByte(SH110X_SETMULTIPLEX);
		I2CWriteByte(0x7F);
	}

	delayMilliSecRDL(_INITDELAY);
	I2CWriteByte(SH110X_DISPLAYON);
}

/*!
	@brief Turns On Display
	@param bits   1  on , 0 off
*/
void SH110X_RDL::OLEDEnable(uint8_t bits)
{
	
	bits ? I2CWriteByte(SH110X_DISPLAYALLON ) : I2CWriteByte(SH110X_DISPLAYOFF);
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void SH110X_RDL::OLEDContrast(uint8_t contrast)
{
	
	I2CWriteByte(SH110X_SETCONTRAST);
	I2CWriteByte(contrast);
}

/*!
	@brief invert the display
	@param value true invert , false normal
*/
void SH110X_RDL::OLEDInvert(bool value)
{
	
	value ? I2CWriteByte(SH110X_INVERTDISPLAY) : I2CWriteByte(SH110X_NORMALDISPLAY);
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay in milliseconds can be set to zero normally.
*/
void SH110X_RDL::OLEDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		I2CWriteByte( SH110X_SETPAGEADDR  | row);
		I2CWriteByte(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F));
		I2CWriteByte(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4 )); // 0x10 | 2 >>4 = 10
		for (uint8_t col = 0; col < _OLED_WIDTH; col++)
		{
			I2CWriteByte(dataPattern, SH110X_DATA_BYTE);
			delayMilliSecRDL(delay);
		}
	}
}

/*!
	@brief Fill the chosen page with a data pattern
	@param page_num chosen page (1-X)
	@param dataPattern can be set to 0 to FF (not buffer)
	@param mydelay optional delay in milliseconds can be set to zero normally.
*/
void SH110X_RDL::OLEDFillPage(uint8_t page_num, uint8_t dataPattern,uint8_t mydelay)
{
	uint8_t Result =SH110X_SETPAGEADDR | page_num;
	I2CWriteByte(Result);
	I2CWriteByte(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F)); // SH110X_SETLOWCOLUMN   = 0x00
	I2CWriteByte(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4));
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		I2CWriteByte(dataPattern, SH110X_DATA_BYTE);
		delayMilliSecRDL(mydelay);
	}
}

/*!
	@brief Writes a byte to I2C address,command or data, used internally
	@param value write the value to be written
	@param cmd command or data
	@note isDebugEnabled()  ,will output data on I2C failures.
*/
void SH110X_RDL::I2CWriteByte(uint8_t value, uint8_t cmd)
{
	char ByteBuffer[2] = {cmd,value};
	uint8_t attemptI2Cwrite = _I2C_ErrorRetryNum;
	int  ReasonCodes = 0;
	
	ReasonCodes =Display_RDL_I2C_WRITE(_OLEDI2CHandle, ByteBuffer, 2); 
	while(ReasonCodes < 0)
	{//failure to write I2C byte ,Error handling retransmit
		
		if (rdlib_config::isDebugEnabled())
		{
			fprintf(stderr, "Error 602 I2C Write : %s\n", lguErrorText(ReasonCodes));
			fprintf(stderr, "Attempt Count: %u\n", attemptI2Cwrite);
		}
		delayMilliSecRDL(_I2C_ErrorDelay); // delay mS
		ReasonCodes =Display_RDL_I2C_WRITE(_OLEDI2CHandle, ByteBuffer, 2); //retry
		_I2C_ErrorFlag = ReasonCodes; // set reasonCode to flag
		attemptI2Cwrite--; // Decrement retry attempt
		if (attemptI2Cwrite == 0) break;
	}
	_I2C_ErrorFlag = ReasonCodes;
}

/*!
	@brief updates the buffer i.e. writes it to the screen
	@return 
		-# Success 
		-# BufferEmpty if buffer is empty object
*/
rdlib::Return_Codes_e SH110X_RDL::OLEDupdate()
{
	if (_OLEDbuffer.empty())
	{
		fprintf(stderr, "Error: OLEDupdate: Buffer is empty, cannot update screen\r\n");
		return rdlib::BufferEmpty;
	}
	uint8_t w = this->_OLED_WIDTH; 
	uint8_t h = this->_OLED_HEIGHT;
	OLEDBufferScreen(w,  h, this->_OLEDbuffer);
	return rdlib::Success;
}

/*!
	@brief clears the buffer memory i.e. does NOT write to the screen
	@return 
		-# Success 
		-# BufferEmpty is buffer empty object
*/
rdlib::Return_Codes_e SH110X_RDL::OLEDclearBuffer()
{
	if (_OLEDbuffer.empty())
	{
		fprintf(stderr, "Error: OLEDclearBuffer: Buffer is empty, cannot clear\r\n");
		return rdlib::BufferEmpty;
	}

	std::fill(_OLEDbuffer.begin(), _OLEDbuffer.end(), 0x00);
	return rdlib::Success;
}

/*!
	@brief Draw the buffer to screen directly to the screen
	@param w width
	@param h height
	@param data the buffer data
	@note Called by OLEDupdate internally
*/
void SH110X_RDL::OLEDBufferScreen(uint8_t w, uint8_t h, std::span<uint8_t> data)
{
	uint8_t page;

	for (page = 0; page < (h/8); page++) 
	{
		I2CWriteByte(SH110X_SETPAGEADDR + page);
		I2CWriteByte(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F)); // SH110X_SETLOWCOLUMN   = 0x00
		I2CWriteByte(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4));
		//write data
		for(int i = 0; i< w; i++ ) 
		{
			I2CWriteByte(data[i+page*w], SH110X_DATA_BYTE);
		}
	}

}

/*!
	@brief Draws a Pixel to the screen overrides the graphics lib
	@param x x axis  position
	@param y y axis  position
	@param color color of pixel.
*/
void SH110X_RDL::drawPixel(int16_t x, int16_t y, uint8_t color)
{

	if ((x < 0) || (x >= this->_width) || (y < 0) || (y >= this->_height)) {
	return;
	}
	int16_t temp;
	uint8_t rotation = getRotation();
	switch (rotation) {
	case 1:
		temp = x;
		x = WIDTH - 1 - y;
		y = temp;
	break;
	case 2:
		x = WIDTH - 1 - x;
		y = HEIGHT - 1 - y;
	break;
	case 3:
		temp = x;
		x = y;
		y = HEIGHT - 1 - temp;
	break;
	}
	uint16_t tc = (_OLED_WIDTH * (y /8)) + x;
	switch (color)
	{
		case WHITE:  this->_OLEDbuffer[tc] &= ~(1 << (y & 7)); break;
		case BLACK:  this->_OLEDbuffer[tc] |= (1 << (y & 7)); break;
		case INVERSE: this->_OLEDbuffer[tc] ^= (1 << (y & 7)); break;
	}
}


/*!
	@brief get I2C error Flag
	@details See Error Codes at bottom of https://abyz.me.uk/lg/lgpio.html
	@return error code stored in _I2C_ErrorFlag
*/
uint8_t SH110X_RDL::OLEDI2CErrorGet(void) { return _I2C_ErrorFlag;}

/*!
	 @brief Sets the I2C timeout, in the event of an I2C write error
	@param newTimeout I2C timeout delay in mS
	@details Delay between retry attempts in event of an error , mS
*/
void SH110X_RDL::OLEDI2CErrorTimeoutSet(uint16_t newTimeout)
{
	_I2C_ErrorDelay = newTimeout;
}

/*!
	 @brief Gets the I2C timeout, used in the event of an I2C write error
	 @details Delay between retry attempts in event of an error , mS
	 @return  I2C timeout delay in mS, _I2C_ErrorDelay
*/
uint16_t SH110X_RDL::OLEDI2CErrorTimeoutGet(void){return _I2C_ErrorDelay;}

/*!
	 @brief Gets the I2C error retry attempts, used in the event of an I2C write error
	 @details Number of times to retry in event of an error
	 @return   _I2C_ErrorRetryNum
*/
uint8_t SH110X_RDL::OLEDI2CErrorRetryNumGet(void){return _I2C_ErrorRetryNum;}

/*!
	 @brief Sets the I2C error retry attempts used in the event of an I2C write error
	 @details Number of times to retry in event of an error
	 @param AttemptCount I2C retry attempts
*/
void SH110X_RDL::OLEDI2CErrorRetryNumSet(uint8_t AttemptCount)
{
	_I2C_ErrorRetryNum = AttemptCount;
}

/*!
	@brief checks if OLED on I2C bus
	@return lg Error codes, LG_OKAY   0x00 = Success
	@note Error codes are here https://abyz.me.uk/lg/lgpio.html
*/
int  SH110X_RDL::OLEDCheckConnection(void)
{
	char rxdatabuf[1]; //buffer to hold return byte
	int I2CReadStatus = 0;

	I2CReadStatus = Display_RDL_I2C_READ(_OLEDI2CHandle, rxdatabuf, 1);
	if (I2CReadStatus < 0 )
	{
		fprintf(stderr, "Error:OLED CheckConnection :: Cannot read device %s\n",lguErrorText(I2CReadStatus));
	}

	_I2C_ErrorFlag = I2CReadStatus;
	return _I2C_ErrorFlag;
}

// --- EOF ---
