/*!
	@file MAX7219_7SEG_RDL.cpp
	@author Gavin Lyons
	@brief library source file to drive MAX7219 displays
*/
#include "../../include/max7219/MAX7219_7SEG_RDL.hpp"


// Public methods

/*!
	@brief Constructor for class MAX7219_SS_RPI software SPI
	@param clock CLk pin
	@param chipSelect CS pin
	@param data DIO pin
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3@
	@note overloaded this one is for Software SPI
*/
MAX7219_SS_RPI::MAX7219_SS_RPI(uint8_t clock, uint8_t chipSelect , uint8_t data, int gpioDev)
{
	_Display_SCLK = clock;
	_Display_CS  = chipSelect;
	_Display_SDATA = data;
	_HardwareSPI = false;
	_DeviceNumGpioChip = gpioDev;
}

/*!
	@brief Constructor for class MAX7219_SS_RPI hardware SPI
	@param device A SPI device, >= 0. 
	@param channel A SPI channel, >= 0. 
	@param speed The speed of serial communication in bits per second. 
	@param flags The flags may be used to modify the default behaviour. Set to 0(mode 0) for this device.
	@note overloaded this one is for Hardware SPI 
*/
MAX7219_SS_RPI::MAX7219_SS_RPI(int device, int channel, int speed, int flags)
{
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;
	_HardwareSPI = true;
}

/*!
	@brief End display operations, called at end of program
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinFree
		-# rpiDisplay_SPICloseFailure
		-# rpiDisplay_GpioChipDevice
*/
rpiDisplay_Return_Codes_e MAX7219_SS_RPI::DisplayEndOperations(void)
{
	uint8_t ErrorFlag = 0; // Becomes >0 in event of error

	if (_HardwareSPI == false)
	{
		// 1A Free GPIO 3 lines 
		int GpioStrobeErrorstatus = 0;
		int GpioClockErrorstatus = 0;
		int GpioDataErrorstatus = 0;

		Display_CS_SetLow;
		Display_SCLK_SetLow;
		Display_SDATA_SetLow;

		GpioStrobeErrorstatus = Display_GPIO_FREE_CS;
		GpioClockErrorstatus =  Display_GPIO_FREE_CLK;
		GpioDataErrorstatus =   Display_GPIO_FREE_SDATA;
	
		if (GpioStrobeErrorstatus < 0 )
		{
			fprintf(stderr,"Error: Can't Free CS GPIO (%s)\n", lguErrorText(GpioStrobeErrorstatus));
			ErrorFlag = 2;
		}
		if (GpioClockErrorstatus < 0 )
		{
			fprintf(stderr,"Error: Can't Free CLK GPIO (%s)\n", lguErrorText(GpioClockErrorstatus));
			ErrorFlag = 2;
		}
		if (GpioDataErrorstatus < 0 )
		{
			fprintf(stderr, "Error: Can't free DATA GPIO (%s)\n", lguErrorText(GpioDataErrorstatus));
			ErrorFlag = 2;
		}
		 // 2 close gpio chip device
		int GpioCloseStatus = 0;
		GpioCloseStatus = Display_CLOSE_GPIO_HANDLE;
		if ( GpioCloseStatus < 0)
		{
			fprintf(stderr,"Error: Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
			ErrorFlag = 3;
		}
	}else
	{// 1B Close SPI device 
		int spiErrorStatus = 0;
		spiErrorStatus = Display_CLOSE_SPI;
		if (spiErrorStatus <0)
		{
			fprintf(stderr, "Error: Cannot Close SPI :(%s)\n", lguErrorText(spiErrorStatus));
			ErrorFlag = 4;
		}
	}
	// 3 Check error flag ( we don't want to return early just for one failure)
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
	@brief get value of _HardwareSPI , true hardware SPI on , false off.
	@return _HardwareSPI , true hardware SPI on , false off.
*/
bool MAX7219_SS_RPI::GetHardwareSPI(void)
{return _HardwareSPI;}


/*!
	@brief Init the display
	@param numDigits scan limit set to 8 normally , advanced use only
	@param decodeMode Must users will use 0x00 here
	@return a rpiDisplay_Return_Codes_e  code
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinCliam
		-# rpiDisplay_SPIOpenFailure
		-# rpiDisplay_GpioChipDevice
	@note when cascading supplies init display one first always!
*/
rpiDisplay_Return_Codes_e MAX7219_SS_RPI::InitDisplay(ScanLimit_e numDigits, DecodeMode_e decodeMode)
{
	if (_CurrentDisplayNumber == 1)
	{
		if (_HardwareSPI == false)
		{
			int GpioStrobeErrorstatus = 0;
			int GpioClockErrorstatus = 0;
			int GpioDataErrorstatus = 0;

			_GpioHandle = Display_OPEN_GPIO_CHIP; // open /dev/gpiochipX
			if ( _GpioHandle < 0)	// open error
			{
				fprintf(stderr,"Errror : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
				return rpiDisplay_GpioChipDevice;
			}

			// Clain GPIO as outputs
			GpioStrobeErrorstatus = Display_CS_SetDigitalOutput;
			GpioClockErrorstatus =  Display_SCLK_SetDigitalOutput;
			GpioDataErrorstatus =   Display_SDATA_SetDigitalOutput ;

			if (GpioStrobeErrorstatus < 0 )
			{
				fprintf(stderr,"Error : Can't claim CS GPIO for output (%s)\n", lguErrorText(GpioStrobeErrorstatus));
				return rpiDisplay_GpioPinClaim;
			}
			if (GpioClockErrorstatus < 0 )
			{
				fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorstatus));
				return rpiDisplay_GpioPinClaim;
			}
			if (GpioDataErrorstatus < 0 )
			{
				fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
				return rpiDisplay_GpioPinClaim;
			}
			Display_CS_SetHigh;
		}else
		{
			_spiHandle = Display_OPEN_SPI;
			if (_spiHandle < 0)
			{
				fprintf(stderr, "Error : Cannot open SPI :(%s)\n", lguErrorText(_spiHandle));
				return rpiDisplay_SPIOpenFailure;
			}
			delayMilliSecRDL(50); // small init delay before commencing transmissions
		}
	}

	_NoDigits = numDigits+1;
	CurrentDecodeMode = decodeMode;

	SetScanLimit(numDigits);
	SetDecodeMode(decodeMode);
	ShutdownMode(false);
	DisplayTestMode(false);
	ClearDisplay();
	SetBrightness(IntensityDefault);
	return rpiDisplay_Success;
}

/*!
	@brief Clear the display
*/
void MAX7219_SS_RPI::ClearDisplay(void)
{

	switch(CurrentDecodeMode)
	{
	case DecodeModeNone: // Writes zero to blank display
		for(uint8_t digit = 0; digit<_NoDigits ; digit++)
		{
			WriteDisplay(digit+1, 0x00);
		}
	break;
	case DecodeModeBCDOne:  // Mode BCD on digit 0 , rest of display write Zero
		DisplayBCDChar(0, CodeBFontSpace);
		for(uint8_t digit=1; digit<_NoDigits ; digit++)
		{
			WriteDisplay(digit+1, 0x00);
		}
	break;
	case DecodeModeBCDTwo: // Mode BCD on digit 0-3 , rest of display write  Zero
		for(uint8_t digitBCD = 0; digitBCD<_NoDigits-4 ; digitBCD++)
		{
			DisplayBCDChar(digitBCD, CodeBFontSpace);
		}
		for(uint8_t digit=4; digit<_NoDigits ; digit++)
		{
			WriteDisplay(digit+1, 0x00);
		}
	break;
	case DecodeModeBCDThree: // BCD digit 7-0
		for(uint8_t digit=0; digit<_NoDigits ; digit++)
		{
			DisplayBCDChar(digit, CodeBFontSpace);
		}
	break;
	} // end of switch
}

/*!
	@brief Displays a character on display using MAX7219 Built in BCD code B font
	@param digit The digit to display character in, 7-0 ,7 = LHS 0 =RHS
	@param value  The BCD character to display
	@note sets BCD code B font (0-9, E, H, L,P, and -) Built-in font
*/
void MAX7219_SS_RPI::DisplayBCDChar(uint8_t digit, CodeBFont_e value)
{
	WriteDisplay(digit+1, value);
}

/*!
	@brief Displays a character on display
	@param digit The digit to display character in, 7-0 ,7 = LHS 0 =RHS
	@param character  The ASCII character to display
	@param decimalPoint Is the decimal point(dp) to be set or not.
*/
void MAX7219_SS_RPI::DisplayChar(uint8_t digit, uint8_t character , DecimalPoint_e decimalPoint)
{
	WriteDisplay(digit+1,ASCIIFetch(character , decimalPoint));
}

/*!
	@brief Set a seven segment LED ON
	@param digit The digit to set segment in, 7-0 ,7 = LHS 0 =RHS
	@param segment The segment of seven segment to set dpabcdefg
*/
void MAX7219_SS_RPI::SetSegment(uint8_t digit, uint8_t segment)
{
	WriteDisplay(digit+1, segment);
}

/*!
	@brief Displays a text string on display
	@param text pointer to character array containg text string
	@param TextAlignment  left or right alignment or leading zeros
	@note This method is overloaded, see also DisplayText(char *)
*/
void MAX7219_SS_RPI::DisplayText(char *text, TextAlignment_e TextAlignment){

	char character;
	char pos =0;

	// We need the length of the string - no of decimal points set
	uint8_t LengthOfStr;
	LengthOfStr=strlen(text);
	for(uint8_t index =0; text[index]; index++)
	{
		if(text[index] == '.') LengthOfStr--; // decrement string for dp's
	}
	if (LengthOfStr > (_NoDigits)) LengthOfStr = (_NoDigits);

	while ((character = (*text++)) && pos < _NoDigits)
	{
		if (*text == '.' && character != '.')
		{
			switch (TextAlignment) // Display a character with dp set
			{
				case AlignLeft  : DisplayChar((_NoDigits-1)- pos ,character, DecPointOn); break;
				case AlignRight : DisplayChar((LengthOfStr-1)- pos ,character, DecPointOn); break;
				case AlignRightZeros: return; break;
			}
			pos++;
			text++;
		}  else
		{
			switch (TextAlignment) // Display a character without dp set
			{
				case AlignLeft  : DisplayChar((_NoDigits-1) -pos, character, DecPointOff); break;
				case AlignRight : DisplayChar((LengthOfStr-1) - pos, character, DecPointOff); break;
				case AlignRightZeros : return; break;
			}
			pos++;
		}
	}
}


/*!
	@brief Displays a text string on display
	@param text  pointer to character array containg text string
	@note This method is overloaded, see also DisplayText(char *, TextAlignment_e )
*/
void MAX7219_SS_RPI::DisplayText(char *text){

	char character;
	char pos = _NoDigits-1;

	while ((character = (*text++)) && pos < _NoDigits)
	{
		if (*text == '.' && character != '.')
		{
			DisplayChar(pos  ,character, DecPointOn);
			pos--;
			text++;
		}  else
		{
			DisplayChar(pos  ,character, DecPointOff);
			pos--;
		}
	}
}

/*!
	@brief Displays a BCD text string on display using MAX7219 Built in BCD code B font
	@param text  pointer to character array containg text string
	@note sets BCD code B font (0-9, E, H, L,P, and -) Built-in font
*/
void MAX7219_SS_RPI::DisplayBCDText(char *text){

	char character;
	char pos =_NoDigits-1;

	while ((character = (*text++)) )
	{
		switch (character)
		{
			case '0' : DisplayBCDChar(pos,CodeBFontZero);  break;
			case '1' : DisplayBCDChar(pos,CodeBFontOne);   break;
			case '2' : DisplayBCDChar(pos,CodeBFontTwo);   break;
			case '3' : DisplayBCDChar(pos,CodeBFontThree); break;
			case '4' : DisplayBCDChar(pos,CodeBFontFour);  break;
			case '5' : DisplayBCDChar(pos,CodeBFontFive);  break;
			case '6' : DisplayBCDChar(pos,CodeBFontSix);   break;
			case '7' : DisplayBCDChar(pos,CodeBFontSeven); break;
			case '8' : DisplayBCDChar(pos,CodeBFontEight); break;
			case '9' : DisplayBCDChar(pos,CodeBFontNine);  break;
			case '-' : DisplayBCDChar(pos,CodeBFontDash);  break;
			case 'E' :
			case 'e' :
				DisplayBCDChar(pos,CodeBFontE);
			break;
			case 'H' :
			case 'h' :
				DisplayBCDChar(pos,CodeBFontH);
			break;
			case 'L' :
			case 'l' :
				DisplayBCDChar(pos,CodeBFontL);
			break;
			case 'P' :
			case 'p' :
				DisplayBCDChar(pos,CodeBFontP);
			break;
			case ' ' : DisplayBCDChar(pos,CodeBFontSpace); break;
			default  : DisplayBCDChar(pos,CodeBFontSpace); break;
		}
	pos--;
	}
}

/*!
	@brief sets the brighttness of display
	@param brightness rang 0x00 to 0x0F , 0x00 being least bright.
*/
void MAX7219_SS_RPI::SetBrightness(uint8_t brightness)
{
	brightness &= IntensityMax;
	WriteDisplay(MAX7219_REG_Intensity, brightness);
}


/*!
	@brief Turn on and off the Shutdown Mode
	@param OnOff true = Shutdown mode on , false shutdown mode off
	@note power saving mode
*/
void MAX7219_SS_RPI::ShutdownMode(bool OnOff)
{
	OnOff ? WriteDisplay(MAX7219_REG_ShutDown, 0) : WriteDisplay(MAX7219_REG_ShutDown, 1);
}


/*!
	@brief Turn on and off the Display Test Mode
	@param OnOff true = display test mode on , false display Test Mode off
	@note Display-test mode turns all LEDs on
*/
void MAX7219_SS_RPI:: DisplayTestMode(bool OnOff)
{
	OnOff ? WriteDisplay(MAX7219_REG_DisplayTest, 1) : WriteDisplay(MAX7219_REG_DisplayTest, 0);
}


/*!
	@brief Set the communication delay value
	@param commDelay Set the communication delay value uS software SPI
*/
void MAX7219_SS_RPI::SetCommDelay(uint16_t commDelay) {_CommDelay = commDelay;}

/*!
	@brief Get the communication delay value
	@return Get the communication delay value uS Software SPi
*/
uint16_t  MAX7219_SS_RPI::GetCommDelay(void) {return _CommDelay;}

/*!
	@brief Get the Current Display Number
	@return Get the Current Display Number
*/
uint8_t MAX7219_SS_RPI::GetCurrentDisplayNumber(void){return _CurrentDisplayNumber; }

/*!
	@brief Set the Current Display Number
	@param DisplayNum Set the Current Display Number
*/
void MAX7219_SS_RPI::SetCurrentDisplayNumber(uint8_t DisplayNum )
{
if (DisplayNum == 0 ) DisplayNum = 1; // Zero user error check

_CurrentDisplayNumber  = DisplayNum  ;
}

/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32
	@param TextAlignment enum text alignment, left or right alignment or leading zeros
*/
void  MAX7219_SS_RPI::DisplayIntNum(unsigned long number, TextAlignment_e TextAlignment)
{
	char values[_NoDigits+1];
	char TextDisplay[6] = "%";
	char TextRight[4] = "8ld";
	char TextLeft[3] = "ld";
	char TextLeadZero[5] = "08ld";

	switch(TextAlignment)
	{
		case AlignRight:
			strcat(TextDisplay ,TextRight); // %8ld
		break;
		case AlignLeft:
			strcat(TextDisplay ,TextLeft);  // %ld
		break;
		case AlignRightZeros:
			strcat(TextDisplay ,TextLeadZero);  // %08ld
		break;
	}
	snprintf(values, _NoDigits+1, TextDisplay, number);
	DisplayText(values);
}


/*!
	@brief Display an integer in a nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param TextAlignment  left or right alignment or leading zeros
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble.
*/
void MAX7219_SS_RPI::DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, TextAlignment_e TextAlignment)
{
	char valuesUpper[_NoDigits+ 1];
	char valuesLower[_NoDigits/2 + 1];
	char TextDisplay[5] = "%";
	char TextRight[3] = "4d";
	char TextLeft[4] = "-4d";
	char TextLeadZero[4] = "04d";

	switch(TextAlignment)
	{
		case AlignRight: strcat(TextDisplay ,TextRight); break; // %4d
		case AlignLeft: strcat(TextDisplay ,TextLeft); break;  // %-4d
		case AlignRightZeros: strcat(TextDisplay ,TextLeadZero); break; // %04d
	}

	snprintf(valuesUpper, _NoDigits/2 + 1, TextDisplay, numberUpper);
	snprintf(valuesLower, _NoDigits/2 + 1, TextDisplay, numberLower);
	strcat(valuesUpper ,valuesLower);

	DisplayText(valuesUpper);
}



// Private methods

 /*!
	@brief Shifts out a uint8_t of data on to the MAX7219 SPI-like bus
	@param value The uint8_t of data to shift out
	@note _CommDelay microsecond delay may have to be adjusted depending on processor
*/
void MAX7219_SS_RPI::HighFreqshiftOut(uint8_t value)
{

	for (uint8_t bit = 0; bit < 8; bit++)
	{
		!!(value & (1 << (7 - bit))) ? Display_SDATA_SetHigh: Display_SDATA_SetLow; // MSBFIRST
		Display_SCLK_SetHigh;
		delayMicroSecRDL(_CommDelay);
		Display_SCLK_SetLow;
		delayMicroSecRDL(_CommDelay);
	}
}


/*!
	@brief Fetch's the seven segment code for a given ASCII code from the font
	@param character The ASCII character to  lookup
	@param decimalPoint Is the decimal point(dp) to be set or not.
	@return The seven segment representation of the ASCII character in a byte dpabcdefg
*/
uint8_t MAX7219_SS_RPI::ASCIIFetch(uint8_t character, DecimalPoint_e decimalPoint)
{
	if (character<=31 || character>=123)
	{
		printf("Error : AsciiFetch : ASCII character is outside font range %u, \n", character);
		return 0;
	} // check ASCII font bounds

	const uint8_t  AsciiOffset = 0x20; // The font starts at ASCII 0x20 . space
	uint8_t returnCharValue =0;
	returnCharValue = flipBitsPreserveMSB(pFontSevenSegptr[character - AsciiOffset]);
	switch (decimalPoint)
	{
		case DecPointOn  :  returnCharValue |= (1<<7); break;
		case DecPointOff :  break;
	}

	return returnCharValue;
}

/*!
	@brief Write to the MAX7219 display register
	@param RegisterCode the register to write to
	@param data The data byte to send to register
*/
void MAX7219_SS_RPI::WriteDisplay( uint8_t RegisterCode, uint8_t data)
{

	if (_HardwareSPI == false)
	{
		Display_CS_SetLow;
		HighFreqshiftOut(RegisterCode);
		HighFreqshiftOut(data);
		if (_CurrentDisplayNumber  > 1)
		{
			for (uint8_t i= 1 ; i <_CurrentDisplayNumber; i++)
			{
				HighFreqshiftOut(MAX7219_REG_NOP);
				HighFreqshiftOut(0x00);
			}
		}
		Display_CS_SetHigh;
	}else
	{
		char TransmitBuffer[_CurrentDisplayNumber*2];
		TransmitBuffer[0] = RegisterCode;
		TransmitBuffer[1] = data;
		if (_CurrentDisplayNumber  > 1)
		{
			for (uint8_t i= 2 ; i < (_CurrentDisplayNumber*2) ; i++)
			{
				TransmitBuffer[i] = 0x00;
			}
		}
		int spiErrorStatus = 0;
		spiErrorStatus = Display_SPI_WRITE(_spiHandle, static_cast<const char*>(TransmitBuffer), sizeof(TransmitBuffer));
		if (spiErrorStatus <0)
		{
			fprintf(stderr, "Error : Failure to Write  SPI :(%s)\n", lguErrorText(spiErrorStatus));
		}
	}
}

/*!
	@brief Set the decode mode of the  MAX7219 decode mode register
	@param mode Set to 0x00 for most users
*/
void MAX7219_SS_RPI::SetDecodeMode(DecodeMode_e mode)
{
	WriteDisplay(MAX7219_REG_DecodeMode , mode);
}

/*!
	@brief Set the decode mode of the  MAX7219 decode mode register
	@param numDigits Usually set to 7(digit 8) The scan-limit register sets how many digits are displayed,
	from 1 to 8.
	@note Advanced users only , read datasheet
*/
void MAX7219_SS_RPI::SetScanLimit(ScanLimit_e numDigits)
{
	WriteDisplay(MAX7219_REG_ScanLimit, numDigits);
}

/*!
	@brief Flips the positions of bits in a byte while preserving the MSB bit
	@param byte A byte of data, ASCII character
	@return A byte from font representing LED segment data with bits positions flipped and MSB bit value preserved 
	@details The reason for this function is that the MAX7219 requires ASCII segment data
	in following order : dp-abcdefg but the font we use is dp-gfedcba where
	letters represent seven segment LEDS, and dp represents decimal point.. 
	We flip the bits in the code rather than change the font data in font file because
	the font data is used by other modules(TM1638 + TM1637) and they use dp-gfedcba order.
	Thus we can share same font file between all seven segment modules.
*/
uint8_t MAX7219_SS_RPI::flipBitsPreserveMSB(uint8_t byte) 
{
	uint8_t msb = byte & 0x80; // Extract the most significant bit (MSB)
	uint8_t flipped = 0;       // Variable to store the flipped result

	for (int i = 0; i < 7; ++i) 
	{
		// Shift the i-th bit from the rightmost 7 bits to the new position
		if (byte & (1 << i)) 
		{
			flipped |= (1 << (6 - i));
		}
	}

	return msb | flipped; // Combine the MSB with the flipped bits
}
// == EOF ==
