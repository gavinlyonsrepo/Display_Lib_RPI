/*!
	@file     HD44780_LCD_RDL.cpp
	@author   Gavin Lyons
	@brief    HD44780-based character LCD I2C(PCF8574) source header file for RPI
*/

// Section : Includes
#include "../../include/hd44780/HD44780_LCD_RDL.hpp"

/*!
	@brief Constructor for class HD44780PCF8574LCD
	@param NumRow number of rows in LCD
	@param NumCol number of columns in LCD
	@param I2Caddress  The PCF8574 I2C address, default is 0x27.
	@param I2Cspeed I2C Bus Clock speed in KHz. Default BCM2835_I2C_CLOCK_DIVIDER_626 
	@details 
		-# = 0  //bcm2835_i2c_set_baudrate(100000); 100k baudrate
		-# > 0  BCM2835_I2C_CLOCK_DIVIDER, choices = 2500 , 626 , 150 , 148
		-# BCM2835_I2C_CLOCK_DIVIDER_2500   = 2500, 2500 = 10us = 100 kHz 
		-# BCM2835_I2C_CLOCK_DIVIDER_626    = 626,  626 = 2.504us = 399.3610 kHz 
		-# BCM2835_I2C_CLOCK_DIVIDER_150    = 150, 150 = 60ns = 1.666 MHz (default at reset) 
		-# BCM2835_I2C_CLOCK_DIVIDER_148    = 148 148 = 59ns = 1.689 MHz
*/
HD44780PCF8574LCD::HD44780PCF8574LCD(uint8_t NumRow, uint8_t NumCol, uint8_t I2Caddress, uint16_t I2Cspeed)
{
	_NumRowsLCD = NumRow;
	_NumColsLCD = NumCol;
	_LCDSlaveAddresI2C  = I2Caddress;
	_LCDSpeedI2C = I2Cspeed;
}

// Section : methods

/*!
	@brief  Send data byte to  LCD via I2C
	@param data The data byte to send
	@note if _DebugON is true, will output data on I2C failures.
*/
void HD44780PCF8574LCD::LCDSendData(unsigned char data) {
	
	bcm2835_i2c_setSlaveAddress(_LCDSlaveAddresI2C);  //i2c address
	
	// I2C MASK Byte = DATA-led-en-rw-rs (en=enable rs = reg select)(rw always write)
	const uint8_t LCDDataByteOn= 0x0D; //enable=1 and rs =1 1101  DATA-led-en-rw-rs
	const  uint8_t LCDDataByteOff = 0x09; // enable=0 and rs =1 1001 DATA-led-en-rw-rs
	
	unsigned char dataNibbleLower, dataNibbleUpper;
	char dataBufferI2C[4];
	uint8_t AttemptCount = _I2C_ErrorRetryNum;
	
	dataNibbleLower = (data << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
	dataNibbleUpper = data & 0xf0; //select upper nibble
	dataBufferI2C[0] = dataNibbleUpper | (LCDDataByteOn & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	dataBufferI2C[1] = dataNibbleUpper | (LCDDataByteOff & _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs
	dataBufferI2C[2] = dataNibbleLower | (LCDDataByteOn & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	dataBufferI2C[3] = dataNibbleLower | (LCDDataByteOff &  _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs

	// bcm2835I2CReasonCodes , BCM2835_I2C_REASON_OK 0x00 = Success
	uint8_t  ReasonCodes = bcm2835_i2c_write(dataBufferI2C, 4);

	// Error handling retransmit
	while(ReasonCodes != 0)
	{
		if (_DebugON == true)
		{
			std::cout << "Error 601 I2C  Data bcm2835I2CReasonCodes : " << +ReasonCodes << std::endl;
			std::cout << "Attempt Count: " << +AttemptCount << std::endl;
		}
		delayMilliSecRDL(_I2C_ErrorDelay );
		ReasonCodes = bcm2835_i2c_write(dataBufferI2C, 4); // retransmit
		_I2C_ErrorFlag = ReasonCodes;
		AttemptCount--;
		if (AttemptCount == 0) break;
	}
	_I2C_ErrorFlag = ReasonCodes;
}

/*!
	@brief  Send command byte to lcd
	@param cmd command byte
	@note if _DebugON == true  ,will output data on I2C failures.
*/
void HD44780PCF8574LCD::LCDSendCmd(unsigned char cmd) {
	
	bcm2835_i2c_setSlaveAddress(_LCDSlaveAddresI2C);  //i2c address
	
	// I2C MASK Byte = COMD-led-en-rw-rs (en=enable rs = reg select)(rw always write)
	const uint8_t LCDCmdByteOn = 0x0C;  // enable=1 and rs =0 1100 COMD-led-en-rw-rs 
	const uint8_t LCDCmdByteOff = 0x08; // enable=0 and rs =0 1000 COMD-led-en-rw-rs 
	

	unsigned char cmdNibbleLower, cmdNibbleUpper;
	char cmdBufferI2C[4];
	uint8_t AttemptCount = _I2C_ErrorRetryNum;
	
	cmdNibbleLower = (cmd << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
	cmdNibbleUpper = cmd & 0xf0; //select upper nibble
	cmdBufferI2C[0] = cmdNibbleUpper | (LCDCmdByteOn & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmdBufferI2C[1] = cmdNibbleUpper | (LCDCmdByteOff & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
	cmdBufferI2C[2] = cmdNibbleLower | (LCDCmdByteOn & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmdBufferI2C[3] = cmdNibbleLower | (LCDCmdByteOff & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0

	// bcm2835I2CReasonCodes, BCM2835_I2C_REASON_OK 0x00 = Success
	uint8_t ReasonCodes = bcm2835_i2c_write(cmdBufferI2C, 4);
	while(ReasonCodes != 0)
	{
		if (_DebugON == true)
		{
			std::cout << "Error 602: I2C Command bcm2835I2CReasonCodes : " << +ReasonCodes << std::endl;
			std::cout << "Attempt Count : " << +AttemptCount<< std::endl;
		}
		delayMilliSecRDL(_I2C_ErrorDelay);
		ReasonCodes = bcm2835_i2c_write(cmdBufferI2C,4); // retransmit
		_I2C_ErrorFlag = ReasonCodes;
		AttemptCount--;
		if (AttemptCount== 0) break;
	}
	_I2C_ErrorFlag = ReasonCodes;
}

/*!
	@brief  Clear a line by writing spaces to every position
	@param lineNo LCDLineNumber_e enum lineNo  1-4
*/
void HD44780PCF8574LCD::LCDClearLine(LCDLineNumber_e lineNo) {

	switch (lineNo) {
		case LCDLineNumberOne:LCDSendCmd(LCDLineAddressOne); break;
		case LCDLineNumberTwo:LCDSendCmd(LCDLineAddressTwo); break;
		case LCDLineNumberThree:
			switch (_NumColsLCD)
			{
				case 16: LCDSendCmd(LCDLineAddress3Col16); break;
				case 20: LCDSendCmd(LCDLineAddress3Col20); break;
			}
		break;
		case LCDLineNumberFour:
			switch (_NumColsLCD)
			{
				case 16: LCDSendCmd(LCDLineAddress4Col16); break;
				case 20: LCDSendCmd(LCDLineAddress4Col20); break;
			}
		break;
	}

	for (uint8_t i = 0; i < _NumColsLCD; i++) {
		LCDSendData(' ');
	}
}

/*!
	@brief  Clear screen by writing spaces to every position
	@note : See also LCDClearScreenCmd for software command clear alternative.
*/
void HD44780PCF8574LCD::LCDClearScreen(void) {
	if (_NumRowsLCD < 1 || _NumRowsLCD >4)
	{
		if (_DebugON == true)
		{
			std::cout << "Error 603 : Number of rows invalid, must be:" << _NumRowsLCD << std::endl;
			return;
		}
	}

	LCDClearLine(LCDLineNumberOne);
	
	if (_NumRowsLCD >= 2)
		LCDClearLine(LCDLineNumberTwo);
	if (_NumRowsLCD >= 3)
		LCDClearLine(LCDLineNumberThree);
	if (_NumRowsLCD == 4)
		LCDClearLine(LCDLineNumberFour);
}


/*!
	@brief  Reset screen
	@param CursorType LCDCursorType_e enum cursor type, 4 choices
*/
void HD44780PCF8574LCD::LCDResetScreen(LCDCursorType_e CursorType) {
	LCDSendCmd(LCDCmdModeFourBit);
	LCDSendCmd(LCDCmdDisplayOn);
	LCDSendCmd(CursorType);
	LCDSendCmd(LCDCmdClearScreen);
	LCDSendCmd(LCDEntryModeThree);
	delayMilliSecRDL(5);
}

/*!
	@brief  Turn Screen on and off
	@param OnOff  True = display on , false = display off
*/
void HD44780PCF8574LCD::LCDDisplayON(bool OnOff) {
	OnOff ? LCDSendCmd(LCDCmdDisplayOn) : LCDSendCmd(LCDCmdDisplayOff);
	delayMilliSecRDL(5);
}


/*!
	@brief  Initialise LCD
	@param CursorType  The cursor type 4 choices.
*/
void HD44780PCF8574LCD::LCDInit(LCDCursorType_e CursorType) {

	delayMilliSecRDL(15);
	LCDSendCmd(LCDCmdHomePosition);
	delayMilliSecRDL(5);
	LCDSendCmd(LCDCmdHomePosition);
	delayMilliSecRDL(5);
	LCDSendCmd(LCDCmdHomePosition);
	delayMilliSecRDL(5);
	LCDSendCmd(LCDCmdModeFourBit);
	LCDSendCmd(LCDCmdDisplayOn);
	LCDSendCmd(CursorType);
	LCDSendCmd(LCDEntryModeThree);
	LCDSendCmd(LCDCmdClearScreen);
	delayMilliSecRDL(5);
}

/*!
	@brief  Send a string to LCD
	@param str  Pointer to the char array
*/
void HD44780PCF8574LCD::LCDSendString(char *str) {
	while (*str) LCDSendData(*str++);
}


/*!
	@brief  Sends a character to screen , simply wraps SendData command.
	@param data Character to display
*/
void HD44780PCF8574LCD::LCDSendChar(char data) {
	LCDSendData(data);
}


/*!
	@brief  Moves cursor
	@param direction enum LCDDirectionType_e left or right
	@param moveSize number of spaces to move
*/
void HD44780PCF8574LCD::LCDMoveCursor(LCDDirectionType_e direction, uint8_t moveSize) {
	uint8_t i = 0;
	const uint8_t LCDMoveCursorLeft = 0x10;  //Command Byte Code:  Move cursor one character left 
	const uint8_t LCDMoveCursorRight = 0x14;  // Command Byte Code : Move cursor one character right 
	switch(direction)
	{
	case LCDMoveRight:
		for (i = 0; i < moveSize; i++) {
			LCDSendCmd(LCDMoveCursorRight);
		}
	break;
	case LCDMoveLeft:
		for (i = 0; i < moveSize; i++) {
			LCDSendCmd(LCDMoveCursorLeft);
		}
	break;
	}
}

/*!
	@brief  Scrolls screen
	@param direction  left or right
	@param ScrollSize number of spaces to scroll
*/
void HD44780PCF8574LCD::LCDScroll(LCDDirectionType_e direction, uint8_t ScrollSize) {
	uint8_t i = 0;

	const uint8_t LCDScrollRight = 0x1E;  // Command Byte Code: Scroll display one character right (all lines) 
	const uint8_t LCDScrollLeft = 0x18;  //Command Byte Code: Scroll display one character left (all lines) 
	
	switch(direction)
	{
	case LCDMoveRight:
		for (i = 0; i < ScrollSize; i++) {
			LCDSendCmd(LCDScrollRight);
		}
	break;
	case LCDMoveLeft:
		for (i = 0; i < ScrollSize; i++) {
			LCDSendCmd(LCDScrollLeft);
		}
	break;
	}
}

/*!
	@brief  moves cursor to an x , y position on display.
	@param  line  x row 1-4
	@param col y column  0-15 or 0-19
*/
void HD44780PCF8574LCD::LCDGOTO(LCDLineNumber_e line, uint8_t col) {

	switch (line) {
		case LCDLineNumberOne: LCDSendCmd(LCDLineAddressOne| col); break;
		case LCDLineNumberTwo: LCDSendCmd(LCDLineAddressTwo | col); break;
		case LCDLineNumberThree:
			switch (_NumColsLCD)
			{
				case 16: LCDSendCmd(LCDLineAddress3Col16 | col); break;
				case 20: LCDSendCmd(LCDLineAddress3Col20 + col); break;
			}
		break;
		case LCDLineNumberFour:
			switch (_NumColsLCD)
			{
				case 16: LCDSendCmd(LCDLineAddress4Col16 | col); break;
				case 20: LCDSendCmd(LCDLineAddress4Col20 + col); break;
			}
		break;
	}
}

/*!
	@brief  Saves a custom character to a location in character generator RAM 64 bytes.
	@param location CG_RAM location 0-7, we only have 8 locations 64 bytes
	@param charmap An array of 8 bytes representing a custom character data
*/
void HD44780PCF8574LCD::LCDCreateCustomChar(uint8_t location, uint8_t * charmap)
{

	const uint8_t LCD_CG_RAM = 0x40;  //  character-generator RAM (CG RAM address) 
	 if (location >= 8) {return;}
	 
	LCDSendCmd(LCD_CG_RAM | (location<<3));
	for (uint8_t i=0; i<8; i++) {
		LCDSendData(charmap[i]);
	}
}

/*!
	@brief  Turn LED backlight on and off
	@param OnOff passed bool True = LED on , false = display LED off
	@note another data or command must be issued before it takes effect.
*/
void HD44780PCF8574LCD::LCDBackLightSet(bool OnOff)
{
	 OnOff ? (_LCDBackLight= LCDBackLightOnMask) : (_LCDBackLight= LCDBackLightOffMask);
}

/*!
	@brief  get the backlight flag status
	@return the status of backlight on or off , true or false.
*/

bool HD44780PCF8574LCD::LCDBackLightGet(void)
{
	switch(_LCDBackLight){
		case LCDBackLightOnMask : return true; break;
		case LCDBackLightOffMask: return false; break;
		default : return true ; break ;
	}
}

/*!
	@brief Switch on the I2C
	@note Start I2C operations. Forces RPi I2C pins P1-03 (SDA) and P1-05 (SCL)
		to alternate function ALT0, which enables those pins for I2C interface.
	@return error for failure to switch on I2C(user not running as root?)
*/
rpiDisplay_Return_Codes_e HD44780PCF8574LCD::LCD_I2C_ON(void)
{
	if (!bcm2835_i2c_begin())
	{
		return rpiDisplay_I2CbeginFail;
	}
	else
	{
		LCD_I2C_SetSpeed();
		return rpiDisplay_Success;
	}
}

/*!
	@brief Switch on the I2C speed Settings
	@note turns on I2C settings  + speed
*/
void HD44780PCF8574LCD::LCD_I2C_SetSpeed()
{
		uint32_t I2CBaudRate = 100000;// 100K 
		// BCM2835_I2C_CLOCK_DIVIDER enum choice 2500 626 150 148
		// Clock divided is based on nominal base clock rate of 250MHz
		switch(_LCDSpeedI2C) 
		{
			case 0:
				// default or use set_baudrate instead of clockdivder 100k if zero passed
				bcm2835_i2c_set_baudrate(I2CBaudRate); 
			break;
			case BCM2835_I2C_CLOCK_DIVIDER_2500:// ~100K
			case BCM2835_I2C_CLOCK_DIVIDER_626: // ~400k
			case BCM2835_I2C_CLOCK_DIVIDER_150:
			case BCM2835_I2C_CLOCK_DIVIDER_148:
				bcm2835_i2c_setClockDivider(_LCDSpeedI2C);
			break;
			default:
				// error message 
				if (_DebugON == true)
				{
					std::cout << "Warning 610: Invalid BCM2835_I2C_CLOCK_DIVIDER value : " << _LCDSpeedI2C<< std::endl;
					std::cout << "	Must be 2500 626 150 or 148 " <<  std::endl;
					std::cout << "	Setting I2C baudrate to 100K with bcm2835_i2c_set_baudrate: " <<  std::endl;
				}
				bcm2835_i2c_set_baudrate(I2CBaudRate); 
			break; 
		}
}


/*!
	@brief End I2C operations
	@note I2C pins P1-03 (SDA) & P1-05 (SCL) returned to default INPUT behaviour.
*/
void HD44780PCF8574LCD::LCD_I2C_OFF(void){bcm2835_i2c_end();}

/*!
	@brief Print out a customer character from character generator CGRAM 64 bytes 8 characters
	@param location CGRAM  0-7
*/
void HD44780PCF8574LCD::LCDPrintCustomChar(uint8_t location)
{
	if (location >= 8) {return;}
	LCDSendData(location);
}

/*!
	@brief  Called by print class, used to print out numerical data types etc
	@param character write a character 
	@note used internally. Called by the print method using virtual   
*/
size_t HD44780PCF8574LCD::write(uint8_t character)
{
	LCDSendChar(character) ;
	return 1;
}

/*!
	@brief Clear display using software command , set cursor position to zero
	@note  See also LCDClearScreen for manual clear
*/
void HD44780PCF8574LCD::LCDClearScreenCmd(void) {
	LCDSendCmd(LCDCmdClearScreen);
	delayMilliSecRDL(3); // Requires a delay
}

/*!
	@brief Set cursor position to home position .
*/
void HD44780PCF8574LCD::LCDHome(void) {
	LCDSendCmd(LCDCmdHomePosition);
	delayMilliSecRDL(3); // Requires a delay
}

/*!
	@brief Change entry mode  
	@param newEntryMode  1-4 , 4 choices.
*/
void HD44780PCF8574LCD::LCDChangeEntryMode(LCDEntryMode_e newEntryMode)
{
	LCDSendCmd(newEntryMode);
	delayMilliSecRDL(3); // Requires a delay
}

/*!
	 @brief Turn DEBUG mode on or off setter
	 @param OnOff passed bool True = debug on , false = debug off
	 @note prints out statements, if ON and if errors occur
*/
void HD44780PCF8574LCD::LCDDebugSet(bool OnOff)
{
	 OnOff ? (_DebugON  = true) : (_DebugON  = false);
}

/*!
	 @brief get DEBUG mode status
	 @return debug mode status flag
*/
bool HD44780PCF8574LCD::LCDDebugGet(void) { return _DebugON;}


/*!
	@brief get I2C error Flag
	@details bcm2835I2Creasoncode.
		-# BCM2835_I2C_REASON_OK   	     = 0x00,Success 
		-# BCM2835_I2C_REASON_ERROR_NACK    = 0x01,Received a NACK 
		-# BCM2835_I2C_REASON_ERROR_CLKT    = 0x02,Received Clock Stretch Timeout 
		-# BCM2835_I2C_REASON_ERROR_DATA    = 0x04, Not all data is sent / receive
		-# BCM2835_I2C_REASON_ERROR_TIMEOUT = 0x08 Time out occurred during sending 
	 @return I2C error flag = 0x00 no error , > 0 bcm2835I2Creasoncode.
*/
uint8_t HD44780PCF8574LCD::LCDI2CErrorGet(void) { return _I2C_ErrorFlag;}

/*!
	 @brief Sets the I2C timeout, in the event of an I2C write error
	 @details Delay between retry attempts in event of an error , mS
	 @param newTimeout I2C timeout delay in mS
*/
void HD44780PCF8574LCD::LCDI2CErrorTimeoutSet(uint16_t newTimeout)
{
	_I2C_ErrorDelay = newTimeout;
}

/*!
	 @brief Gets the I2C timeout, used in the event of an I2C write error
	 @details Delay between retry attempts in event of an error , mS
	 @return  I2C timeout delay in mS, _I2C_ErrorDelay
*/
uint16_t HD44780PCF8574LCD::LCDI2CErrorTimeoutGet(void){return _I2C_ErrorDelay;}

/*!
	 @brief Gets the I2C error retry attempts, used in the event of an I2C write error
	 @details Number of times to retry in event of an error
	 @return   _I2C_ErrorRetryNum
*/
uint8_t HD44780PCF8574LCD::LCDI2CErrorRetryNumGet(void){return _I2C_ErrorRetryNum;}

/*!
	 @brief Sets the I2C error retry attempts used in the event of an I2C write error
	 @details Number of times to retry in event of an error
	 @param AttemptCount I2C retry attempts 
*/
void HD44780PCF8574LCD::LCDI2CErrorRetryNumSet(uint8_t AttemptCount)
{
	_I2C_ErrorRetryNum = AttemptCount;
}


/*! 
	@brief checks if LCD on I2C bus
	@return bcm2835I2CReasonCodes , BCM2835_I2C_REASON_OK 0x00 = Success
*/ 
uint8_t HD44780PCF8574LCD::LCDCheckConnection(void)
{
	char rxdata[1]; //buffer to hold return byte
	
	bcm2835_i2c_setSlaveAddress(_LCDSlaveAddresI2C);  // set i2c address
	_I2C_ErrorFlag = bcm2835_i2c_read(rxdata, 1); // returns reason code , 0 success

	return _I2C_ErrorFlag;
}


// **** EOF ****
