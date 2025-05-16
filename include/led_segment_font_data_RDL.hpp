/*!
	@file led_segment_font_data_RDL.hpp
	@brief LED segment font data file for ht16k33 tm1638, tm1637 & max7219 modules
	@details ht16k33 uses all , the other modules just use 7-segment.
	@author Gavin Lyons.
*/

#pragma once

#include <cstdint>

/*!
	@brief SevenSegmentFont Class that provides access to a seven-segment font data table.
 */
class SevenSegmentFont {
protected:
	static const uint8_t* pFontSevenSegptr(); /**<  Pointer to seven segment font*/
private:
	static const uint8_t fontData[];  /**< Font data 7 Segment table */
};

/*!
	@brief NineSegmentFont Class that provides access to a nine-segment font data table.
 */
class NineSegmentFont {
protected:
	static const uint16_t* pFontNineSegptr(); /**<  Pointer to nine segment font*/

private:
	static const uint16_t fontData[]; /**< Font data 9 Segment table */
};

/*!
	@brief FourteenSegmentFont Class that provides access to a fourteen-segment font data table.
 */
class FourteenSegmentFont{
protected:
	static const uint16_t* pFontFourteenSegptr();  /**<  Pointer to Fourteen segment font*/
private:
	static const uint16_t fontData[];  /**< Font data 14 Segment table */
};

/*!
	@brief SixteenSegmentFont Class that provides access to a sixteen-segment font data table.
 */
class SixteenSegmentFont{
protected:
	static const uint16_t* pFontSixteenSegptr();  /**<  Pointer to Sixteen segment font*/
private:
	static const uint16_t fontData[];  /**< Font data 16 Segment table */
};

