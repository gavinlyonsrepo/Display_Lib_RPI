# HT16K33 Readme

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
	* [Seven segment](#seven-segment)
	* [Nine segment](#nine-segment)
	* [Fourteen segment](#fourteen-segment)
	* [Sixteen segment](#sixteen-segment)
  * [Software](#software)
	* [Example files](#example-files)
	* [I2C](#i2c)


## Overview

* Display Name:HT16K33
* Author: Gavin Lyons.
* Description:

A Raspberry pi library to display data on LED segment modules using the HT16K33 controller module.
The Library supports 7, 9, 14, and 16 segment displays. The LED segment displays must be
common cathode. At present the library does not support keyscan or some
 custom products on market(such as backpacks with semi-colons),
this is for the IC module pictured driving standard common cathode LED segments.

## Hardware

[![module ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ht.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ht.jpg)

The HT16K33 is a RAM Mapping 16*8 LED Controller Driver with keyscan.
The 28 pin package can control up to 128 LEDS. In library use case that is 8 digits of an LED segment device.
Segments are connected to the A0-A15 on controller. Digits common are connected to C0-C7 on controller.
We cannot use the decimal point on 16 segment devices as controller
does not have enough control AX lines.
Max. 16 x 8 patterns, 16 segments and 8 commons.

 * 7 segment  = (7 segments + decimal point)  x 8 Digits = 64 LEDS
 * 9 segment  = (9 segments + decimal point)  X 8 Digits = 80 LEDS
 * 14 segment = (14 segments + decimal point) X 8 Digits = 120 LEDS
 * 16 segment = 16 Segments X 8 digits                   = 128 LEDS

[![segments](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/segment.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/segment.jpg)

 I2C-bus interface. I2C Connections to PI.

1. HT16J33 SCLK = SCLK1 I2C pins P1-05 GPIO3
2. HT16J33 SDA = SDA1 I2C pins P1-03 GPIO2

### Seven segment

The seven segment example file was tested on and is set up for a 3 digit common
cathode LED display. LT0565GWK.

| HT16k33 | LED  Segment LT0565GWK|
| --- | --- |
| C0  | Com 12 DIG1 |
| C1  | Com 9 DIG2 |
| C2  | Com 8 DIG3 |
| A0  | A 11  |
| A1  | B 7 |
| A2  | C 4 |
| A3  | D 2 |
| A4  | E 1 |
| A5  | F 10 |
| A6  | G 5 |
| A7  | Dp 3 |

### Nine segment

No example file for this as did not have device. To change which segment display is enabled just pass
the relevant enum value as argument in the DisplayInit() function in setup.

### Fourteen segment

The Fourteen segment example file was tested on two digit common
cathode LED Display. LDD-F5406RI

| HT16k33 | LED Segment LDD-F5406RI |
| --- | --- |
| C0  | Com 16 DIG1 |
| C1  | Com 11 DIG2 |
| A0  | A 12  |
| A1  | B 10 |
| A2  | C 9 |
| A3  | D 7 |
| A4  | E 1 |
| A5  | F 18 |
| A6  | G1 13 |
| A7  | G2 6 |
| A8  | H 17 |
| A9  | J 15 |
| A10  | K 14 |
| A11 | L 5 |
| A12  | M 4 |
| A13  | N 2 |
| A14  | Dp 8 |

### Sixteen segment

No example file for this as did not have device. To change which segment display is enabled just pass
the relevant enum value as argument in the DisplayInit() function in setup. Decimal point not supported as is,
not enough segment lines.

## Software

### Example files

| Filepath | File Function |
| ---- | ---- |
| test_7_segment| Carries out test sequence testing 3 digit 7 segment |
| test_14_segment| Carries out test sequence testing 2 digit 14 segment |

### I2C

Hardware I2C.

1. I2C Address is set by default to 0x70(your module could be different,
user can change argument passed into class constructor). The I2C address of module can be adjusted on PCB.
I2C_FLAGS no flags are currently defined. This parameter should be set to zero.
The value to set I2C_DEVICE, can be found by running command, to view available I2C device numbers

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
Debug configuration flag must be set to true to see this output.
User can set error timeout between retry attempts and number of retry attempts
and can monitor the Error flag.
