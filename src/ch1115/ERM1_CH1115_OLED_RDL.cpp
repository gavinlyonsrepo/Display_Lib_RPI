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
	@param dc GPIO data or command
	@note Hardware SPI version,  mode 2
 */
ERMCH1115::ERMCH1115(int16_t oledwidth, int16_t oledheight ,int8_t rst, int8_t dc) :bicolor_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_Display_DC = dc;
	_Display_RST = rst;
	_OLED_mode = 2;
}

/*!
	@brief init the OLED class object
	@param oledwidth width of oled in pixels
	@param oledheight height of oled in pixels
	@param rst GPIO reset
	@param dc GPIO data or command
	@param cs GPIO Chip select
	@param sclk GPIO SPI Clock
	@param din GPIO MOSI
	@note software SPI version, mode 3
 */
ERMCH1115::ERMCH1115(int16_t oledwidth, int16_t oledheight , int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din) :bicolor_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_Display_DC = dc;
	_Display_RST= rst;
	_Display_CS = cs;
	_Display_SDATA = din;
	_Display_SCLK = sclk;
	_OLED_mode = 3;
}

// Class Functions

/*!
	@brief begin Method initialise OLED for Hardware SPI
	@param OLEDcontrast Contrast of the OLED display default = 0x80 , range 0x00 to 0xFE
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
rpiDisplay_Return_Codes_e ERMCH1115::OLEDbegin (uint8_t OLEDcontrast, int device, int channel, int speed, int flags, int gpioDev)
{
	_OLEDcontrast  = OLEDcontrast;
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;

	// 1. check communication mode being called, if user called wrong one.
	if(GetCommMode() == 3)
	{
		printf("Wrong SPI mode chosen this method is for Hardware SPI : %i\n", _OLED_mode);
		return rpiDisplay_WrongModeChosen;
	}
	// 2. setup gpioDev
	_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Error : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rpiDisplay_GpioChipDevice;
	}

	// 3. Claim 2 GPIO as outputs for RST and DC lines
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
	//Display_CS_SetHigh;
	// 4. set up spi open
	 _spiHandle  = Display_OPEN_SPI;
	if ( _spiHandle  < 0)
	{
		fprintf(stderr, "Error : Cannot open SPI :(%s)\n", lguErrorText( _spiHandle ));
		return rpiDisplay_SPIOpenFailure;
	}

	OLEDinit();
	return rpiDisplay_Success;
}

/*!
	@brief begin Method initialise OLED for software SPI
	@param OLEDcontrast Contrast of the OLED display default = 0x80 , range 0x00 to 0xFE
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_WrongModeChosen
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinClaim
 */
rpiDisplay_Return_Codes_e ERMCH1115::OLEDbegin (uint8_t OLEDcontrast, int gpioDev)
{
	_OLEDcontrast  = OLEDcontrast;
	_DeviceNumGpioChip = gpioDev;
	bool ErrorFlag = true;

	// 1. check communication mode being called, if user called wrong one.
	if(GetCommMode() == 2)
	{
		printf("Wrong SPI mode chosen this method is for Software SPI : %i\n", _OLED_mode);
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
		fprintf(stderr,"Error : Can't claim DC GPIO for output (%s)\n", lguErrorText(GpioDCErrorStatus));
	} else if (GpioCSErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim CS GPIO for output (%s)\n", lguErrorText(GpioCSErrorStatus));
	} else if (GpioSCLKErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim SCLK GPIO for output (%s)\n", lguErrorText(GpioSCLKErrorStatus));
	} else if (GpioDINErrorStatus < 0 ){
		fprintf(stderr,"Error : Can't claim DIN GPIO for output (%s)\n", lguErrorText(GpioDINErrorStatus));
	} else { ErrorFlag = false;}

	if (ErrorFlag == true ) {return rpiDisplay_GpioPinClaim;}

	Display_CS_SetHigh;
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
	@brief stops HW spi operations
	@details End SPI operations. 
	@return
		-#  rpiDisplay_Success
		-#  rpiDisplay_GpioPinFree
		-#  rpiDisplay_SPICloseFailure
		-#  rpiDisplay_GpioChipDevice
*/
rpiDisplay_Return_Codes_e  ERMCH1115::OLEDSPIoff(void)
{
	uint8_t ErrorFlag = 0; // Becomes > 0 in event of error
	
	// 1. Free Reset & DC GPIO lines
	int GpioResetErrorStatus = 0;
	int GpioDCErrorStatus = 0;
	Display_RST_SetLow;
	Display_DC_SetLow;
	GpioResetErrorStatus = Display_GPIO_FREE_RST;
	GpioDCErrorStatus  =  Display_GPIO_FREE_DC;

	if (GpioResetErrorStatus < 0 ){
		fprintf(stderr,"Error: Can't Free Reset GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
		ErrorFlag = 2;
	}else if (GpioDCErrorStatus  < 0 ){
		fprintf(stderr,"Error: Can't Free DC GPIO (%s)\n", lguErrorText(GpioDCErrorStatus));
		ErrorFlag = 2;
	}
	
	// 2A Software SPI other 3 GPIO LINES, CLK DATA & CS.
	if (GetCommMode() == 3) // software spi
	{
		int GpioCSErrorStatus = 0;
		int GpioCLKErrorStatus = 0;
		int GpioDINErrorStatus = 0;
		Display_CS_SetLow;
		Display_SCLK_SetLow;
		Display_SDATA_SetLow;
		GpioCSErrorStatus = Display_GPIO_FREE_CS;
		GpioCLKErrorStatus = Display_GPIO_FREE_CLK;
		GpioDINErrorStatus = Display_GPIO_FREE_SDATA;
		if (GpioCSErrorStatus < 0 ){
			fprintf(stderr,"Error: Can't Free CS GPIO (%s)\n", lguErrorText(GpioCSErrorStatus ));
			ErrorFlag = 2;
		}else if (GpioCLKErrorStatus< 0 ){
			fprintf(stderr,"Error: Can't Free CLK GPIO (%s)\n", lguErrorText(GpioCLKErrorStatus));
			ErrorFlag = 2;
		}else if (GpioDINErrorStatus< 0 ){
			fprintf(stderr, "Error: Can't free DATA GPIO (%s)\n", lguErrorText(GpioDINErrorStatus));
			ErrorFlag = 2;
		}
	}else if (GetCommMode() == 2) // 2B hardware SPi Closes a SPI device 
	{
		int spiErrorStatus = 0;
		spiErrorStatus = Display_CLOSE_SPI;
		if (spiErrorStatus <0) 
		{
			fprintf(stderr, "Error: Cannot Close SPI device (%s)\n", lguErrorText(spiErrorStatus));
			ErrorFlag = 3;
		}
	}
	// 3 Closes the opened gpiochip device.
	int GpioCloseStatus = 0;
	GpioCloseStatus = Display_CLOSE_GPIO_HANDLE;
	if ( GpioCloseStatus < 0)
	{
		fprintf(stderr,"Error :: Failed to close lgGpioChipclose %d , error=  (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
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
	@brief Power down function
	@details Disables screen and sets all independent GPIO low.
		call when powering down before end of operations
*/
void ERMCH1115::OLEDPowerDown(void)
{
	OLEDEnable(0);
	Display_DC_SetLow;
	Display_RST_SetLow;
	if(GetCommMode()== 3)
	{
		Display_SCLK_SetLow;
		Display_SDATA_SetLow;
		Display_CS_SetLow;
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
		!!(value & (1 << (7 - i))) ? Display_SDATA_SetHigh : Display_SDATA_SetLow ;

		Display_SCLK_SetHigh;
		delayMicroSecRDL(_OLEDHighFreqDelay);
		Display_SCLK_SetLow;
		delayMicroSecRDL(_OLEDHighFreqDelay);
	}
}

/*!
	@brief Called from OLEDbegin carries out Power on sequence and register init
*/
void ERMCH1115::OLEDinit()
 {
	switch (GetCommMode())
	{
		case 3: Display_CS_SetLow; break;
	}
	OLEDReset();

	send_command(ERMCH1115_DISPLAY_OFF, 0);

	send_command(ERMCH1115_SET_COLADD_LSB, 0);
	send_command(ERMCH1115_SET_COLADD_MSB, 0);
	send_command(ERMCH1115_SET_PAGEADD, 0);
	send_command(ERMCH1115_SET_DISPLAY_START_LINE, 0);

	send_command(ERMCH1115_CONTRAST_CONTROL  ,0);
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

	send_command(ERMCH1115_SET_PUMP_REG, ERMCH1115_SET_PUMP_SET);

	send_command(ERMCH1115_DC_MODE_SET, 0);
	send_command(ERMCH1115_DC_ONOFF_SET, 0);

	send_command(ERMCH1115_DISPLAY_ON, 0);
	_sleep= false;

	if (GetCommMode() == 3)
		Display_CS_SetHigh ;

	delayMilliSecRDL(ERMCH1115_INITDELAY);
}

/*!
	@brief Sends a command to the display
	@param command Command to send
	@param value the values to change
	@note command and value  will be combined with OR
*/
void ERMCH1115::send_command (uint8_t command,uint8_t value)
{
	Display_DC_SetLow;
	send_data(command | value);
	Display_DC_SetHigh;
}

/*!
	@brief Resets OLED in a four wire setup called at start
*/
void ERMCH1115::OLEDReset ()
{
	Display_RST_SetHigh;
	delayMilliSecRDL(ERMCH1115_RST_DELAY1);
	Display_RST_SetLow;
	delayMilliSecRDL(ERMCH1115_RST_DELAY1);
	Display_RST_SetHigh ;
	delayMilliSecRDL(ERMCH1115_RST_DELAY2);
}

/*!
	@brief Turns On Display
	@param bits  1  display on , 0 display off
*/
void ERMCH1115::OLEDEnable (uint8_t bits)
{
	if (GetCommMode() == 3)
		Display_CS_SetLow;

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
		Display_CS_SetHigh ;
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
		Display_CS_SetLow;

	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SETUP, 0);
	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_SCOL, 0);
	send_command(ERMCH1115_HORIZONTAL_A_SCROLL_SET_ECOL, 0);
	send_command(Direction , 0);
	send_command(ERMCH1115_SPAGE_ADR_SET , 0);
	send_command(Timeinterval , 0);
	send_command(ERMCH1115_EPAGE_ADR_SET , 0);
	send_command(mode, 0);

	if (GetCommMode() == 3)
		Display_CS_SetHigh ;
}

/*!
	@brief Turns on Horizontal scroll
	@param bits 1  Horizontal scroll on , 0 Horizontal scroll off
	@note OLEDscrollSetup must be called before it
*/
void ERMCH1115::OLEDscroll(uint8_t bits)
{
	if (GetCommMode() == 3)
		Display_CS_SetLow;

	bits ? send_command(ERMCH1115_ACTIVATE_SCROLL , 0) :   send_command(ERMCH1115_DEACTIVATE_SCROLL, 0);

	if (GetCommMode() == 3)
		Display_CS_SetHigh ;
}

/*!
	@brief Adjusts contrast
	@param contrast 0x00 to 0xFF , default 0x80
*/
void ERMCH1115::OLEDContrast(uint8_t contrast)
{
	if (GetCommMode() == 3)
		Display_CS_SetLow;

	send_command(ERMCH1115_CONTRAST_CONTROL  ,0);
	send_command(contrast, 0);
	if (GetCommMode() == 3)
		Display_CS_SetHigh ;
}

/*!
	@brief Rotates the display vertically, A 180 degree 'flip'
	@param bits 1  on , 0 off
*/
void ERMCH1115::OLEDFlip(uint8_t  bits)
{
	if (GetCommMode() == 3)
		Display_CS_SetLow;

	bits ? send_command(ERMCH1115_COMMON_SCAN_DIR, 0x08):send_command(ERMCH1115_COMMON_SCAN_DIR, 0x00)  ; // C0H - C8H
	bits ? send_command(ERMCH1115_SEG_SET_REMAP, 0x01):   send_command(ERMCH1115_SEG_SET_REMAP, 0x00); //(A0H - A1H)
	if (GetCommMode() == 3)
		Display_CS_SetHigh ;
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
		Display_CS_SetLow;

	send_command(ERMCH1115_BREATHEFFECT_SET,0);
	send_command(bits,0);

	if (GetCommMode() == 3)
		Display_CS_SetHigh ;
}

/*!
	@brief invert the display
	@param bits 1 invert , 0 normal
*/
void ERMCH1115::OLEDInvert(uint8_t bits)
{
	if (GetCommMode() == 3)
		Display_CS_SetLow;

	bits ? send_command(ERMCH1115_DISPLAY_INVERT, 0) :   send_command(ERMCH1115_DISPLAY_NORMAL, 0);

	if (GetCommMode() == 3)
		Display_CS_SetHigh ;
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
		Display_CS_SetLow;

	// Commands
	send_command(ERMCH1115_SET_COLADD_LSB, 0);
	send_command(ERMCH1115_SET_COLADD_MSB, 0);
	send_command(ERMCH1115_SET_PAGEADD, pageNum);
	if (GetCommMode() == 3)
		Display_CS_SetHigh ;

	delayMicroSecRDL(5);

	// Data
	if (GetCommMode() == 3)
		Display_CS_SetLow;

	for (uint8_t i = 0; i < _OLED_WIDTH; i++)
	{
		send_data(dataPattern);
	}
	if (GetCommMode() == 3)
		Display_CS_SetHigh ;
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
		Display_CS_SetLow;

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
		Display_CS_SetHigh ;
}

/*!
	 @brief Send data byte with SPI to ERMCH1115
	 @param dataByte byte the data byte to send
*/
void ERMCH1115::send_data(uint8_t dataByte)
{
	int spiErrorStatus = 0;
	char TransmitBuffer[1];
	TransmitBuffer[0] = dataByte;
	switch (GetCommMode())
	{
		case 2: 
			spiErrorStatus = Display_SPI_WRITE(_spiHandle, static_cast<const char*>(TransmitBuffer), sizeof(TransmitBuffer));
			if (spiErrorStatus <0) 
			{
				fprintf(stderr, "Error : Failure to Write  SPI :(%s)\n", lguErrorText(spiErrorStatus));
			}
		break;
		case 3: SoftwareSPIShiftOut(dataByte); break;
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
		Display_CS_SetLow;

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
		Display_CS_SetHigh ;
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
