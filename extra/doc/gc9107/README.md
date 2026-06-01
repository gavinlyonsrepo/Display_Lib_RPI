# GC9107 Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

* [Overview](#overview)
* [Software](#software)
  * [User Options](#user-options)
  * [File system](#file-system)
  * [Bitmap](#bitmap)
  * [Color codes](#color-codes)
  * [Advanced screen buffer mode](#advanced-screen-buffer-mode)
* [Hardware](#hardware)
* [Output](#output)
* [Notes](#notes)
  * [Error 99](#error-99)

## Overview

* Name: GC9107_LTSM
* Description:

Arduino C++ library for a TFT SPI LCD, GC9107 Driver IC.

The GC9107 is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with resolution of 128RGBx160dots, comprising a 384-channel source driver, a 160-channel gate driver, 46,080 bytes GRAM for graphic display data of 128RGBx160 dots, and power supply circuit.

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
9. Tested on 60x160 Bar Display

* [DataSheet](https://www.buydisplay.com/download/ic/GC9107.pdf)

## Software

### User Options

In the example files. There are sections in "setup()" function
where user can make adjustments to select for SPI type used, PCB type used and screen size & offsets.

0. USER OPTION 0 GPIO
1. USER OPTION 1 Screen section
2. USER OPTION 2 Offsets
3. USER OPTION 3 SPI settings

#### USER OPTION 0 SPI SPEED + GPIO + TYPE

This library supports both Hardware SPI and software SPI.
The SetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for SetupGPIO define which is used.

#### USER OPTION 1 Screen size + options

In USER OPTION 1.
User can adjust display size (width & height) in pixels, memory base
and colour order (RGB or BGR)

There are two memory base variants called "Display data GRAM mapping" in datasheet
Which I refer to as memory base A and B, These require different init routines.

1. 128x128 mapping (MEMORY_BASE_GM_128x128) A
2. 128x160 mapping (MEMORY_BASE_GM_128x160) B

#### USER OPTION 2 Screen offsets

In addition here are number of different truncated displays on market: These truncated displays require an offset.
Known displays types on market with GC9107 controller.
The offsets for untested devices are estimates and may be incorrect.

| Display Type | Memory Base | VRAM size setup (WbyH) | Offsets rotation | Tested |
| --- | --- | --- | --- | --- |
| 128x128 Square | MEMORY_BASE_GM_128x128 A | 128 , 128 | 0,0 | No device |
| 128x115 Round Truncated | MEMORY_BASE_GM_128x128 A | 128 , 128 | 7,0 | No device |
| 60x160 Truncated | MEMORY_BASE_GM_128x160 B | 128 , 160 | 34,0 | YES |
| 96x160 Truncated | MEMORY_BASE_GM_128x160 B | 128 , 160 | 16,0 | No device |
| 128x160 Full | MEMORY_BASE_GM_128x160 B | 128 , 160 | 0,0 | No device |

Example Offset calculation for a truncated display(60x160) is as follows.

[![ offsets ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc91.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc91.png)

#### USER OPTION 3 SPI settings

InitSPI function is overloaded(2 off, one for HW SPI the other for SW SPI).

| parameter | default value | note | SPi type |
| --- | --- | --- | --- |
| HWSPI_DEVICE | 0 | A SPI device, >= 0. which SPI interface to use , ls /dev/spi* | Hardware |
| HWSPI_CHANNEL | 0 | A SPI channel, >= 0. Which Chip enable pin to use usually 0 or 1 | Hardware |
| HWSPI_SPEED | 1000000 | The speed of serial communication in bits per second. | Hardware |
| HWSPI_FLAGS | 0 | mode 0 for this device | Hardware |
| GPIO_CHIP_DEVICE | 0 | gpio chip device >= 0, check ls/dev/gpiochip* | both |
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

| # | example sub folder name | Description | Note |
| --- | --- | --- | --- |
| 1 | hello_world | Basic use case | |
| 2 | tests | Tests bitmap, text,graphics & function testing | Uses Advanced Screen Buffer Mode |
| 3 | demos | System monitor demo(fake data) | |

### Bitmap

[Bitmap Documentation](../bitmap_16/README.md)

### Color codes

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

### Advanced screen buffer mode

Advanced screen buffer mode. There is advanced buffer mode where the code writes to a global screen buffer instead of the VRAM of display. It is off by default more details at readme,
which is in the 'doc' folder [at link.](../buffer_mode/README.md)
Example 'tests' uses this mode.

## Hardware

[![ display pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc92.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc92.png)

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC thru resistor | VCC thru resistor |
| 2 | CS | SPI_CE0 | GPI21 |
| 3 | DC | GPIO24 | GPIO24 |
| 4 | RESET | GPI025 | GPIO25 |
| 5 | SDA | SPI_MOSI | GPIO16 |
| 6 | SCLK | SPI_CLK | GPIO20 |
| 7 | VCC | VCC | VCC |
| 8 | GND | GND | GND |

1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI CEX, SCLK and SDA will be tied to SPI interface.
3. Backlight on/off control is left to user.

## Output

[![ Demo pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)

## Notes

### Error 99

[Error 99 Documentation](../error_99/README.md)
