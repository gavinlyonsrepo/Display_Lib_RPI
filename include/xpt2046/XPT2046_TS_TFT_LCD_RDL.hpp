/*!
	@file     XPT2046_TS_TFT_LCD_RDL.hpp
	@author   Gavin Lyons
	@brief    Header file for ILI9341_TFT_LCD_RDL library.
			to manage hardware interface with XPT2046 Touch Screen Controller
*/


#include <sys/time.h> // gettimeofday
#include <cstdio> // for printf
#include <cstdint>
#include <cstdbool>
#include <cstring> // for memset
#include <ctime> // for localtime etc
#include <bcm2835.h>
#include "common_data_RDL.hpp"

#define MAXTP 20 /**< Maximum Number of Touch point's to record */


/*! 
	@brief Class to interface hardware of Touch point IC XPT2046 
*/
class XPT_2046_RDL
{

public:

	XPT_2046_RDL();
	~XPT_2046_RDL(){};
	
	// SPI functions
	rpiDisplay_Return_Codes_e XPTSPIInit(uint8_t SPICEX_pin,uint8_t IRQPin , bool begin ,int8_t resPin);
	bool XPTIRQIsPressed();
	void XPTSPIend();
	
	// Read functions
	int XPTReadSensor(int command);
	void XPTReadXY( int *xp, int *yp);
	
	// Calibration & debug Functions 
	void XPTInitValues();
	void XPTPrintValues();
	void XPTSetPoint(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t id);
	int XPTGetPoint();

	struct TouchPoint
	{
		uint32_t x1;
		uint32_t x2;
		uint32_t y1;
		uint32_t y2;
		uint32_t id;
	};

private:
	uint16_t tpc;
	uint16_t tpx;
	time_t lsec;
	suseconds_t lusec;
	TouchPoint tps[MAXTP];
	bool _calibration;
	int16_t _min_xp; /**< Minimum xp calibration */
	int16_t _min_yp; /**< Minimum yp calibration */
	int16_t _max_xp; /**< Maximum xp calibration */
	int16_t _max_yp; /**< Maximum yp calibration */
	int16_t _min_xc; /**< Minimum x coordinate */
	int16_t _min_yc; /**< Minimum y coordinate */
	int16_t _max_xc; /**< Maximum x coordinate */
	int16_t _max_yc; /**< Maximum y coordinate */

	uint8_t _TSSPICEX_pin = 1; /**< Chip select pin for SPI */
	uint8_t _IRQ_pin;          /**< Interrupt request GPIO pin */
};
