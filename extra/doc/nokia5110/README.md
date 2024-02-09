# NOKIA_5110

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
    * [SPI](#spi)
    * [File system](#file-system)
    * [Bitmap](#bitmap)

  * [Output](#output)

## Overview

* Name: NOKIA_5110
* Description:

0. C++ Library for Nokia 5110 LCD, PCD8544 Driver. 84 x 48 pixels.
1. Dynamic install-able Raspberry Pi C++ library.
2. Invert colour, rotate, sleep, contrast bias control supported.
3. 12 fonts.
4. Graphics class included.
5. Polymorphic print class included.
6. Bitmaps supported.
7. Hardware and Software SPI
8. Dependency: bcm2835 Library

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_18F47K42_projects)
* Development Tool chain. 
	1. Raspberry PI 3 model b
	2. C++, g++ (Debian 12.2.0) 
	3. Raspbian , Debian 12 bookworm OS, , 64 bit.
	4. kernel : aarch64 Linux 6.1.0-rpi7-rpi-v8
	5. [bcm2835 Library 1.71 dependency](http://www.airspayce.com/mikem/bcm2835/). Provides low level SPI bus, delays and GPIO control.
	

## Hardware

The Nokia 5110 is a basic graphic LCD screen for lots of applications.
GPIO function on RPI, 5 Nokia 5110 LCD lines SPI bus. Use any GPIO user likes for Software SPI.
For Hardware the user can use any GPIO for reset and data/command line

Example file set up ::

| RPI pin(HW SPI) | RPI pin(SW SPI)| Nokia 5110 LCD |
| ------ | ------ | ------ |
| SPI_CLK | GPIO22 | LCD_CLK Pin 5 clock in |
| SPI_MOSI | GPIO27 | LCD_DIN Pin 4 data in |
| GPIO24 | GPIO24 | LCD_DC Pin 3 data/command|
| SPI_CE0 or SPI_CE1 | GPIO 8 | LCD_CE Pin 2 chip enable |
| GPIO25 | GPIO25 | LCD_RST Pin 1 reset|

Connect Nokia 5110 VCC(pin 6)to 3.3V.
The user may have to adjust LCD contrast and bias settings,
to the screen at hand for optimal display.
A resistor or potentiometer can be connected between (Nokia LCD)
GND(pin8) and LIGHT(pin7) to switch on /off backlight and adjust brightness.

[Nokia 5110 LCD dataSheet ](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf)

![PICTURE](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/NOKIA2.jpg)

## Software

### SPI

This library supports both Hardware SPI and software SPI.
The constructor of the main class is overloaded. 
Which SPI is started depends on which constructor called by user.
Hardware SPI uses bcm2835 library spi module.
The SPI settings are in LCDSPIHWSettings function.
If there are multiple SPI devices on Bus(with different settings) the user can call this method 
to refresh settings before a tranche of LCD commands.
Default Speed is BCM2835_SPI_CLOCK_DIVIDER_64,
6.25MHz on RPI3. This can be adjusted in code or you can pass
the divider value in the contructor as an argument. These values are
defined by enum bcm2835SPIClockDivider. For full list see

![bcm2](https://github.com/gavinlyonsrepo/NOKIA_5110_RPI/blob/main/extra/images/bcm2.png)

User can also adjust which HW SPI chip enable pin they use(CE0 or CE1).
in constructor arguments. This device uses bit order MSB First and SPI Mode 0.

Measured SPI speed results Version 1.3 are in the SpeedTest example file header comment block.
The parameter LCDHardwareSPI is a GPIO uS delay that
can be used to slow down or speed up Software SPI.
By default it is set to 2uS. It may have to be adjusted depending on device/CPU used.

### File system

Example files 

| Filepath | File Function | SPI |
| ---- | ---- | ---- |
| HELLOWORLD | Basic use case | HW |
| HELLOWORLD_SWSPI | Basic use case SW SPI | SW |
| TEXT_GRAPHICS_FUNCTIONS | Text & Graphics & Function testing | HW |
| BITMAP | Bitmaps | HW |
| SPEEDTEST | Frame rate per second test | HW |

### Bitmap

| num | Method name | data addressing | Notes | 
| ------ | ------ | ------ | ------ |   
| 1 | drawBitmap() |vertical| Draws bitmaps to the buffer, Bitmap's height must be divisible by 8| 

The drawBitmap will return an error code if : The Bitmap is completely off screen , 
Invalid Bitmap pointer object, bitmap bigger than screen , bitmap bigger/smaller than provided width and height co-ordinates
( This helps prevents buffer overflow if user enters wrong data.).
The Bitmap's height must be divisible by 8. I.e for a full screen bitmap with width=84 and height=48.
Bitmap excepted size = 84 * (48/8) = 504 bytes.

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) 
See example file "BitmapTests" for more details.

## Output

Example output.

![OUTPUT](https://github.com/gavinlyonsrepo/pic_18F47K42_projects/blob/master/images/nokiagraph1.jpg)
