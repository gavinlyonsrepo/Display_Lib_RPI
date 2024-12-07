/*!
	@file     XPT2046_TS_TFT_LCD_RDL.cpp
	@author   Gavin Lyons
	@brief   Source file for ILI9341_TFT_LCD_RDL library.
			to manage hardware interface with XPT2046 Touch Screen Controller
*/

#include "../../include/xpt2046/XPT2046_TS_TFT_LCD_RDL.hpp"

#define MAX_LEN_BUFFER 3 /**< Maximum lenght of RX and TX buffer */
#define _DEBUGXPT_ 0    /**< Switch on debug mode */
#define DTMAX 800000 /**< Delay 800mS */

// Control Register 
#define XPT_START 0x80 /**< Start bit Control Register*/
#define XPT_XPOS  0x50 /**< A2-A0 Channel select bits for XPOS */
#define XPT_YPOS  0x10 /**< A2-A0 Channel select bits for YPOS */
#define XPT_8BIT  0x08 /**< Mode select 8 bit */
#define XPT_SER   0x04 /**< Single-Ended Select bit. */ 
#define XPT_DEF   0x03 /**< Differential Reference Select bit */


// Constructors
/*! @brief Constructor for class XPT2046_RDL */

XPT_2046_RDL::XPT_2046_RDL(){}

// Functions 

/*! 
	@brief Initialise the SPI interface
	@param device A SPI device, >= 0. 
	@param channel A SPI channel, >= 0. 
	@param speed The speed of serial communication in bits per second. 
	@param flags The flags may be used to modify the default behaviour. Set to 0(mode 0) for this device.
	@param gpioDev The device number of a gpiochip. 4 for RPI5, 0 for RPI3
	@param IRQPin The GPIO to use for interrupt eg T_IRQ on ili9341 PCB 
	@param resPin GPIO for reset pin MOSI only needed if SPI not already on by TFT set to -1 normally
	@return Returns a rpiDisplay_Return_Codes_e 
		-# rpiDisplay_Success
		-# rpiDisplay_GpioChipDevice
		-# rpiDisplay_GpioChipClaim
		-# rpiDisplay_SPIOpenFailure
*/
rpiDisplay_Return_Codes_e XPT_2046_RDL::XPTSPIInit(int device, int channel, int speed, int flags, int gpioDev, uint8_t IRQPin , int8_t resPin)
{
	_DeviceNumGpioChip = gpioDev;
	_spiDev = device;
	_spiChan = channel;
	_spiBaud = speed;
	_spiFlags = flags;
	_IRQ_PIN = IRQPin;

	//  1. gpio Device open?
	_GpioHandle =_XPT_OPEN_GPIO_CHIP; // open /dev/gpiochipX
	if ( _GpioHandle < 0)	// open error
	{
		fprintf(stderr,"Error : Failed to open lgGpioChipOpen : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		return rpiDisplay_GpioChipDevice;
	}
	
	// 2. Setup Reset pin if accessible 
	if (resPin != -1)
	{
		_resetXPTPinOn = true;
		// Claim GPIO as outputs for RST line
		int GpioResetErrorStatus = 0;
		GpioResetErrorStatus= _XPT_RST_SetDigitalOutput;
		if (GpioResetErrorStatus < 0 )
		{
			fprintf(stderr,"Error : Can't claim Reset GPIO for output (%s)\n", lguErrorText(GpioResetErrorStatus));
			return rpiDisplay_GpioPinClaim;
		}
		_XPT_RST_SetLow;
		delayMilliSecRDL(100);
		_XPT_RST_SetHigh;
		delayMilliSecRDL(100);
	}
	// 3. Setup IRQ pin as input.
	int GpioIRQErrorStatus = 0;
	GpioIRQErrorStatus  = _XPT_IRQ_SetDigitalInput;
	if (GpioIRQErrorStatus < 0 )
	{
		fprintf(stderr,"Error : Can't claim IRQ GPIO for input (%s)\n", lguErrorText(GpioIRQErrorStatus));
		return rpiDisplay_GpioPinClaim;
	}

	// 4 Open SPI device
	_spiHandle =_XPT_OPEN_SPI;
	if ( _spiHandle  < 0)
	{
		fprintf(stderr, "Error : Cannot open SPI :(%s)\n", lguErrorText( _spiHandle ));
		return rpiDisplay_SPIOpenFailure;
	}
	return rpiDisplay_Success;
}

/*! 
	@brief Stop the SPI interface
	@return Returns a rpiDisplay_Return_Codes_e 
		-# rpiDisplay_Success
		-# rpiDisplay_GpioPinFree
		-# rpiDisplay_SPICloseFailure
		-# rpiDisplay_GpioChipDevice
*/
rpiDisplay_Return_Codes_e   XPT_2046_RDL::XPTSPIend()
{
	uint8_t ErrorFlag = 0; // Becomes > 0 in event of error
	// 1. free reset & IRQ GPIO lines
	if (_resetXPTPinOn == true)
	{
		int GpioResetErrorStatus = 0;
		_XPT_RST_SetLow;
		GpioResetErrorStatus =_XPT_GPIO_FREE_RST;
		if (GpioResetErrorStatus < 0 )
		{
			fprintf(stderr,"Error :: Can't Free RST GPIO (%s)\n", lguErrorText(GpioResetErrorStatus));
			ErrorFlag = 2;
		}
	}

	int GpioIRQErrorStatus = 0;
	GpioIRQErrorStatus  = _XPT_GPIO_FREE_IRQ;
	if (GpioIRQErrorStatus  < 0 )
	{
		fprintf(stderr,"Error :: Can't Free IRQ GPIO (%s)\n", lguErrorText(GpioIRQErrorStatus));
		ErrorFlag = 2;
	}

	// 2 hardware SPi Closes a SPI device 
	int spiErrorStatus = 0;
	spiErrorStatus = _XPT_CLOSE_SPI;
	if (spiErrorStatus <0) 
	{
		fprintf(stderr, "Error : Cannot Close SPI device :(%s)\n", lguErrorText(spiErrorStatus));
		ErrorFlag = 3;
	}

	// 3 Closes the opened gpiochip device.
	int GpioCloseStatus = 0;
	GpioCloseStatus = _XPT_CLOSE_GPIO_HANDLE; // close gpiochip
	if ( GpioCloseStatus < 0)
	{
		// open error
		fprintf(stderr,"Error :: Failed to close lgGpioChipclose error : %d (%s)\n", _DeviceNumGpioChip, lguErrorText(_GpioHandle));
		ErrorFlag = 4;
	}

	// 4 Check error flag (we don't want to return early for any failure)
	switch (ErrorFlag)
	{
		case 0:return rpiDisplay_Success;break;
		case 2:return rpiDisplay_GpioPinFree;break;
		case 3:return rpiDisplay_SPICloseFailure;;break;
		case 4:return rpiDisplay_GpioChipDevice;;break;
		default:printf("Warning::Unknown error flag value in SPIEnd"); break;
	}
	return rpiDisplay_Success;
}

/*! 
	@brief Poll the IRQ pin
	@return true for touchpen down false for pen up
*/
bool XPT_2046_RDL::XPTIRQIsPressed()
{
	int level=0;
	level =_XPT_IRQ_Read;
	if (level < 0)
	{
		fprintf(stderr,"Error :: Can't Read IRQ GPIO (%s)\n", lguErrorText(level));
	}
	if (level == 0)
		return true; // pressed
	else if (level == 1)
		return false; //not pressed

	return false;
}

/*! 
	@brief Read the touch screen sensor Data
	@return The readbuffer data
*/
int XPT_2046_RDL::XPTReadSensor(int Command){
	char ReadBuffer[MAX_LEN_BUFFER];
	char WriteBuffer[MAX_LEN_BUFFER];
	int spiErrorStatus = 0;
	memset(WriteBuffer, 0, sizeof(ReadBuffer));
	memset(ReadBuffer, 0, sizeof(ReadBuffer));
	WriteBuffer[0] = Command;
	spiErrorStatus = lgSpiXfer(_spiHandle, (const char *)WriteBuffer,(char *)ReadBuffer, sizeof(WriteBuffer));
	if (spiErrorStatus <0 ) 
	{
		fprintf(stderr, "Error : spiWriteDataBuffer 1: Failure to Write SPI :(%s)\n", lguErrorText(spiErrorStatus));
	}
if( _DEBUGXPT_)printf("ReadBuffer[0]=%02x ReadBuffer[1]=%02x ReadBuffer[2]=%02x\n", ReadBuffer[0], ReadBuffer[1], ReadBuffer[2]);
	return((ReadBuffer[1]<<4) + (ReadBuffer[2]>>4));
}

/*! 
	@brief Get the  X Y position data
	@param xp the X position 
	@param yp the Y position 
*/
void XPT_2046_RDL::XPTReadXY(int *xp, int *yp){
	*xp = XPTReadSensor(XPT_START | XPT_XPOS | XPT_SER);
	*yp = XPTReadSensor(XPT_START | XPT_YPOS | XPT_SER);

}


void XPT_2046_RDL::XPTInitValues(){
	tpc = 0;
	tpx = MAXTP;
	lsec = 0;
	lusec = 0;
}

void XPT_2046_RDL::XPTPrintValues(){
	int i;
	if (_DEBUGXPT_ == 0) return;
	printf("tpc=%d\n",tpc);
	printf("tpx=%d\n",tpx);
	for(i=0;i<tpc;i++) {
		printf("x1[%02d]=%d",i,tps[i].x1);
		printf(" x2[%02d]=%d",i,tps[i].x2);
		printf(" y1[%02d]=%d",i,tps[i].y1);
		printf(" y2[%02d]=%d",i,tps[i].y2);
		printf(" id[%02d]=%d\n",i,tps[i].id);
	}
}

void XPT_2046_RDL::XPTSetPoint(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t id){
	if (tpc == tpx) return;
	int index = tpc;
	if (x1 > x2) {
		tps[index].x1 = x2;
		tps[index].x2 = x1;
	} else {
		tps[index].x1 = x1;
		tps[index].x2 = x2;
	}
	if (y1 > y2) {
		tps[index].y1 = y2;
		tps[index].y2 = y1;
	} else {
		tps[index].y1 = y1;
		tps[index].y2 = y2;
	}
	tps[index].id = id;
	tpc++;
}


int XPT_2046_RDL::XPTGetPoint() {
	int i;
	struct timeval myTime;
	suseconds_t dt;

	float _xd = _max_xp - _min_xp;
	float _yd = _max_yp - _min_yp;
	float _xs = _max_xc - _min_xc;
	float _ys = _max_yc - _min_yc;

	int _xp;
	int _yp;
	XPTReadXY(&_xp, &_yp);
if(_DEBUGXPT_)printf("touch : _xp=%5d  _min_xp=%5d _max_xp=%5d\n", _xp, _min_xp, _max_xp);
if(_DEBUGXPT_)printf("touch : _yp=%5d  _min_yp=%5d _max_yp=%5d\n", _yp, _min_yp, _max_yp);
	if (_xp < _min_xp && _xp > _max_xp) return -1;
	if (_yp < _min_yp && _yp > _max_yp) return -1;
	uint32_t _xpos = ( (float)(_xp - _min_xp) / _xd * _xs ) + _min_xc;
	uint32_t _ypos = ( (float)(_yp - _min_yp) / _yd * _ys ) + _min_yc;

	// Disable double touch
	gettimeofday(&myTime, NULL);
	localtime(&myTime.tv_sec);
	if (myTime.tv_sec == lsec) {
		dt = myTime.tv_usec - lusec;
		if (dt < DTMAX) return -1;
	}

	// Check range
	for(i=0;i<tpc;i++) {
		if( _DEBUGXPT_) {
		printf("x1[%02d]=%d",i,tps[i].x1);
		printf(" x2[%02d]=%d",i,tps[i].x2);
		printf(" y1[%02d]=%d",i,tps[i].y1);
		printf(" y2[%02d]=%d",i,tps[i].y2);
		printf(" id[%02d]=%d\n",i,tps[i].id);
		}
		if (_xpos > tps[i].x1 && _xpos < tps[i].x2) {
			if (_ypos > tps[i].y1 && _ypos < tps[i].y2) {
				gettimeofday(&myTime, NULL);
				localtime(&myTime.tv_sec);
				lsec = myTime.tv_sec;
				lusec = myTime.tv_usec;
				return tps[i].id;
			}
		}
	}
	return -1;
}

