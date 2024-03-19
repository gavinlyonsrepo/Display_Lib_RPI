/*!
* @file SH110X_OLED_RDL.cpp
* @brief   OLED driven by SH110X_RDL controller. Sh1106 and Sh1107, Source file
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
	@param I2C_speed default=0 , 0=bcm2835_i2c_set_baudrate(100000) 100k baudrate, >0 = BCM2835_I2C_CLOCK_DIVIDER,choices=2500 ,626 ,150 ,148
	@param I2c_address by default 0x3C
	@param I2c_debug default false
	@param resetPin Used only if reset pin present on device, iF not = set to -1
*/
void SH110X_RDL::OLEDbegin(OLED_IC_type_e OLEDtype, uint16_t I2C_speed , uint8_t I2c_address, bool I2c_debug, int8_t resetPin)
{
	_I2C_speed = I2C_speed;
	_I2C_address = I2c_address;
	_I2C_DebugFlag = I2c_debug;
	_OLED_IC_type = OLEDtype;
	_Display_RST = resetPin;
	OLED_I2C_Settings();
	OLEDinit();
}

/*!
	@brief sets the buffer pointer to the users screen data buffer
	@param width width of buffer in pixels
	@param height height of buffer in pixels
	@param pBuffer the buffer array which decays to pointer
	@param sizeOfBuffer size of buffer
	@return Will return rpiDisplay_Return_Codes_e enum
		-# Success rpiDisplay_Success
		-# Error 1 rpiDisplay_BufferSize
		-# Error 2 rpiDisplay_BUfferNullptr
*/
rpiDisplay_Return_Codes_e SH110X_RDL::OLEDSetBufferPtr(uint8_t width, uint8_t height , uint8_t* pBuffer, uint16_t sizeOfBuffer)
{
	if(sizeOfBuffer !=  width * (height/8))
	{
		printf("OLEDSetBufferPtr Error 1: buffer size does not equal : width * (height/8))\n");
		return rpiDisplay_BufferSize;
	}
	OLEDbuffer = pBuffer;
	if(OLEDbuffer ==  nullptr)
	{
		printf("OLEDSetBufferPtr Error 2: Problem assigning buffer pointer, not a valid pointer object\r\n");
		return rpiDisplay_BufferNullptr;
	}
	return rpiDisplay_Success;
}

/*!
	@brief  Start I2C operations. Forces RPi I2C pins P1-03 (SDA) and P1-05 (SCL)
	to alternate function ALT0, which enables those pins for I2C interface.
	@return Error on I2C init failure , User not running as root?
*/
rpiDisplay_Return_Codes_e SH110X_RDL::OLED_I2C_ON()
{
	if (!bcm2835_i2c_begin())
		return rpiDisplay_I2CbeginFail;
	else
		return rpiDisplay_Success;
}

/*!
	@brief Sets the address and speed I2C bus , should be called before all writes.
*/
void SH110X_RDL::OLED_I2C_Settings()
{
	bcm2835_i2c_setSlaveAddress(_I2C_address);  //i2c address
	uint32_t I2CBaudRate = 100000;// 100K
	// BCM2835_I2C_CLOCK_DIVIDER enum choice 2500 626 150 148
	// Clock divided is based on nominal base clock rate of 250MHz
	switch(_I2C_speed)
	{
		case 0:
			// default or use set_baudrate instead of clockdivder 100k if zero passed
			bcm2835_i2c_set_baudrate(I2CBaudRate);
		break;
		case BCM2835_I2C_CLOCK_DIVIDER_2500:// ~100K
		case BCM2835_I2C_CLOCK_DIVIDER_626: // ~400k
		case BCM2835_I2C_CLOCK_DIVIDER_150:
		case BCM2835_I2C_CLOCK_DIVIDER_148:
			bcm2835_i2c_setClockDivider(_I2C_speed);
		break;
		default:
			// error message
			if (_I2C_DebugFlag == true)
			{
				printf("Warning OLED_I2C_Settings 610: Invalid BCM2835_I2C_CLOCK_DIVIDER value : %u\n", _I2C_speed);
				printf("	Must be 2500 626 150 or 148 \n");
				printf("	Setting I2C baudrate to 100K with bcm2835_i2c_set_baudrate:\n");
			}
			bcm2835_i2c_set_baudrate(I2CBaudRate);
		break;
	}
}

/*!
	@brief getter to read I2C bus speed
	@return I2C bus speed see Readme for details
*/
uint16_t SH110X_RDL::getI2Cspeed(void){return _I2C_speed;}

/*!
	@brief setter to set I2C bus speed
	@param I2CSpeed I2C bus speed see Readme for details
*/
void SH110X_RDL::setI2Cspeed(uint16_t I2CSpeed){_I2C_speed = I2CSpeed;}

/*!
	@brief End I2C operations. I2C pins P1-03 (SDA) and P1-05 (SCL)
	are returned to their default INPUT behaviour.
*/
void SH110X_RDL::OLED_I2C_OFF(void){bcm2835_i2c_end();}

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
			printf("Warning OLEDinit:: Unknown OLED type, Init Sh1106 by default\n");
			SH1106_begin();
		break;
	}
}

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
	const uint16_t SH110X_RDL_INITDELAY = 100; /**< Initialisation delay in mS */

	if(_Display_RST >= 0)
	{
		OLEDReset();
	}
	pageStartOffset = 2; // the SH1106 display  requires a small offset 

	SH110X_RDL_command(SH110X_DISPLAYOFF);
	SH110X_RDL_command(SH110X_SETDISPLAYCLOCKDIV);
	SH110X_RDL_command(0x80);

	SH110X_RDL_command(SH110X_SETMULTIPLEX);
	SH110X_RDL_command(0x3F);

	SH110X_RDL_command(SH110X_SETDISPLAYOFFSET);
	SH110X_RDL_command(0x00);
	SH110X_RDL_command(SH110X_SETSTARTLINE);

	SH110X_RDL_command(SH110X_DCDC);
	SH110X_RDL_command(0x8B);

	SH110X_RDL_command(SH110X_SEGREMAP + 1); // Left rotation +1
	SH110X_RDL_command(SH110X_COMSCANDEC);
	SH110X_RDL_command(SH110X_SETCOMPINS);
	SH110X_RDL_command(0x12);  // Alternative (POR)

	SH110X_RDL_command(SH110X_SETCONTRAST);
	SH110X_RDL_command(0xFF);

	SH110X_RDL_command(SH110X_SETPRECHARGE);
	SH110X_RDL_command(0x1F);

	SH110X_RDL_command(SH110X_SETVCOMDETECT);
	SH110X_RDL_command(0x40);
	SH110X_RDL_command(0x33);

	SH110X_RDL_command(SH110X_NORMALDISPLAY);
	SH110X_RDL_command(SH110X_MEMORYMODE);
	SH110X_RDL_command(0x10);
	SH110X_RDL_command(SH110X_DISPLAYALLON_RESUME);

	delayMilliSecRDL(SH110X_RDL_INITDELAY);
	SH110X_RDL_command(SH110X_DISPLAYON);
}

/*!
 @brief Init routine for SH1107 OLED
*/
void SH110X_RDL::SH1107_begin(void)
{
	const uint16_t SH110X_RDL_INITDELAY = 100; /**< Initialisation delay in mS */

	if(_Display_RST >= 0)
	{
		OLEDReset();
	}

	SH110X_RDL_command(SH110X_DISPLAYOFF);
	SH110X_RDL_command(SH110X_SETDISPLAYCLOCKDIV);
	SH110X_RDL_command(0x51);
	SH110X_RDL_command(SH110X_MEMORYMODE);
	SH110X_RDL_command(SH110X_SETCONTRAST);
	SH110X_RDL_command(0x4F);
	SH110X_RDL_command(SH110X_DCDC);
	SH110X_RDL_command(0x8A);
	SH110X_RDL_command(SH110X_SEGREMAP);
	SH110X_RDL_command(SH110X_COMSCANINC);
	SH110X_RDL_command(SH110X_SETDISPSTARTLINE);
	SH110X_RDL_command(0x0);
	SH110X_RDL_command(SH110X_SETDISPLAYOFFSET);
	SH110X_RDL_command(0x60);
	SH110X_RDL_command(SH110X_SETPRECHARGE);
	SH110X_RDL_command(0x22);
	SH110X_RDL_command(SH110X_SETVCOMDETECT);
	SH110X_RDL_command(0x35);
	SH110X_RDL_command(SH110X_SETMULTIPLEX);
	SH110X_RDL_command(0x3F);
	SH110X_RDL_command(SH110X_DISPLAYALLON_RESUME);
	SH110X_RDL_command(SH110X_NORMALDISPLAY);

	if (_width == 128 && _height == 128)
	{
		SH110X_RDL_command(SH110X_SETDISPLAYOFFSET);
		SH110X_RDL_command(0x00);
		SH110X_RDL_command(SH110X_SETMULTIPLEX);
		SH110X_RDL_command(0x7F);
	}

	delayMilliSecRDL(SH110X_RDL_INITDELAY);
	SH110X_RDL_command(SH110X_DISPLAYON);
}

/*!
	@brief Turns On Display
	@param bits   1  on , 0 off
*/
void SH110X_RDL::OLEDEnable(uint8_t bits)
{
	OLED_I2C_Settings();
	bits ? SH110X_RDL_command(SH110X_DISPLAYALLON ) : SH110X_RDL_command(SH110X_DISPLAYALLON_RESUME);
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void SH110X_RDL::OLEDContrast(uint8_t contrast)
{
	OLED_I2C_Settings();
	SH110X_RDL_command(SH110X_SETCONTRAST);
	SH110X_RDL_command(contrast);
}

/*!
	@brief invert the display
	@param value true invert , false normal
*/
void SH110X_RDL::OLEDInvert(bool value)
{
	OLED_I2C_Settings();
	value ? SH110X_RDL_command(SH110X_INVERTDISPLAY) : SH110X_RDL_command(SH110X_NORMALDISPLAY);
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay in milliseconds can be set to zero normally.
*/
void SH110X_RDL::OLEDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	OLED_I2C_Settings();
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		SH110X_RDL_command( SH110X_SETPAGEADDR  | row);
		SH110X_RDL_command(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F));
		SH110X_RDL_command(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4 )); // 0x10 | 2 >>4 = 10
		for (uint8_t col = 0; col < _OLED_WIDTH; col++)
		{
			SH110X_RDL_data(dataPattern);
			delayMilliSecRDL(delay);
		}
	}
}

/*!
	@brief Fill the chosen page(1-8)  with a datapattern
	@param page_num chosen page (1-8)
	@param dataPattern can be set to 0 to FF (not buffer)
	@param mydelay optional delay in milliseconds can be set to zero normally.
*/
void SH110X_RDL::OLEDFillPage(uint8_t page_num, uint8_t dataPattern,uint8_t mydelay)
{
	OLED_I2C_Settings();
	uint8_t Result =SH110X_SETPAGEADDR | page_num;
	SH110X_RDL_command(Result);
	SH110X_RDL_command(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F)); // SH110X_SETLOWCOLUMN   = 0x00
	SH110X_RDL_command(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4));
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		SH110X_RDL_data(dataPattern);
		delayMilliSecRDL(mydelay);
	}
}

/*!
	@brief Writes a byte to I2C address,command or data, used internally
	@param value write the value to be written
	@param cmd command or data
	@note In the event of an error will loop  _I2C_ErrorRetryNum times each time.with delay _I2C_ErrorDelay
	Printing the error code , see bcm2835I2CReasonCodes in bcm2835 docs.
*/
void SH110X_RDL::I2C_Write_Byte(uint8_t value, uint8_t cmd)
{
	char ByteBuffer[2] = {cmd,value};
	uint8_t attemptI2Cwrite = _I2C_ErrorRetryNum;
	uint8_t ReasonCodes = 0;

	ReasonCodes = bcm2835_i2c_write(ByteBuffer, 2);
	while(ReasonCodes != 0)
	{//failure to write I2C byte ,Error handling retransmit

		if (_I2C_DebugFlag == true)
		{
			printf("Error I2C_Write_Byte : Cannot Write byte attempt no :: %u\n", +attemptI2Cwrite);
			printf("bcm2835I2CReasonCodes :: Error code :: %u\n", +ReasonCodes);
		}
		delayMilliSecRDL(_I2C_ErrorDelay); // delay mS
		ReasonCodes  = bcm2835_i2c_write(ByteBuffer, 2); //retry
		_I2C_ErrorFlag = ReasonCodes; // set reasonCode to flag
		attemptI2Cwrite--; // Decrement retry attempt
		if (attemptI2Cwrite == 0) break;
	}
	_I2C_ErrorFlag = ReasonCodes;
}

/*!
	@brief updates the buffer i.e. writes it to the screen
*/
void SH110X_RDL::OLEDupdate()
{
	uint8_t w = this->_OLED_WIDTH; uint8_t h = this->_OLED_HEIGHT;
	OLEDBufferScreen(w,  h, this->OLEDbuffer);
}

/*!
	@brief clears the buffer memory i.e. does NOT write to the screen
*/
void SH110X_RDL::OLEDclearBuffer()
{
	memset( this->OLEDbuffer, 0x00, (this->_OLED_WIDTH* (this->_OLED_HEIGHT /8)));
}

/*!
	@brief Draw the buffer to screen directly to the screen
	@param x x axis  offset 0-128
	@param y y axis offset 0-64
	@param w width 0-128
	@param h height 0-64
	@param data the buffer data
	@note Called by OLEDupdate internally
*/
void SH110X_RDL::OLEDBufferScreen(uint8_t w, uint8_t h, uint8_t* data)
{
	OLED_I2C_Settings();
	uint8_t page;

	for (page = 0; page < (h/8); page++) 
	{
		SH110X_RDL_command(SH110X_SETPAGEADDR + page);
		SH110X_RDL_command(SH110X_SETLOWCOLUMN + (pageStartOffset & 0x0F)); // SH110X_SETLOWCOLUMN   = 0x00
		SH110X_RDL_command(SH110X_SETHIGHCOLUMN + (pageStartOffset >> 4));
		//write data
		for(int i = 0; i< w; i++ ) 
		{
			SH110X_RDL_data(data[i+page*w]);
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
		case RDL_WHITE:  this->OLEDbuffer[tc] &= ~(1 << (y & 7)); break;
		case RDL_BLACK:  this->OLEDbuffer[tc] |= (1 << (y & 7)); break;
		case RDL_INVERSE: this->OLEDbuffer[tc] ^= (1 << (y & 7)); break;
	}
}



/*!
	 @brief Turn DEBUG mode on or off setter
	 @param OnOff passed bool True = debug on , false = debug off
	 @note prints out statements, if ON and if errors occur
*/
void SH110X_RDL::OLEDDebugSet(bool OnOff)
{
	 OnOff ? (_I2C_DebugFlag  = true) : (_I2C_DebugFlag = false);
}

/*!
	 @brief get DEBUG mode status
	 @return debug mode status flag
*/
bool SH110X_RDL::OLEDDebugGet(void) { return _I2C_DebugFlag;}


/*!
	@brief get I2C error Flag
	@details bcm2835I2Creasoncode.
		-# BCM2835_I2C_REASON_OK   	     = 0x00,Success
		-# BCM2835_I2C_REASON_ERROR_NACK    = 0x01,Received a NACK
		-# BCM2835_I2C_REASON_ERROR_CLKT    = 0x02,Received Clock Stretch Timeout
		-# BCM2835_I2C_REASON_ERROR_DATA    = 0x04, Not all data is sent / receive
		-# BCM2835_I2C_REASON_ERROR_TIMEOUT = 0x08 Time out occurred during sending
	 @return I2C error flag = 0x00 no error , > 0 bcm2835I2Creasoncode.
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
	@return bcm2835I2CReasonCodes , BCM2835_I2C_REASON_OK 0x00 = Success
*/
uint8_t SH110X_RDL::OLEDCheckConnection(void)
{
	char rxdata[1]; //buffer to hold return byte

	bcm2835_i2c_setSlaveAddress(_I2C_address);  // set i2c address
	_I2C_ErrorFlag = bcm2835_i2c_read(rxdata, 1); // returns reason code , 0 success

	return _I2C_ErrorFlag;
}

// --- EOF ---
