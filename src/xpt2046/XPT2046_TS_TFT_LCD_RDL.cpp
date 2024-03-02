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
	@param SPICEXPin The chip select pin to use 0 or 1
	@param IRQPin The GPIO to use for interrupt eg T_IRQ on ili9341 PCB 
	@param begin issue spibegin command true or false only needed if SPI not already on by TFT
	@param resPin GPIO for reset pin MOSI only needed if SPI not already on by TFT set to -1 normally
	@return rpiDisplay_Return_Codes_e Will return error only if begin is false and spiBegin falls 
*/
rpiDisplay_Return_Codes_e XPT_2046_RDL::XPTSPIInit(uint8_t SPICEXPin, uint8_t IRQPin , bool begin , int8_t resPin)
{
	if (resPin != -1)
	{
		bcm2835_gpio_fsel(resPin,BCM2835_GPIO_FSEL_OUTP); // RES
		bcm2835_gpio_write(resPin, LOW);
		bcm2835_delay(100);
		bcm2835_gpio_write(resPin, HIGH);
		bcm2835_delay(100);
	}
	_TSSPICEX_pin = SPICEXPin;
	_IRQ_pin = IRQPin;
	bcm2835_gpio_fsel(_IRQ_pin,BCM2835_GPIO_FSEL_INPT);
	if (begin == true)
	{
		if (!bcm2835_spi_begin())
		{
			return rpiDisplay_SPIbeginFail;
		}
	}
	return rpiDisplay_Success;
}

/*! 
	@brief Stop the SPI interface
*/
void  XPT_2046_RDL::XPTSPIend(){bcm2835_spi_end();}

/*! 
	@brief Poll the IRQ pin
	@return true for touchpen down false for pen up
*/
bool XPT_2046_RDL::XPTIRQIsPressed()
{
	uint8_t level=0;
	level = bcm2835_gpio_lev(_IRQ_pin);
	if (level == LOW)
		return true; // pressed
	else if (level == HIGH)
		return false; //not pressed

	return false;
}

/*! 
	@brief Read the touch screen sensor Data
	@return The readbuffer
*/
int XPT_2046_RDL::XPTReadSensor(int Command){
	char ReadBuffer[MAX_LEN_BUFFER];
	char WriteBuffer[MAX_LEN_BUFFER];

	memset(WriteBuffer, 0, sizeof(ReadBuffer));
	memset(ReadBuffer, 0, sizeof(ReadBuffer));
	WriteBuffer[0] = Command;
	bcm2835_spi_transfernb(WriteBuffer, ReadBuffer, sizeof(WriteBuffer));
if( _DEBUGXPT_)printf("ReadBuffer[0]=%02x ReadBuffer[1]=%02x ReadBuffer[2]=%02x\n", ReadBuffer[0], ReadBuffer[1], ReadBuffer[2]);
	return((ReadBuffer[1]<<4) + (ReadBuffer[2]>>4));
}

/*! 
	@brief Get the  X Y position data
	@param xp the X position 
	@param yp the Y position 
*/
void XPT_2046_RDL::XPTReadXY(int *xp, int *yp){

	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
	if (_TSSPICEX_pin == 0)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	}else if (_TSSPICEX_pin == 1)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
	}

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

