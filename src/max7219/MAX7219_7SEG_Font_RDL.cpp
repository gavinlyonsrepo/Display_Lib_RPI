/*!
	@file MAX7219_7SEG_Font_RDL.cpp
	@author Gavin Lyons
	@brief Font data file to store the ASCII font used in project
*/


#include "../../include/max7219/MAX7219_7SEG_Font_RDL.hpp"

/*! ASCII Font Data Table ASCII to Seven Segment dp-abcdefg */
static const uint8_t SevenSegASCIIFont[91] = {
    0x00,    /*     */
    0x30,    /*  !  */
    0x22,    /*  "  */
    0x3f,    /*  #  */
    0x5b,    /*  $  */
    0x25,    /*  %  */
    0x31,    /*  &  */
    0x02,    /*  '  */
    0x4a,    /*  (  */
    0x68,    /*  )  */
    0x42,    /*  *  */
    0x07,    /*  +  */
    0x04,    /*  ,  */
    0x01,    /*  -  */
    0x80,    /*  .  */
    0x25,    /*  /  */
    0x7e,    /*  0  */
    0x30,    /*  1  */
    0x6d,    /*  2  */
    0x79,    /*  3  */
    0x33,    /*  4  */
    0x5b,    /*  5  */
    0x5f,    /*  6  */
    0x70,    /*  7  */
    0x7f,    /*  8  */
    0x7b,    /*  9  */
    0x48,    /*  :  */
    0x58,    /*  ;  */
    0x43,    /*  <  */
    0x09,    /*  =  */
    0x61,    /*  >  */
    0x65,    /*  ?  */
    0x7d,    /*  @  */
    0x77,    /*  A  */
    0x1f,    /*  B  */
    0x4e,    /*  C  */
    0x3d,    /*  D  */
    0x4f,    /*  E  */
    0x47,    /*  F  */
    0x5e,    /*  G  */
    0x37,    /*  H  */
    0x06,    /*  I  */
    0x3c,    /*  J  */
    0x57,    /*  K  */
    0x0e,    /*  L  */
    0x54,    /*  M  */
    0x76,    /*  N  */
    0x7e,    /*  O  */
    0x67,    /*  P  */
    0x6b,    /*  Q  */
    0x66,    /*  R  */
    0x5b,    /*  S  */
    0x0f,    /*  T  */
    0x3e,    /*  U  */
    0x3e,    /*  V  */
    0x2a,    /*  W  */
    0x37,    /*  X  */
    0x3b,    /*  Y  */
    0x6d,    /*  Z  */
    0x4e,    /*  [  */
    0x13,    /*  \  */
    0x78,    /*  ]  */
    0x62,    /*  ^  */
    0x08,    /*  _  */
    0x20,    /*  `  */
    0x7d,    /*  a  */
    0x1f,    /*  b  */
    0x0d,    /*  c  */
    0x3d,    /*  d  */
    0x6f,    /*  e  */
    0x47,    /*  f  */
    0x7b,    /*  g  */
    0x17,    /*  h  */
    0x04,    /*  i  */
    0x18,    /*  j  */
    0x57,    /*  k  */
    0x06,    /*  l  */
    0x14,    /*  m  */
    0x15,    /*  n  */
    0x1d,    /*  o  */
    0x67,    /*  p  */
    0x73,    /*  q  */
    0x05,    /*  r  */
    0x5b,    /*  s  */
    0x0f,    /*  t  */
    0x1c,    /*  u  */
    0x1c,    /*  v  */
    0x14,    /*  w  */
    0x37,    /*  x  */
    0x3b,    /*  y  */
    0x6d    /*  z  */
};

const uint8_t * pSevenSegASCIIFont = SevenSegASCIIFont;
