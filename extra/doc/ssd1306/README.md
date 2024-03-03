# SSD1306 OLED Readme

[![ OLED image ](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/device.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/device.jpg)

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
	* [File system](#file-system)
	* [I2C](#i2c)
	* [Bitmaps](#bitmaps)
  * [Output](#output)
  * [Notes and issues](#notes-and-issues)


## Overview

* Name : SSD1306_OLED
* Description :

0. Library to support the I2C 128X64 OLED Display Module
   driven by the SSD1306 controller for the Raspberry PI.
1. Dynamic install-able system level Raspberry Pi C++ library.
2. Inverse color, rotate, sleep, scroll and contrast control.
3. Bitmaps supported.
4. Hardware I2C
5. Also tested on 128X32 display size. Should work for 96X16 display size.


## Hardware

Manufacturers diagram showing connections.

[![ Wiring Image](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)

## Software

### File system

Example files 

| Filepath | File Function | Screen Size |
| ---- | ---- | ---- |
| HELLO_WORLD | Basic use case | 128x64 |
| BITMAP  | Shows use of bitmaps | 128x64 |
| CLOCK_DEMO | A basic clock Demo | 128x64 |
| SPEED_TEST | Frame rate per second test | 128x64 |
| TEXT_GRAPHICS_FUNCTIONS |Tests Text,graphics and functions| 128x64 |

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
| 1 | OLEDBitmap() |horizontal| Draws bitmaps to the buffer |
| 2 | OLEDBuffer() |vertical | Write a full screen bitmap direct to screen, used internally|
| 3 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) |
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/)
See example file "BITMAP" for more details.

## Output

[![ Output image ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1306.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1306.jpg)

[![ Output image 2 ](https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO/blob/main/extra/image/3.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO/blob/main/extra/image/3.jpg)

## Notes and issues

1. To test on a different size of display edit the myOLEDwidth & myOLEDheight
variables in examples files.

| Display size | Supported | Tested |
| ------ | ------ | ------ |
| 128x64 | Yes | Yes |
| 128x32 | Yes | Yes |
| ???x16 | Yes | NO |
