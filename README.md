
![ OLED ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/oled.jpg)

# ER_OLEDM1_CH1115_RPI

## Table of contents

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Test](#test)
  * [Hardware](#hardware)
  * [Software](#software)
 	* [SPI](#spi)
	* [Buffer](#buffer)
	* [Fonts](#fonts)
	* [Bitmaps](#bitmaps)
	* [User adjustments](#user-adjustments)
  
## Overview

* Name : ER_OLEDM1_CH1115_RPI
* Description : 

0. C++ Library to support the ER-OLEDM1.09-1 128X64 OLED Display Module 
   driven by the CH1115 controller for the Raspberry PI.
1. Raspberry Pi C++ library.      
2. Inverse colour, vertical rotate, sleep, fade effect, horizontal scroll and contrast control. 
3. 8 fonts included 
4. Graphics class included.
5. buffer.
6. Bitmaps supported.
7. Hardware and Software SPI 

* Developed on 
	1. Raspberry PI 3 model b, 
	2. C++ complier g++ (Raspbian 8.3.0-6)
	3. Raspbian 10 buster  OS
	4. bcm2835 Library 1.71 (Dependency)
    
* Author: Gavin Lyons
* Port of my Arduino [library link](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115).
	There is an API for the arduino library which is very similar to this port. 

## Output

Output Screenshots, From left to right, top to bottom.

1. Full screen bitmap displayed 
2. Text + graphics
3. Clock Demo + bitmaps.
4. Different size and inverted text.
5. Available ASCII font printed out 0-127
6. Extended ASCII font printed out 128-255  
7. Font 1-4 
8. Font 7 

![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/output.jpg)
![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/blob/main/extras/image/fontpic.jpg)

## Installation

1. Make sure SPI bus is enabled on your raspberry PI

2. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.68.)
	* The bcm2835 library is a dependency and provides SPI bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

3. Download the ER_OLEDM1_CH1115_RPI library 
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.
    
```sh
curl -sL https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/archive/1.3.2.tar.gz | tar xz
```

4. Run "make" to run the makefile at repo root folder to install library, it will be 
    installed to usr/lib and usr/include
    
```sh
cd ER_OLEDM1_CH1115_RPI-1.3.2
sudo make
```

## Test

1. Next step is to test OLED and installed library with an example.
Wire up your OLED. Next enter the examples folder and run the makefile in THAT folder, 
This makefile builds the examples file using the just installed library.
and creates a test exe file in "bin". Be sure to use "sudo" as the bcm2835 requires root permissions by default [ see here for more details on that](http://www.airspayce.com/mikem/bcm2835/) 
The default example file is "hello world",  user should see hello world on the OLED
by end of this step.

```sh
cd examples/
make
sudo bin/test
```

2. There are six examples files to try out. 
To decide which one the makefile builds simply edit "SRC" variable at top of the makefile in examples folder.
in the "User SRC directory Option Section". Pick an example "SRC" directory path and ONE ONLY.
Comment out the rest and repeat 1.

| Examples files main.cpp  | Description |
| ------ | ------ | 
| HELLO_WORLD | Hello world , Basic usage | 
| BITMAP | Shows use of bitmaps methods  | 
| FPS_TEST |  Frames per second test  | 
| OLED_FUNCTIONS | misc functions, rotate, flip, scroll,  etc  | 
| SOFTWARE_SPI | Shows setup for software SPI | 
| TEXT_GRAPHICS | Fonts + graphics test | 

## Hardware

CH1115 is a single-chip CMOS OLED driver with controller for organic light emitting diode dot-matrix graphic display system. CH1115 consists of 128 segments, 64 commons that can support a maximum display resolution of 128 X 64. It is designed for Common Cathode type OLED panel. ER-OLEDM1.09-1W-SPI is a White 1.09" OLED Display Panel with Breakout Board. This module is a combination of the two.(controller and OLED)

For SWSPI pick any GPIO you want for the 5 control lines.

| pin no| GPIO HWSPI | pin name | pin desc | 
|---|---|---| ---| 
| 1 | n/a |Gnd | Ground |
| 2 | n/a |VCC | voltage supply |
| 3 | SPISCLK GPIO 11 |SCL | Serial clock input |
| 4 | SPIMOSI GPIO 10 |SDA | Serial data input |
| 5 | Any you want | RES | This pin is reset signal input.  |
| 6 | Any you want | DC | This pin is Data or Command control pin. |
| 7 | SPICEO GPIO 8 | CS | This pin is the chip select input.  |


![ wiring ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/blob/main/extras/image/wiring.png)

## Software

There are two makefiles

    1. Makefile at root directory builds and installs library at a system level.
    2. Makefile in example directory build chosen example file to an executable.

### SPI

Hardware and software SPI. Two different class constructors. 
User can pick the relevant constructor, see examples files. 
Hardware SPI is recommended, far faster and more reliable 
but Software SPI allows for more flexible GPIO selection.
The SPI switches on and off after each data transfer to allow
other devices with different SPI settings to use bus.
The SPI settings are in OLEDSPIon function.
Speed is currently at BCM2835_SPI_CLOCK_DIVIDER_64. 
6.25MHz on RPI3. This can be adjusted in code or you can pass 
the divider value in the begin method as a parameter. These values are
defined by enum bcm2835SPIClockDivider. For full list see
[link.](http://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e)

User can also adjust which SPI chip enable pin the use.


### Fonts

There are 8 fonts.
A print class is available to print out most passed data types.
The fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
Font 7 & 8 are special fixed size large font but it is numbers only + ':' &'.' only


Font data table: 

| Font num | Font enum name | Font size xbyy |  ASCII range | Size in bytes |
| ------ | ------ | ------ | ------ |  ------ | 
| 1 | OLEDFontType_Default | 5x8 | ASCII 0 - 0xFF, Full Extended  | 1275 |
| 2 | OLEDFontType_Thick   | 7x8 |  ASCII  0x20 - 0x5A, no lowercase letters | 406 | 
| 3 | OLEDFontType_SevenSeg  | 4x8 | ASCII  0x20 - 0x7A | 360 |
| 4 | OLEDFontType_Wide | 8x8 |  ASCII 0x20 - 0x5A, no lowercase letters| 464 |
| 5 | OLEDFontType_Tiny | 3x8 | ASCII  0x20 - 0x7E | 285 |
| 6 | OLEDFontType_Homespun  | 7x8 | ASCII  0x20 - 0x7E |  658 |
| 7 | OLEDFontType_Bignum | 16x32 | ASCII 0x30-0x3A ,Numbers + : . only | 704 |
| 8 | OLEDFontType_Mednum | 16x16 | ASCII 0x30-0x3A , Numbers + : . only | 352 |

### Bitmaps

There is a few different ways of displaying bitmaps, 

| Num | Method |  Data addressing | Note |
| ------ | ------  | ------ |  ------ |  
| 1 | OLEDBitmap() |  Vertical |  Writes directly to screen , no buffer used. | 
| 2 | OLEDBuffer() |  Vertical  | For internal use mostly | 
| 3 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) | 
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

See the bitmap example file for more details on each method. Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) , Bitmaps should be defined as const  buffers non-const, for method 2 buffer can be initialised with bitmap data.

### User adjustments

When the user calls OLEDbegin() to start OLED they can specify a contrast setting from 0x00 to 0xFF. Datasheet says 0x80 is default. User can also change contrast on the fly.
Screen can be disabled to enter a sleep type mode where OLED draws 500uA.
Screen pixel colour can be inverted and also screen can be vertically rotated. 

There is a "fade or breath display effect". Whose parameters(time brightness) can be adjusted by passing data to function see "OLEDfadeEffect" function header  in .cpp and datasheet for details.
default is 0x81.

There is a  Horizontal scroll effect. Whose parameters(TimeInterval , direction , mode)
can be adjusted by passing data to function see "OLEDscrollSetup" function header in .cpp and datasheet for details. defaults are in header file are 6 frames , right , continuous mode.

