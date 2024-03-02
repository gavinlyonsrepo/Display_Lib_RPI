# ILI9341 LCD TFT Readme

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system) 
  * [Hardware](#hardware)
  * [Touchscreen](#touchscreen)

## Overview

* Name: ILI9341_TFT
* Description:

0. C++ Library for a TFT SPI LCD, ILI9341 Driver (might also work with ILI9340)
1. Dynamic install-able Raspberry Pi C++ library.
2. Inverse colour, rotate, scroll, modes supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color & bi-color Bitmaps supported.
5. Hardware and Software SPI
6. Dependency: bcm2835 Library
7. Support for XPT2046 Touchscreen IC included

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
The TFTSetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for TFTSetupGPIO define which is used.
HW SPI is far faster and more reliable than SW SPI

*USER OPTION 2 Screen size*

User can adjust screen pixel height, screen pixel width 
The function TFTInitScreenSize sets them.

*USER OPTION 3  SPI SPEED , SPI_CE_PIN*

TFTInitSPI function is overloaded(2 off, one for HW SPI the other for SW SPI).

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
| 3 | Bitmap_Tests | bitmap + FPS testing |
| 4 | Mandelbrot_set | Drawing a mandelbrot set demo |
| 5 | Touch_Screen | Basic Touch screen  demo |
| 6 | xpt_Test | Touch screen test  without TFT |

## Hardware

Tested and developed on
    * Size 2.4" SPI Serial  IPS color TFT LCD
    * Resolution: 240 (H) RGB x 320 (V)
    * Color Depth: 262K/65K (65K used)
    * Control chip:ILI9341
    * Display area 27.972 (H) x 32.634 (V)
    * Panel size 36.72(W)X48.96(H)mm
    * Logic voltage 3.3V
    * Touch panel with XPT2046 IC


Connections as setup in main.cpp test file.

| PinNum | Pin description | RPI | note |
| --- | --- | --- | --- |
| 1 | VCC | VCC | 3.3 or 5V ,CAUTION your display must have 3.3V regulator on back to connect to 5V |
| 2 | GND | GND | |
| 3 | CS | SPICE0 |TFT Chip select |
| 4 | RESET | GPIO25 |Reset, Use any GPIO for this, If no reset pin, pass -1 in here
& display will use software rst| 
| 5 | DC | GPIO24 | Data or command, Use any GPIO for this line |
| 6 | SDI(MOSI) | SPIMOSI | |
| 7 | SCLK | SPICLK | | 
| 8 | LED | VCC |CAUTION Your display may need current limit resistor|
| 9 | SDO(MISO) | nc |Only needed to read diagnostics from TFT (not implemented yet) |
| 10| T_CLK | SPICLK | |
| 11| T_CS | SPICE1 |XPT2046 Chip select |
| 12| T_DIN | SPIMOSI | |
| 13 | T_DO | SPIMISO | |
| 14 | T_IRQ | GPIO22 | |


1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. LED Backlight control is left to user.
3. Pins marked with T_ prefix are related to the touchscreen IC XP2046 if user is not using the touch
screen do not connect these. The touch screen and TFT share the SPI bus but have different chip select lines.
TFT SPI settings(Speed, active chip select) should be refreshed after ever read cycle of XPT2046 sensor, see example.


## Touchscreen

If the users display has a touchscreen controller IC on the back (XPT2046).
A simple class to interface with the XPT2046 touchscreen controller IC has been included.
Note :It may not be populated on your 
display or there may be a different model of IC there.

The T_IRQ line goes low when the touchscreen is touched and the data can be read from sensor.

Co-ord system returned by XPT_2046_RDL class is as follows:

1. Top left :    X = 1800 Y = 150
2. Top Right :   X = 150  Y = 150
3. Bottom Left : X = 1800 Y = 1800
3. Bottom Right : X = 150 Y = 1800

Output of the basic touch screen example included. 

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output.jpg)
