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
	_Display_DC = cd;
	_Display_RST= rst;
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
	_Display_DC = cd;
	_Display_RST= rst;
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_LCD_mode = 3;
}

/*!
	@brief begin Method initialise LCD  Sets pinmodes and SPI setup HW SPI
	@param AddressSet AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
	@param VbiasPOT contrast default = 0x49 , range 0x00 to 0xFE
	@param device A SPI device, >= 0. 
	@param channel A SPI channel, >= 0. 
	@param speed The speed of serial communication in bits per second. 
	@param flags The flags may be used to modify the default behaviour. Set to 0(mode 0) for this device.
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_WrongModeChosen
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinClaim
		-# rpiDisplay_SPIOpenFailure
*/
rpiDisplay_Return_Codes_e ERM19264_UC1609::LCDbegin(uint8_t AddressSet ,uint8_t VbiasPOT, int device, int channel, int speed, int flags, int gpioDev)
{

	if (AddressSet > 7 ) AddressSet = 0x02;
	_AddressCtrl =  AddressSet;
	_VbiasPOT  = VbiasPOT;
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;

	// 1. check communication mode being called, if user called wrong one.
	if(GetCommMode() == 3)
	{
		printf("Wrong SPI mode chosen this method is for Hardware SPI : %i\n", _LCD_mode);
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

	LCDinit();
	return rpiDisplay_Success;
}

/*!
	@brief begin Method initialise LCD for software SPI
	@param AddressSet AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
	@param VbiasPOT contrast default = 0x49 , range 0x00 to 0xFE
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_WrongModeChosen
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinClaim
 */
rpiDisplay_Return_Codes_e ERM19264_UC1609::LCDbegin(uint8_t AddressSet ,uint8_t VbiasPOT, int gpioDev)
{
	if (AddressSet > 7 ) AddressSet = 0x02;
	_AddressCtrl =  AddressSet;
	_VbiasPOT  = VbiasPOT;
	_DeviceNumGpioChip = gpioDev;
	bool ErrorFlag = true;

	// 1. check communication mode being called, if user called wrong one.
	if(GetCommMode() == 2)
	{
		printf("Wrong SPI mode chosen this method is for Software SPI : %i\n", _LCD_mode);
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
	int GpioCSErrorStatus = 0;
	int GpioSCLKErrorStatus = 0;
	int GpioDINErrorStatus = 0;

	GpioResetErrorStatus= Display_RST_SetDigitalOutput;
	GpioDCErrorStatus= Display_DC_SetDigitalOutput;
	GpioCSErrorStatus= Display_CS_SetDigitalOutput;
	GpioSCLKErrorStatus= Display_SCLK_SetDigitalOutput;
	GpioDINErrorStatus= Display_SDATA_SetDigitalOutput;

	if (GpioResetErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorStatus));
	} else if (GpioDCErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim CD GPIO for output (%s)\n", lguErrorText(GpioDCErrorStatus));
	} else if (GpioCSErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim CS GPIO for output (%s)\n", lguErrorText(GpioCSErrorStatus));
	} else if (GpioSCLKErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim SCLK GPIO for output (%s)\n", lguErrorText(GpioSCLKErrorStatus));
	} else if (GpioDINErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim DIN GPIO for output (%s)\n", lguErrorText(GpioDINErrorStatus));
	} else { ErrorFlag = false;}

	if (ErrorFlag == true ) {return rpiDisplay_GpioPinClaim;}

	Display_CS_SetHigh;
	LCDinit();
	return rpiDisplay_Success;
}


/*!
	@brief End SPI operations.
	@return
		-#  rpiDisplay_Success
		-#  rpiDisplay_GpioPinFree
		-#  rpiDisplay_SPICloseFailure
		-#  rpiDisplay_GpioChipDevice
*/
rpiDisplay_Return_Codes_e  ERM19264_UC1609::LCDSPIoff(void)
{
	uint8_t ErrorFlag = 0; // Becomes >0 in event of error
	// 1. Free reset & DC GPIO lines
	int GpioResetErrorStatus = 0;
	int GpioDCErrorStatus = 0;
	Display_RST_SetLow;
	Display_DC_SetLow;
	GpioResetErrorStatus = Display_GPIO_FREE_RST;
	GpioDCErrorStatus  =  Display_GPIO_FREE_DC;

	if (GpioResetErrorStatus < 0 ){
		fprintf(stderr,"Error: Can't Free RST GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
		ErrorFlag = 2;
	}else if (GpioDCErrorStatus  < 0 ){
		fprintf(stderr,"Error: Can't Free CD GPIO (%s)\n", lguErrorText(GpioDCErrorStatus));
		ErrorFlag = 2;
	}
	
	// 2A Software SPI other 3 GPIO LINES
	if (GetCommMode() == 3) // software spi
	{
		int GpioCSErrorStatus = 0;
		int GpioCLKErrorStatus = 0;
		int GpioSDAErrorStatus = 0;
		Display_CS_SetLow;
		Display_SCLK_SetLow;
		Display_SDATA_SetLow;
		GpioCSErrorStatus = Display_GPIO_FREE_CS;
		GpioCLKErrorStatus =  Display_GPIO_FREE_CLK;
		GpioSDAErrorStatus =   Display_GPIO_FREE_SDATA;
		if (GpioCSErrorStatus < 0 ){
			fprintf(stderr,"Error: Can't Free CS GPIO (%s)\n", lguErrorText(GpioCSErrorStatus ));
			ErrorFlag = 2;
		}else if (GpioCLKErrorStatus< 0 ){
			fprintf(stderr,"Error: Can't Free CLK GPIO (%s)\n", lguErrorText(GpioCLKErrorStatus));
			ErrorFlag = 2;
		}else if (GpioSDAErrorStatus< 0 ){
			fprintf(stderr, "Error: Can't free DATA GPIO (%s)\n", lguErrorText(GpioSDAErrorStatus));
			ErrorFlag = 2;
		}
	 // 2B hardware SPi Closes a SPI device 
	}else if (GetCommMode() == 2)
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
	GpioCloseStatus = Display_CLOSE_GPIO_HANDLE; // close gpiochip
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
		default:printf("Warning:Unknown error flag value in SPI-PowerDown"); break;
	}

	return rpiDisplay_Success;
}

/*!
	@brief Powerdown procedure for LCD see datasheet P40
*/
void ERM19264_UC1609::LCDPowerDown(void)
{
	LCDReset();
	LCDEnable(0);
	Display_DC_SetLow ;
	Display_RST_SetLow ;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
	}

	Display_DC_SetHigh;

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
		case 3: Display_CS_SetHigh; break;
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
	Display_DC_SetLow;
	send_data(command | value);
	Display_DC_SetHigh;
}

/*!
	@brief Resets LCD in a four wire setup called at start
	and  should also be called in a controlled power down setting
*/
void ERM19264_UC1609::LCDReset ()
{
	Display_RST_SetLow;
	delayMilliSecRDL(UC1609_RESET_DELAY);
	Display_RST_SetHigh;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
	}

	bits ? _sleep = false: _sleep = true;
	send_command(UC1609_DISPLAY_ON, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: Display_CS_SetHigh; break;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
	}

	send_command(UC1609_SCROLL, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: Display_CS_SetHigh; break;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
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
		case 3: Display_CS_SetHigh; break;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
	}

	send_command(UC1609_INVERSE_DISPLAY, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: Display_CS_SetHigh; break;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
	}

	send_command(UC1609_ALL_PIXEL_ON, bits);

	switch (GetCommMode())
	{
		case 2:  break;
		case 3: Display_CS_SetHigh; break;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
	}
	send_command(UC1609_SET_COLADD_LSB, 0);
	send_command(UC1609_SET_COLADD_MSB, 0);
	send_command(UC1609_SET_PAGEADD, page_num);
	switch (GetCommMode())
	{
		case 2:  break;
		case 3: Display_CS_SetHigh; break;
	}

	delayMicroSecRDL(5);

	switch (GetCommMode())
	{
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
	}

	for (uint8_t i = 0; i < _LCD_WIDTH; i++)
	{
		send_data(dataPattern);
	}
	switch (GetCommMode())
	{
		case 2:  break;
		case 3: Display_CS_SetHigh; break;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
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
		case 3: Display_CS_SetHigh; break;
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
void ERM19264_UC1609::SoftwareSPIShiftOut(uint8_t value)
{
	for (uint8_t  i = 0; i < 8; i++)
	{
	 	!!(value & (1 << (7 - i))) ? Display_SDATA_SetHigh : Display_SDATA_SetLow ;

		Display_SCLK_SetHigh;
		delayMicroSecRDL(_LCD_HighFreqDelay);
		Display_SCLK_SetLow;
		delayMicroSecRDL(_LCD_HighFreqDelay);
	}
}

/*!
	 @brief Send data byte with SPI to UC1609
	 @param dataByte byte the data byte to send
*/
void ERM19264_UC1609::send_data(uint8_t dataByte)
{
	int spiErrorStatus = 0;
	char TransmitBuffer[1];
	TransmitBuffer[0] = dataByte;
	switch (GetCommMode())
	{
		case 2: 
			spiErrorStatus = Display_SPI_WRITE( _spiHandle, static_cast<const char*>(TransmitBuffer), sizeof(TransmitBuffer));
			if (spiErrorStatus <0) 
			{
				fprintf(stderr, "Error : Failure to Write  SPI :(%s)\n", lguErrorText(spiErrorStatus));
			}
		break;
		case 3: SoftwareSPIShiftOut(dataByte); break;
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
		case 2: ; break;
		case 3: Display_CS_SetLow; break;
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
		case 3: Display_CS_SetHigh; break;
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
