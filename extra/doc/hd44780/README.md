# HD44780 LCD Readme


[![ lcd image](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/LCDPCF.jpg)](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/LCDPCF.jpg)

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
	* [I2C](#i2c)
	* [Debug](#debug)
	* [Example files](#example-files)
  * [Output](#output)
  * [Notes and issues](#notes-and-issues)
 

## Overview

* Name : HD44780_LCD
* Description :

0. Library to support the HD44780 LCD , (I2C PCF8574 "backpack" interface) 
   for the Raspberry PI.
1. Dynamic install-able system level Raspberry Pi C++ library.
2. Backlight, scroll, cursor and entrymode control.
3. Custom character support + print class for numerical data.
4. Hardware I2C using bcm2835 library
5. Dependency: bcm2835 Library
6. Tested on size 16x02 and 20x04 (but may work on other sizes eg 16x04 , untested)

* Author: Gavin Lyons

## Hardware

Connections 

1. LCD SCLK = SCLK1 I2C pins P1-05 GPIO3
2. LCD SDA = SDA1 I2C pins P1-03 GPIO2

## Software 

### I2C

Hardware I2C.

1. I2C Address is set by default to 0x27(your module could be different, 
user can change argument passed into LCD class constructor).

2. I2C Clock rate can be a passed into in the LCD class constructor method as a argument, 
User can pass 1 of 4 BCM2835_I2C_CLOCK_DIVIDER values 2500, 626 150 or 148.
See image below.

3. In the event of an error writing a byte, debug info with error code will be written to console. 
This error code is the bcm2835I2CReasonCodes enum. Debug flag must be set to true to see this output.
See image below for  bcm2835I2CReasonCodes. User can set error timeout between retry attempts and number of retry attempts 
and can monitor the Error flag to see current bcm2835I2CReasonCodes.

4. If you have multiple devices on I2C bus at different clock speeds.
The I2C clock speed function may have to called before each tranche of LCD commands.
and not just at start. 

[![ bcm image](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/bcm.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/bcm.jpg)

For more info on bcm2835I2CClockDivider & bcm2835I2CReasonCodes see [bcm2835 doc's for details](http://www.airspayce.com/mikem/bcm2835/group__constants.html)

### Debug

User can turn on debug messages with LCDDebugSet method see example file.

### Example files

| Filepath | File Function | Screen Size |
| ---- | ---- | ---- | 
| TEST_16x02 | Carries out test sequence testing features | 16x02 |
| TEST_20x04 | Carries out test sequence testing features | 20x04 |
| CLOCK_16x02 | A basic clock Demo | 16x02 |
| TEST_I2C_16x02 | Check I2C connection | 16x02 |

## Output

Output  of custom character test in test file.

[![ op image ](https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/blob/main/extras/image/custom_output.jpg)](https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/blob/main/extras/image/custom_output.jpg)

20x04 display. 

[![ op image 2 ](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574/blob/main/extras/image/2004.jpg)](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574/blob/main/extras/image/2004.jpg)

## Notes and issues

1. For detailed graphical description of entry modes , cursor types, custom characters 
and more see here [link](http://dinceraydin.com/lcd/commands.htm)

2. 16X04 board not tested but should work
