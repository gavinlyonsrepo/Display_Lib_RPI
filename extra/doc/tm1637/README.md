# TM1637 Readme

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)

## Overview

* Display Name: TM1637
* Author: Gavin Lyons.
* Description: A Raspberry pi library to display data on TM1637 seven segment modules.

## Hardware

Connections to RPI:

1. GPIO  = CLK  = Clock
2. GPIO = DIO = Data input / output
3. VCC 3.3V (or 5V see Note)
4. GND


NOTE : If VCC is set to 5V, level shift GPIO to 3.3V or damage could result.

This library is tested on one variant of the TM1637 (model 4)
which has 4 digits with centred semi-colon , the library should work with other models
(six digit, decimal points, etc) but is untested.


Model 4: 

[![ model4 ](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/tm1637.jpg)](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/tm1637.jpg)


## Software

There is one example file. A table in file 'tm163X_font_data_RDL.cpp' provides ASCII to Seven Segment data.
. gfedcba (seven segments ) In the gfedcba representation,
a byte value of 0x06 would turn on segments "c" and "b",
which would display a "1". https://en.wikipedia.org/wiki/Seven-segment_display

### GPIO_CHIP_DEVICE

To get the number for GPIO_CHIP_DEVICE parameter you can check ls /dev/gpiochip*
At time of testing it was found that Raspberry pi 5 = 4 and raspberry pi 3 = zero 
But now it look like both are zero after software update, although 4 should still work for RPi5 
if a symlink is in /dev/ folder.

### Comms delay

The Serial Communications delay used is set by default to 75 uS user can change this in constructor parameters.
