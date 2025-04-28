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
#include <ctime> // for localtime etc
#include <lgpio.h>
#include "common_data_RDL.hpp"

#pragma once 

/*! 
	@brief Class to interface hardware of Touch point IC XPT2046 
*/
class XPT_2046_RDL
{

public:

	XPT_2046_RDL();
	~XPT_2046_RDL(){};
	
	// SPI functions
	rdlib::Return_Codes_e XPTSPIInit(int device, int channel, int speed, int flags, int gpioDev, uint8_t IRQPin ,int8_t resPin);
	bool XPTIRQIsPressed();
	rdlib::Return_Codes_e XPTSPIend(void);
	
	// Read functions
	int XPTReadSensor(int command);
	void XPTReadXY( int *xp, int *yp);
	
	// Calibration & debug Functions 
	void XPTInitValues();
	void XPTPrintValues();
	void XPTSetPoint(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t id);
	int XPTGetPoint();
	/*!
	 * @brief Structure representing a touch point on the screen.
	 * This structure is used to define a touch area on the screen,
	 * x1` and `x2` represent the horizontal boundaries of the touch region.
	 * y1` and `y2` represent the vertical boundaries of the touch region.
	 * The `id` field is used to uniquely identify the touch point
	 */
	struct Touch_Point_t
	{
		uint32_t x1; /**< Left boundary of the touch region (x-coordinate) */
		uint32_t x2; /**< Right boundary of the touch region (x-coordinate) */
		uint32_t y1; /**< Top boundary of the touch region (y-coordinate) */
		uint32_t y2; /**< Bottom boundary of the touch region (y-coordinate) */
		uint32_t id; /**< Unique identifier for the touch point */
	};

	static constexpr auto MaxTouchPoints = 20; /**< Maximum Number of Touch point's to record */
	static constexpr uint8_t MAX_LEN_BUFFER = 3; /**< Maximum length of RX and TX buffer */
	static constexpr uint32_t DTMAX = 800000; /**< Delay 800mS */

private:
	// Control Register
	static constexpr uint8_t XPT_START = 0x80; /**< Start bit Control Register */
	static constexpr uint8_t XPT_XPOS  = 0x50; /**< A2-A0 Channel select bits for XPOS */
	static constexpr uint8_t XPT_YPOS  = 0x10; /**< A2-A0 Channel select bits for YPOS */
	static constexpr uint8_t XPT_8BIT  = 0x08; /**< Mode select 8 bit */
	static constexpr uint8_t XPT_SER   = 0x04; /**< Single-Ended Select bit */
	static constexpr uint8_t XPT_DEF   = 0x03; /**< Differential Reference Select bit */

	uint16_t tpc; /**< Number of touch points stored */
	uint16_t tpx; /**< Maximum number of touch points */
	time_t lsec; /**< Last recorded second for touch event timestamp */
	suseconds_t lusec; /**< Last recorded microsecond for touch event timestamp */
	Touch_Point_t tps[MaxTouchPoints]; /**< Array of touch points */
	bool _calibration; /**< Calibration flag */

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
	int _DeviceNumGpioChip = 0;  /**< The device number of a gpiochip ls /dev/gpio */
	int _GpioHandle = 0;         /**< This holds a handle to a gpiochip device opened by lgGpiochipOpen  */
	uint8_t _RESET_PIN = 0;      /**< Reset pin for SPI */
	uint8_t _IRQ_PIN = 0;        /**< Interrupt request GPIO pin */
	bool _resetXPTPinOn = false; /**< Defines if reset pin is bring used */
};
