# NOKIA 5110 Readme

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
    * [SPI](#spi)
    * [File system](#file-system)
  * [Output](#output)

## Overview

* Name: NOKIA_5110
* Description:

0. C++ Library for Nokia 5110 LCD, PCD8544 Driver. 84 x 48 pixels.
1. Dynamic install-able Linux based Single Board Computers C++ library.
2. Invert colour, rotate, sleep, contrast bias control supported.
3. Bitmaps supported.
4. Hardware and Software SPI

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_18F47K42_projects)


## Hardware

The Nokia 5110 is a basic graphic LCD screen for lots of applications.
GPIO function on RPI, 5 Nokia 5110 LCD lines SPI bus. Use any GPIO user likes for Software SPI.
For Hardware the user can use any GPIO for reset and data/command line

Example file set up :

| RPI pin(HW SPI) | RPI pin(SW SPI)| Nokia 5110 LCD |
| ------ | ------ | ------ |
| SPI_CLK | GPIO6 | LCD_CLK Pin 5 clock in |
| SPI_MOSI | GPIO5 | LCD_DIN Pin 4 data in |
| GPIO24 | GPIO24 | LCD_DC Pin 3 data/command|
| SPI_CE0  | GPIO 21 | LCD_CE Pin 2 chip enable |
| GPIO25 | GPIO25 | LCD_RST Pin 1 reset|

Connect Nokia 5110 VCC(pin 6)to 3.3V.
The user may have to adjust LCD contrast and bias settings,
to the screen at hand for optimal display.
A resistor or potentiometer can be connected between (Nokia LCD)
GND(pin8) and LIGHT(pin7) to switch on /off backlight and adjust brightness.

[Nokia 5110 LCD dataSheet ](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf)

[![ image nokia ](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/NOKIA2.jpg)](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/NOKIA2.jpg)

## Software

### SPI

This library supports both Hardware SPI and software SPI.
The constructor of the main class is overloaded.
Which SPI is started depends on which constructor called by user.

*Software SPI*

For Software SPI Pick any GPIO you want.
To get the number for GPIO_CHIP_DEVICE parameter you can check ls /dev/gpiochip*
At time of testing it was found that Raspberry pi 5 = 4 and raspberry pi 3 = zero 
But now it look like both are zero after software update, although 4 should still work for RPi5 
if a symlink is in /dev/ folder. 
The member LCDHighFreqDelay is a GPIO uS delay that
can be used to slow down or speed up Software SPI.
By default it is set to 0uS. It may have to be increased depending on device/CPU used.

*Hardware SPI*

For Hardware SPI the User must use fixed SPI pins SPIMOSI and SPISCLK, user can choice between SPICE0 and SPICE1 
for chip select. In hardware SPI user can pick SPI bus speed. SPI must be turned on device.

| parameter | default value | note  |
| --- | --- | --- |
| HWSPI_DEVICE | 0| A SPI device, >= 0. which SPI interface to use , ls /dev/spi*|
| HWSPI_CHANNEL | 0 |A SPI channel, >= 0. Which Chip enable pin to use usually 0 or 1|
| HWSPI_SPEED |  1000000| The speed of serial communication in bits per second.|
| HWSPI_FLAGS | 0|  mode 0 for this device |
| GPIO_CHIP_DEVICE | 0| gpio chip device >= 0, check ls /dev/gpiochip |

### File system

Example files 

| Filepath | File Function | SPI |
| ---- | ---- | ---- |
| HELLOWORLD | Basic use case | HW |
| HELLOWORLD_SWSPI | Basic use case SW SPI | SW |
| TEXT_GRAPHICS_FUNCTIONS | Text & Graphics & Function testing | HW |
| BITMAP | Bitmaps | HW |
| SPEEDTEST | Frame rate per second test | HW |


## Output

Example output.

[![output image](https://github.com/gavinlyonsrepo/pic_18F47K42_projects/blob/master/images/nokiagraph1.jpg)](https://github.com/gavinlyonsrepo/pic_18F47K42_projects/blob/master/images/nokiagraph1.jpg)
