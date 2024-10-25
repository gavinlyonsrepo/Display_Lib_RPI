# ST7789 TFT Readme

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system) 
  * [Hardware](#hardware)
  * [Output](#output)
  * [Notes](#notes)
     * [Multiple SPI devices](#multiple-spi-devices)

## Overview

* Name: ST7789_TFT
* Description:

0. C++ Library for a TFT SPI LCD, ST7789 Driver
1. Dynamic install-able Raspberry Pi C++ library.
2. Inverse colour, rotate, sleep modes supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color & bi-color Bitmaps supported.
5. Hardware and Software SPI
6. Dependency: bcm2835 Library

* Author: Gavin Lyons

## Software

### User options

In the example files. There are 3 sections in "Setup()" function 
where user can make adjustments to select for SPI type used, and screen size.

1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 SPI SPEED , SPI_CE_PIN

*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The SetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for SetupGPIO define which is used.
HW SPI is far faster and more reliable than SW SPI

*USER OPTION 2 Screen size  + Offsets*

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge 
such as cropped data or defective pixels.
The function InitScreenSize sets them.

*USER OPTION 3  SPI SPEED , SPI_CE_PIN*

InitSPI function is overloaded(2 off, one for HW SPI the other for SW SPI).

Param SPI_Speed (HW SPI Only)

Here the user can pass the SPI Bus freq in Hertz,
Maximum 125 Mhz , Minimum 30Khz, The default in file is 8Mhz 
Although it is possible to select high speeds for the SPI interface, up to 125MHz,
Don't expect any speed faster than 32MHz to work reliably.
If you set to 0 .Speed is set to bcm2835 constant BCM2835_SPI_CLOCK_DIVIDER_32.

Param SPI_CE_PIN (HW SPI Only)

Which Chip enable pin to use two choices.
	* SPICE0 = 0
	* SPICE1 = 1

Param SPI_CommDelay (SW SPI Only)

The user can adjust If user is having reliability issues with SW SPI in some setups.
This is a microsecond delay in SW SPI GPIO loop. It is set to 0 by default, Increasing it will slow 
down SW SPI further.

### File system

In example folder:
The Main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder, 3 16-bit and 5 24-bit images.

| # | example file name  | Desc|
| ------ | ------ |  ------ |
| 1 | Hello_world| Basic use case |
| 2 | Text_Graphic_Functions | Tests text,graphics & function testing  |
| 3 | Bitmap_Tests | bitmap |
| 4 | Frame_rate_test_bmp | Frame rate per second (FPS) bitmaps |

## Hardware

Tested and developed on:

* Size 1.69" IPS color TFT LCD
* Resolution: 240 (H) RGB x280 (V)
* Control chip: st7789v2
* Display area 27.972 (H) x 32.634 (V)
* Panel size 30.07 (H) x37.43 (V) x1.56 (d)
* Logic voltage 3.3V

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC |  VCC |
| 2 | SS/CS | SPI_CE0 | GPIO8 |
| 3 | DC | GPIO24 | GPIO24  |
| 4 | RESET | GPI025  | GPIO25 |
| 5 | SDA | SPI_MOSI | GPIO6 |
| 6 | SCLK | SPI_CLK | GPIO5 |
| 7 | VCC | VCC | VCC  |
| 8 | GND | GND | GND |


1. Connect LED backlight pin 1 thru a resistor to VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. Pick any GPIO you want for SW SPI,  for HW SPI: reset and DC lines are flexible.
4. User can select  SPI_CE0  or SPI_CE1 for HW SPI
5. Backlight control is left to user.

## Output

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)

## Notes

### Multiple SPI devices

When using hardware SPI for multiple devices on the bus.
If the devices require different SPI settings (speed of bus, bit order , chip enable pins , SPI data mode).
The user must call function **TFTSPIHWSettings()** before each block of SPI transactions for display in order to refresh the SPI hardware settings for that device. See github [issue #1](https://github.com/gavinlyonsrepo/Display_Lib_RPI/issues/1).
