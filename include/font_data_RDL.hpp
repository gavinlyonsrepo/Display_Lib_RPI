/*!
	@file font_data_RDL.hpp
	@brief font data file. Project Name: Display_Lib_RPI
	@author Gavin Lyons.
	@details
		-#  1. pFontDefault  6 by 8
		-#  2. pFontWide  9 by 8 (NO LOWERCASE letters)
		-#  3. pFontPico 3 by 6
		-#  4. pFontSinclairS 8 by 8
		-#  5. pFontOrla 8x12
		-#  6. pFontRetro 8x16
		-#  7. pFontMega  16 by 16
		-#  8. pFontArialBold 16 by 16
		-#  9. pFontHallfetica 16 by 16
		-#  10. pFontArialRound 16 by 24
		-#  11. pFontGroTesk 16 by 32
		-#  12. pFontSixteenSeg 32 by 48 (NUMBERS ONLY + : . -)
*/

#pragma once

#include <cstdint>
#include <cstdio>
#include "common_data_RDL.hpp" //error enum

// Font data is in the cpp file accessed thru extern pointers.
extern const uint8_t * pFontDefault;
extern const uint8_t * pFontWide;
extern const uint8_t * pFontPico;
extern const uint8_t * pFontSinclairS;
extern const uint8_t * pFontRetro;
extern const uint8_t * pFontMega;
extern const uint8_t * pFontArialBold;
extern const uint8_t * pFontHallfetica;
extern const uint8_t * pFontOrla;
extern const uint8_t * pFontArialRound;
extern const uint8_t * pFontGroTesk;
extern const uint8_t * pFontSixteenSeg;


/*! Enum to define current font type selected  */
enum display_Font_name_e : uint8_t
{
	font_default = 1,     /**< default 6by8  (FUll ASCII with mods) */
	font_wide = 2,        /**< wide 9by8 (NO LOWERCASE) */
	font_pico = 3,        /**< Pico 3by6 */
	font_sinclairS = 4,   /**< Sinclair 8by8 */
	font_retro = 5,       /**< Retro 8x16 */
	font_mega = 6,        /**< Mega 16by16 */
	font_arialBold = 7,   /**< Arial bold 16by16 */
	font_hallfetica = 8,  /**< Hallfetica 16by16 */
	font_orla = 9,        /**< Orla 16by24 */
	font_arialRound = 10, /**< ArialRound 16by24 */
	font_groTesk = 11,    /**< GroTesk 16by32 */
	font_sixteenSeg = 12  /**< pFontSixteenSeg 32by48 (NUMBERS ONLY + : . -) */
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
		const uint8_t *_FontSelect = pFontDefault; /**< Pointer to the active font,  Fonts Stored are Const */
		uint8_t _Font_X_Size = 0x06; /**< Width Size of a Font character */
		uint8_t _Font_Y_Size = 0x08; /**< Height Size of a Font character */
		uint8_t _FontOffset = 0x00; /**< Offset in the ASCII table 0x00 to 0xFF, where font begins */
		uint8_t _FontNumChars = 0xFE; /**< Number of characters in font 0x00 to 0xFE */
	private:
		bool _FontInverted = false; /**< Is the font inverted , False = normal , true = inverted*/
};


