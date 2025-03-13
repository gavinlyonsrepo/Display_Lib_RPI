/*!
	@file led_segment_font_data_RDL.hpp
	@brief LED segment font data file for ht16k33 tm1638, tm1637 & max7219 modules
	@details ht16k33 uses all , the other modules just use 7-segment.
	@author Gavin Lyons.
*/

#pragma once

#include <cstdint>

extern const uint8_t   * pFontSevenSegptr;       /**< Pointer 7  segment font data */
extern const uint16_t  * pFontNineSegptr;         /**< Pointer 9 segment font data */
extern const uint16_t  * pFontFourteenSegptr; /**< Pointer 14 segment font data */
extern const uint16_t  * pFontSixteenSegptr;   /**< Pointer 16 segment font data */
