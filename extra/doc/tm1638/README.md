# TM1638 Readme

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)

## Overview

* Display Name: TM1638
* Author: Gavin Lyons.
* Description: A Raspberry pi library to display data on 8-digit TM1638 seven segment modules.

## Hardware

Connections to RPI:

1. GPIO = STB = Strobe
2. GPIO  = CLK  = Clock
3. GPIO = DIO = Data input / output
4. GND
5. VCC 3.3V (or 5V see Note)

NOTE : If VCC is set to 5V, level shift GPIO to 3.3V or damage could result.

This library supports three variants of the TM1638,
which for purposes of this documentation,
will be named Model 1 ,Model 2 and Model 3.
Pictured at from left to right.

| Model No | Module Name | LEDS | Push buttons |
| ------ | ------ |  ------ | ------ |
| Model 1 | TM1638 LED & KEY | 8 red only | 8 |
| Model 2 | TM1638 KEYS, QYF  | 0 | 16 |
| Model 3 | TM1638 V1.3 or LKM1638  | 8 bi color,  red and green  | 8 |

[![ module pics ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16383.jpg)](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16383.jpg)

Library uses a type of software SPI like bus.

## Software

This library section is a port of my Arduino Library.

[ Arduino library github Link ](https://github.com/gavinlyonsrepo/TM1638plus)

There are two test example files, One does model 1 and model 3 , the other model 2.
In the former file: at top of file there is a 'User selection section' which lets user swap between model 1 & 3.

### GPIO_CHIP_DEVICE

To get the number for GPIO_CHIP_DEVICE parameter you can check ls /dev/gpiochip*
At time of testing it was found that Raspberry pi 5 = 4 and raspberry pi 3 = zero 
But now it look like both are zero after software update, although 4 should still work for RPi5 
if a symlink is in /dev/ folder.

### Comms delay

Communications optional delay.
It may be necessary to adjust the constant  TMCommDelay in the TM1638plus_common.h file. It is Microsecond delay used in communications clocking, it is currently set to 1, 
It can be set to 0 or higher. On a different CPU Frequency to one tested, it may be necessary to increase/decrease this.
The user can do this with the Getter method provided. TMCommDelayGet.
