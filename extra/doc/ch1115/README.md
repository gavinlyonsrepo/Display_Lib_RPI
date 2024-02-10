# ER_OLEDM1_CH1115

![ OLED ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/oled.jpg)

## Table of contents

  * [Overview](#overview)
  * [Output](#output)
  * [Hardware](#hardware)
  * [Software](#software)
	* [SPI](#spi)
	* [Bitmaps](#bitmaps)
	* [User adjustments](#user-adjustments)


## Overview

* Name : ER_OLEDM1_CH1115
* Description :

0. C++ Library to support the ER-OLEDM1.09-1 128X64 OLED Display Module
   driven by the CH1115 controller for the Raspberry PI.
1. Raspberry Pi C++ library.
2. Inverse colour, vertical rotate, sleep, fade effect, horizontal scroll and contrast control.
3. Bitmaps supported.
4. Hardware and Software SPI

* Development Tool chain.
	1. Raspberry PI 3 model b
	2. C++, g++ (Debian 12.2.0)
	3. Raspbian , Debian 12 bookworm OS, , 64 bit.
	4. kernel : aarch64 Linux 6.1.0-rpi7-rpi-v8
	5. [bcm2835 Library 1.73 dependency](http://www.airspayce.com/mikem/bcm2835/). Provides low level SPI bus, delays and GPIO control.

* Author: Gavin Lyons
* Port of my Arduino [library link](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115).

## Output

![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_PICO/blob/main/extra/image/all.jpg)


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

![ wiring ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/wiring.png)

## Software

Example files : 

| Examples files main.cpp  | Description |
| ------ | ------ |
| HELLO_WORLD | Hello world , Basic usage |
| BITMAP | Shows use of bitmaps methods  |
| TEXT_GRAPHICS_FUNCTIONS | Fonts graphics + misc functions |
| FPS_TEST |  Frames per second test  |
| FPS_TEST_SW_SPI | Frames per second test but with software SPI setup|

### SPI

This library supports both Hardware SPI and software SPI.
The constructor of the main class is overloaded.
Which SPI is started depends on which constructor called by user.
Hardware SPI uses bcm2835 library spi module.
The SPI settings are in OLEDSPIHWSettings function.
If there are multiple SPI devices on Bus(with different settings) the user can call this method
to refresh settings before a tranche of OLED commands.
Default Speed is BCM2835_SPI_CLOCK_DIVIDER_64,
6.25MHz on RPI3. This can be adjusted in code or you can pass
the divider value in the begin method as an argument. These values are
defined by enum bcm2835SPIClockDivider. For full list see

![bcm2](https://github.com/gavinlyonsrepo/NOKIA_5110_RPI/blob/main/extra/images/bcm2.png)

User can also adjust which HW SPI chip enable pin they use(CE0 or CE1).
in begin method  arguments. This device uses bit order MSB First and SPI Mode 0.

Measured SPI speed results Version 1.3 are in the FPS test example file header comment block.
The member OLEDHighFreqDelay is a GPIO uS delay that
can be used to slow down or speed up Software SPI.
By default it is set to 0uS. It may have to be increased depending on device/CPU used.


### Bitmaps

There is a few different ways of displaying bitmaps,

| Num | Method |  Data addressing | Note |
| ------ | ------  | ------ |  ------ |
| 1 | OLEDBitmap() |  Vertical |  Writes directly to screen , no buffer used. |
| 2 | OLEDBufferScreen() |  Vertical  | For internal use mostly |
| 3 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) |
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

The drawBitmap method will return an error if : The Bitmap is completely off screen ,
Invalid Bitmap pointer object, bitmap bigger than screen , bitmap bigger/smaller than provided width and height calculation
( This helps prevents buffer overflow).
A vertical addressed Bitmap's height must be divisible by 8. eg, for a full screen bitmap with width=128 and height=64.
Bitmap excepted size = 128 * (64/8) = 1024 bytes.
A horizontal addressed Bitmap's width must be divisible by 8. eg, for a bitmap with width=88 and height=48.
Bitmap excepted size = (88/8) * 48 = 528 bytes.

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/)
See example file "BITMAP" for more details.

### User adjustments

When the user calls OLEDbegin() to start OLED they can specify a contrast setting from 0x00 to 0xFF. Datasheet says 0x80 is default. User can also change contrast on the fly.
Screen can be disabled to enter a sleep type mode where OLED draws 500uA.
Screen pixel colour can be inverted and also screen can be vertically rotated.

There is a "fade or breath display effect". Whose parameters(time brightness) can be adjusted by passing data to function see "OLEDfadeEffect" function header  in .cpp and datasheet for details.
default is 0x81.

There is a  Horizontal scroll effect. Whose parameters(TimeInterval , direction , mode)
can be adjusted by passing data to function see "OLEDscrollSetup" function header in .cpp and datasheet for details. defaults are in header file are 6 frames , right , continuous mode.


