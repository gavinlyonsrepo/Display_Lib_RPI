/*!
	@file     TM1638plus_Model3_RDL.hpp
	@author   Gavin Lyons
	@brief    RPI library Tm1638plus, Source file for for TM1638 module(LED & KEY). Model 3
*/


#pragma once

#include "TM1638plus_Model1_RDL.hpp"

/*!
	@brief Class for Model 3
*/
class TM1638plus_Model3 : public TM1638plus_Model1  {

public:

	/*! Tm1638 bi-color LED colors */
	enum TMLEDColors : uint8_t
	{
		TM_RED_LED = 0x02,   /**< Turn on Red LED*/
		TM_GREEN_LED = 0x01, /**< Turn on Green LED*/
		TM_OFF_LED = 0x00    /**< Turn off  LED*/
	};

	// Constructor 
	TM1638plus_Model3 (uint8_t strobe, uint8_t clock, uint8_t data, int gpioDev) ;
	
	// These methods over-ride the super class.
	virtual void setLEDs(uint16_t greenred) override;
	virtual rpiDisplay_Return_Codes_e setLED(uint8_t position, uint8_t value) override;

};

