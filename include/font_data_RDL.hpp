/*!
	@file font_data_RDL.hpp
	@brief font data file. Project Name: Display_Lib_RPI
	@author Gavin Lyons.
	@details
		-#  FontDefault  8 by 8
		-#  FontGll  8 by 8 (NO LOWERCASE letters)
		-#  FontSinclairS 8 by 8
		-#  FontPico 8 by 12
		-#  FontRetro 8x16
		-#  FontMega  16 by 16
		-#  FontArialBold 16 by 16
		-#  FontHallfetica 16 by 16
		-#  FontArialRound 16 by 24
		-#  FontOrla 16x24
		-#  FontGroTesk 16 by 32
		-#  FontInconsola 24 by 32
		-#  FontMint 24 by 32
		-#  FontSixteenSeg 32 by 48 (NUMBERS ONLY + : . -)
		-#  FontSevenSeg 32 by 50 (NUMBERS ONLY + : . -)
		-#  FontGroTesk 32 by 64 (NO LOWERCASE letters)
*/

#pragma once

#include <cstdint>
#include <cstdio>
#include "common_data_RDL.hpp" //error enum

// Font data is in the cpp file accessed thru extern pointers.
extern const uint8_t * pFontDefault;
extern const uint8_t * pFontPico;
extern const uint8_t * pFontGll;
extern const uint8_t * pFontSinclairS;
extern const uint8_t * pFontRetro;
extern const uint8_t * pFontMega;
extern const uint8_t * pFontArialBold;
extern const uint8_t * pFontHallfetica;
extern const uint8_t * pFontOrla;
extern const uint8_t * pFontArialRound;
extern const uint8_t * pFontGroTesk;
extern const uint8_t * pFontInconsola; 
extern const uint8_t * pFontMint;
extern const uint8_t * pFontSixteenSeg;
extern const uint8_t * pFontSevenSeg;
extern const uint8_t * pFontGroTeskBig;


/*! Enum to define current font type selected  */
enum display_Font_name_e : uint8_t
{
	font_default = 1,     /**< default 8by8 */
	font_pico = 2,        /**< Pico 8by12 */
	font_gll = 3,         /**< gll 8x8 (NO LOWERCASE) */
	font_sinclairS = 4,   /**< Sinclair 8by8 */
	font_retro = 5,       /**< Retro 8x16 */
	font_mega = 6,        /**< Mega 16by16 */
	font_arialBold = 7,   /**< Arial bold 16by16 */
	font_hallfetica = 8,  /**< Hallfetica 16by16 */
	font_orla = 9,        /**< Orla 16by24 */
	font_arialRound = 10, /**< ArialRound 16by24 */
	font_groTesk = 11,    /**< GroTesk 16by32 */
	font_inconsola =12,   /**< Inconsola 24by32 */
	font_mint = 13,       /**< Mint 24by32 */
	font_sixteenSeg = 14, /**< Sixteen Segment 32by48 (NUMBERS ONLY + : . -) */
	font_sevenSeg = 15,   /**< Seven Segment 32by50 (NUMBERS ONLY + : . -) */
	font_groTeskBig = 16  /**< Grotesk big 32by64 (NO LOWERCASE) */
};

/*! @brief Font class to hold font data object  */
class display_Fonts
{
	public:
		display_Fonts();
		~display_Fonts(){};
		
		rpiDisplay_Return_Codes_e setFont(display_Font_name_e);
		void setInvertFont(bool invertStatus);
		bool getInvertFont(void);

	protected:
		const uint8_t *_FontSelect = pFontDefault; /**< Pointer to the active font, Fonts Stored are Const */
		uint8_t _Font_X_Size = 0x08; /**< Width Size of a Font character */
		uint8_t _Font_Y_Size = 0x08; /**< Height Size of a Font character */
		uint8_t _FontOffset = 0x20; /**< Offset in the ASCII table 0x00 to 0xFF, where font begins */
		uint8_t _FontNumChars = 0x5F; /**< Number of characters in font 0x00 to 0xFE */
	private:
		bool _FontInverted = false; /**< display font inverted? , False = no invert , true = invert*/
};


