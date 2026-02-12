# GC9D01N Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

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

* Name: GC9D01N
* Author: Gavin Lyons
* Description:

C++ library for a TFT SPI LCD, GC9D01N Driver.

GC9D01N is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with
resolution of 160RGBx160 dots, comprising a 240-channel source driver, a 32-channel gate
driver, 57,600 bytes GRAM for graphic display data of 160RGBx160 dots, and power supply
circuit.

0. Dynamic install-able Linux C++ library.
1. Power modes, Invert, Scroll, Rotate supported.
2. Hardware & software SPI
3. Tested on 160x160 Round Display
also tested by third party on 40x160 bar display by third party.
4. Raspberry Pi PICO C++ SDK eco-system library.
5. 16 ASCII fonts included, fonts can easily be removed or added.
6. 24 bit colour , 16 bit color, bi-color Bitmaps & sprites su
7. Dependency: lgpio Library
8. NB The Spidev buffer size setting must be larger than biggest buffer the code will attempt
    to write If not you will get 'Error 99'. See Notes section for more details.
9. Make sure SPI is enabled in your computers configuration.

* [DataSheet](https://buydisplay.com/download/ic/GC9D01N.pdf)

## Software

### User Options

In the example files. There are sections in "Setup()" function 
where user can make adjustments to select for SPI type used, and screen size.

1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 SPI SETTINGS

*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The SetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for SetupGPIO define which is used.

*USER OPTION 2 Screen size*

There are options here user can adjust:

1. 2A screen pixel height
2. 2B screen pixel width
3. 2C Resolution and gate type.
4. 2D screen pixel X offset
5. 2E screen Pixel Y offset

USER OPTION 2C- Resolution & Gate Configuration (Resolution_e)

The GC9D01 controller supports multiple display sizes and gate driving modes.
Select the correct enum value for your physical module.
In addition to passing the parameter.

| Enum Value | Resolution | Gate Mode | Typical Module Description |
| ---------------------- | ---------- | ------ | --- |
| `RGB160x160_DualGate` | 160×160 | Dual | Most round GC9D01 displays **Default** |
| `RGB120x160_DualGate` | 120×160 | Dual | Some rectangular / bar-type modules |
| `RGB80x160_SingleGate` | 80×160 | Single | Smaller rectangular / bar displays |
| `RGB40x160_SingleGate` | 40×160 | Single | Very narrow bar-type or specialty modules |

USER OPTION 2D & 2E

On some displays an offset is required, I made this a user input
rather than hard-coded due to diversity of variant modules available.

| Display type | X offset | Y offset | Notes |
| ------------- | -------- | -------- | ----- |
| `160x160 Dual Gate` | 0 | 0 | No offsets required in any rotation. Default |
| `120x160 Dual Gate` | TBD | TBD | Expected to require centering offsets (~20 px) depending on rotation. Not hardware tested. |
| `80x160 Single Gate` | TBD | TBD | Expected to require centering offsets (~40 px) depending on rotation. Not hardware tested. |
| `40x160 Single Gate` | -60 | 60 | **Tested**. Offsets required for 90° and 270° rotations due to centered 40-pixel active area. |

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

The user can adjust if having reliability issues with SW SPI in some setups.
This is a microsecond delay in SW SPI GPIO loop. It is set to 0 by default, Increasing it will slow 
down SW SPI further.


### File system

In example folder:
The main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder.

| # | example file name  | Description |
| ------ | ------ |  ------ |
| 1 | hello_world| Basic use case |
| 2 | tests |  bitmap, text,graphics & function testing  |
| 3 | demos | Various demos |

The Hello_world has an option to switch between hardware and software spi.
It also has a ''USER SCREEN TYPE SECTION'' where user can switch between
1. 160X160 Dual gate display test
2. 40X160 single gate display test

*Note:* All examples for GC9D01 are for Advanced Screen Buffer Mode.
Advanced buffer mode must be enabled for these examples to work.
This mode is non-default. This is done in examples by: function SetupBufferMode().

Some examples of memory usage for the frame buffer used in advanced buffer mode for selected display sizes.

| Display size |  bytes |
| ---- | --- |
| 40x160x2 | 12800 |
| 160x160x2 | 51200 |

### Bitmap

[Bitmap Documentation](../bitmap_16/README.md)

These class functions will return an error code in event of error see API for more details.

###  Color codes 

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

### Advanced screen buffer mode

Advanced screen buffer mode. There is advanced buffer mode where the code writes to a global screen buffer
instead of the VRAM of display. It is off by default more details at readme, 
which is in the 'doc' folder [at link.](../buffer_mode/README.md)

## Hardware

[![pic ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/gc2.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/gc2.jpg)

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
4. If no Reset pin present: pass -1 as argument for Reset pin number in SetupGPIO()functions

## Output

160x160 Round Display:

[![pic2 ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)

40x160 Bar Display:

[![pic4 ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output2.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output2.jpg)

## Notes

### Error 99

[Error 99 Documentation](../error_99/README.md)
