# ERM CH1115 OLED Readme

[![ OLED image](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/oled.jpg)](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/oled.jpg)

## Table of contents


  * [Overview](#overview)
  * [Output](#output)
  * [Hardware](#hardware)
  * [Software](#software)
      * [SPI](#spi)
      * [Bitmaps](#bitmaps)
      * [User adjustments](#user-adjustments)


## Overview

* Name : ER_OLEDM1_CH1115
* Description :

0. C++ Library to support the ER-OLEDM1.09-1 128X64 OLED Display Module
   driven by the CH1115 controller.
1. Linux based Single Board Computers C++ library.
2. Inverse colour, vertical rotate, sleep, fade effect, horizontal scroll and contrast control.
3. Bitmaps supported.
4. Hardware and Software SPI

* Author: Gavin Lyons
* Port of my Arduino [library link](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115).

## Output

[![ output image ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_PICO/blob/main/extra/image/all.jpg)](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_PICO/blob/main/extra/image/all.jpg)


## Hardware

CH1115 is a single-chip CMOS OLED driver with controller for organic light emitting diode dot-matrix graphic display system. CH1115 consists of 128 segments, 64 commons that can support a maximum display resolution of 128 X 64. It is designed for Common Cathode type OLED panel. ER-OLEDM1.09-1W-SPI is a White 1.09 inch OLED Display Panel with Breakout Board. This module is a combination of the two.(controller and OLED)

For SWSPI pick any GPIO you want for the 5 control lines.


| pin no | GPIO HWSPI | pin name | pin desc |
|-----|-----|-----|-----|
| 1 | n/a |Gnd | Ground |
| 2 | n/a |VCC | voltage supply |
| 3 | SPISCLK GPIO 11 |SCL | Serial clock input |
| 4 | SPIMOSI GPIO 10 |SDA | Serial data input |
| 5 | Any you want | RES | This pin is reset signal input.  |
| 6 | Any you want | DC | This pin is Data or Command control pin. |
| 7 | SPICEO GPIO 8 | CS | This pin is the chip select input.  |



[![ wiring ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ch1115.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ch1115.png)

## Software

Example files : 

| Examples files main.cpp  | Description |
| ------ | ------ |
| HELLO_WORLD | Hello world , Basic usage |
| BITMAP | Shows use of bitmaps methods  |
| TEXT_GRAPHICS_FUNCTIONS | Fonts graphics + misc functions |
| FPS_TEST |  Frames per second test  |
| FPS_TEST_SW_SPI | Frames per second test but with software SPI setup|

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
| GPIO_CHIP_DEVICE | 0| gpio chip device >= 0, check ls /dev/gpiochip* |

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
