/*!
	@file ERM19264_UC1609_LCD_RDL.cpp
	@brief ERM19264 LCD driven by UC1609C controller, source file.
	@author Gavin Lyons.
*/

#include "../../include/erm19264/ERM19264_UC1609_LCD_RDL.hpp"

// Class Constructors 2 off:
// HWSPI buffer
// SWSPI buffer

// Class Constructors main  class, 2 off Hardware + software SPI

/*!
	@brief init the LCD  class object
	@param lcdwidth width of LCD in pixels
	@param lcdheight height of LCD in pixels
	@param rst GPIO reset
	@param cd GPIO data or command
	@note Hardware SPI version (mode 2), sub class of bicolor_graphics
 */
ERM19264_UC1609 :: ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight ,int8_t rst, int8_t cd) :bicolor_graphics(lcdwidth, lcdheight)
{
	_LCD_HEIGHT = lcdheight;
	_LCD_WIDTH = lcdwidth;
	_LCD_CD = cd;
	_LCD_RST= rst;
	_LCD_mode = 2;
}

/*!
	@brief init the LCD  class object
	@param lcdwidth width of LCD in pixels
	@param lcdheight height of LCD in pixels
	@param rst GPIO reset
	@param cd GPIO data or command
	@param cs GPIO Chip select
	@param sclk GPIO SPI Clock
	@param din GPIO MOSI
	@note Software  SPI version (mode 3) , sub class of bicolor_graphics
 */
ERM19264_UC1609 :: ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd, int8_t cs, int8_t sclk, int8_t din) :bicolor_graphics(lcdwidth, lcdheight)
{
	_LCD_HEIGHT = lcdheight;
	_LCD_WIDTH = lcdwidth;
	_LCD_CD = cd;
	_LCD_RST= rst;
	_LCD_CS = cs;
	_LCD_DIN = din;
	_LCD_SCLK = sclk;
	_LCD_mode = 3;
}

/*!
	@brief begin Method initialise LCD  Sets pinmodes and SPI setup
	@param AddressSet AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
	@param VbiasPOT contrast default = 0x49 , range 0x00 to 0xFE
	@param spi_divider default = 64 ,see bcm2835SPIClockDivider enum , bcm2835
	@param SPICE_Pin default = 0 , which SPI_CE pin to use , 0 or 1
	@details Start SPI operations(if HWSPI). Forces RPi SPI0 pins P1-19 (MOSI), P1-21 (MISO),
			P1-23 (CLK), P1-24 (CE0) or P1-26 (CE1) to alternate function ALT0, which enables those pins for SPI interface.
	@return rpiDisplay_Return_Codes_e enum.
		-# Success rpiDisplay_Success
		-# Error 1 rpiDisplay_SPIbeginFail;
 */
rpiDisplay_Return_Codes_e ERM19264_UC1609::LCDbegin(uint8_t AddressSet ,uint8_t VbiasPOT, uint32_t spi_divider, uint8_t SPICE_Pin)
{
	UC1609_RST_SetDigitalOutput;
	UC1609_CD_SetDigitalOutput;

	_VbiasPOT  = VbiasPOT;
	if (AddressSet > 7 ) AddressSet = 0x02;
	_AddressCtrl =  AddressSet;
	_SPICLK_DIVIDER  = spi_divider;
	_SPICE_PIN = SPICE_Pin;
	
	switch (GetCommMode())
	{
		case 2:
			if (!bcm2835_spi_begin())
				return rpiDisplay_SPIbeginFail;
		break;
		case 3:
			UC1609_CS_SetDigitalOutput;
			UC1609_SCLK_SetDigitalOutput;
			UC1609_DIN_SetDigitalOutput;
		break;
	}

	LCDinit();
	return rpiDisplay_Success;
}

/*!
	@brief Spi HW settings sets SPI HW settings Speed , Bit order ,mode + CEO pin 
*/
void ERM19264_UC1609::LCDSPIHWSettings(void)
{
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	
	if (_SPICLK_DIVIDER > 0)
		bcm2835_spi_setClockDivider(_SPICLK_DIVIDER);
	else // default BCM2835_SPI_CLOCK_DIVIDER_64 3.90MHz Rpi2, 6.250MHz RPI3
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); 
	
	if (_SPICE_PIN == 0)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	}else if (_SPICE_PIN  == 1)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
	}

}

/*!
	@brief End SPI operations.
	@details End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK),
	P1-24 (CE0) and P1-26 (CE1) are returned to their default INPUT behaviour.
*/
void ERM19264_UC1609::LCDSPIoff(void)
{
	bcm2835_spi_end();
}

/*!
	@brief Powerdown procedure for LCD see datasheet P40
*/
void ERM19264_UC1609::LCDPowerDown(void)
{
	LCDReset();
	LCDEnable(0);
	UC1609_CD_SetLow ;
	UC1609_RST_SetLow ;

	switch (GetCommMode())
	{
		case 2:LCDSPIoff(); break;
		case 3:
			UC1609_SDA_SetLow;
			UC1609_SCLK_SetLow ;
			UC1609_CS_SetLow ;
		break;
	}
	_sleep= true;
}

/*!
	@brief Called from LCDbegin carries out Power on sequence and register init
	Can be used to reset LCD to default values.
*/
void ERM19264_UC1609::LCDinit()
 {
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	UC1609_CD_SetHigh;

	delayMilliSecRDL(UC1609_INIT_DELAY2);

	LCDReset();

	send_command(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET);
	send_command(UC1609_ADDRESS_CONTROL, _AddressCtrl); //  changed by user
	send_command(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
	send_command(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);
	send_command(UC1609_POWER_CONTROL,  UC1609_PC_SET);
	delayMilliSecRDL(UC1609_INIT_DELAY);

	send_command(UC1609_GN_PM, 0);
	send_command(UC1609_GN_PM, _VbiasPOT); //  changed by user

	send_command(UC1609_DISPLAY_ON, 0x01); // turn on display
	send_command(UC1609_LCD_CONTROL, UC1609_ROTATION_NORMAL); // rotate to normal
	_sleep= false;

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}


/*!
	 @brief Sends a command to the display
	 @param command Command to send
	 @param value the values to change
	 @note command and value  will be combined with OR
*/
void ERM19264_UC1609::send_command (uint8_t command, uint8_t value)
{
	UC1609_CD_SetLow;
	send_data(command | value);
	UC1609_CD_SetHigh;
}

/*!
	@brief Resets LCD in a four wire setup called at start
	and  should also be called in a controlled power down setting
*/
void ERM19264_UC1609::LCDReset ()
{
	UC1609_RST_SetLow;
	delayMilliSecRDL(UC1609_RESET_DELAY);
	UC1609_RST_SetHigh;
	delayMilliSecRDL(UC1609_RESET_DELAY2);
}

/*!
	 @brief Turns On Display
	 @param bits  1  display on , 0 display off
*/
void ERM19264_UC1609::LCDEnable (uint8_t bits)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	bits ? _sleep = false: _sleep = true;
	send_command(UC1609_DISPLAY_ON, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*! @brief LCD Is Sleeping?
	@returns bool value of _sleep if true LCD is off and in sleep mode
*/
bool ERM19264_UC1609::LCDIssleeping() { return  _sleep ;}

/*!
	@brief Scroll the displayed image up by SL rows.
	@details The valid SL value is between 0 (for no
		scrolling) and (64).
		Setting SL outside of this range causes undefined effect on the displayed image.
	@param bits 0-64 line number y-axis
*/
void ERM19264_UC1609::LCDscroll (uint8_t bits)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	send_command(UC1609_SCROLL, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*!
	@brief Rotates the display by sending commands to display
	@details Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
		Param1: 4 possible values 000 010 100 110 (defined)
	@note If Mx is changed the buffer must BE updated see examples. 
*/
void ERM19264_UC1609::LCDrotate(uint8_t rotatevalue)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	switch (rotatevalue)
	{
		case 0: rotatevalue = 0; break;
		case 0x02: rotatevalue = UC1609_ROTATION_FLIP_ONE; break;
		case 0x04: rotatevalue = UC1609_ROTATION_NORMAL; break;
		case 0x06: rotatevalue = UC1609_ROTATION_FLIP_TWO; break;
		default: rotatevalue = UC1609_ROTATION_NORMAL; break;
	}
	send_command(UC1609_LCD_CONTROL, rotatevalue);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*!
	 @brief invert the display
	 @param bits 1 invert , 0 normal
*/
void ERM19264_UC1609::LCDinvert (uint8_t bits)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	send_command(UC1609_INVERSE_DISPLAY, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*!
	@brief turns on all Pixels
	@param bits Set DC[1] to force all SEG drivers to output ON signals
	1 all on ,  0 all off
*/
void ERM19264_UC1609::LCDallpixelsOn(uint8_t bits)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	send_command(UC1609_ALL_PIXEL_ON, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*!
	@brief Fill the chosen page at page_num  with a datapattern
	@param page_num the page number 0-7 
	@param dataPattern can be set to 0 to FF
*/
void ERM19264_UC1609::LCDFillPage(uint8_t page_num, uint8_t dataPattern)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}
	send_command(UC1609_SET_COLADD_LSB, 0);
	send_command(UC1609_SET_COLADD_MSB, 0);
	send_command(UC1609_SET_PAGEADD, page_num);
	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}

	delayMicroSecRDL(5);

	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	for (uint8_t i = 0; i < _LCD_WIDTH; i++)
	{
		send_data(dataPattern);
	}
	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
*/
void ERM19264_UC1609::LCDFillScreen(uint8_t dataPattern)
{
	for (uint8_t row = 0; row < _LCD_PAGE_NUM; row++)
	{
		LCDFillPage(row,dataPattern);
	}
}

/*!
	@brief Draw a bitmap into the screen
	@param x offset 0-192
	@param y offset 0-64
	@param w width 0-192
	@param h height 0-64
	@param data  pointer to the bitmap
*/
void ERM19264_UC1609::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >>3;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT) {continue;}
		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _LCD_WIDTH) {continue;}
			offset = (w * (ty >> 3)) + tx;
			send_data(data[offset]);
		}
	}

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*!
	 @brief Checks if software SPI is on
	 @returns 2 if hardware SPi on ,  3 for software spi
*/
int8_t  ERM19264_UC1609::GetCommMode(){return (_LCD_mode);}

/*!
	 @brief used in software SPI mode to shift out data
	 @param value the byte to go out 
	 @details if using high freq MCU the uS can be changed by LCDHighFreqDelaySet function, zero by default
*/
void ERM19264_UC1609::CustomshiftOut(uint8_t value)
{
	for (uint8_t  i = 0; i < 8; i++)
	{
	 	!!(value & (1 << (7 - i))) ? UC1609_SDA_SetHigh : UC1609_SDA_SetLow ;

		UC1609_SCLK_SetHigh;
		delayMicroSecRDL(_LCD_HighFreqDelay);
		UC1609_SCLK_SetLow;
		delayMicroSecRDL(_LCD_HighFreqDelay);
	}
}

/*!
	 @brief Send data byte with SPI to UC1609
	 @param byte the data byte to send 
*/
void ERM19264_UC1609::send_data(uint8_t byte)
{
	switch (GetCommMode())
	{
		case 2: bcm2835_spi_transfer(byte); break;
		case 3: CustomshiftOut(byte); break;
	}
}

/*!
	@brief updates the buffer i.e. writes it to the screen
*/
void ERM19264_UC1609::LCDupdate()
{
	uint8_t x = 0; 
	uint8_t y = 0; 
	uint8_t w = this->_LCD_WIDTH; 
	uint8_t h = this->_LCD_HEIGHT;
	LCDBuffer( x,  y,  w,  h, this->LCDbufferScreen);
}

/*!
	@brief clears the buffer i.e. does NOT write to the screen
*/
void ERM19264_UC1609::LCDclearBuffer()
{
	memset( this->LCDbufferScreen, 0x00, (this->_LCD_WIDTH * (this->_LCD_HEIGHT /8))  ); 
}

/*!
	 @brief Draw a data array  to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data pointer to the data array
	 @note Called by LCDupdate internally to write buffer to screen , can be called standalone 	as well
*/
void ERM19264_UC1609::LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data)
{
	switch (GetCommMode())
	{
		case 2: LCDSPIHWSettings(); break;
		case 3: UC1609_CS_SetLow; break;
	}

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y/8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT) {continue;}

		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0XF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _LCD_WIDTH) {continue;}
			offset = (w * (ty /8)) + tx;
			send_data(data[offset++]);
		}
	}

	switch (GetCommMode())
	{
		case 2: break;
		case 3: UC1609_CS_SetHigh; break;
	}
}

/*!
	@brief Draws a Pixel to the screen , overrides the  graphics library 
	@param x x co-ord of pixel
	@param y y co-ord of pixel
	@param colour colour of  pixel
*/
void ERM19264_UC1609::drawPixel(int16_t x, int16_t y, uint8_t colour)
{
	if ((x < 0) || (x >= this->_width) || (y < 0) || (y >= this->_height)){return;}

	int16_t temp;
	uint8_t rotation = getRotation();
	switch (rotation) 
	{
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
	uint16_t tc = (_LCD_WIDTH * (y /8)) + x;
	switch (colour)
	{
		case RDL_WHITE:  this->LCDbufferScreen[tc] &= ~(1 << (y & 7)); break;
		case RDL_BLACK:  this->LCDbufferScreen[tc] |= (1 << (y & 7)); break;
		case RDL_INVERSE: this->LCDbufferScreen[tc] ^= (1 << (y & 7)); break;
	}

}

/*! 
	@brief Freq delay used in SW SPI getter, uS delay used in CustomshiftOut method
	@return _LCD_HighFreqDelay
*/
uint16_t ERM19264_UC1609::LCD_HighFreqDelayGet(void){return _LCD_HighFreqDelay;}

/*! 
	@brief Freq delay used in SW SPI setter, uS delay used in CustomshiftOut method
	@param CommDelay uS  GPIO delay used in software SPI
*/
void ERM19264_UC1609::LCD_HighFreqDelaySet(uint16_t CommDelay){_LCD_HighFreqDelay = CommDelay;}

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
rpiDisplay_Return_Codes_e ERM19264_UC1609::LCDSetBufferPtr(uint8_t width, uint8_t height , uint8_t* pBuffer, uint16_t sizeOfBuffer)
{
	if(sizeOfBuffer !=  width * (height/8))
	{
		printf("LCDSetBufferPtr Error 1: buffer size does not equal : width * (height/8))\n");
		return rpiDisplay_BufferSize;
	}
	LCDbufferScreen = pBuffer;
	if(LCDbufferScreen ==  nullptr)
	{
		printf("LCDSetBufferPtr Error 2: Problem assigning buffer pointer, not a valid pointer object\r\n");
		return rpiDisplay_BufferNullptr;
	}
	return rpiDisplay_Success;
}

//****************  EOF ******************************8
