# GC9A01A Readme

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
1. Dynamic install-able Linux C++ library.
2. Power modes, Invert , Scroll, Rotate, Bitmaps supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color, bi-color Bitmaps & sprites supported.
5. Hardware and Software SPI
6. Dependency: lgpio Library
7. NB The Spidev buffer size setting must be larger than biggest buffer the code will attempt
    to write If not you will get 'Error 99'. See Notes section for more details.
8. Make sure SPI is enabled in your computers configuration.
9. Tested on 240x240 pixel round display.

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

In example sub folders:
The main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder.
Hello world can be setup for software or hardware SPI by changing "HardwareSPI" at top of file.

| # | example sub folder name  | Description |
| ------ | ------ |  ------ |
| 1 | hello_world | Basic use case |
| 2 | tests | Tests bitmap, text,graphics & function testing |
| 3 | demos | Various demos |

### Bitmap

[Bitmap Documentation](../bitmap_16/README.md)

###  Color codes 

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

### Advanced screen buffer mode

Advanced screen buffer mode. There is advanced buffer mode where the code writes to a global screen buffer instead of the VRAM of display. It is off by default more details at readme, 
which is in the 'doc' folder [at link.](../buffer_mode/README.md)

## Hardware

[![pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)

Connections as setup in main.cpp test file.
Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC |  VCC |
| 2 | CS | SPI_CE0 | GPI21 |
| 3 | DC | GPIO24 | GPIO24  |
| 4 | RESET | GPI025  | GPIO25 |
| 5 | SDA | SPI_MOSI | GPIO16 |
| 6 | SCLK | SPI_CLK | GPIO20 |
| 7 | VCC | VCC | VCC  |
| 8 | GND | GND | GND |

1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI CEX, SCLK and SDA will be tied to SPI interface.
3. Backlight on/off control is left to user.


## Output

[![ Demo pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)


## Notes

### Error 99

[Error 99 Documentation](../error_99/README.md)

