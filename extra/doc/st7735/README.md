# ST7735 TFT Readme

[![ Image TFT ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system)
      * [Bitmap](#bitmap)  
  * [Hardware](#hardware)
  * [Output](#output)

## Overview

* Name: ST7735_TFT
* Description:

0. C++ Library for a TFT SPI LCD, ST7735 Driver
1. Dynamic install-able Raspberry Pi C++ library.
2. Inverse colour, rotate, sleep, idle  & vertical scroll modes supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color & bi-color Bitmaps supported.
5. Hardware and Software SPI
6. Dependency: bcm2835 Library

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_16F18346_projects)

## Software

### User options

In the example files. There are 3 sections in "Setup()" function 
where user can make adjustments to select for SPI type used, PCB type used and screen size.

1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 PCB_TYPE, SPI SPEED , SPI_CE_PIN

*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The TFTSetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for TFTSetupGPIO define which is used.
HW SPI is far faster and more reliable than SW SPI

*USER OPTION 2 Screen size  + Offsets*

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge 
such as cropped data or defective pixels.
The function TFTInitScreenSize sets them.

*USER OPTION 3 PCB Version, SPI SPEED , SPI_CE_PIN*

TFTInitPCBType function is overloaded(2 off, one for HW SPI the other for SW SPI).

PCB_TYPE

In the main.cpp in USER OPTION 3 PCB_TYPE select your display type.
By passing an enum type to function  TFTInitPCBType.
Default is "TFT_ST7735R_Red". There are 4 types of the ST7735 TFT display supported.
If your display works but RGB colors are wrong you may have chosen wrong display type.

| Number | Description | Enum label| Tested |
| ---- | ---- | --- | ---| 
| 1 | ST7735B controller| TFT_ST7735B | n/a |
| 2 | ST7735R Green Tab | TFT_ST7735R_Green | n/a |
| 3 | ST7735R Red Tab   | TFT_ST7735R_Red | RED PCB v1.1, 1.44, 128x128 pixels |
| 4 | ST7735S Black Tab | TFT_ST7735S_Black | RED PCB v1.2, 1.8, 128 x 160 pixels |


SPI_Speed (HW SPI Only)

Here the user can pass the SPI Bus freq in Hertz,
Maximum 125 Mhz , Minimum 30Khz, The default in file is 8Mhz 
Although it is possible to select high speeds for the SPI interface, up to 125MHz,
Don't expect any speed faster than 32MHz to work reliably.
If you set to 0 .Speed is set to bcm2835 
constant BCM2835_SPI_CLOCK_DIVIDER_32. If using SW spi, ignore.

SPI_CE_PIN (HW SPI Only)

Which Chip enable pin to use two choices. If using SW spi, ignore.
	* SPICE0 = 0
	* SPICE1 = 1

SPI_CommDelay (SW SPI Only)

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
| 5 | Frame_rate_test_two  | Frame rate per second (FPS) text + graphics |

### Bitmap

There are four class functions to support drawing bitmaps

| Function Name | Colour support | Pixel size |  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour | (0-127) X 8  | Data vertically addressed |
| drawBitmap | bi-colour |  2048 bytes max | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 BMP files |  32K max | ------ |
| drawBitmap24  | 24 bit color BMP files | 48K max | Converted by software to 16-bit color  |

1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Pixel size column assumes 128 by 128 screen.

These class functions will return an error code in event of error see API for more details.

## Hardware

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC |  VCC |
| 2 | SCLK | SPI_CLK | GPIO5 |
| 3 | SDA | SPI_MOSI | GPIO6 |
| 4 | A0/DC | GPIO24 | GPIO24  |
| 5 | RESET | GPI025  | GPIO25 |
| 6 | SS/CS | SPI_CE0 | GPIO8 |
| 7 | GND | GND | GND |
| 8 | VCC | VCC | VCC  |

1. Connect LED backlight pin 1 thru a 150R/220R ohm resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. Pick any GPIO you want for SW SPI for HW SPI: reset and DC lines are flexible.
5. User can select  SPI_CE0  or SPI_CE1 for HW SPI
6. Backlight control is left to user.

[![ Wiring pic ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/wiring.jpg)](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/wiring.jpg)

## Output

1. Clock Demo showing icons, small bitmaps
2. 24-bit color bitmap test image
3. 16-bit color bitmap test image

[![ Demo pic ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)

