/*!
	@file     HT16K33plus_Model1_RDL.cpp
	@author   Gavin Lyons
	@brief    Source file for for HT16K33 module. Model 1
*/

#include "../../include/ht16k33/HT16K33plus_Model1_RDL.hpp"


/*!
	@brief Constructor for class HT16K33plus_Model1
	@param I2CDevice An I2C device number.
	@param I2Caddress  The address of a device on the I2C bus. default is 0x70.
	@param I2CFlags Flags which modify an I2C open command. None are currently defined.
*/
HT16K33plus_Model1::HT16K33plus_Model1(int I2CDevice, int I2Caddress, int I2CFlags) 
{
	_I2CDevice = I2CDevice;
	_I2CAddress = I2Caddress;
	_I2CFlags = I2CFlags;
}

/*!
	@brief  Send data buffer to  via I2C
	@param data The data buffer to send
	@param length length of data to send
	@note if debug flag is true, will output data on I2C failures.
*/
void HT16K33plus_Model1::SendData(const unsigned char* data, size_t length) {
	
	uint8_t AttemptCount = _I2C_ErrorRetryNum;
	int ErrorCode = Display_RDL_I2C_WRITE(_I2CHandle, reinterpret_cast<const char*>(data), length);
	// Error handling retransmit
	while (ErrorCode < 0) {
		if (rdlib_config::isDebugEnabled()) {
			fprintf(stderr, "Error: SendData I2C: (%s) \n", lguErrorText(ErrorCode));
			fprintf(stderr, "Attempt Count: %u \n", AttemptCount);
		}
		delayMilliSecRDL(_I2C_ErrorDelay);
		ErrorCode = Display_RDL_I2C_WRITE(_I2CHandle, reinterpret_cast<const char*>(data), length); // retransmit
		_I2C_ErrorFlag = ErrorCode;
		AttemptCount--;
		if (AttemptCount == 0) break;
	}
	_I2C_ErrorFlag = ErrorCode;
}


/*!
	@brief  Send command byte to display
	@param cmd command byte
	@note if debug flag == true  ,will output data on I2C failures.
*/
void HT16K33plus_Model1::SendCmd(uint8_t cmd) {

	char cmdBufferI2C[1];
	cmdBufferI2C[0] = cmd;
	uint8_t AttemptCount = _I2C_ErrorRetryNum;
	int ErrorCode = Display_RDL_I2C_WRITE (_I2CHandle, cmdBufferI2C, sizeof(cmdBufferI2C));
	// Error handling retransmit
	while(ErrorCode < 0)
	{
		if (rdlib_config::isDebugEnabled())
		{
			fprintf(stderr, "Error:  SendCmd I2C: (%s) \n", lguErrorText(ErrorCode) );
			fprintf(stderr, "Attempt Count: %u \n", AttemptCount );
		}
		delayMilliSecRDL(_I2C_ErrorDelay );
		ErrorCode = Display_RDL_I2C_WRITE (_I2CHandle, cmdBufferI2C,  sizeof(cmdBufferI2C)); // retransmit
		_I2C_ErrorFlag = ErrorCode;
		AttemptCount--;
		if (AttemptCount == 0) break;
	}
	_I2C_ErrorFlag = ErrorCode;
}

/*!
	@brief Switch on the I2C
	@note Start I2C operations. 
	@return error for failure to init a I2C bus
		-# rdlib::Success
		-# rdlib::I2CbeginFail
*/
rdlib::Return_Codes_e HT16K33plus_Model1::Display_I2C_ON(void)
{
	int I2COpenHandle = 0;
	I2COpenHandle = Display_RDL_I2C_OPEN(_I2CDevice, _I2CAddress, _I2CFlags);
	if (I2COpenHandle < 0 )
	{
		fprintf(stderr, "Error: Display_I2C_ON: Can't open I2C (%s) \n", lguErrorText(I2COpenHandle));
		return rdlib::I2CbeginFail;
	}
	else
	{
		_I2CHandle = I2COpenHandle;
		return rdlib::Success;
	}
}

/*!
	@brief End I2C operations
	@return error for failure to close a I2C bus device
		-# rdlib::Success
		-# rdlib::I2CcloseFail
*/
rdlib::Return_Codes_e HT16K33plus_Model1::Display_I2C_OFF(void)
{
	int I2CCloseHandleStatus = 0;

	I2CCloseHandleStatus = Display_RDL_I2C_CLOSE(_I2CHandle);
	if (I2CCloseHandleStatus < 0 )
	{
		fprintf(stderr,  "Error:  Display_I2C_OFF : Can't Close I2C (%s) \n", lguErrorText(I2CCloseHandleStatus));
		return rdlib::I2CcloseFail;
	}
	return rdlib::Success;
}

/*!
	@brief get I2C error Flag
	@details See Error Codes at bottom of https://abyz.me.uk/lg/lgpio.html
	@return I2C error flag
*/
int HT16K33plus_Model1::DisplayI2CErrorGet(void) const { return _I2C_ErrorFlag;}

/*!
	@brief Sets the I2C timeout, in the event of an I2C write error
	@param newTimeout I2C timeout delay in mS
	@details Delay between retry attempts in event of an error , mS
*/
void HT16K33plus_Model1::DisplayI2CErrorTimeoutSet(uint16_t newTimeout)
{
	_I2C_ErrorDelay = newTimeout;
}

/*!
	@brief Gets the I2C timeout, used in the event of an I2C write error
	@details Delay between retry attempts in event of an error , mS
	@return  I2C timeout delay in mS, _I2C_ErrorDelay
*/
uint16_t HT16K33plus_Model1::DisplayI2CErrorTimeoutGet(void) const{return _I2C_ErrorDelay;}

/*!
	@brief Gets the I2C error retry attempts, used in the event of an I2C write error
	@details Number of times to retry in event of an error
	@return   _I2C_ErrorRetryNum
*/
uint8_t HT16K33plus_Model1::DisplayI2CErrorRetryNumGet(void) const {return _I2C_ErrorRetryNum;}

/*!
	@brief Sets the I2C error retry attempts used in the event of an I2C write error
	@details Number of times to retry in event of an error
	@param AttemptCount I2C retry attempts
*/
void HT16K33plus_Model1::DisplayI2CErrorRetryNumSet(uint8_t AttemptCount)
{
	_I2C_ErrorRetryNum = AttemptCount;
}

/*!
	@brief checks if Display on I2C bus
	@return lg Error codes, LG_OKAY   0x00 = Success
	@note Error codes are here https://abyz.me.uk/lg/lgpio.html
		prints error code text to console
*/
int HT16K33plus_Model1::DisplayCheckConnection(void)
{
	char rxdatabuf[1]; //buffer to hold return byte
	int I2CReadStatus = 0;

	I2CReadStatus = Display_RDL_I2C_READ(_I2CHandle, rxdatabuf, 1);
	if (I2CReadStatus < 0 )
	{
		fprintf(stderr, "Error: DisplayCheckConnection :Cannot read device (%s)\n",lguErrorText(I2CReadStatus));
	}
	_I2C_ErrorFlag = I2CReadStatus;
	return _I2C_ErrorFlag;
}


/*!
	@brief Initializes the HT16K33 display with specified settings.
	@param brightLevel Brightness level (0-15). If greater than 15, it defaults to 15.
	@param blinklevel Blink frequency setting (enumeration BlinkFreq_e 4 settings).
	@param numOfDigits Number of digits to be displayed.
	@param displayType Type of display configuration (enumeration DisplayType_e 4 settings).
*/
void HT16K33plus_Model1::DisplayInit(uint8_t brightLevel, BlinkFreq_e  blinklevel, uint8_t numOfDigits, DisplayType_e displayType)
{
	SendCmd(HT16K33_NORMAL); //normal operation mode
	SendCmd(HT16K33_BRIGHTNESS + brightLevel);//brightness level
	SendCmd(HT16K33_DISPLAYON | blinklevel); //display ON
	if (brightLevel > 0x0F) {
		printf("Warning : DisplayInit : Brightness value must be lower than 17 setting to 16\n");
		brightLevel = 0x0F;
	}
	_blinkSetting = blinklevel;
	_brightness = brightLevel;
	_numOfDigits = numOfDigits;
	_displayType = displayType;
}

/*!
	@brief Turns on the display with the previously set blink setting.
	@details Sends the command to enable the display, using the stored blink setting.
*/
void HT16K33plus_Model1::DisplayOn(void){
	SendCmd(HT16K33_DISPLAYON | _blinkSetting);
}

/*!
	@brief Turns off the display.
*/
void HT16K33plus_Model1::DisplayOff(void){
	SendCmd(HT16K33_DISPLAYOFF);
}

/*!
	@brief Puts the display into standby mode. Turn off System oscillator
*/
void HT16K33plus_Model1::DisplaySleep(void){
	SendCmd(HT16K33_STANDBY);
}

/*!
	@brief Restores the display to normal operation mode. Turn on System oscillator
*/
void HT16K33plus_Model1::DisplayNormal(void){
	SendCmd(HT16K33_NORMAL);
}

/*!
	@brief Resets the display to default settings.
	@details This function restores normal operation mode, sets brightness to a default level of 7,
	         and turns the display on with the stored blink setting.
*/
void HT16K33plus_Model1::DisplayResetDefault(void){
	DisplayNormal();
	setBrightness(7);
	DisplayOn();
}


/*!
	@brief Sets the display blink frequency.
	@param blinklevel Blink frequency setting (enumeration BlinkFreq_e 4 settings).
*/
void HT16K33plus_Model1::setBlink( BlinkFreq_e  blinklevel){
	_blinkSetting = blinklevel;
	SendCmd(HT16K33_DISPLAYON | _blinkSetting );
}

/*!
	@brief Gets the current blink frequency setting.
	@returns The current blink frequency (enumeration BlinkFreq_e).
*/
HT16K33plus_Model1::BlinkFreq_e HT16K33plus_Model1::getBlink() const{
	return  _blinkSetting;
}

/*!
	@brief Gets the display type.
	@returns The current display type setting (enumeration DisplayType_e).
*/
HT16K33plus_Model1::DisplayType_e HT16K33plus_Model1::getDisplayType() const{
	return  _displayType;
}

/*!
	@brief Sets the display brightness level.
	@param value Brightness level (0-15). If greater than 15, it defaults to 15.
*/
void HT16K33plus_Model1::setBrightness(uint8_t value)
{
	if (value == _brightness) 
		return;
	_brightness = value;
	if (_brightness > 0x0F) {
		printf("Warning : setBrightness : Brightness value must be lower than 17 setting to 16\n");
		_brightness = 0x0F;
	}
	SendCmd(HT16K33_BRIGHTNESS + _brightness);
}

/*!
	@brief Gets the current brightness level.
	@returns The current brightness level (0-15).
*/
uint8_t HT16K33plus_Model1::getBrightness() const{
	return _brightness;
}

/*!
	@brief Displays a single character at the specified digit position.
	@param digitPosition The position of the digit on the display (0-based index 0 = LHS).
	@param character The ASCII character to display.
	@param decimalOnPoint Specifies whether the decimal point should be enabled (enumeration DecimalPoint_e).
	@returns Return code indicating success or an error (enumeration rdlib::Return_Codes_e).
	@details If the character is out of the supported ASCII font range, an error is logged and a corresponding error code is returned.
	         If the display type is not 7-segment, the function delegates to displayMultiSegNum().
	         Otherwise, the function retrieves the 7-segment font data, applies the decimal point if needed, 
	         and sends the data to the display.
*/
rdlib::Return_Codes_e HT16K33plus_Model1::displayChar(uint8_t digitPosition, char character, DecimalPoint_e decimalOnPoint)
{
	if (character <= (_HT_ASCIIOffset-1) || character >= _HT_ASCIIEnd)
	{// check ASCII font bounds
		fprintf(stderr, "Error : displayChar: ASCII character is outside font range %c, \n", character);
		return rdlib::CharFontASCIIRange;
	}
	if (_displayType != SegType7){
			return displayMultiSegNum(digitPosition, character, decimalOnPoint);
	} else {
		uint8_t characterConverted;
		const uint8_t* font = SevenSegmentFont::pFontSevenSegptr();
		characterConverted = font[character- _HT_ASCIIOffset];
		if (decimalOnPoint ==  DecPointOn) characterConverted |= _HT_DEC_POINT_7_MASK;
		uint8_t txDataBuffer[2];
		size_t length = sizeof(txDataBuffer);
		txDataBuffer[0] = digitPosition*2;
		txDataBuffer[1] = characterConverted;
		SendData(txDataBuffer, length);
	}
	return rdlib::Success;
}

/*!
	@brief Displays a character on multi-segment displays (9, 14, or 16 segments).
	@param digitPosition The position of the digit on the display (0-based index, 0=LHS).
	@param character The ASCII character to display.
	@param decimalOnPoint Specifies whether the decimal point should be enabled (enumeration DecimalPoint_e).
	@returns Return code indicating success or an error (enumeration rdlib::Return_Codes_e).
	@details Converts the ASCII character into the appropriate segment mapping for the display type (9-segment, 14-segment, or 16-segment).
	         The decimal point is applied if applicable.
	         The processed data is then sent to the display.
*/
rdlib::Return_Codes_e HT16K33plus_Model1::displayMultiSegNum(uint8_t digitPosition, char character, DecimalPoint_e decimalOnPoint)
{
	uint16_t characterConverted;
	switch (_displayType)
	{
		case SegType7:
			return rdlib::GenericError; // this will never occur in normal user operation
		break; 
		case SegType9:{
			const uint16_t* fontNine = NineSegmentFont::pFontNineSegptr();
			characterConverted = fontNine[character- _HT_ASCIIOffset];
			if (decimalOnPoint ==  DecPointOn) characterConverted |= _HT_DEC_POINT_9_MASK;
			}
		break;
		case SegType14:{
			const uint16_t* fontFourteen = FourteenSegmentFont::pFontFourteenSegptr();
			characterConverted = fontFourteen[character- _HT_ASCIIOffset];
			if (decimalOnPoint ==  DecPointOn) characterConverted |= _HT_DEC_POINT_14_MASK;
			}
		break;
		case SegType16:{
			const uint16_t* fontSixteen = SixteenSegmentFont::pFontSixteenSegptr();
			characterConverted = fontSixteen[character- _HT_ASCIIOffset];
			(void)decimalOnPoint;
			}
		break;
	}
	uint8_t txDataBuffer[3];
	size_t bufferLength= sizeof(txDataBuffer);
	txDataBuffer[0] = digitPosition * 2;
	txDataBuffer[1] = characterConverted & 0x00FF;
	txDataBuffer[2] = (characterConverted & 0xFF00) >> 8;
	SendData(txDataBuffer, bufferLength);
	return rdlib::Success;
}

/*!
	@brief Sends raw segment data to a specific digit position.
	@param digitPosition The position of the digit on the display (0-based index).
	@param rawData The raw segment data to be displayed (bit-mapped for the display type).
	@details This function allows direct control of the display segments by sending raw data.
	         The rawData value is split into two bytes and transmitted to the display.
*/
void HT16K33plus_Model1::displayRawData(uint8_t digitPosition, uint16_t rawData)
{
	uint8_t txDataBuffer[3];
	size_t bufferLength = sizeof(txDataBuffer); 
	txDataBuffer[0] = digitPosition * 2;
	txDataBuffer[1] = rawData & 0x00FF;
	txDataBuffer[2] = (rawData & 0xFF00) >> 8;
	SendData(txDataBuffer, bufferLength);
}

/*!
	@brief Clears all digits on the display.
	@details Iterates through all digit positions and replaces each character with a blank space.
	         The decimal points are also turned off.
*/
void HT16K33plus_Model1::ClearDigits(void)
{
	for(uint8_t i=0; i<=_numOfDigits; i++) 
	{
		displayChar(i, ' ', DecPointOff);
	}
}

/*!
	@brief Displays a text string on display
	@param text pointer to character array containg text string
	@param TextAlignment  left or right alignment
	@note This method is overloaded, see also DisplayText(char *)
		leading zeros is not currently an option as a workaround
		user can add them to string before hand.
	@return WIll return error for null pointer string or leading zeros option requested
*/
rdlib::Return_Codes_e HT16K33plus_Model1::displayText(const char *text, TextAlignment_e TextAlignment) {
	if (text == nullptr) {
		fprintf(stderr, "Error: displayText: String is a null pointer.\n");
		return rdlib::CharArrayNullptr;
	}
	uint8_t charCount = strlen(text); 
	// Adjust character count for non-16 segment types by subtracting non-consecutive dots
	if (_displayType != SegType16)
	{
		for (uint8_t i = 1; text[i]; i++) {
			if (text[i] == '.' && text[i - 1] != '.') {
				charCount--; 
			}
		}
	}
	if (charCount > _numOfDigits) charCount = _numOfDigits;
	// Compute starting position based on alignment
	uint8_t pos = 0;
	switch (TextAlignment)
	{
		case AlignLeft: pos = 0; break;
		case AlignRight: pos = _numOfDigits - charCount; break;
		case AlignRightZeros:
			fprintf(stderr, "Error: displayText 2: Leading zeros not an option in this function\n");
			return rdlib::GenericError; 
		break;
	}
	// Display characters with correct alignment
	uint8_t displayPos = pos;
	while (*text && displayPos < _numOfDigits)
	{
		char character = *text++;
		if (_displayType != SegType16 && *text == '.' && character != '.') {
			displayChar(displayPos++, character, DecPointOn);
			text++;  // Skip the decimal point
		} else {
			displayChar(displayPos++, character, DecPointOff);
		}
	}
	return rdlib::Success;
}


/*!
	@brief Display a text string  on display
	@param text pointer to a character array
	@return error code  if string is nullptr or too long 
	@note 
		Dots are removed from string and dot on preceding digit switched on
		"abc.def" will be shown as "abcdef" with c decimal point turned on,
		Unless the Display is sixteen segment.
*/
rdlib::Return_Codes_e HT16K33plus_Model1::displayText(const char *text) {
	// Check for null pointer
	if (text == nullptr) 
	{
		fprintf(stderr ,"Error: displayText1: String is a null pointer.\n");
		return rdlib::CharArrayNullptr;
	}
	char character, pos;
	pos = 0;
	while ((character = (*text++)) && pos < _numOfDigits)
		{
			if (_displayType != SegType16 && *text == '.' && character != '.') {
				displayChar(pos++ ,character, DecPointOn);
				text++;
			}else{
				displayChar(pos++ ,character, DecPointOff);
			}
		}
	return rdlib::Success;
}

/*!
	@brief  Send Hexadecimal value to seven segment
	@param position The position on display 
	@param hex  hexadecimal  value (DEC) 0-15  (0x00 - 0x0F)
	@return Will return an error if not hexadecimal.
*/
rdlib::Return_Codes_e  HT16K33plus_Model1::displayHexChar(uint8_t position, char hex) 
{
		if (hex <= '9')
		{
			displayChar(position, hex , DecPointOff);
		}else
		{
			char offset = 0;
			switch(hex) 
			{
			 case 'A': offset = 'A'; break;
			 case 'B': offset = 'b'; break;
			 case 'C': offset = 'C'; break;
			 case 'D': offset = 'd'; break;
			 case 'E': offset = 'E'; break;
			 case 'F': offset = 'F'; break;
			 default: 
				fprintf(stderr,"Error: displayHexChar:  This is not a hexadecimal character\n");
				return rdlib::CharFontASCIIRange;
				break;
			}
			displayChar(position , offset , DecPointOff);
		}
	return rdlib::Success;
}

/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32 
	@param TextAlignment enum text alignment, left or right alignment or leading zeros
	@return will return error user tries to display  if too much data
*/
rdlib::Return_Codes_e HT16K33plus_Model1::displayIntNum(int32_t number, TextAlignment_e TextAlignment)
{
	// Ensure the number can fit within the display
	if (number >= pow(10, _numOfDigits) || number < -(pow(10, (_numOfDigits-1)))) {
		fprintf(stderr, "Error: displayIntNum: Number too many digits for display\n");
		return rdlib::GenericError; 
	}
	char values[_numOfDigits + 1] = {0};  
	char TextDisplay[12]; 
	// Generate the format string dynamically
	switch (TextAlignment) 
	{
		case AlignRight:
			snprintf(TextDisplay, sizeof(TextDisplay), "%%%dd", (int)_numOfDigits);
			break;
		case AlignLeft:
			snprintf(TextDisplay, sizeof(TextDisplay), "%%-d"); 
			break;
		case AlignRightZeros:
			snprintf(TextDisplay, sizeof(TextDisplay), "%%0%dd", (int)_numOfDigits);
			break;
	}
	// Print formatted number into values buffer
	snprintf(values, sizeof(values), TextDisplay, number);
	displayText(values);
	return rdlib::Success;
}

/*!
	@brief Displays a floating-point number on the display.
	@param number The floating-point number to be displayed.
	@param TextAlignment Text alignment option (enumeration TextAlignment_e 2 options).
	@param fractionDigits Number of fractional digits to display.
	@returns Return code indicating success or an error (enumeration rdlib::Return_Codes_e).
	@details Converts the floating-point number into a formatted string and displays it on the device.
	         If the total number of required digits (integer + fractional + sign) exceeds the available display space,
	         an error is returned. Leading zero alignment is not supported.
*/
rdlib::Return_Codes_e HT16K33plus_Model1::displayFloatNum(float number, TextAlignment_e TextAlignment, uint8_t fractionDigits)
{
	if (TextAlignment == AlignRightZeros){
			fprintf(stderr, "Error: displayFloatNum 2: Leading zeros not an option\n");
			return rdlib::GenericError; 
	}
	// Calculate integer and fractional part lengths
	int intPart = (int)number;
	int intLen = (intPart == 0) ? 1 : (int)log10(abs(intPart)) + 1;  // Length of integer part
	int totalLen = intLen + fractionDigits;  // Total length includes integer and fractional digits
	if (number < 0) {
		totalLen++;  // Add space for the negative sign if needed
	}
	// Check if the number fits within the available display space
	if (totalLen > _numOfDigits) {
		fprintf(stderr, "Error: displayFloatNum: Number too large for display (Total digits: %d, Max allowed: %d)\n", totalLen, _numOfDigits);
		return rdlib::GenericError;
	}
	// Allocate buffer to store the formatted string
	char workStr[15];
	snprintf(workStr, sizeof(workStr), "%.*f", fractionDigits, number);
	displayText(workStr, TextAlignment);
	return rdlib::Success;
}
