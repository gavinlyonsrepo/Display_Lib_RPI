/*
* Project Name: ERMCH1115
* File: ERMCH1115.h
* Description: ER_OLEDM1 OLED driven by CH1115 controller source file
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
*/


#include "ER_OLEDM1_CH1115.h"
#include <stdbool.h>

// Class Constructors 2 off:
// HWSPI 
// SWSPI 

// Hardware SPI , mode 2
ERMCH1115  :: ERMCH1115(int16_t oledwidth, int16_t oledheight ,int8_t rst, int8_t cd) :ERMCH1115_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	
	_OLED_CD = cd;
	_OLED_RST= rst;
	
	_OLED_mode = 2;
}

// software SPI , mode 3
ERMCH1115  :: ERMCH1115(int16_t oledwidth, int16_t oledheight , int8_t rst, int8_t cd, int8_t cs, int8_t sclk, int8_t din) :ERMCH1115_graphics(oledwidth, oledheight)
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



// Desc: begin Method initialise OLED
// Sets pinmodes and SPI setup
// Param1: OLEDcontrast default = 0x80 , range 0x00 to 0xFE
void ERMCH1115::OLEDbegin (uint8_t OLEDcontrast)
{
	_OLEDcontrast  = OLEDcontrast ;
	bcm2835_gpio_fsel(_OLED_RST, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(_OLED_CD, BCM2835_GPIO_FSEL_OUTP);
	if(GetCommMode() == 3)
	{
		bcm2835_gpio_fsel( _OLED_CS, BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(_OLED_SCLK, BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(_OLED_DIN, BCM2835_GPIO_FSEL_OUTP);
	}
		
	OLEDinit();
}


// Desc: Start SPI operations. Forces RPi SPI0 pins P1-19 (MOSI), P1-21 (MISO), 
// P1-23 (CLK), P1-24 (CE0) and P1-26 (CE1) 
// to alternate function ALT0, which enables those pins for SPI interface.
void ERMCH1115::OLEDSPIon(void)
{
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

// Desc: stop  Spi
// End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), 
// P1-24 (CE0) and P1-26 (CE1) 
// are returned to their default INPUT behaviour.
void ERMCH1115::OLEDSPIoff(void)
{
   bcm2835_spi_end();
}

// Call when powering down
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


// Desc: Checks if software SPI is on
// Returns: true 2 if hardware SPi on ,  3 for software spi
int8_t  ERMCH1115::GetCommMode() 
{
  return (_OLED_mode);
}

// Desc: used in software SPI mode to shift out data
// Param1: bit order LSB or MSB set to MSBFIRST for ERMCH1115
// Param2: the byte to go
void ERMCH1115::CustomshiftOut(uint8_t value)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  
	{
   		!!(value & (1 << (7 - i))) ? ERMCH1115_SDA_SetHigh : ERMCH1115_SDA_SetLow ;
			
		ERMCH1115_SCLK_SetHigh; 
		bcm2835_delayMicroseconds(ERMCH1115_HIGHFREQ_DELAY); 
		ERMCH1115_SCLK_SetLow;  
		bcm2835_delayMicroseconds(ERMCH1115_HIGHFREQ_DELAY);
	}
}

// Desc: Called from OLEDbegin carries out Power on sequence and register init
// Can be used to reset OLED to default values.
void ERMCH1115::OLEDinit()
 {

	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
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

	switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
	bcm2835_delay(ERMCH1115_INITDELAY);
}

// Desc: Sends a command to the display
// Param1: the command
// Param2: the values to change
void ERMCH1115::send_command (uint8_t command,uint8_t value)
{
	ERMCH1115_CD_SetLow;
	send_data(command | value);
	ERMCH1115_CD_SetHigh;
}

// Desc: Resets OLED in a four wire setup called at start
void ERMCH1115::OLEDReset ()
{
	ERMCH1115_RST_SetHigh;
	bcm2835_delay(ERMCH1115_RST_DELAY1);
	ERMCH1115_RST_SetLow;
	bcm2835_delay(ERMCH1115_RST_DELAY1);
	ERMCH1115_RST_SetHigh ;
	bcm2835_delay(ERMCH1115_RST_DELAY2);
}

// Desc: Turns On Display
// Param1: bits,  1  on , 0 off
void ERMCH1115::OLEDEnable (uint8_t bits)
{
 	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	
	if (bits)
	{
		_sleep= false;
		send_command(ERMCH1115_DISPLAY_ON, 0);
	 }else
	{
		 _sleep= true;
		 send_command(ERMCH1115_DISPLAY_OFF, 0);
	}
	
 	switch (GetCommMode())
	{
	case 2: OLEDSPIoff(); break;
	case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: OLEDIsOff
// Returns: bool  value of _sleep if true OLED is off and in sleep mode, 500uA.
bool ERMCH1115::OLEDIssleeping() { return  _sleep ;}

// Desc: Sets up Horionztal Scroll
// Param1: TimeInterval 0x00 -> 0x07 , 0x00 = 6 frames
// Param2: Direction 0x26 right 0x27 left (A2 – A0)
// Param3: Mode. Set Scroll Mode: (28H – 2BH)  0x28 = continuous
void ERMCH1115::OLEDscrollSetup(uint8_t Timeinterval, uint8_t Direction, uint8_t mode)
{
	 switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	 send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SETUP, 0);
	 send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_SCOL, 0);
	 send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_ECOL, 0);
	 send_command(Direction , 0);
	 send_command(ERMCH1115_SPAGE_ADR_SET , 0);
	 send_command(Timeinterval , 0);
	 send_command(ERMCH1115_EPAGE_ADR_SET , 0);
	 send_command(mode, 0);
	 switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: Turns on Horizontal scroll
// Param1: bits 1  on , 0 off
// Note OLEDscrollSetup must be called before it
void ERMCH1115::OLEDscroll(uint8_t bits)
{
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	
	bits ? send_command(ERMCH1115_ACTIVATE_SCROLL , 0) :   send_command(ERMCH1115_DEACTIVATE_SCROLL, 0);
	
	switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: Adjusts contrast
// Param1: Contrast 0x00 to 0xFF , default 0x80
// Note: Setup during init.
void ERMCH1115::OLEDContrast(uint8_t contrast)
{
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	send_command(ERMCH115_CONTRAST_CONTROL  ,0);
	send_command(contrast, 0);
	switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: Rotates the display vertically
// Param1: bits 1  on , 0 off
void ERMCH1115::OLEDFlip(uint8_t  bits)
{
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	bits ? send_command(ERMCH1115_COMMON_SCAN_DIR, 0x08):send_command(ERMCH1115_COMMON_SCAN_DIR, 0x00)  ; // C0H - C8H
	bits ? send_command(ERMCH1115_SEG_SET_REMAP, 0x01):   send_command(ERMCH1115_SEG_SET_REMAP, 0x00); //(A0H - A1H)
	switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: Turns on fade effect
// Param1: bits
// bits = 0x00 to stop
// bits values: (see datasheet breatheffect P25 for more details)
//		ON/OFF * * A4 A3 A2 A1 A0
//		When ON/OFF =”H”, Breathing Light ON.
//	 	Breathing Display Effect Maximum Brightness Adjust Set: (A4 – A3)
//		Breathing Display Effect Time Interval Set: (A2 – A0)
//		Default on is 0x81
void ERMCH1115::OLEDfadeEffect(uint8_t bits)
{
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	send_command(ERMCCH1115_BREATHEFFECT_SET,0);
	send_command(bits,0);
	switch (GetCommMode())
	{	
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: invert the display
// Param1: bits, 1 invert , 0 normal
void ERMCH1115::OLEDInvert(uint8_t bits)
{
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	bits ? send_command(ERMCH1115_DISPLAY_INVERT, 0) :   send_command(ERMCH1115_DISPLAY_NORMAL, 0);
	switch (GetCommMode())
	{	
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: Fill the screen NOT the buffer with a datapattern
// Param1: datapattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff

void ERMCH1115::OLEDFillScreen(uint8_t dataPattern)
{
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++) 
	{
		OLEDFillPage(row,dataPattern);
	}
}

// Desc: Fill the chosen page(1-8)  with a datapattern
// Param1: datapattern can be set to 0 to FF (not buffer)
void ERMCH1115::OLEDFillPage(uint8_t page_num, uint8_t dataPattern)
{
	// Commands
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	send_command(ERMCH1115_SET_COLADD_LSB, 0);
	send_command(ERMCH1115_SET_COLADD_MSB, 0);
	send_command(ERMCH1115_SET_PAGEADD, page_num);
	switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS,HIGH); break;
	}
	bcm2835_delayMicroseconds(5);
	
	// Data
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
	 
	for (uint8_t i = 0; i < _OLED_WIDTH; i++)
	{
		send_data(dataPattern);
	}
	switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

//Desc: Draw a bitmap to the screen
//Param1: x offset 
//Param2: y offset 
//Param3: width 
//Param4 height 
void ERMCH1115::OLEDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data)
{
	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
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
	switch (GetCommMode())
	{		
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

//Desc: Send data byte with SPI to ERMCH1115
//Param1: the data byte
void ERMCH1115::send_data(uint8_t byte)
{
	if(GetCommMode() == 2 )
		bcm2835_spi_transfer(byte);
	else if(GetCommMode() == 3)
		CustomshiftOut(byte); 
}


//Desc: updates the buffer i.e. writes it to the screen
void ERMCH1115::OLEDupdate()
{
	OLEDBuffer( this->ActiveBuffer->xoffset, this->ActiveBuffer->yoffset, this->ActiveBuffer->width, this->ActiveBuffer->height, (uint8_t*) this->ActiveBuffer->screenbitmap);
		return;
}

//Desc: clears the buffer i.e. does NOT write to the screen
void ERMCH1115::OLEDclearBuffer()
{
	 memset( this->ActiveBuffer->screenbitmap, 0x00, (this->ActiveBuffer->width * (this->ActiveBuffer->height/ 8))  );
	 return;
}

//Desc: Draw a bitmap to the screen
//Param1: x offset 
//Param2: y offset 
//Param3: width 
//Param4 height 
//Param5 the bitmap data
//Note: Called by OLEDupdate
void ERMCH1115::OLEDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data)
{

	switch (GetCommMode())
	{	
		case 2: OLEDSPIon(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, LOW); break;
	}
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

	switch (GetCommMode())
	{
		case 2: OLEDSPIoff(); break;
		case 3: bcm2835_gpio_write(_OLED_CS, HIGH); break;
	}
}

// Desc: Draws a Pixel to the buffer overides the  graphics library
// Passed x and y co-ords and colour of pixel.
void ERMCH1115::drawPixel(int16_t x, int16_t y, uint8_t colour)
{

	if ((x < 0) || (x >= this->ActiveBuffer->width) || (y < 0) || (y >= this->ActiveBuffer->height)) {
	return;
	}
		uint16_t offset = (this->ActiveBuffer->width * (y/8)) + x;
		switch (colour)
		{
		case FOREGROUND: this->ActiveBuffer->screenbitmap[offset] |= (1 << (y & 7)); break;
		case BACKGROUND: this->ActiveBuffer->screenbitmap[offset] &= ~(1 << (y & 7)); break;
		case INVERSE: this->ActiveBuffer->screenbitmap[offset] ^= (1 << (y & 7)); break;
		}
	return;
}

// Func Desc: init the Multibuffer struct
// Param 1 Pointer to a struct
// Param 2 Pointer to buffer array data(arrays decay to  pointers)
// Param 3. width of buffer
// Param 4. height of buffer
// Param 5. x offset of buffer
// Param 6. y offset of buffer
void ERMCH1115::OLEDinitBufferStruct(MultiBuffer* mystruct, uint8_t* mybuffer, uint8_t w,  uint8_t h, int16_t  x, int16_t y)
{
   mystruct->screenbitmap = mybuffer; // point it to the buffer
   mystruct->width = w ;
   mystruct->height = h;
   mystruct->xoffset = x;
   mystruct->yoffset = y; 
}

//***********************************************
