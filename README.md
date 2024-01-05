
![ OLED ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/oled.jpg)

# ER_OLEDM1_CH1115_RPI

## Table of contents

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Test](#test)
  * [Hardware](#hardware)
  * [Software](#software)
	* [API](#api)
	* [SPI](#spi)
	* [Buffer](#buffer)
	* [Fonts](#fonts)
	* [Bitmaps](#bitmaps)
	* [User adjustments](#user-adjustments)
  * [Notes and Issues](#notes-and-issues)


# ER_OLEDM1_CH1115_RPI

## Overview

* Name : ER_OLEDM1_CH1115_RPI
* Description : 

0. C++ Library to support the ER-OLEDM1.09-1 128X64 OLED Display Module 
   driven by the CH1115 controller for the Raspberry PI.
1. Raspberry Pi C++ library.      
2. Inverse colour, vertical rotate, sleep, fade effect, horizontal scroll and contrast control. 
3. 10 fonts included 
4. Graphics class included.
5. Polymorphic print class included.
6. Bitmaps supported.
7. Hardware and Software SPI 

* Development Tool chain. 
	1. Raspberry PI 3 model b
	2. C++, g++ (Debian 12.2.0) 
	3. Raspbian , Debian 12 bookworm OS, , 64 bit.
	4. kernel : aarch64 Linux 6.1.0-rpi7-rpi-v8
	5. [bcm2835 Library 1.73 dependency](http://www.airspayce.com/mikem/bcm2835/). Provides low level SPI bus, delays and GPIO control.
    
* Author: Gavin Lyons
* Port of my Arduino [library link](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115).


## Output

Output Screenshots, From left to right, top to bottom.

1. Full screen bitmap displayed.
2. Text + graphics
3. Clock Demo + bitmaps.
4. Different size and inverted text.
5. Available ASCII font printed out 0-127
6. Extended ASCII font printed out 128-255
7. Font 1-4 
8. Font 7 

![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/output.jpg)
![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/blob/main/extras/image/fontpic.jpg)

## Installation


1. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.73)
	* The bcm2835 library is a dependency and provides SPI bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

2. Download the ER_OLEDM1_CH1115_RPI library 
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.
    
```sh
curl -sL https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/archive/1.3.3.tar.gz | tar xz
```

3. Run "make" to run the makefile at repo root folder to install library, it will be 
    installed to usr/lib and usr/include
    
```sh
cd ER_OLEDM1_CH1115_RPI-1.3.3
make
sudo make install
```

## Test

1. Next step is to test OLED and installed library with an example.
Wire up your OLED. Next enter the examples folder and run the makefile in THAT folder, 
This makefile builds the examples file using the just installed library.
and creates a test exe file in "bin". Be sure to use "sudo" as the bcm2835 requires root permissions by default [ see here for more details on that](http://www.airspayce.com/mikem/bcm2835/) 
The default example file is "hello world",  user should see hello world on the OLED
by end of this step.

```sh
cd examples/
make
make run
```

2. There are 7 examples files to try out. 
To decide which one the makefile builds simply edit "SRC" variable at top of the makefile in examples folder.
in the "User SRC directory Option Section". Pick an example "SRC" directory path and ONE ONLY.
Comment out the rest and repeat 1.

| Examples files main.cpp  | Description |
| ------ | ------ | 
| HELLO_WORLD | Hello world , Basic usage | 
| BITMAP | Shows use of bitmaps methods  | 
| OLED_FUNCTIONS | misc functions, rotate, flip, scroll,  etc|
| TEXT | Fonts + text modes test | 
| GRAPHICS |  graphics test | 
| FPS_TEST |  Frames per second test  | 
| FPS_TEST_SW_SPI | Frames per second test but with software SPI setup| 

## Hardware

CH1115 is a single-chip CMOS OLED driver with controller for organic light emitting diode dot-matrix graphic display system. CH1115 consists of 128 segments, 64 commons that can support a maximum display resolution of 128 X 64. It is designed for Common Cathode type OLED panel. ER-OLEDM1.09-1W-SPI is a White 1.09" OLED Display Panel with Breakout Board. This module is a combination of the two.(controller and OLED)

For SWSPI pick any GPIO you want for the 5 control lines.

| pin no| GPIO HWSPI | pin name | pin desc | 
|---|---|---| ---| 
| 1 | n/a |Gnd | Ground |
| 2 | n/a |VCC | voltage supply |
| 3 | SPISCLK GPIO 11 |SCL | Serial clock input |
| 4 | SPIMOSI GPIO 10 |SDA | Serial data input |
| 5 | Any you want | RES | This pin is reset signal input.  |
| 6 | Any you want | DC | This pin is Data or Command control pin. |
| 7 | SPICEO GPIO 8 | CS | This pin is the chip select input.  |


![ wiring ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/blob/main/extras/image/wiring.png)

## Software

There are two makefiles

    1. Makefile at root directory builds and installs library at a system level.
    2. Makefile in example directory build chosen example file to an executable.


### API

The Software is commented mostly for"doxygen" and if users uses "doxygen" software
an API document  can be generated. A Doxyfile is in doc sub folder in repository.

### SPI

This library supports both Hardware SPI and software SPI.
The constructor of the main class is overloaded.
Which SPI is started depends on which constructor called by user.
Hardware SPI uses bcm2835 library spi module.
The SPI settings are in OLEDSPIHWSettings function.
If there are multiple SPI devices on Bus(with different settings) the user can call this method 
to refresh settings before a tranche of OLED commands.
Default Speed is BCM2835_SPI_CLOCK_DIVIDER_64,
6.25MHz on RPI3. This can be adjusted in code or you can pass
the divider value in the begin method as an argument. These values are
defined by enum bcm2835SPIClockDivider. For full list see

![bcm2](https://github.com/gavinlyonsrepo/NOKIA_5110_RPI/blob/main/extra/images/bcm2.png)

User can also adjust which HW SPI chip enable pin they use(CE0 or CE1).
in begin method  arguments. This device uses bit order MSB First and SPI Mode 0.

Measured SPI speed results Version 1.3 are in the FPS test example file header comment block.
The member OLEDHighFreqDelay is a GPIO uS delay that
can be used to slow down or speed up Software SPI.
By default it is set to 0uS. It may have to be increased depending on device/CPU used.


### Fonts

Font data table:

| num | enum name | Char size XbyY | ASCII range | Size bytes | Scale-able |
| ------ | ------ | ------ | ------ |  ------ | ----- |
| 1 | $_Default | 5x8 |0-0xFF, Full Extended|1275 |Y|
| 2 | $_Thick   | 7x8 |0x20-0x5A, no lowercase letters |406|Y|
| 3 | $_SevenSeg  | 4x8 |0x20-0x7A |360|Y|
| 4 | $_Wide | 8x8 |0x20-0x5A, no lowercase letters|464|Y|
| 5 | $_Tiny | 3x8 |0x20-0x7E |285|Y|
| 6 | $_Homespun  | 7x8 |0x20-0x7E |658|Y|
| 7 | $_Bignum | 16x32 |0x2D-0x3A ,0-10 - . / : |896|N|
| 8 | $_Mednum | 16x16 |0x2D-0x3A ,0-10 - . / :|448|N|
| 9 | $_ArialRound| 16x24 | 0x20-0x7E |4608|N|
| 10 | $_ArialBold | 16x16 |0x20-0x7E |3072|N|

1. $ = OLEDFontType
2. A print class is available to print out many data types.
3. Fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
4. Font 7-8 are large numerical fonts and cannot be scaled(just one size).
5. Fonts 9-10 large Alphanumeric fonts and cannot be scaled(just one size)

Font Methods:

| Font num | Method | Notes |
| ------ | ------ | ------ |
| 1-6 | drawChar| draws single  character |
| 1-6 | drawText | draws character array |
| 7-10 | drawCharBigFont | draws single  character |
| 7-10 | drawTextBigFont | draws character array |
| 1-10 | print | Polymorphic print class which will print out many data types |

These methods return false in event of an error, such as wrong font chosen , ASCII character outside
chosen fonts range, character out of screen bounds and invalid character array pointer object.

### Bitmaps

There is a few different ways of displaying bitmaps, 

| Num | Method |  Data addressing | Note |
| ------ | ------  | ------ |  ------ |  
| 1 | OLEDBitmap() |  Vertical |  Writes directly to screen , no buffer used. | 
| 2 | OLEDBufferScreen() |  Vertical  | For internal use mostly | 
| 3 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) | 
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

The drawBitmap method will return an error if : The Bitmap is completely off screen , 
Invalid Bitmap pointer object, bitmap bigger than screen , bitmap bigger/smaller than provided width and height calculation
( This helps prevents buffer overflow).
A vertical addressed Bitmap's height must be divisible by 8. eg, for a full screen bitmap with width=128 and height=64.
Bitmap excepted size = 128 * (64/8) = 1024 bytes.
A horizontal addressed Bitmap's width must be divisible by 8. eg, for a bitmap with width=88 and height=48.
Bitmap excepted size = (88/8) * 48 = 528 bytes.

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) 
See example file "BITMAP" for more details.


### User adjustments

When the user calls OLEDbegin() to start OLED they can specify a contrast setting from 0x00 to 0xFF. Datasheet says 0x80 is default. User can also change contrast on the fly.
Screen can be disabled to enter a sleep type mode where OLED draws 500uA.
Screen pixel colour can be inverted and also screen can be vertically rotated. 

There is a "fade or breath display effect". Whose parameters(time brightness) can be adjusted by passing data to function see "OLEDfadeEffect" function header  in .cpp and datasheet for details.
default is 0x81.

There is a  Horizontal scroll effect. Whose parameters(TimeInterval , direction , mode)
can be adjusted by passing data to function see "OLEDscrollSetup" function header in .cpp and datasheet for details. defaults are in header file are 6 frames , right , continuous mode.

## Notes and Issues


