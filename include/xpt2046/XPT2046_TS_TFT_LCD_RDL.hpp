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
#include <lgpio.h>
#include "common_data_RDL.hpp"

#define MAXTP 20 /**< Maximum Number of Touch point's to record */

// GPIO LEVELS
#define _XPT_RST_SetHigh  lgGpioWrite(_GpioHandle, _RESET_PIN , 1)
#define _XPT_RST_SetLow  lgGpioWrite(_GpioHandle, _RESET_PIN , 0)
#define _XPT_IRQ_Read lgGpioRead(_GpioHandle, _IRQ_PIN)
// GPIO Set IO
#define _XPT_RST_SetDigitalOutput lgGpioClaimOutput(_GpioHandle, 0,_RESET_PIN,  0);
#define _XPT_IRQ_SetDigitalInput lgGpioClaimInput(_GpioHandle, 0,_IRQ_PIN);
// GPIO open and close
#define _XPT_OPEN_GPIO_CHIP lgGpiochipOpen(_DeviceNumGpioChip)
#define _XPT_CLOSE_GPIO_HANDLE lgGpiochipClose(_GpioHandle)
// GPIO free modes
#define _XPT_GPIO_FREE_IRQ lgGpioFree(_GpioHandle , _IRQ_PIN)
#define _XPT_GPIO_FREE_RST lgGpioFree(_GpioHandle , _RESET_PIN )
// SPI 
#define _XPT_OPEN_SPI lgSpiOpen(_spiDev, _spiChan, _spiBaud, _spiFlags)
#define _XPT_CLOSE_SPI lgSpiClose(_spiHandle)

/*! 
	@brief Class to interface hardware of Touch point IC XPT2046 
*/
class XPT_2046_RDL
{

public:

	XPT_2046_RDL();
	~XPT_2046_RDL(){};
	
	// SPI functions
	rpiDisplay_Return_Codes_e XPTSPIInit(int device, int channel, int speed, int flags, int gpioDev, uint8_t IRQPin ,int8_t resPin);
	bool XPTIRQIsPressed();
	rpiDisplay_Return_Codes_e XPTSPIend(void);
	
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


	// SPI related 
	int _spiDev = 0;       /**< A SPI device, >= 0. */
	int _spiChan = 1;      /**< A SPI channel, >= 0. */
	int _spiBaud = 50000;  /**< The speed of serial communication in bits per second. */
	int _spiFlags = 0;     /**<The flags 2 LSB defines SPI mode */
	int _spiHandle = 0;    /**< Hold a handle for the SPI device on the channel.*/
	// GPIO
	int _DeviceNumGpioChip = 0;  /**< The device number of a gpiochip 4=rpi5 0=rpi4,3 /dev/gpio */
	int _GpioHandle = 0;         /** This holds a handle to a gpiochip device opened by lgGpiochipOpen  */
	uint8_t _RESET_PIN = 0;      /**< Reset pin for SPI */
	uint8_t _IRQ_PIN = 0;        /**< Interrupt request GPIO pin */
	bool _resetXPTPinOn = false; /**< Defines if reset pin is bring used */
};
