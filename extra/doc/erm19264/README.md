# ERM19264 UC1609 Readme

[![ ERM19264 image](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

## Table of contents

  * [Overview](#overview)
  * [Output](#output)
  * [Hardware](#hardware)
  * [Software](#software)
	* [SPI](#spi)
	* [Bitmaps](#bitmaps)
	* [User Adjustments](#user-adjustments)


## Overview

* Name : ERM19264_UC1609
* Title : Library for ERM19264-5 v3 LCD (UC1609C controller)
* Description :

1. Dynamic install-able system level Linux based Single Board Computers C++ library.
2. Inverse, Scroll, rotate and contrast control.
3. Sleep mode.
4. Bitmaps supported.
5. Hardware & software SPI options

* Author: Gavin Lyons

* This is a port of my [Arduino library](https://github.com/gavinlyonsrepo/ERM19264_UC1609)

## Output

[![ output image ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/erm19264.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/erm19264.jpg)

## Hardware

9 pins , Vcc and GND, anode and cathode for the backlight LED and an SPI interface.
The backlight control is left up to user , connect to 3.3V thru a resistor.
If using Hardware SPI 3 pins will be tied to the SPI CEO, CLK and MOSI lines. if using software SPI you should be able use any GPIO you want for all five pins. Datasheets URLS are in the extras folder.

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)

1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

Wiring Diagram from supplier showing connection to RPI.

[![ Wiring image](https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI/blob/main/extras/image/wiring.png)](https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI/blob/main/extras/image/wiring.png)

This wiring Diagram from the manufacturer showing hardware setup connected to an ~8051 MCU, showing both 3.3 volt and 5 volt systems.
Note status of J1 PCB jumper.

[![ Connection image ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/connect.jpg)](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/connect.jpg)

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
The member OLEDHighFreqDelay is a GPIO uS delay that
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

### Bitmaps

There is a few different ways of displaying bitmaps, 

| Num | Method | Data addressing | Note |
| ------ | ------ |  ------ |  ------ |  
| 1 | LCDBitmap() |  Vertical | Writes directly to screen , no buffer used. | 
| 2 | LCDBuffer() |   Vertical  | For internal use mostly | 
| 3 | buffer init  |  Vertical  |  Can be used when initialising  at start up| 
| 4 | drawBitmap() | Vertical | default,  setDrawBitmapAddr(true) | 
| 5 | drawBitmap() |   Horizontal | setDrawBitmapAddr(false) |

See the bitmap example file for more details on each method. Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) , Bitmaps should be defined as const  buffers non-const, for methods 3 buffer can be initialised with bitmap data.

### User Adjustments

If the LCD does not initialize correctly. 
Try adjusting the RAM address control setting from 0x02 to 0x01. It can be set in LCDbegin() argument list.
See github issue 4 at the Arduino source port github repository for more details, link in overview.

When the user calls LCDbegin() to start LCD they can specify a contrast setting from 0x00 to 0xFF.
Datasheet says 0x49 is default. (VbiasPOT). Lower contrast works better on the blue color version.

It is also possible for user to change LCD bias ,  Temperature coefficient, frame rate and power control but this must be done by changing defines in header file. Choose lower frame rate for lower power, and choose higher frame rate to improve LCD contrast and minimize flicker. See Data sheet for range of values here. Defaults where found to be fine during all testing of this library.

| Parameter | default Values |  Define | Register Bits|
| ------ | ------ |  ------ | ------ |
| LCD bias |  9 | BIAS_RATIO_SET | BR 1:0 |
| Temp coefficient | -0.00%/ C |  TEMP_COMP_SET | TC 1:0  |
| Frame rate | 95 fps |  FRAMERATE_SET |  LC 4:3 |
| Power control | 1.4mA + Internal VLCD (7x charge pump) |  PC_SET | PC 2:0 |
| V bias Bot(contrast) | 0x49 |  Set by user with LCDbegin | PM 7:0 |
| Ram Address Control | 0x02 |  Set by user with LCDbegin  | AC 2:0 |
