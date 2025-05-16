# GC9A01A Readme Readme

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system)
      * [Bitmap](#bitmap)
      * [Color codes](#color-codes)
      * [Advanced screen buffer mode](#Advanced-screen-buffer-mode)
  * [Hardware](#hardware)
  * [Output](#output)
  * [Notes](#notes)
     * [Error 99](#error-99)

## Overview

* Name: GC9A01A
* Description:

0. C++ Library for a TFT SPI LCD, GC9A01A Driver
1. Dynamic install-able Raspberry Pi C++ library.
2. Power modes, Invert , Scroll, Rotate, Bitmaps supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color, bi-color Bitmaps & sprites supported.
5. Hardware and Software SPI
6. Dependency: lgpio Library
7. NB The Spidev buffer size setting must be larger than biggest buffer the code will attempt
    to write If not you will get 'Error 99'. See Notes section for more details.
8. Make sure SPI is enabled in your computers configuration.
9. Tested on 240x240 pxiel Round Display

* Author: Gavin Lyons

* [DataSheet](https://buydisplay.com/download/ic/GC9A01A.pdf)

## Software

### User options

In the example files. There are 3 sections in "Setup()" function 
where user can make adjustments to select for SPI type used, and screen size.

1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 SPI SETTINGS

*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The SetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for SetupGPIO define which is used.


*USER OPTION 2 Screen size*

User can adjust screen pixel height, screen pixel width.
The function InitScreenSize sets them.

*USER OPTION 3  SPI Settings*

InitSPI function is overloaded(2 off, one for HW SPI the other for SW SPI).

| parameter | default value | note | SPi type |
| --- | --- | --- |  --- |
| HWSPI_DEVICE | 0| A SPI device, >= 0. which SPI interface to use , ls /dev/spi*|  Hardware  |
| HWSPI_CHANNEL | 0 |A SPI channel, >= 0. Which Chip enable pin to use usually 0 or 1| Hardware  |
| HWSPI_SPEED |  1000000| The speed of serial communication in bits per second.| Hardware  |
| HWSPI_FLAGS | 0|  mode 0 for this device | Hardware  |
| GPIO_CHIP_DEVICE | 0| gpio chip device >= 0, check ls/dev/gpiochip* | both |
| SWSPI_CommDelay | 0 | uS delay for GPIO | software | 

The user can adjust if  having reliability issues with SW SPI in some setups.
This is a microsecond delay in SW SPI GPIO loop. It is set to 0 by default, Increasing it will slow 
down SW SPI further.

### File system

In example folder:
The Main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder.

| # | example file name  | Desc|
| ------ | ------ |  ------ |
| 1 | hello_world| Basic use case |
| 2 | text_graphic_functions | Tests text,graphics & function testing  |
| 3 | bitmap_Tests | bitmap | bitmap testing|
| 4 | hello_world_swspi | Basic use case, software SPI |
| 5 | demos | Various demos |

### Bitmap

Functions to support drawing bitmaps

| Function Name | Colour support | Pixel size KiB|  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour array | (0-240) X 8  | Data vertically addressed |
| drawBitmap | bi-colour array |  7 | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 BMP files or array | 112.5 | ------ |
| drawBitmap24  | 24 bit color BMP files or array  | 168.8 | Converted by software to 16-bit color  |
| drawSprite| 16 bit color 565 array | 112.5 | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Pixel size column assumes 240 by 240 bitmap.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 5  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)
5. For 3 and 4 better just to use bmp files direct from file system see examples.

These class functions will return an error code in event of error see API for more details.

###  Color codes 

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

### Advanced screen buffer mode

Advanced screen buffer mode. There is advanced buffer mode where the code writes to a global screen buffer instead of the VRAM of display. It is off by default more details at readme, 
which is in the 'doc' folder [at link.](../buffer_mode/README.md)

## Hardware

[![pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)

Connections as setup in main.cpp test file.

| Pin desc |  HW SPI |
| --- | --- |
| GND Ground | GND |
| VCC |  3.3 VCC  |
| SCL Clock | GPIO 18 |
| SDA Data in | GPIO 19 |
| RST Reset| GPIO 4 |
| DC Data or command | GPIO 3  |
| CS Chip select | GPIO 2 |
| BL Backlight  | VCC |

1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
3. Backlight on/off control is left to user.

## Output

[![ Demo pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)


## Notes

### Error 99

[Error 99 Documentation](../error_99/Readme.md)

