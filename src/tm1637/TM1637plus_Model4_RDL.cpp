/*!
	@file     TM1637plus_Model4_RDL.cpp
	@author   Gavin Lyons
	@brief    RPI library Tm1637plus, Source file for for TM1637 module. Model 4
*/

#include "../../include/tm1637/TM1637plus_Model4_RDL.hpp"

/*!
	@brief Initialize a TM1637 object
	@param clock GPIO connected to the clock pin of the module
	@param data GPIO connected to the DIO pin of the module
	@param gpioDev The device number of a gpiochip.
	@param delay microseconds delay, between bit transition on the serial
			bus connected to the display
	@param displaySize number of digits in display.
*/
TM1637plus_Model4::TM1637plus_Model4(uint8_t clock, uint8_t data, int gpioDev, int delay, int displaySize)
{
	_DATA_IO = data;
	_CLOCK_IO = clock;
	_DeviceNumGpioChip = gpioDev;
	_BitDelayUS = delay;
	_DisplaySize = displaySize;
}

/*! 
	@brief Clears the display
*/
void  TM1637plus_Model4::displayClear()
{
	uint8_t data[_DisplaySize] = {0};
	setSegments(data, _DisplaySize, 0);
}
/*!
	@brief Begin method , set and claims GPIO
	@return 
		-# rpiDisplay_Success
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinClaim
	@note Call in Setup
*/
rpiDisplay_Return_Codes_e TM1637plus_Model4::displayBegin(void)
{
	int GpioClockErrorstatus = 0;
	int GpioDataErrorstatus = 0;

	// open /dev/gpiochipX
	_GpioHandle = TM163X_OPEN_GPIO_CHIP; 
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Errror : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rpiDisplay_GpioChipDevice;
	}

	// Clain GPIO
	GpioClockErrorstatus = TM163X_SET_INPUT_CLOCK;
	GpioDataErrorstatus = TM163X_SET_INPUT_DATA;
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for input (%s)\n", lguErrorText(GpioClockErrorstatus));
		return rpiDisplay_GpioPinClaim;
	}
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for input (%s)\n", lguErrorText(GpioDataErrorstatus));
		return rpiDisplay_GpioPinClaim;
	}

	return rpiDisplay_Success;
}

/*!
	@brief Sets the brightness of the display.
	@param brightness A number from 0 to 7 (highest brightness)
	@param on Turn display on or off
	@note The setting takes effect when a command is given to change the data being
	displayed.
*/
void TM1637plus_Model4::setBrightness(uint8_t brightness, bool on)
{
	_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

/*!
	@brief Display data on the module
	@details This function receives segment values as input and displays them. The segment data
	is given as a byte array, each byte corresponding to a single digit. Within each byte,
	bit 0 is segment A, bit 1 is segment B etc.
	The function may either set the entire display or any desirable part on its own. The first
	digit is given by the reference position argument with 0 being the leftmost digit. The reference length
	argument is the number of digits to be set. Other digits are not affected.
	@param segments An array of size length containing the raw segment values
	@param length The number of digits to be modified
	@param position The position from which to start the modification (0 - leftmost, 3 - rightmost)
*/
void TM1637plus_Model4::setSegments(const uint8_t segments[], uint8_t length, uint8_t position)
{
	// Write Command 1
	CommStart();
	writeByte(TM1637_COMMAND_1);
	CommStop();

	// Write Command 2 + first digit address
	CommStart();
	writeByte(TM1637_COMMAND_2 + (position & 0x03));
	// Write the data
	for (uint8_t i=0; i < length; i++)
		writeByte(segments[i]);
	CommStop();

	// Write Command 3 + brightness
	CommStart();
	writeByte(TM1637_COMMAND_3 + (_brightness & 0x0F));
	CommStop();
}


/*!
	@brief Displays a decimal number
	@details Displays the given argument as a decimal number
	@param number The number to be shown
	@param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are blank
	@param length The number of digits to set. The user must ensure that the number to be shown
			fits to the number of digits requested (for example, if two digits are to be displayed,
			the number must be between 0 to 99)
	@param position The position most significant digit (0 - leftmost, 3 - rightmost)
*/
void TM1637plus_Model4::DisplayDecimal(int number,  bool leading_zero ,uint8_t length, uint8_t position)
{
	DisplayDecimalwDot(number, 0, leading_zero, length, position);
}

/*!
	@brief Displays a decimal number, with decimal point control 
	@details Displays the given argument as a decimal number. The dots between the digits (or colon)
	can be individually controlled
	@param number The number to be shown
	@param dots decimal point enable. The argument is a bitmask, with each bit corresponding to a dot
		  between the digits (or colon mark, as implemented by each module). The MSB is the 
		  leftmost dot of the digit being update. For example, if pos is 1, the MSB of dots
		  will correspond the dot near digit no. 2 from the left. Dots are updated only on
		  those digits actually being update (that is, no more than len digits)
	@param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
		  blank
	@param length The number of digits to set. The user must ensure that the number to be shown
		  fits to the number of digits requested (for example, if two digits are to be displayed,
		  the number must be between 0 to 99)
	@param position The position least significant digit (0 - leftmost, 3 - rightmost)
*/
void TM1637plus_Model4::DisplayDecimalwDot(int number, uint8_t dots,  bool leading_zero ,uint8_t length, uint8_t position)
{
	// Array to store the encoded digits for the display
	uint8_t digits[_DisplaySize];
	// Array of divisors used to extract each digit from the number
	const static int divisors[] = { 1, 10, 100, 1000, 10000, 100000 };
	bool leading = true;

	// Loop through each digit position
	for (int8_t i = 0; i < _DisplaySize; i++) 
	{
		// Determine the divisor for the current digit position
		int divisor = divisors[_DisplaySize - 1 - i];
		// Extract the current digit from the number
		int character = number / divisor;
		uint8_t digit = 0;
		// Handle leading zeros and actual digits
		if (character == 0) 
		{
			// If leading zero is enabled or we're no longer in leading zeros
			if (leading_zero || !leading || (i == (_DisplaySize-1))) 
				digit = encodeCharacter('0' + character);
			else
				digit = 0;                // Leave the segment blank
		} else 
		{
			digit = encodeCharacter('0' + character);
			number -= character * divisor;   // Remove the processed digit from the number
			leading = false;                 // Leading zeros end as soon as a non-zero digit is found
		}
		digit |= (dots & 0x80);
		dots <<= 1;
		digits[i] = digit;
	}

	// Set the segments on the display starting at the specified position
	// Use only the relevant portion of the `digits` array based on `length`
	setSegments(digits + (_DisplaySize - length), length, position);
}

/*!
	@brief Displays a character array
	@param numStr The character array to be shown
	@param dots decimal point enable. The argument is a bitmask, with each bit corresponding to a dot
		  between the digits (or colon mark, as implemented by each module). The MSB is the 
		  leftmost dot of the digit being update. For example, if pos is 1, the MSB of dots
		  will correspond the dot near digit no. 2 from the left. Dots are updated only on
		  those digits actually being update (that is, no more than len digits)
	@param length The number of digits to set. The user must ensure that the number to be shown
		  fits to the number of digits requested
	@param position The position most significant digit (0 - leftmost, 3 - rightmost)
*/
void TM1637plus_Model4::DisplayString(const char* numStr, uint8_t dots, uint8_t length, uint8_t position)
{
	uint8_t digits[_DisplaySize] = {0}; // Initialize all segments to 0
	uint8_t digit = 0;
	
	for (int8_t i = 0; i < length; i++) 
	{
		char currentChar = numStr[i];
		digit = encodeCharacter(currentChar);
		// Add the decimal point/colon to the digit if specified in `dots`
		digit |= (dots & 0x80); // Check the MSB of `dots` and add it to the current digit
		dots <<= 1;             // Shift the `dots` value to apply the next dot/colon to the next digit
		digits[i] = digit;
	}
	// Set the segments based on the processed digits
	setSegments(digits, length, position);
}


/*!
	@brief Translate a single Character into 7 segment code
	@details The method accepts a ASCII character and converts it to the
		seven segment code required to display the number on a 7 segment display.
		by referencing the ASCII font table on file.
	@param digit An ASCII character
	@return A code representing the 7 segment image of the digit (LSB - segment A;
			bit 6 - segment G; bit 7 - always zero) from the font.
*/
unsigned char TM1637plus_Model4::encodeCharacter(unsigned char digit)
{
	unsigned char ascii = pFontSevenSegptr[digit- TM1637_ASCII_OFFSET];
	return ascii;
}

/*!
	@brief Close method , frees GPIO and deactivate display.
	@return 
		-# rpiDisplay_Success
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioPinFree
	@note call at end of program
*/
rpiDisplay_Return_Codes_e TM1637plus_Model4::displayClose(void)
{
	uint8_t ErrorFlag = 0; // Becomes > 0 in event of error

	int GpioClockErrorstatus = 0;
	int GpioDataErrorstatus = 0;
	int GpioCloseStatus = 0;

	GpioClockErrorstatus =  TM163X_GPIO_FREE_CLOCK;
	GpioDataErrorstatus =   TM163X_GPIO_FREE_DATA;

	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error :: Can't Free CLK GPIO  (%s)\n", lguErrorText(GpioClockErrorstatus));
		ErrorFlag = 2;
	}
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error :: Can't free DATA GPIO (%s)\n", lguErrorText(GpioDataErrorstatus));
		ErrorFlag = 2;
	}

	GpioCloseStatus = TM163X_CLOSE_GPIO_HANDLE; // close gpiochip
	if ( GpioCloseStatus < 0)
	{
		fprintf(stderr,"Error :: Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		ErrorFlag = 3;
	}

	// 4 Check error flag (we don't want to return early for any failure)
	switch (ErrorFlag)
	{
		case 0:return rpiDisplay_Success;break;
		case 2:return rpiDisplay_GpioPinFree;break;
		case 3:return rpiDisplay_GpioChipDevice;;break;
		default:printf("Warning::Unknown error flag value in displayClose"); break;
	}
	return rpiDisplay_Success;
}


/*!
	@brief Sets the delay, in microseconds, between bit transition on the serial
		bus connected to the display
*/
void TM1637plus_Model4::CommBitDelay(void)
{
	delayMicroSecRDL(_BitDelayUS);
}

/*! 
	@brief Starts the communication sequence 
*/
void TM1637plus_Model4::CommStart(void)
{
	int GpioDataErrorstatus = 0;
	GpioDataErrorstatus = TM163X_SET_OUTPUT_DATA;
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
	}
	CommBitDelay();
}

/*! 
	@brief Stops the communication sequence
*/
void TM1637plus_Model4::CommStop(void)
{
	int GpioDataErrorstatus = 0;
	int GpioClockErrorstatus = 0;
	
	GpioDataErrorstatus = TM163X_SET_OUTPUT_DATA;
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
	}
	CommBitDelay();

	GpioClockErrorstatus = TM163X_SET_INPUT_CLOCK;
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for input (%s)\n", lguErrorText(GpioClockErrorstatus));
	}
	CommBitDelay();

	GpioDataErrorstatus = TM163X_SET_INPUT_DATA;
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for input (%s)\n", lguErrorText(GpioDataErrorstatus));
	}
	CommBitDelay();
}

/*! 
	@brief Writes a byte to the Display
	@param byte the Byte to write
	@return status of acknowledge bit
*/
bool TM1637plus_Model4::writeByte(uint8_t byte)
{
	uint8_t data = byte;
	int GpioDataErrorstatus = 0;
	int GpioClockErrorstatus = 0;

	// 8 Data Bits
	for(uint8_t i = 0; i < 8; i++) 
	{
		// Set clock low
		GpioClockErrorstatus = TM163X_SET_OUTPUT_CLOCK;
		if (GpioClockErrorstatus < 0 )
		{
			fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorstatus));
		}
		CommBitDelay();

		// Set data bit
		if (data & 0x01)
		{
			GpioDataErrorstatus = TM163X_SET_INPUT_DATA;
			if (GpioDataErrorstatus < 0 )
			{
				fprintf(stderr, "Error : Can't claim DATA GPIO for input (%s)\n", lguErrorText(GpioDataErrorstatus));
			}
		}
		else
		{
			GpioDataErrorstatus = TM163X_SET_OUTPUT_DATA;
			if (GpioDataErrorstatus < 0 )
			{
				fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
			}
		}

		CommBitDelay();

		// Set Clock high
		GpioClockErrorstatus = TM163X_SET_INPUT_CLOCK;
		if (GpioClockErrorstatus < 0 )
		{
			fprintf(stderr,"Error : Can't claim CLK GPIO for input (%s)\n", lguErrorText(GpioClockErrorstatus));
		}
		CommBitDelay();
		data = data >> 1;
	}

	// Wait for acknowledge
	// Set Clock to Low
	GpioClockErrorstatus = TM163X_SET_OUTPUT_CLOCK;
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorstatus));
	}
	GpioDataErrorstatus = TM163X_SET_INPUT_DATA;
	if (GpioDataErrorstatus < 0 )
	{
		fprintf(stderr, "Error : Can't claim DATA GPIO for input (%s)\n", lguErrorText(GpioDataErrorstatus));
	}
	CommBitDelay();

	// Set clock to high
	GpioClockErrorstatus = TM163X_SET_INPUT_CLOCK;
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for input (%s)\n", lguErrorText(GpioClockErrorstatus));
	}
	CommBitDelay();

	uint8_t acknowledge = TM163X_DATA_READ;
	if (acknowledge == 0)
	{
		GpioDataErrorstatus = TM163X_SET_OUTPUT_DATA;
		if (GpioDataErrorstatus < 0 )
		{
			fprintf(stderr, "Error : Can't claim DATA GPIO for output (%s)\n", lguErrorText(GpioDataErrorstatus));
		}
	}
	CommBitDelay();

	GpioClockErrorstatus = TM163X_SET_OUTPUT_CLOCK;
	if (GpioClockErrorstatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim CLK GPIO for output (%s)\n", lguErrorText(GpioClockErrorstatus));
	}
	CommBitDelay();

	return acknowledge;
}
