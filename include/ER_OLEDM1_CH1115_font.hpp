/*
* Project Name: ER_OLEDM1_CH1115
* File: ER_OLEDM1_CH1115_font.h
* Description: ER_OLEDM1 OLED driven by CH1115 controller, font file 
* 8 fonts 
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI
*/

#ifndef _ER_OLEDM1_CH1115_font_H
#define _ER_OLEDM1_CH1115_font_H

#include <inttypes.h>

// (1) default  (FUll ASCII with mods)
// (2) thick (NO LOWERCASE)
// (3) seven segment 
// (4) wide (NO LOWERCASE)
// (5) tiny
// (6) homespun 
// (7) big numbers (NUMBERS ONLY )
// (8) med numbers (NUMBERS ONLY )

extern const unsigned char * pFontDefaultptr;
extern const unsigned char * pFontThickptr;
extern const unsigned char * pFontSevenSegptr;
extern const unsigned char * pFontWideptr;
extern const  unsigned char * pFontTinyptr;
extern const unsigned char * pFontHomeSpunptr;
extern const uint8_t (* pFontBigNumptr)[64];
extern const uint8_t (* pFontMedNumptr)[32]; 

#endif // guard header
