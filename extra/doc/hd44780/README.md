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
   for the Raspberry PI and other linux based SBC.
1. Dynamic install-able system level C++ library.
2. Backlight, scroll, cursor and entrymode control.
3. Custom character support + print class for numerical data.
4. Hardware I2C
5. Dependency: lgpio Library
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
LCD_I2C_FLAGS No flags are currently defined. This parameter should be set to zero.
The value to set LCD_I2C_DEVICE, can be found by running command, to view available I2C device numbers

```sh
i2cdetect -l
```

2. I2C Clock rate is set by device installed on. During development and testing on raspberry pi 5
it was found that by default the baudrate was set to 100,000 baud. By modifying the file 
/boot/firmware/config.txt baud can be increased 

```sh
param=i2c_arm=on,i2c_arm_baudrate=400000
```

3. In the event of an error writing a byte, debug info with error code will be written to console. 
Debug flag must be set to true to see this output. User can set error timeout between retry attempts and number of retry attempts 
and can monitor the Error flag.

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
