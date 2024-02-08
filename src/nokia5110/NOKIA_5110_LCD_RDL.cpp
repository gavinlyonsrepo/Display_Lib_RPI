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
	_LCD_RST = LCD_RST;
	_LCD_CE = LCD_CE;
	_LCD_DC  = LCD_DC;
	_LCD_DIN  = LCD_DIN;
	_LCD_CLK  = LCD_CLK;
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
	@param spi_divider SPI clock divider , bcm2835SPIClockDivider
	@param SPICE_Pin which SPI_CE pin to use , 0 or 1 ,
	@note Hardware SPI default , RST pin 1, DC pin 2 , overloaded
*/
NOKIA_5110_RPI::NOKIA_5110_RPI(int16_t lcdwidth, int16_t lcdheight, uint8_t LCD_RST, uint8_t LCD_DC, uint32_t spi_divider, uint8_t SPICE_Pin) : bicolor_graphics(lcdwidth, lcdheight)

{
	_LCD_RST = LCD_RST;
	_LCD_DC  = LCD_DC;
	_LCD_SPICLK_DIVIDER  = spi_divider; // HW SPI only
	_LCD_SPICE_PIN = SPICE_Pin; // HW SPI only
	_LCD_WIDTH = lcdwidth;
	_LCD_HEIGHT = lcdheight;
	_LCDHardwareSPI = true;
}


/*!
	@brief  This sends the commands to the PCD8544 to  init LCD
	@param Inverse false normal mode true display inverted
	@param Contrast Set LCD VOP contrast range 0xB1-BF
	@param Bias LCD Bias mode 1:48 0x12 to 0x14
	@return error  if spi hw begin fails (most likely user not running as root)
*/
rpiDisplay_Return_Codes_e NOKIA_5110_RPI::LCDBegin(bool Inverse, uint8_t Contrast,uint8_t Bias)
{
	_inverse = Inverse;
	_bias = Bias;
	_contrast = Contrast;

	LCD_RST_SetDigitalOutput;
	LCD_DC_SetDigitalOutput;
	bcm2835_delay(100);
	LCD_RST_SetHigh;
	LCD_RST_SetLow;
	bcm2835_delay(50);
	LCD_RST_SetHigh;
if (isHardwareSPI() == false)
{
	LCD_CE_SetDigitalOutput;
	LCD_CE_SetHigh;
	LCD_CLK_SetDigitalOutput;
	LCD_DIN_SetDigitalOutput;
}else{
	 if(!bcm2835_spi_begin())
		return rpiDisplay_SPIbeginFail;
	else
		LCDSPIHWSettings();
}

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
	return rpiDisplay_Success;
}

/*!
	@brief Init Hardware SPI
	@note can be called during program to refresh NOKIA 5110 SPI settings
	if another device is on bus using different settings.
	@details Sets the  Bit Order, SPI mode , SPI bus speed , Chip enable pin.
*/
void NOKIA_5110_RPI::LCDSPIHWSettings()
{
	// 1. Bit Order
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	// 2. SPI mode
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

	// 3.  SPI bus speed
	if (_LCD_SPICLK_DIVIDER > 0)
		bcm2835_spi_setClockDivider(_LCD_SPICLK_DIVIDER);
	else // default, BCM2835_SPI_CLOCK_DIVIDER_64 3.90MHz Rpi2, 6.250MHz RPI3
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);

	// 4. Chip enable pin select
	if (_LCD_SPICE_PIN == 0)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	}else if (_LCD_SPICE_PIN  == 1)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
	}
}

/*!
	@brief End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK),
	P1-24 (CE0) and P1-26 (CE1) are returned to their default INPUT behavior.
	@note Hardware SPI only
*/
void NOKIA_5110_RPI::LCDSPIoff(void)
{
	if(isHardwareSPI() == true){bcm2835_spi_end();}
}


/*!
	@brief Call when powering down LCD
*/
void NOKIA_5110_RPI::LCDPowerDown(void)
{
	bcm2835_gpio_write(_LCD_DC, LOW);
	bcm2835_gpio_write(_LCD_RST, LOW);

	if(isHardwareSPI() == false)
	{
		bcm2835_gpio_write(_LCD_CLK, LOW);
		bcm2835_gpio_write(_LCD_DIN, LOW);
		bcm2835_gpio_write(_LCD_CE, LOW);
	}
}

/*!
	@brief  Writes a byte to the PCD8544
	@param data byte will be sent as command or data depending on status of DC line
*/
void NOKIA_5110_RPI::LCDWriteData(uint8_t data)
{
	if (isHardwareSPI() == false)
	{
		uint8_t bit_n;
		for (bit_n = 0x80; bit_n; bit_n >>= 1)
		{
			LCD_CLK_SetLow;
			bcm2835_delayMicroseconds(_LCDHighFreqDelay);
			if (data & bit_n)
				LCD_DIN_SetHigh;
			else
				LCD_DIN_SetLow;
			LCD_CLK_SetHigh;
			bcm2835_delayMicroseconds(_LCDHighFreqDelay);
		}
	}else{
			bcm2835_spi_transfer(data);
		}
}

/*!
	@brief  Writes a command byte to the PCD8544
	@param command The command byte to send
*/
void NOKIA_5110_RPI::LCDWriteCommand(uint8_t command) {
	LCD_DC_SetLow;
	if (isHardwareSPI() == false)LCD_CE_SetLow;
	LCDWriteData(command);
	if (isHardwareSPI() == false)LCD_CE_SetHigh;
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

	LCD_DC_SetHigh; //Data send
	if (isHardwareSPI() == false)LCD_CE_SetLow;

	for(i = 0; i < _LCD_Display_size; i++)  
	LCDWriteData( LCDDisplayBuffer[i] );

	if (isHardwareSPI() == false)LCD_CE_SetHigh;
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
	@brief Checks if software SPI is on
	@return true 1 if hardware SPi on , false 0 for software spi
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
