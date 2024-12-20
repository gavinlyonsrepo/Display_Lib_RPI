/*!
 * @file examples/include/Bitmap_test_data.hpp
 * @brief data file to hold bitmap test data for graphic displays
 * @details SW used to make code https://javl.github.io/image2cpp/
*/

# pragma once

/*
 Icons ::
	Power icon, 12x8 vertically addressed
	Lighting symbol, 12x8 vertically addressed
	Alarm icon  8x8px vertically addressed
	Alarm icon  8x8px horizontally addressed
	Message icon  16x8px vertically addressed
	Message icon  16x8px horizontally addressed
	Mobile icon  16x8px vertical addressed
	Mobile icon  16x8px horizontal addressed
	Battery Icon  16x8px vertical addressed
	Battery Icon  16x8px horizontally addressed
 Bitmaps ::
	Temperature Sun image, 16x16px , vertical addressed
	'SUN' In text 40x16 horizontally addressed.
	'backupicon', 64x64px , horizontal addressed
	'backupicon', 84x48px vertical addressed 
	'backupicon', 128x64px horizontally addressed
	'backupicon', 128x128px horizontally addressed 
*/

#include <cstdint>

//power icon, 12x8 vertically addressed
const uint8_t PowerIcon[12] =
{
0xff, 0xe7, 0xc3, 0x99, 0xa5, 0xad, 0xad, 0xa5, 0x99, 0xc3, 0xe7, 0xff
};

//lighting symbol, 12x8 vertically addressed
const  uint8_t SpeedIcon[12] =
{
0xff, 0xff, 0xf7, 0xb3, 0xd1, 0xc0, 0xe0, 0xf4, 0xf6, 0xfe, 0xff, 0xff
};

// Alarm icon  8x8px vertically addressed data
const uint8_t AlarmIconVa[8] =
{
  0x83, 0xbd, 0x42, 0x4a, 0x52, 0x52, 0xbd, 0x83
};

// Alarm icon  8x8px horizontally addressed data
const uint8_t AlarmIconHa[8] =
{
	0xC3,0xBD,0x42,0x52,0x4E,0x42,0x3C,0xC3
};

// Message icon  16x8px vertically addressed  data
const uint8_t  MsgIconVa[16] =
{
  0x00, 0x00, 0x00, 0xff, 0x85, 0x89, 0x91, 0x91, 0x91, 0x91, 0x89,
  0x85, 0xff, 0x00, 0x00, 0x00
};

// Message icon  16x8px horizontally addressed data
const uint8_t MsgIconHa[16] =
{
	0x1F,0xF8,0x10,0x08,0x18,0x18,0x14,0x28,0x13,0xC8,0x10,
	0x08,0x10,0x08,0x1F,0xF8
};

// Mobile icon  16x8px Vertical addressed
const uint8_t SignalIconVa[16] =
{
	0x03, 0x05, 0x09, 0xff, 0x09, 0x05, 0xf3, 0x00, 0xf8,
	0x00, 0xfc, 0x00, 0xfe, 0x00, 0xff, 0x00
};

// Mobile icon  16x8px horizontal addressed
const  uint8_t  SignalIconHa[16] = {
	0xfe, 0x02, 0x92, 0x0a, 0x54, 0x2a, 0x38, 0xaa, 0x12, 0xaa, 0x12,
	0xaa, 0x12, 0xaa, 0x12, 0xaa
	};

// Battery Icon  16x8px Vertical addressed
const uint8_t  BatIconVa[16] =
{
	0x00, 0x00, 0x7e, 0x42, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81,
	0xbd, 0xbd, 0x81, 0xff, 0x00
};

// Battery Icon  16x8px horizontally addressed data
const uint8_t BatIconHa[16] =
{
	0x0F,0xFE,0x30,0x02,0x26,0xDA,0x26,0xDA,0x26,0xDA,0x26,0xDA,0x30,0x02,
	0x0F,0xFE
};

// temperature Sun image, 16x16px , vertical addressed
const uint8_t TemperatureImageVA[32]= {
	0xff, 0xdf, 0xdf, 0xff, 0x1f, 0x09, 0x0f, 0x07, 0x07, 0x0f,
	0x09, 0x1f, 0xff, 0xdf, 0xdf, 0xff,0xff, 0xfb, 0xfb, 0xff,
	0xf8, 0x90, 0xf0, 0xe0, 0xe0, 0xf0, 0x90, 0xf8, 0xff, 0xfb,
	0xfb, 0xff
};

const uint8_t SunText[80] =  //SUN In text 40x16 horizontally addressed.
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xF1,0x81,0x8F,0xFC,0x3F,
  0xF1,0x81,0x8F,0xFC,0x30,0x31,0x81,0x8C,0x0C,0x30,0x01,0x81,0x8C,0x0C,0x30,0x01,
  0x81,0x8C,0x0C,0x3F,0xF1,0x81,0x8C,0x0C,0x3F,0xF1,0x81,0x8C,0x0C,0x00,0x31,0x81,
  0x8C,0x0C,0x00,0x31,0x81,0x8C,0x0C,0x30,0x31,0x81,0x8C,0x0C,0x3F,0xF1,0xFF,0x8C,
  0x0C,0x3F,0xF1,0xFF,0x8C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// 'backupicon', 64x64px , horizontal addressed
const  uint8_t  bigImage64x64[512] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf0, 0x00, 0x00,
0x00, 0x00, 0x0f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00,
0x00, 0x00, 0x3f, 0xe0, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0xff, 0x00, 0x00,
0x00, 0x00, 0xfe, 0x00, 0x80, 0x3f, 0x80, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x80, 0x1f, 0xc0, 0x00,
0x00, 0x01, 0xf8, 0x00, 0x80, 0x07, 0xe0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x80, 0x07, 0xe0, 0x00,
0x00, 0x03, 0xe0, 0x00, 0x80, 0x03, 0xf0, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x80, 0x01, 0xf0, 0x00,
0x00, 0x07, 0xc0, 0x00, 0x80, 0x01, 0xf8, 0x00, 0x00, 0x07, 0xc0, 0x01, 0xc0, 0x00, 0xf8, 0x00,
0x00, 0x0f, 0x80, 0x01, 0xc0, 0x00, 0xf8, 0x00, 0x00, 0x0f, 0x80, 0x01, 0xc0, 0x00, 0x78, 0x00,
0x00, 0x0f, 0x80, 0x01, 0xc0, 0x00, 0x7c, 0x00, 0x00, 0x0f, 0x80, 0x01, 0xc0, 0x00, 0x7c, 0x00,
0x01, 0xff, 0xfc, 0x01, 0xc0, 0x00, 0x7c, 0x00, 0x01, 0xff, 0xfc, 0x01, 0xc0, 0x00, 0x7c, 0x00,
0x01, 0xff, 0xf8, 0x01, 0xc0, 0x00, 0x7c, 0x00, 0x00, 0xff, 0xf8, 0x01, 0xe0, 0x00, 0x7c, 0x00,
0x00, 0x7f, 0xf0, 0x00, 0xf0, 0x00, 0x7c, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x78, 0x00, 0x7c, 0x00,
0x00, 0x1f, 0xc0, 0x00, 0x1c, 0x00, 0x78, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x0e, 0x00, 0xf8, 0x00,
0x00, 0x07, 0x00, 0x00, 0x03, 0x00, 0xf8, 0x00, 0x00, 0x06, 0x00, 0x00, 0x03, 0x01, 0xf8, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00,
0x00, 0x00, 0x04, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x3f, 0x80, 0x00,
0x00, 0x00, 0x1f, 0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x01, 0xff, 0x00, 0x00,
0x00, 0x00, 0x0f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xfc, 0x00, 0x00,
0x00, 0x00, 0x03, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


// 'backupicon', 84x48px data vertical addressed 504 bytes
const  uint8_t  backUpIcon84x48[504] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 0x78,
	0x7c, 0x3c, 0x3c, 0x3e, 0x1e, 0x1e, 0x1e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0xcf, 0xcf,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x1e, 0x1e, 0x1e, 0x1e, 0x3e, 0x3c, 0x7c, 0x78, 0xf8,
	0xf8, 0xf0, 0xf0, 0xe0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xf0, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0x9f, 0x87,
	0x83, 0x81, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0xe0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 0x3f, 0x3f, 0x3f,
	0x1f, 0x1f, 0x0f, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07, 0x06, 0x0e, 0x0c, 0x1c, 0x18, 0x30, 0x30,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xf0, 0xfc, 0xff, 0xff, 0xff,
	0x7f, 0x1f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x1c, 0x1e, 0x3e, 0x3c, 0x3c, 0x7c,
	0x78, 0x78, 0x78, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
	0xf0, 0xf0, 0xf0, 0xf0, 0x78, 0x78, 0x78, 0x7c, 0x3c, 0x3e, 0x3e, 0x1f, 0x1f, 0x0f, 0x0f, 0x07,
	0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'backupicon', 128x64px horizontally addressed data
const uint8_t  backupicon128x64[1024] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0xc0, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x00, 0xc0, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00,
0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00,
0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00,
0x00, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00,
0x00, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00,
0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00,
0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'backupicon', 128x128px , icon from https://github.com/gavinlyonsrepo/backupmenu
// 128 * (128/8) = 2048 , fullscreen bitmap bi-colour horizontally addressed.
const uint8_t BackupMenuBitmap[2048] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0xc0, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x01, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const uint8_t * pBackupMenuBitmap = BackupMenuBitmap;
const uint8_t * pSunText = SunText;
const uint8_t * pPowerIcon = PowerIcon;
const uint8_t * pSpeedIcon = SpeedIcon; 
const uint8_t * pSignalIcon = SignalIconVa;
const uint8_t * pMsgIcon = MsgIconVa;
const uint8_t * pAlarmIcon = AlarmIconVa;
const uint8_t * pBatIcon = BatIconVa;
