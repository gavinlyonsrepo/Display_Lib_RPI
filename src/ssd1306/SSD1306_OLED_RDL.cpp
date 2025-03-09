/*!
* @file SSD1306_OLED_RDL.cpp
* @brief   OLED driven by SSD1306 controller, Source file
* @author Gavin Lyons.
* @details Project Name: Display_Lib_RPI
* 	URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
*/

#include "../../include/ssd1306/SSD1306_OLED_RDL.hpp"

/*!
	@brief init the screen object
	@param oledwidth width of OLED in pixels 
	@param oledheight height of OLED in pixels 
*/
SSD1306_RDL::SSD1306_RDL(int16_t oledwidth, int16_t oledheight) :bicolor_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_PAGE_NUM = (_OLED_HEIGHT/8); 
}

/*!
	@brief  begin Method initialise OLED
*/
void  SSD1306_RDL::OLEDbegin()
{
	OLEDinit();
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
rdlib::Return_Codes_e SSD1306_RDL::OLEDSetBufferPtr(uint8_t width, uint8_t height , std::span<uint8_t> buffer)
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
	@brief  Start I2C operations
	@param I2C_device An I2C device number.
	@param I2C_addr  The address of a device on the I2C bus. PCF8574  default is 0x3C
	@param I2C_flags Flags which modify an I2C open command. None are currently defined.
	@return
		-#  rdlib::Success everything worked
		-#  rdlib::I2CbeginFail Cannot open I2C device
*/
rdlib::Return_Codes_e SSD1306_RDL::OLED_I2C_ON(int I2C_device, int I2C_addr , int I2C_flags)
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
	@brief End I2C operations. This closes the I2C device. 
	@return in event of any error
		-#  rdlib::Success everything worked
		-#  rdlib::I2CcloseFail cannot close I2c bus device
*/
rdlib::Return_Codes_e  SSD1306_RDL::OLED_I2C_OFF(void)
{
	int I2COpenHandleStatus = 0;
	I2COpenHandleStatus = Display_RDL_I2C_CLOSE(_OLEDI2CHandle);
	if (I2COpenHandleStatus < 0 )
	{
		fprintf(stderr, "Error OLED_I2C_OFF :: Can't Close I2C %s \n", lguErrorText(I2COpenHandleStatus));
		return rdlib::I2CcloseFail;
	}
	return rdlib::Success;
}

/*! 
	@brief Disables  OLED Call when powering down
*/
void SSD1306_RDL::OLEDPowerDown(void)
{
	OLEDEnable(0);
	delayMilliSecRDL(100);
}

/*!
	@brief Called from OLEDbegin carries out Power on sequence and register init
*/
void SSD1306_RDL::OLEDinit()
 {

	const uint8_t  SSD1306_INITDELAY = 100 ;/**< Initialisation delay in mS */
	delayMilliSecRDL(SSD1306_INITDELAY);
	I2CWriteByte( SSD1306_DISPLAY_OFF);
	I2CWriteByte( SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
	I2CWriteByte( 0x80); // default RESET values. 
	I2CWriteByte( SSD1306_SET_MULTIPLEX_RATIO );
	I2CWriteByte( _OLED_HEIGHT - 1 );
	I2CWriteByte( SSD1306_SET_DISPLAY_OFFSET );
	I2CWriteByte(0x00);
	I2CWriteByte( SSD1306_SET_START_LINE);
	I2CWriteByte( SSD1306_CHARGE_PUMP );
	I2CWriteByte(0x14); // Enable Charge Pump
	I2CWriteByte( SSD1306_MEMORY_ADDR_MODE );
	I2CWriteByte(0x00);  //Horizontal Addressing Mode is Used
	I2CWriteByte( SSD1306_SET_SEGMENT_REMAP| 0x01);
	I2CWriteByte( SSD1306_COM_SCAN_DIR_DEC );

switch (_OLED_HEIGHT)
{
	case 64: 
		I2CWriteByte( SSD1306_SET_COM_PINS );
		I2CWriteByte( 0x12 );
		I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
		I2CWriteByte(0xCF);
	break;
	case 32: 
		I2CWriteByte( SSD1306_SET_COM_PINS );
		I2CWriteByte( 0x02 );
		I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
		I2CWriteByte(0x8F);
	break;
	case 16: // NOTE: not tested, lacking part.
		I2CWriteByte( SSD1306_SET_COM_PINS );
		I2CWriteByte( 0x2 );
		I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
		I2CWriteByte(0xAF);
	break;
}

	I2CWriteByte( SSD1306_SET_PRECHARGE_PERIOD );
	I2CWriteByte( 0xF1 );
	I2CWriteByte( SSD1306_SET_VCOM_DESELECT );
	I2CWriteByte( 0x40 );
	I2CWriteByte( SSD1306_DISPLAY_ALL_ON_RESUME );
	I2CWriteByte( SSD1306_NORMAL_DISPLAY );
	I2CWriteByte( SSD1306_DEACTIVATE_SCROLL );
	I2CWriteByte( SSD1306_DISPLAY_ON );

	delayMilliSecRDL(SSD1306_INITDELAY);
}

/*!
	@brief Turns On Display
	@param bits   1  on , 0 off
*/
void SSD1306_RDL::OLEDEnable(uint8_t bits)
{
	bits ? I2CWriteByte(SSD1306_DISPLAY_ON) : I2CWriteByte(SSD1306_DISPLAY_OFF);
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void SSD1306_RDL::OLEDContrast(uint8_t contrast)
{
	I2CWriteByte( SSD1306_SET_CONTRAST_CONTROL );
	I2CWriteByte(contrast);
}

/*!
	@brief invert the display
	@param value true invert , false normal
*/
void SSD1306_RDL::OLEDInvert(bool value)
{
	value ? I2CWriteByte( SSD1306_INVERT_DISPLAY ) : I2CWriteByte( SSD1306_NORMAL_DISPLAY );
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay in milliseconds can be set to zero normally.
*/
void SSD1306_RDL::OLEDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		I2CWriteByte( 0xB0 | row);
		I2CWriteByte(SSD1306_SET_LOWER_COLUMN);
		I2CWriteByte(SSD1306_SET_HIGHER_COLUMN);
		for (uint8_t col = 0; col < _OLED_WIDTH; col++)
		{
			I2CWriteByte(dataPattern, SSD1306_DATA_CONTINUE);
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
void SSD1306_RDL::OLEDFillPage(uint8_t page_num, uint8_t dataPattern,uint8_t mydelay)
{
	uint8_t Result =0xB0 | page_num; 
	I2CWriteByte(Result);
	I2CWriteByte(SSD1306_SET_LOWER_COLUMN);
	I2CWriteByte(SSD1306_SET_HIGHER_COLUMN);
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		I2CWriteByte(dataPattern, SSD1306_DATA_CONTINUE);
		delayMilliSecRDL(mydelay);
	}
}

/*!
	@brief Writes a byte to I2C address,command or data, used internally
	@param value write the value to be written
	@param cmd command or data
	@note isDebugEnabled()  ,will output data on I2C failures.
*/
void SSD1306_RDL::I2CWriteByte(uint8_t value, uint8_t cmd)
{
	char ByteBuffer[2] = {cmd,value};
	uint8_t attemptI2Cwrite = _I2C_ErrorRetryNum;
	int  ReasonCodes = 0;
	
	ReasonCodes =Display_RDL_I2C_WRITE(_OLEDI2CHandle, ByteBuffer, 2); 
	while(ReasonCodes < 0)
	{//failure to write I2C byte ,Error handling retransmit
		
		if (rdlib_config::isDebugEnabled())
		{
			fprintf(stderr, "Error 602 I2C Command : %s\n", lguErrorText(ReasonCodes));
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
rdlib::Return_Codes_e SSD1306_RDL::OLEDupdate()
{
	if (_OLEDbuffer.empty())
	{
		fprintf(stderr, "Error: OLEDupdate: Buffer is empty, cannot update screen\r\n");
		return rdlib::BufferEmpty;
	}
	uint8_t x = 0; 
	uint8_t y = 0; 
	uint8_t w = this->_OLED_WIDTH; 
	uint8_t h = this->_OLED_HEIGHT;
	OLEDBufferScreen( x,  y,  w,  h, this->_OLEDbuffer);
	return rdlib::Success;
}

/*!
	@brief clears the buffer memory i.e. does NOT write to the screen
	@return 
		-# Success 
		-# BufferEmpty is buffer empty object
*/
rdlib::Return_Codes_e SSD1306_RDL::OLEDclearBuffer()
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
	@brief Draw the buffer directly to the screen
	@param x x axis  offset
	@param y y axis offset
	@param w width
	@param h height
	@param data the span of the buffer data
	@note Called by OLEDupdate internally 
*/
void SSD1306_RDL::OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h,  std::span<uint8_t> data)
{
	
	uint8_t tx, ty;
	uint16_t offset = 0;
		
	I2CWriteByte( SSD1306_SET_COLUMN_ADDR );
	I2CWriteByte(0);   // Column start address (0 = reset)
	I2CWriteByte( _OLED_WIDTH-1 ); // Column end address (127 = reset)

	I2CWriteByte( SSD1306_SET_PAGE_ADDR );
	I2CWriteByte(0); // Page start address (0 = reset)
	
	switch (_OLED_HEIGHT)
	{
		case 64: I2CWriteByte(7); break;
		case 32: I2CWriteByte(3); break;
		case 16: I2CWriteByte(1); break;
	}
	
	for (ty = 0; ty < h; ty = ty + 8)
		{
		if (y + ty < 0 || y + ty >= _OLED_HEIGHT) {continue;}
		for (tx = 0; tx < w; tx++)
		{

			if (x + tx < 0 || x + tx >= _OLED_WIDTH) {continue;}
			offset = (w * (ty /8)) + tx;
			I2CWriteByte(data[offset++], SSD1306_DATA_CONTINUE);
		}
	}

}

/*!
	@brief Draws a Pixel to the screen overrides the graphics lib
	@param x x axis  position  
	@param y y axis  position
	@param color color of pixel.
*/
void SSD1306_RDL::drawPixel(int16_t x, int16_t y, uint8_t color)
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
	@brief Scroll OLED data to the right
	@param start start position
	@param stop stop position 
*/
void SSD1306_RDL::OLEDStartScrollRight(uint8_t start, uint8_t stop) 
{
	
	I2CWriteByte(SSD1306_RIGHT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);  // start page
	I2CWriteByte(0X00);
	I2CWriteByte(stop);   // end page
	I2CWriteByte(0X00);
	I2CWriteByte(0XFF);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data to the left
	@param start start position
	@param stop stop position 
*/
void SSD1306_RDL::OLEDStartScrollLeft(uint8_t start, uint8_t stop) 
{
	
	I2CWriteByte(SSD1306_LEFT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);
	I2CWriteByte(0X00);
	I2CWriteByte(stop);
	I2CWriteByte(0X00);
	I2CWriteByte(0XFF);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data diagonally to the right
	@param start start position
	@param stop stop position 
*/
void SSD1306_RDL::OLEDStartScrollDiagRight(uint8_t start, uint8_t stop) 
{
	
	I2CWriteByte(SSD1306_SET_VERTICAL_SCROLL_AREA);
	I2CWriteByte(0X00);
	I2CWriteByte(_OLED_HEIGHT);
	I2CWriteByte(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);
	I2CWriteByte(0X00);
	I2CWriteByte(stop);
	I2CWriteByte(0X01);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief Scroll OLED data diagonally to the left
	@param start start position
	@param stop stop position 
*/
void SSD1306_RDL::OLEDStartScrollDiagLeft(uint8_t start, uint8_t stop) 
{
	
	I2CWriteByte(SSD1306_SET_VERTICAL_SCROLL_AREA);
	I2CWriteByte(0X00);
	I2CWriteByte(_OLED_HEIGHT);
	I2CWriteByte(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	I2CWriteByte(0X00);
	I2CWriteByte(start);
	I2CWriteByte(0X00);
	I2CWriteByte(stop);
	I2CWriteByte(0X01);
	I2CWriteByte(SSD1306_ACTIVATE_SCROLL);
}

/*!
	@brief  Stop scroll mode
*/
void SSD1306_RDL::OLEDStopScroll(void) 
{
	
	I2CWriteByte(SSD1306_DEACTIVATE_SCROLL);
}


/*!
	@brief get I2C error Flag
	@return See Error Codes at bottom of https://abyz.me.uk/lg/lgpio.html
*/
int SSD1306_RDL::OLEDI2CErrorGet(void) { return _I2C_ErrorFlag;}

/*!
	 @brief Sets the I2C timeout, in the event of an I2C write error
	@param newTimeout I2C timeout delay in mS
	@details Delay between retry attempts in event of an error , mS
*/
void SSD1306_RDL::OLEDI2CErrorTimeoutSet(uint16_t newTimeout)
{
	_I2C_ErrorDelay = newTimeout;
}

/*!
	 @brief Gets the I2C timeout, used in the event of an I2C write error
	 @details Delay between retry attempts in event of an error , mS
	 @return  I2C timeout delay in mS, _I2C_ErrorDelay
*/
uint16_t SSD1306_RDL::OLEDI2CErrorTimeoutGet(void){return _I2C_ErrorDelay;}

/*!
	 @brief Gets the I2C error retry attempts, used in the event of an I2C write error
	 @details Number of times to retry in event of an error
	 @return   _I2C_ErrorRetryNum
*/
uint8_t SSD1306_RDL::OLEDI2CErrorRetryNumGet(void){return _I2C_ErrorRetryNum;}

/*!
	 @brief Sets the I2C error retry attempts used in the event of an I2C write error
	 @details Number of times to retry in event of an error
	 @param AttemptCount I2C retry attempts 
*/
void SSD1306_RDL::OLEDI2CErrorRetryNumSet(uint8_t AttemptCount)
{
	_I2C_ErrorRetryNum = AttemptCount;
}

/*!
	@brief checks if OLED on I2C bus
	@return lg Error codes, LG_OKAY   0x00 = Success
	@note Error codes are here https://abyz.me.uk/lg/lgpio.html
*/
int  SSD1306_RDL::OLEDCheckConnection(void)
{
	char rxdatabuf[1]; //buffer to hold return byte
	int I2CReadStatus = 0;

	I2CReadStatus = Display_RDL_I2C_READ(_OLEDI2CHandle, rxdatabuf, 1);
	if (I2CReadStatus < 0 )
	{
		fprintf(stderr, "Error :: OLED CheckConnection :: Cannot read device %s\n",lguErrorText(I2CReadStatus));
		rdlib_log::logData<int> error1("Error Cannot read device", I2CReadStatus);
	}

	_I2C_ErrorFlag = I2CReadStatus;
	return _I2C_ErrorFlag;
}


// ---  EOF ---
