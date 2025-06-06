/*!
	@file  led_segment_font_data_RDL.cpp
	@brief font data file LED segment font used by ht16k33, tm1638, tm1637 & max7219 modules.
	@details ht16k33 uses all , the other modules just use 7-segment.
	@author Gavin Lyons.
*/

#include "../../include/led_segment_font_data_RDL.hpp"


/*!
 	@brief Font data table for ASCII values mapped to seven-segment representation.
 	 	Offset starts at ASCII value 32 (space).
 		Encoded in dp-gfedcba bit order.
 */
const uint8_t SevenSegmentFont::fontData[91] = {
	0x00, 0x86, 0x22, 0x7E, 0x6D, 0xD2, 0x46, 0x20, 0x29, 0x0B, /* space - ) */
	0x21, 0x70, 0x10, 0x40, 0x80, 0x52, 0x3F, 0x06, 0x5B, 0x4F, /* * - 3 */
	0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x09, 0x0D, 0x61, 0x48, /* 4 - = */
	0x43, 0xD3, 0x5F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, /* > - G */
	0x76, 0x30, 0x1E, 0x75, 0x38, 0x15, 0x37, 0x3F, 0x73, 0x6B, /* H - Q */
	0x33, 0x6D, 0x78, 0x3E, 0x3E, 0x2A, 0x76, 0x6E, 0x5B, 0x39, /* R - [ */
	0x64, 0x0F, 0x23, 0x08, 0x02, 0x5F, 0x7C, 0x58, 0x5E, 0x7B, /* \ - e */
	0x71, 0x6F, 0x74, 0x10, 0x0C, 0x75, 0x30, 0x14, 0x54, 0x5C, /* f - o */
	0x73, 0x67, 0x50, 0x6D, 0x78, 0x1C, 0x1C, 0x14, 0x76, 0x6E, /* p - y */
	0x5B                                                        /* z  */
};

/*!
	@brief Retrieves a pointer to the seven-segment font data table.
	@return Pointer to the font data array.
 */
const uint8_t *SevenSegmentFont::pFontSevenSegptr() 
{
	return fontData;
}

/*! 
 * @brief font , map of ASCII values/table to 9-segment, 
 * offset to position 32. dp-ihgfedcba order. 
 */
const uint16_t NineSegmentFont::fontData[91] = {
	0x000, 0x206, 0x022, 0x1FE, 0x0ED, 0x1A4, 0x1E9, 0x080, 0x188, 0x00F, /* space - ) */
	0x083, 0x1C0, 0x100, 0x040, 0x200, 0x1C0, 0x1BF, 0x086, 0x05B, 0x04F, /* * - 3 */
	0x066, 0x06D, 0x07D, 0x181, 0x07F, 0x06F, 0x084, 0x120, 0x0C0, 0x041, /* 4 - = */
	0x081, 0x091, 0x0BB, 0x077, 0x0FD, 0x039, 0x1B1, 0x079, 0x071, 0x03D, /* > - G */
	0x076, 0x006, 0x01E, 0x0F4, 0x038, 0x0B7, 0x037, 0x03F, 0x073, 0x13F, /* H - Q */
	0x0F5, 0x06D, 0x1A3, 0x03E, 0x1B0, 0x13E, 0x1E4, 0x162, 0x189, 0x039, /* R - [ */
	0x064, 0x00F, 0x082, 0x008, 0x003, 0x14C, 0x07C, 0x058, 0x05E, 0x158, /* backslash - e */
	0x0D0, 0x0CE, 0x074, 0x005, 0x00D, 0x178, 0x018, 0x154, 0x054, 0x05C, /* f - o */
	0x0F1, 0x067, 0x050, 0x0CC, 0x1C8, 0x01C, 0x110, 0x11C, 0x1C4, 0x06E, /* p - y */
	0x148                                                                 /* z */
};

/*!
	@brief Retrieves a pointer to the nine-segment font data table.
	@return Pointer to the font data array.
 */
const uint16_t *NineSegmentFont::pFontNineSegptr() 
{
	return fontData;
}

/*! 
 * @brief font , map of ASCII values/table to 14-segment, offset to position 32. 
 * dp-nmlkjh-g2-g1-fedcba order. 
 */
const uint16_t FourteenSegmentFont::fontData[91] = {
	0x0000, 0x4006, 0x0202, 0x12CE, 0x12ED, 0x3FE4, 0x2359, 0x0200, 0x2400, 0x0900, /* space - ) */
	0x3FC0, 0x12C0, 0x0800, 0x00C0, 0x4000, 0x0C00, 0x0C3F, 0x0406, 0x00DB, 0x008F, /* * - 3 */
	0x00E6, 0x00ED, 0x00FD, 0x0007, 0x00FF, 0x00EF, 0x1200, 0x0A00, 0x2440, 0x00C8, /* 4 - = */
	0x0980, 0x5083, 0x02BB, 0x00F7, 0x128F, 0x0039, 0x120F, 0x0079, 0x0071, 0x00BD, /* > - G */
	0x00F6, 0x1209, 0x001E, 0x2470, 0x0038, 0x0536, 0x2136, 0x003F, 0x00F3, 0x203F, /* H - Q */
	0x20F3, 0x00ED, 0x1201, 0x003E, 0x0C30, 0x2836, 0x2D00, 0x00EE, 0x0C09, 0x0039, /* R - [ */
	0x2100, 0x000F, 0x2800, 0x0008, 0x0100, 0x1058, 0x2078, 0x00D8, 0x088E, 0x0858, /* \ - e */
	0x14C0, 0x048E, 0x1070, 0x1000, 0x0A10, 0x3600, 0x0030, 0x10D4, 0x1050, 0x00DC, /* f - o */
	0x0170, 0x0486, 0x0050, 0x2088, 0x0078, 0x001C, 0x0810, 0x2814, 0x2D00, 0x028E, /* p - y */
	0x0848                                                                          /* z */
};

/*!
	@brief Retrieves a pointer to the fourteen-segment font data table.
	@return Pointer to the font data array.
 */
const uint16_t *FourteenSegmentFont::pFontFourteenSegptr() 
{
	return fontData;
}

/*! 
 * @brief font , map of ASCII values/table to 16-segment, 
 * offset to position 32. utsrpnmkhgfedcba order. 
 */
const uint16_t SixteenSegmentFont::fontData[91] = {
	0x0000, 0x000C, 0x0204, 0xAA3C, 0xAABB, 0xEE99, 0x9371, 0x0200, 0x1400, 0x4100, /* space - ) */
	0xFF00, 0xAA00, 0x4000, 0x8800, 0x1000, 0x4400, 0x44FF, 0x040C, 0x8877, 0x083F, /* * - 3 */
	0x888C, 0x90B3, 0x88FB, 0x000F, 0x88FF, 0x88BF, 0x2200, 0x4200, 0x9400, 0x8830, /* 4 - = */
	0x4900, 0x2807, 0x0AF7, 0x88CF, 0x2A3F, 0x00F3, 0x223F, 0x80F3, 0x80C3, 0x08FB, /* > - G */
	0x88CC, 0x2233, 0x007C, 0x94C0, 0x00F0, 0x05CC, 0x11CC, 0x00FF, 0x88C7, 0x10FF, /* H - Q */
	0x98C7, 0x88BB, 0x2203, 0x00FC, 0x44C0, 0x50CC, 0x5500, 0x88BC, 0x4433, 0x2212, /* R - [ */
	0x1100, 0x2221, 0x5000, 0x0030, 0x0100, 0xA070, 0xA0E0, 0x8060, 0x281C, 0xC060, /* \ - e */
	0xAA02, 0xA2A1, 0xA0C0, 0x2000, 0x2260, 0x3600, 0x00C0, 0xA848, 0xA040, 0xA060, /* f - o */
	0x82C1, 0xA281, 0x8040, 0xA0A1, 0x80E0, 0x2060, 0x4040, 0x5048, 0x5500, 0x0A1C, /* p - y */
	0xC020                                                                          /* z */                                                     /* z  */
};

/*!
	@brief Retrieves a pointer to the sixteen-segment font data table.
	@return Pointer to the font data array.
 */
const uint16_t *SixteenSegmentFont::pFontSixteenSegptr() 
{
	return fontData;
}



