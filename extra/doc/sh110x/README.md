# SH110X OLED Readme

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
	* [File system](#file-system)
	* [I2C](#i2c)
	* [Bitmaps](#bitmaps)


## Overview

* Name : SH110X
* Description :

0. Library to support the I2C 128X64 OLED Display Module
   driven by the Sh1106 or Sh1107 controller for the Raspberry PI.
1. Dynamic install-able system level Raspberry Pi C++ library.
2. Inverse color, rotate, sleep, scroll and contrast control.
3. Bitmaps supported.
4. Hardware I2C


## Hardware

Manufacturers diagram showing connections.

[![ Wiring Image](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)

## Software

### File system

Not many examples files as I did not have device for test.
These where tested by a third party.

Example files 

| Controller | Filepath | File Function | Screen Size |
| ---- | ---- | ---- | ---- |
| Sh1106 | HELLO_WORLD | Basic use case | 128x64 |
| SH1106 | BITMAP  | Shows use of bitmaps | 128x64 |
| Sh1107 | HELLO_WORLD | Basic use case | 128x64 |

### I2C

Hardware I2C.

1. I2C Address is set by default to 0x3C(your module could be different,
	user can change argument passed into "OLEDbegin" method).

2.  I2C Clock rate can be a passed into in the LCD class constructor method as a argument,
User can pass 1 of 4 BCM2835_I2C_CLOCK_DIVIDER values 2500, 626 150 or 148.
See image below.

3. In the event of an error writing a byte, debug info with error code will be written to console.
	This error code is the bcm2835I2CReasonCodes enum. Debug flag must be set to true to see this output.
	See image below for  bcm2835I2CReasonCodes.

[![ bcm image](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/bcm.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/bcm.jpg)

For more info on bcm2835I2CClockDivider & bcm2835I2CReasonCodes see [bcm2835 doc's for details](http://www.airspayce.com/mikem/bcm2835/group__constants.html)

### Bitmaps

Different bitmaps methods can be used.

| num | Method name | data addressing | Notes |
| ------ | ------ | ------ | ------ |
| 1 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) |
| 2 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/)
See example file "BITMAP" for more details.
