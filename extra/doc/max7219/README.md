# MAX7219 7 Segment Readme

[![ max image ](https://github.com/gavinlyonsrepo/MAX7219_7SEG_RPI/blob/main/extra/image/max.jpg)](https://github.com/gavinlyonsrepo/MAX7219_7SEG_RPI/blob/main/extra/image/max.jpg)

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
  * [Hardware](#hardware)
  * [Notes and Issues](#notes-and-issues)
	* [Cascaded Displays](#cascaded-displays)


## Overview

* Name: MAX7219 
* Author: Gavin Lyons.

* Description:
A Raspberry pi library to display data on a 8-digit MAX7219 seven segment module.
Dynamic install-able system level Raspberry Pi C++ library.
Hardware or software SPI, Shutdown mode, test mode and Brightness control supported.

## Software

### File system

Example files list:

| List No | Example file name  | Description | SPI type |
| ------ | ------ |   ------ | ----- |
| 1 | src/TESTS/main.cpp |  test sequence  | software |
| 2 | src/HELLOWORLD/main.cpp | Hello world | hardware & software user adjust|
| 3 | src/BCDMODE/main.cpp | Shows use of BCD built-in font  | hardware |
| 4 | src/CLOCK_DEMO/main.cpp |  Clock demo , Shows use of cascaded displays | hardware |
| 5 | src/CASCADE_DEMO/main.cpp | simple Demo showing use of cascaded displays | hardware |


## Hardware

### Software SPI

For Software SPI Pick any GPIO you want.
To get the number for GPIO_CHIP_DEVICE parameter you can check ls /dev/gpiochip*
At time of testing it was found that Raspberry pi 5 = 4 and raspberry pi 3 = zero 
But now it look like both are zero after software update, although 4 should still work for RPi5 
if a symlink is in /dev/ folder. In software SPI user may need to increase or decrease 
CommDelay variable (uS Communication delay) depending on speed of CPU on system. 

### Hardware SPI

For Hardware SPI the User must use fixed SPI pins SPIMOSI and SPISCLK, user can choice between SPICE0 and SPICE1 
for chip select. The Datasheet says it's a 10MHZ device, In hardware SPI user can pick SPI bus speed.

| parameter | default value | note  |
| --- | --- | --- |
| HWSPI_DEVICE | 0| A SPI device, >= 0. which SPI interface to use , ls /dev/spi*|
| HWSPI_CHANNEL | 0 |A SPI channel, >= 0. Which Chip enable pin to use usually 0 or 1|
| HWSPI_SPEED |  1000000| The speed of serial communication in bits per second.|
| HWSPI_FLAGS | 0|  mode 0 for this device |

### Connections to RPI:

| Pin no  | RPI SW SPI | RPI HW SPI  | Pin function |
| --- | --- | --- | --- |
| 1 | any GPIO output | SPISCLK |  CLK = Clock |
| 2 | any GPIO output | SPICE0 or SP1CE1 | CS = Chip select |
| 3 | any GPIO output | SPIMOSI |  DIN = Data in |


VCC 5V in theory but works at 3.3V in testing, albeit with a dimmer display.
Logic seems to work fine at 3.3V , It safe as it is all one way communications
From RPI to MAX. User can adjust brightness from 0x00 to 0x0f by default it is 0x08. 0x0f being brightest

## Notes and Issues

### Cascaded Displays

Support for Cascaded Displays added is untested as only one display available.
Casacded Displays are displays connected together. Din-> Dout and CS lines tied together.
