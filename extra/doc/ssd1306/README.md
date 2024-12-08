# SSD1306 OLED Readme

[![ OLED image ](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/device.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/device.jpg)

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
  * [Software](#software)
	* [File system](#file-system)
	* [I2C](#i2c)
	* [Bitmaps](#bitmaps)
  * [Output](#output)
  * [Notes and issues](#notes-and-issues)


## Overview

* Name : SSD1306_OLED
* Description :

0. Library to support the I2C 128X64 OLED Display Module
   driven by the SSD1306 controller for Linux based SBC.
1. Dynamic install-able system level C++ library.
2. Inverse color, rotate, sleep, scroll and contrast control.
3. Bitmaps supported.
4. Hardware I2C
5. Also tested on 128X32 display size. Should work for 96X16 display size.


## Hardware

Manufacturers diagram showing connections.

[![ Wiring Image](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI/blob/main/extras/image/wiring.jpg)

## Software

### File system

Example files 

| Filepath | File Function | Screen Size |
| ---- | ---- | ---- |
| HELLO_WORLD | Basic use case | 128x64 |
| BITMAP  | Shows use of bitmaps | 128x64 |
| CLOCK_DEMO | A basic clock Demo | 128x64 |
| SPEED_TEST | Frame rate per second test | 128x64 |
| TEXT_GRAPHICS_FUNCTIONS |Tests Text,graphics and functions| 128x64 |

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

## Output

[![ Output image ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1306.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1306.jpg)

[![ Output image 2 ](https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO/blob/main/extra/image/3.jpg)](https://github.com/gavinlyonsrepo/SSD1306_OLED_PICO/blob/main/extra/image/3.jpg)

## Notes and issues

1. To test on a different size of display edit the my_OLED_width & my_OLED_height
variables in examples files.

| Display size | Supported | Tested |
| ------ | ------ | ------ |
| 128x64 | Yes | Yes |
| 128x32 | Yes | Yes |
| ???x16 | Yes | NO |
