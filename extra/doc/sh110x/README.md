# SH110X OLED Readme

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
	* [File system](#file-system)
	* [I2C](#i2c)
	* [Bitmaps](#bitmaps)


## Overview

* Name : SH110X
* Description :

0. Library to support the I2C 128X64 OLED Display Module
   driven by the Sh1106 or Sh1107 controller for Linux SBC's
1. Dynamic install-able system level Raspberry Pi C++ library.
2. Inverse color, rotate, sleep, scroll and contrast control.
3. Bitmaps supported.
4. Hardware I2C


## Hardware

Manufacturers diagram showing connections.

[![ Wiring Image](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)

## Software

### File system

Not many examples files as I did not have device for test.
These where tested by a third party.

Example files 

| Controller | Filepath | File Function | Screen Size |
| ---- | ---- | ---- | ---- |
| Sh1106 | HELLO_WORLD | Basic use case | 128x64 |
| Sh1107 | HELLO_WORLD | Basic use case | 128x64 |

### I2C

Hardware I2C.

1. I2C Address is set by default to 0x3C(your module could be different, 
user can change argument passed into OLED class constructor). 
OLED_I2C_FLAGS No flags are currently defined. This parameter should be set to zero.
The value to set OLED_I2C_DEVICE , can be found by running command, to view available I2C device numbers

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

### Bitmaps

Different bitmaps methods can be used.

| num | Method name | data addressing | Notes |
| ------ | ------ | ------ | ------ |
| 1 | drawBitmap() |  Vertical | default, setDrawBitmapAddr(true) |
| 2 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/)
See example file "BITMAP" for more details.
