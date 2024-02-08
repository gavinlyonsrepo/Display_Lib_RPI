 /*!
	@file ERM1_CH1115_OLED_RDL.cpp
	@brief ER_OLEDM1 OLED driven by CH1115 controller source file
	@details Project Name: Display_Lib_RPI
		URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
	@author  Gavin Lyons
*/

#include "../../include/ch1115/ERM1_CH1115_OLED_RDL.hpp"

// Class Constructors 2 off( overloaded): Hardware or software SPI.

/*!
	@brief init the OLED class object
	@param oledwidth width of oled in pixels
	@param oledheight height of oled in pixels
	@param rst GPIO reset
	@param cd GPIO data or command
	@note Hardware SPI version,  mode 2
 */
ERMCH1115  :: ERMCH1115(int16_t oledwidth, int16_t oledheight ,int8_t rst, int8_t cd) :bicolor_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_CD = cd;
	_OLED_RST= rst;
	_OLED_mode = 2;
}

/*!
	@brief init the OLED class object
	@param oledwidth width of oled in pixels
	@param oledheight height of oled in pixels
	@param rst GPIO reset
	@param cd GPIO data or command
	@param cs GPIO Chip select
	@param sclk GPIO SPI Clock
	@param din GPIO MOSI
	@note software SPI version, mode 3
 */
ERMCH1115  :: ERMCH1115(int16_t oledwidth, int16_t oledheight , int8_t rst, int8_t cd, int8_t cs, int8_t sclk, int8_t din) :bicolor_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_CD = cd;
	_OLED_RST= rst;
	_OLED_CS = cs;
	_OLED_DIN = din;
	_OLED_SCLK = sclk;
	_OLED_mode = 3;
}

// Class Functions

/*!
	@brief begin Method initialise OLED Sets pinmodes and SPI setup
	@param OLEDcontrast Contrast of the OLED display default = 0x80 , range 0x00 to 0xFE
	@param OLED_spi_divider default = 64 ,see bcm2835SPIClockDivider enum , bcm2835
	@param SPICE_Pin default = 0 , which SPI_CE pin to use , 0 or 1
	@return Error on , HW SPI failure init(bcm2835_spi_begin) 
	@details
		If HW SPI: Start SPI operations. Forces RPi SPI0 pins P1-19 (MOSI), P1-21 (MISO),
		P1-23 (CLK), P1-24 (CE0) and P1-26 (CE1) to alternate function ALT0, which enables those pins for SPI interface.
 */
rpiDisplay_Return_Codes_e ERMCH1115::OLEDbegin (uint8_t OLEDcontrast, uint32_t OLED_spi_divider, uint8_t SPICE_Pin)
{
	_OLEDcontrast  = OLEDcontrast ;
	_OLED_SPICLK_DIVIDER  = OLED_spi_divider;
	_OLED_SPICE_PIN = SPICE_Pin;

	bcm2835_gpio_fsel(_OLED_RST, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(_OLED_CD, BCM2835_GPIO_FSEL_OUTP);
	if(GetCommMode() == 3)
	{
		bcm2835_gpio_fsel( _OLED_CS, BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(_OLED_SCLK, BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(_OLED_DIN, BCM2835_GPIO_FSEL_OUTP);
	}else{
		if(!bcm2835_spi_begin())
			return rpiDisplay_SPIbeginFail;
	}

	OLEDinit();
	return rpiDisplay_Success;
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
rpiDisplay_Return_Codes_e  ERMCH1115::OLEDSetBufferPtr(uint8_t width, uint8_t height , uint8_t* pBuffer, uint16_t sizeOfBuffer)
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
	@brief Sets HW SPI settings
	@details sets bitmode bitorder, bus speed and CEX pin(1 or 0)
	usually just called at initialization internally but If multiple devices on SPI bus with different SPI settings
	can be called by user again before a tranche of OLED commands.
	@note Sets SPI mode 0 , MSBFIRST ,  BCM2835_SPI_CLOCK_DIVIDER_64  and CE0 by default.
*/
void ERMCH1115::OLEDSPIHWSettings(void)
{
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

	if (_OLED_SPICLK_DIVIDER > 0)
		bcm2835_spi_setClockDivider(_OLED_SPICLK_DIVIDER);
	else // default BCM2835_SPI_CLOCK_DIVIDER_64 3.90MHz Rpi2, 6.250MHz RPI3
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);

	if (_OLED_SPICE_PIN == 0)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	}else if (_OLED_SPICE_PIN  == 1)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
	}
}

/*!
	@brief stops HW spi operations
	@details End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK),
		P1-24 (CE0) and P1-26 (CE1) are returned to their default INPUT behaviour.
*/
void ERMCH1115::OLEDSPIoff(void)
{
	if (GetCommMode() == 3) bcm2835_spi_end();
}

/*! 
	@brief Power down function
	@details Disables screen and sets all independent GPIO low.
		call when powering down before end of operations 
*/
void ERMCH1115::OLEDPowerDown(void)
{
	OLEDEnable(0);
	bcm2835_gpio_write(_OLED_CD, LOW);
	bcm2835_gpio_write(_OLED_RST, LOW);
	if(GetCommMode()== 3)
	{
		bcm2835_gpio_write(_OLED_SCLK, LOW);
		bcm2835_gpio_write(_OLED_DIN, LOW);
		bcm2835_gpio_write(_OLED_CS, LOW);
	}
	_sleep= true;
}

/*!
	@brief Checks if software SPI is on
	@return _OLED_mode 2 if hardware SPi on ,  3 for software spi
*/
int8_t  ERMCH1115::GetCommMode(){return (_OLED_mode);}

/*!
	@brief used in software SPI mode to shift out data
	@param value Byte to send in MSBFIRST
	@details a GPIO uS delay is used here(_OLEDHighFreqDelay)
		user adjust for different CPU speeds possible
*/
void ERMCH1115::SoftwareSPIShiftOut(uint8_t value)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		!!(value & (1 << (7 - i))) ? ERMCH1115_SDA_SetHigh : ERMCH1115_SDA_SetLow ;

		ERMCH1115_SCLK_SetHigh;
		bcm2835_delayMicroseconds(_OLEDHighFreqDelay);
		ERMCH1115_SCLK_SetLow;
		bcm2835_delayMicroseconds(_OLEDHighFreqDelay);
	}
}

/*!
	@brief Called from OLEDbegin carries out Power on sequence and register init
*/
void ERMCH1115::OLEDinit()
 {
	switch (GetCommMode())
	{
		case 2: OLEDSPIHWSettings(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	OLEDReset();

	send_command(ERMCH1115_DISPLAY_OFF, 0);

	send_command(ERMCH1115_SET_COLADD_LSB, 0);
	send_command(ERMCH1115_SET_COLADD_MSB, 0);
	send_command(ERMCH1115_SET_PAGEADD, 0);
	send_command(ERMCH115_SET_DISPLAY_START_LINE, 0);

	send_command(ERMCH115_CONTRAST_CONTROL  ,0);
	send_command(_OLEDcontrast, 0);

	send_command(ERMCH1115_IREF_REG, 0);
	send_command(ERMCH1115_IREF_SET, 0);

	send_command(ERMCH1115_SEG_SET_REMAP, 0);
	send_command(ERMCH1115_SEG_SET_PADS, 0);
	send_command(ERMCH1115_ENTIRE_DISPLAY_ON, 0);
	send_command(ERMCH1115_DISPLAY_NORMAL, 0);

	send_command(ERMCH1115_MULTIPLEX_MODE_SET, 0);
	send_command(ERMCH1115_MULTIPLEX_DATA_SET , 0);

	send_command(ERMCH1115_COMMON_SCAN_DIR, 0);

	send_command(ERMCH1115_OFFSET_MODE_SET, 0);
	send_command(ERMCH1115_OFFSET_DATA_SET, 0);

	send_command(ERMCH1115_OSC_FREQ_MODE_SET, 0);
	send_command(ERMCH1115_OSC_FREQ_DATA_SET, 0);

	send_command(ERMCH1115_PRECHARGE_MODE_SET, 0);
	send_command(ERMCH1115_PRECHARGE_DATA_SET, 0);

	send_command(ERMCH1115_COM_LEVEL_MODE_SET, 0);
	send_command(ERMCH1115_COM_LEVEL_DATA_SET, 0);

	send_command(ERMCH1115_SET_PUMP_REG, ERMCH115_SET_PUMP_SET);

	send_command(ERMCH1115_DC_MODE_SET, 0);
	send_command(ERMCH1115_DC_ONOFF_SET, 0);

	send_command(ERMCH1115_DISPLAY_ON, 0);
	_sleep= false;

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);

	bcm2835_delay(ERMCH1115_INITDELAY);
}

/*!
	@brief Sends a command to the display
	@param command Command to send
	@param value the values to change
	@note command and value  will be combined with OR
*/
void ERMCH1115::send_command (uint8_t command,uint8_t value)
{
	ERMCH1115_CD_SetLow;
	send_data(command | value);
	ERMCH1115_CD_SetHigh;
}

/*!
	@brief Resets OLED in a four wire setup called at start
*/
void ERMCH1115::OLEDReset ()
{
	ERMCH1115_RST_SetHigh;
	bcm2835_delay(ERMCH1115_RST_DELAY1);
	ERMCH1115_RST_SetLow;
	bcm2835_delay(ERMCH1115_RST_DELAY1);
	ERMCH1115_RST_SetHigh ;
	bcm2835_delay(ERMCH1115_RST_DELAY2);
}

/*!
	@brief Turns On Display
	@param bits  1  display on , 0 display off
*/
void ERMCH1115::OLEDEnable (uint8_t bits)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	if (bits)
	{
		_sleep= false;
		send_command(ERMCH1115_DISPLAY_ON, 0);
	 }else
	{
		 _sleep= true;
		 send_command(ERMCH1115_DISPLAY_OFF, 0);
	}

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief getting for _sleep member . is OLED in sleep mode
	@returns value of _sleep if true OLED is off and in sleep mode, 500uA.
*/
bool ERMCH1115::OLEDIsSleeping() { return  _sleep ;}

/*!
	@brief Sets up Horizontal Scroll
	@param Timeinterval 0x00 -> 0x07 , 0x00 = 6 frames
	@param Direction 0x26 right 0x27 left (A2 – A0)
	@param mode Set Scroll Mode: (28H – 2BH)  0x28 = continuous
*/
void ERMCH1115::OLEDscrollSetup(uint8_t Timeinterval, uint8_t Direction, uint8_t mode)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SETUP, 0);
	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_SCOL, 0);
	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_ECOL, 0);
	send_command(Direction , 0);
	send_command(ERMCH1115_SPAGE_ADR_SET , 0);
	send_command(Timeinterval , 0);
	send_command(ERMCH1115_EPAGE_ADR_SET , 0);
	send_command(mode, 0);

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief Turns on Horizontal scroll
	@param bits 1  Horizontal scroll on , 0 Horizontal scroll off
	@note OLEDscrollSetup must be called before it
*/
void ERMCH1115::OLEDscroll(uint8_t bits)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	bits ? send_command(ERMCH1115_ACTIVATE_SCROLL , 0) :   send_command(ERMCH1115_DEACTIVATE_SCROLL, 0);

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void ERMCH1115::OLEDContrast(uint8_t contrast)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	send_command(ERMCH115_CONTRAST_CONTROL  ,0);
	send_command(contrast, 0);
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief Rotates the display vertically, A 180 degree 'flip'
	@param bits 1  on , 0 off
*/
void ERMCH1115::OLEDFlip(uint8_t  bits)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	bits ? send_command(ERMCH1115_COMMON_SCAN_DIR, 0x08):send_command(ERMCH1115_COMMON_SCAN_DIR, 0x00)  ; // C0H - C8H
	bits ? send_command(ERMCH1115_SEG_SET_REMAP, 0x01):   send_command(ERMCH1115_SEG_SET_REMAP, 0x00); //(A0H - A1H)
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief Turns on fade effect
	@param bits 0x00 to stop
	@note bits values: (see datasheet breatheffect P25 for more details)
		-# ON/OFF * * A4 A3 A2 A1 A0
		-# When ON/OFF =”H”, Breathing Light ON.
		-# Breathing Display Effect Maximum Brightness Adjust Set: (A4 – A3)
		-# Breathing Display Effect Time Interval Set: (A2 – A0)
		-# Default on is 0x81
*/
void ERMCH1115::OLEDfadeEffect(uint8_t bits)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	send_command(ERMCCH1115_BREATHEFFECT_SET,0);
	send_command(bits,0);

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief invert the display
	@param bits 1 invert , 0 normal
*/
void ERMCH1115::OLEDInvert(uint8_t bits)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	bits ? send_command(ERMCH1115_DISPLAY_INVERT, 0) :   send_command(ERMCH1115_DISPLAY_NORMAL, 0);

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
*/
void ERMCH1115::OLEDFillScreen(uint8_t dataPattern)
{
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		OLEDFillPage(row,dataPattern);
	}
}

/*!
	 @brief Fill the chosen page(0-7)  with a datapattern
	 @param pageNum  page 0-7 divides 64 pixel screen into 8 pages or blocks  64/8  
	 @param dataPattern can be set to 0 to FF (not buffer)
*/
void ERMCH1115::OLEDFillPage(uint8_t pageNum, uint8_t dataPattern)
{

	if (pageNum >= 8) 
	{
		printf("Error OLEDFillPage 1 :page number must be between 0 and 7 \n");
		return;
	}
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	// Commands
	send_command(ERMCH1115_SET_COLADD_LSB, 0);
	send_command(ERMCH1115_SET_COLADD_MSB, 0);
	send_command(ERMCH1115_SET_PAGEADD, pageNum);
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);

	bcm2835_delayMicroseconds(5);

	// Data
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	for (uint8_t i = 0; i < _OLED_WIDTH; i++)
	{
		send_data(dataPattern);
	}
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	 @brief Draw a bitmap  to the screen
	 @param x offset 0-128
	 @param y offset 0-64
	 @param w width 0-128
	 @param h height 0-64
	 @param data  pointer to the bitmap
	 @note data is const. writes direct to screen , no buffer.
*/
void ERMCH1115::OLEDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data)
{
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >>3;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _OLED_HEIGHT) {continue;}
		send_command(ERMCH1115_SET_COLADD_LSB, (column & 0x0F));
		send_command(ERMCH1115_SET_COLADD_MSB, (column & 0xF0) >> 4);
		send_command(ERMCH1115_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
				if (x + tx < 0 || x + tx >= _OLED_WIDTH) {continue;}
				offset = (w * (ty >> 3)) + tx;
				send_data(data[offset]);
		}
	}
	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	 @brief Send data byte with SPI to ERMCH1115
	 @param byte the data byte to send 
*/
void ERMCH1115::send_data(uint8_t byte)
{
	switch (GetCommMode())
	{
		case 2: bcm2835_spi_transfer(byte); break;
		case 3: SoftwareSPIShiftOut(byte); break;
	}
}


/*!
	 @brief updates the OLED i.e. writes  buffer to the screen
*/
void ERMCH1115::OLEDupdate()
{
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t w = this->_OLED_WIDTH;
	uint8_t h = this->_OLED_HEIGHT;
	OLEDBufferScreen( x,  y,  w,  h,this->OLEDbuffer);
}

/*!
	 @brief clears the active shared buffer i.e. does NOT write to the screen
*/
void ERMCH1115::OLEDclearBuffer()
{
	memset( this->OLEDbuffer, 0x00, (this->_OLED_WIDTH * (this->_OLED_HEIGHT /8))  );
}

/*!
	 @brief Draw a bitmap to the screen
	 @param x offset 0-128
	 @param y offset 0-64
	 @param w width 0-128
	 @param h height 0-64
	 @param data pointer to the bitmap data array
	 @note Called by OLEDupdate, used internally mostly 
*/
void ERMCH1115::OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data)
{

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, LOW);

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y/8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
	if (y + ty < 0 || y + ty >= _OLED_HEIGHT) {continue;}

	send_command(ERMCH1115_SET_COLADD_LSB, (column & 0x0F));
	send_command(ERMCH1115_SET_COLADD_MSB, (column & 0XF0) >> 4);
	send_command(ERMCH1115_SET_PAGEADD, page++);

	for (tx = 0; tx < w; tx++)
	{
			if (x + tx < 0 || x + tx >= _OLED_WIDTH) {continue;}
			offset = (w * (ty /8)) + tx;
			send_data(data[offset++]);
	}
	}

	if (GetCommMode() == 3)
		bcm2835_gpio_write(_OLED_CS, HIGH);
}

/*!
	@brief Draws a Pixel to the screen , overides the  graphics library 
	@param x x co-ord of pixel
	@param y y co-ord of pixel
	@param colour colour of  pixel
	@note virtual function overides the  graphics library 
*/
void ERMCH1115::drawPixel(int16_t x, int16_t y, uint8_t colour)
{

	if ((x < 0) || (x >= this->_width) || (y < 0) || (y >= this->_height)){return;}

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
	switch (colour)
	{
		case RDL_WHITE:  this->OLEDbuffer[tc] &= ~(1 << (y & 7)); break;
		case RDL_BLACK:  this->OLEDbuffer[tc] |= (1 << (y & 7)); break;
		case RDL_INVERSE: this->OLEDbuffer[tc] ^= (1 << (y & 7)); break;
	}

}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ERMCH1115::OLEDHighFreqDelayGet(void){return _OLEDHighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ERMCH1115::OLEDHighFreqDelaySet(uint16_t CommDelay){_OLEDHighFreqDelay = CommDelay;}

// EOF
