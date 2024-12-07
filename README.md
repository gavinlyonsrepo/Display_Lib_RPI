[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

# Raspberry PI Display Library

[![logo image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/logo.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/logo.png)

## Table of contents

  * [Overview](#overview)
  * [Installation](#installation)
  * [Test](#test)
  * [Documentation](#documentation)
    * [Supported devices](#supported-devices)
    * [API documentation](#api-documentation)
    * [Fonts](#fonts)
  * [Software](#software)
    * [File-system](#file-system)
    * [Tool chain](#tool-chain)
  * [Notes](#notes)
    * [Older versions](#older-versions)


## Overview

* Project Name: Display_Lib_RPI
* Author: Gavin Lyons.
* Description:

0. A C++ Library to connect electronic displays to linux based single board computers.
1. Dynamic install-able Linux C++ library.
2. 16 fonts included, new Fonts can easily be added by user
3. Common graphics + print class included
4. Dependency: [lgpio C Library](https://abyz.me.uk/lg/lgpio.html), Provides SPI,I2C, and GPIO control.
5. Mutiple displays supported, see supported-devices, new components can be added.
6. Tested and developed on Raspberry Pis see tool chain for more details.

## Installation

1. Download & Install the dependency lgpio C Library, if not installed.
	* lgpio is a C library for Linux Single Board Computers(SBC) which allows control of the General Purpose Input Output pins. 
	* Install the lgpio, [Installation instructions here](https://abyz.me.uk/lg/download.html)

```sh
cd ~/Downloads
wget http://abyz.me.uk/lg/lg.zip
unzip lg.zip
cd lg
make
sudo make install
```

2. Download the Display_Lib_RPI library
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download latest release from github.

```sh
curl -sL https://github.com/gavinlyonsrepo/Display_Lib_RPI/archive/2.0.0.tar.gz | tar xz
```

3. Install library : Run 'make' and 'sudo make install' to run the makefile to build and then install library. 
	* It will be installed to usr/local/lib and usr/local/include by default. 
	* You can run 'make help' here to see other make options(uninstall etc).

```sh
cd Display_Lib_RPI-2.0.0
make
sudo make install
```


## Test

1. Next step is to test your display and installed library with the included test example files, connect up display.
2. Enter the example folder.
3. Edit the makefile in that folder to select the desired example file path for your Display.
		Simply edit "SRC" variable at top of the makefile. In the "User SRC directory Option Section" at top of file.
		Pick an example "SRC" directory path and One ONLY.
4. Run 'make' commmand. This builds the examples file using the just installed library,
		and creates a test executable file in "Bin".
5. Run 'make run' to run that built executable file. 
6. User should now see the test routine in that file running on the display. Run 'make help' to see other options.

```sh
cd examples
// edit the makefile in examples folder by picking the "SRC" file path you want to run
make
make run
```

## Documentation

### Supported devices

| Component name | Type | Interface | Readme URL link |
| ----- | ----- | ----- | ----- |
| ILI9341 | 16 bit Colour Graphic TFT LCD + touchscreen |SPI HW & SW| [Readme](extra/doc/ili9341/README.md)|
| ST7789 | 16 bit Colour Graphic TFT LCD |SPI HW & SW| [Readme](extra/doc/st7789/README.md)|
| ST7735 | 16 bit Colour Graphic TFT LCD |SPI HW & SW| [Readme](extra/doc/st7735/README.md)|
| ERM19264 UC1609| Bi colour Graphic LCD |SPI HW & SW| [Readme](extra/doc/erm19264/README.md) |
| Nokia 5110 PCD8544| Bi colour Graphic LCD |SPI HW & SW| [Readme](extra/doc/nokia5110/README.md) |
| ERM1 CH1115 | Bi colour Graphic OLED |SPI HW & SW| [Readme](extra/doc/ch1115/README.md) |
| SSD1306| Bi colour Graphic OLED | I2C | [Readme](extra/doc/ssd1306/README.md) |
| SH1106 SH1107| Bi colour Graphic OLED | I2C | [Readme](extra/doc/sh110x/README.md) |
| HD44780_PCF8574 | Character LCD | I2C | [Readme](extra/doc/hd44780/README.md) |
| TM1638 | LED 7 segment display| ~SPI SW | [Readme](extra/doc/tm1638/README.md) |
| MAX7219 | LED 7 segment display | SPI HW & SW| [Readme](extra/doc/max7219/README.md) |

### API Documentation

[The application programming interface documentation is at link](https://gavinlyonsrepo.github.io/misc/software_docs/Display_Lib_RPI/index.html) 
 hosted on github pages and generated by Doxygen.

### Fonts

The font system readme for the graphic displays [is here at link.](extra/doc/fonts/README.md)

## Software

### File system

There are 2 makefiles.

1. Root directory, builds and installs library at a system level.
2. Example directory, builds a chosen example file using installed library to an executable.
which can then be run.  An editable list of file paths to examples can be found in makefile.

Library naming :

1. library name = librpidisplaygl
2. Linker flags for complier = -lrpidisplaygl (also needs -llgpio for lgpio library)
3. Library File suffix  = RDL
4. Project name = Display_Lib_RPI
5. Installed size = ~330 KiB

Basic project overview, see API documentation for more detailed diagrams :

[![Overview image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/diagram.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/diagram.png)

### Tool chain

* Development Tool chain.
	1. Raspberry PI 3 model b & Raspberry PI 5 
	2. C++, g++ (Debian 12.2.0)
	3. Raspbian , Debian 12 bookworm OS, 64 bit.
	4. lgpio library Version Number :: 131584

## Notes

### Older versions

The last version of Display_lib_RPI (V1.3.0) which used the bcm2835 library as a dependency low level interface 
is in releases and can still be downloaded and used. Version 1.3.0 will NOT work on raspberry pi 5 and uses direct register access.
It is generally speaking faster than the lgpio. The lgpio replaced it in version 2.0.0.
The lgpio library works with linux device drivers like spidev.
Using Linux device drivers allow the library to work on Raspberry 5 as well as other Linux Single Board Computers that lgpio can be installed on.
This provides more Flexibility, portability, better API, user space access,better compatibility and other advantages.

