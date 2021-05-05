
![ OLED ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/oled.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Hardware](#hardware)
  * [Features](#features)
 
  
Overview
--------------------
* Name : ER_OLEDM1_CH1115_RPI
* Description : 

0. C++ Library to support the ER-OLEDM1.09-1 128X64 OLED Display Module 
   driven by the CH1115 controller for the Raspberry PI.
1. Raspberry Pi C++ library.      
2. Inverse colour, vertical rotate, sleep, fade effect, horizontal scroll and contrast control. 
3. Extended ASCII, scalable font. 
4. Graphics class included.
5. 3 different modes: Multi-buffer , single buffer , no buffer.
6. Bitmaps supported.
7. Hardware and Software SPI 

* Author: Gavin Lyons
* Tested on Raspberry PI 3 model b, C++ ,Raspbian 9.13 stretch
* Port of my Arduino [library](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115)
 

Output
-----------------------------

Output Screenshots, From left to right, top to bottom.

1. Full screen bitmap displayed 
2. Multi buffer mode screen divided into two horizontal buffers
3. Multi buffer mode screen divided into vertical buffers.
4. Different size and type of fonts 
5. Available ASCII font printed out 0-127
6. Extended ASCII font printed out 128-255  

![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/output.jpg)

Installation
------------------------------

0. Install the C libraries of bcm2835, see: http://www.airspayce.com/mikem/bcm2835/ 
1. curl -sL https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/archive/1.0.tar.gz | tar xz
2. cd ER_OLEDM1_CH1115_RPI
3. Make 
4. sudo ./bin/test
5. There are 8 different main.cpp in the examples folder copy the one to run into src folder


Hardware
----------------------------

CH1115 is a single-chip CMOS OLED driver with controller for organic light emitting diode dot-matrix graphic display system. CH1115 consists of 128 segments, 64 commons that can support a maximum display resolution of 128 X 64. It is designed for Common Cathode type OLED panel. ER-OLEDM1.09-1W-SPI is a White 1.09" OLED Display Panel with Breakout Board. This module is a combination of the two.(controller and OLED)

For SWSPI pick any GPIO you want for 5 control lines

| pin no| GPIO HWSPI | pin name | pin desc | 
|---|---|---| ---| 
| 1 | n/a |Gnd | Ground |
| 2 | n/a |VCC | voltage supply |
| 3 | SPISCLK GPIO 11 |SCL | Serial clock input |
| 4 | SPIMOSI GPIO 10 |SDA | Serial data input |
| 5 | Any you want | RES | This pin is reset signal input.  |
| 6 | Any you want | DC | This pin is Data or Command control pin. |
| 7 | SPICEO GPIO 8 |  CS | This pin is the chip select input.  |


![ wiring ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/blob/main/extras/image/wiring.png)

Features
-------------------------

*SPI*

Hardware and software SPI. Two different class constructors. 
User can pick the relevant constructor, see examples files. 
Hardware SPI is recommended, far faster and more reliable 
but Software SPI allows for more flexible GPIO selection.

*buffers*

3 buffer modes 

1. MULTI_BUFFER (default)
2. SINGLE_BUFFER 
3. NO_BUFFER , Text only,  light weight. 

To switch between modes, user must make a change to the USER BUFFER OPTION SECTION  at top of ER_OLEDM1_CH1115.h file.  Pick ONE option and one option ONLY. The example files at top, say which option to pick. If wrong option is picked, example files will not work or maybe even compile. Bitmaps can still be written directly to screen in NO_BUFFER mode but no graphics possible.

*fonts*

The ASCII font(in the ER_OLEDM1_CH1115_font.h file) 
The scale-able font is a standard 5 by 7 ASCII font with two  columns  of padding added. So 7 by 8 in effect. In standard text size and "no buffer" mode, this means: 128/7 * 64/8 = 27 * 8 = 144 characters. 

*bitmaps*

Bitmaps are written directly to screen not buffer, Updating the buffer will overwrite them(unless bitmap set to buffer) so to share screen with text and graphics best to divide screen into buffers using multi buffer mode,  See examples files. Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) use vertical addressing draw mode. 

*User adjustments*

When the user calls OLEDbegin() to start OLED they can specify a contrast setting from 0x00 to 0xFF. Datasheet says 0x80 is default. User can also change contrast on the fly.
Screen can be disabled to enter a sleep type mode where OLED draws 500uA.
Screen pixel colour can be inverted and also screen can be vertically rotated. 

There is a "fade or breath display effect". Whose parameters(time brightness) can be adjusted by passing data to function see "OLEDfadeEffect" function header  in .cpp and datasheet for details.
default is 0x81.

There is a  Horizontal scroll effect. Whose parameters(TimeInterval , direction , mode)
can be adjusted by passing data to function see "OLEDscrollSetup" function header in .cpp and datasheet for details. defaults are in header file are 6 frames , right , continuous mode.

*Functions*

Functions: Detailed information on the functions can be found in comment headers in the ER_OLEDM1_CH1115.cpp  file and a list of them in keywords.txt. The graphic functions can be found in the ER_OLEDM1_CH1115_graphic.h file. 
