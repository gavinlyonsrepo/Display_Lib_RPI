# ST7735 TFT Readme

[![ Image TFT ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system)
      * [Bitmap](#bitmap)
      * [Color codes](#color-codes)
  * [Hardware](#hardware)
  * [Output](#output)
  * [Notes](#notes)
     * [spidev buf size](#spidev-buf-size)

## Overview

* Name: ST7735_TFT
* Description:

0. C++ Library for a TFT SPI LCD, ST7735 Driver
1. Dynamic install-able Linux based Single Board ComputersC++ library.
2. Inverse colour, rotate, sleep, idle  & vertical scroll modes supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color, bi-color Bitmaps & sprites supported.
5. Hardware and Software SPI
6. Dependency: lgpio Library
7. NB 'spidev.bufsiz' setting must be 65536 or greater see Notes section. 

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_16F18346_projects)

## Software

### User options

In the example files. There are 3 sections in "Setup()" function 
where user can make adjustments to select for SPI type used, PCB type used and screen size.

1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 PCB_TYPE, + SPI SETTINGS

*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The TFTSetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for TFTSetupGPIO define which is used.
HW SPI is far faster and more reliable than SW SPI

*USER OPTION 2 Screen size  + Offsets*

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge 
such as cropped data or defective pixels.
The function TFTInitScreenSize sets them.

*USER OPTION 3 PCB Version, SPI Settings*

TFTInitPCBType function is overloaded(2 off, one for HW SPI the other for SW SPI).

| parameter | default value | note | SPi type |
| --- | --- | --- |  --- |
| SWSPI_CommDelay | 0 | uS delay for GPIO to change speed| software | 
| HWSPI_DEVICE | 0| A SPI device, >= 0. which SPI interface to use , ls /dev/spi*|  Hardware  |
| HWSPI_CHANNEL | 0 |A SPI channel, >= 0. Which Chip enable pin to use usually 0 or 1| Hardware  |
| HWSPI_SPEED |  1000000| The speed of serial communication in bits per second.| Hardware  |
| HWSPI_FLAGS | 0|  mode 0 for this device | Hardware  |
| GPIO_CHIP_DEVICE | 0| gpio chip device >= 0, check ls/dev/gpiochip* | both |
| PCB_TYPE | TFT_ST7735R_Red | see table and text below |  Both  |

PCB_TYPE: In the main.cpp in USER OPTION 3 PCB_TYPE select your display type,
by passing an enum type to function  TFTInitPCBType.
Default is "TFT_ST7735R_Red". There are 4 types of the ST7735 TFT display supported.
If your display works but RGB colors are wrong you may have chosen wrong display type.

| Number | Description | Enum label| Tested |
| ---- | ---- | --- | ---| 
| 1 | ST7735B controller| TFT_ST7735B | n/a |
| 2 | ST7735R Green Tab | TFT_ST7735R_Green | n/a |
| 3 | ST7735R Red Tab   | TFT_ST7735R_Red | RED PCB v1.1, 1.44, 128x128 pixels |
| 4 | ST7735S Black Tab | TFT_ST7735S_Black | RED PCB v1.2, 1.8, 128 x 160 pixels |


### File system

In example folder:
The Main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder, 3 16-bit and 5 24-bit images.

| # | example file name  | Desc|
| ------ | ------ |  ------ |
| 1 | Hello_world| Basic use case |
| 2 | Hello_world_SWSPI| Basic use case fro software spi |
| 3 | Text_Graphic_Functions | Tests text,graphics & function testing  |
| 4 | Bitmap_Tests | bitmap |
| 5 | Frame_rate_test_bmp | Frame rate per second (FPS) bitmaps |
| 6 | Frame_rate_test_two  | Frame rate per second (FPS) text + graphics |
| 7 | Multiple_displays  | shows use of two displays on same spi bus(OLED AND LCD) |

### Bitmap

Functions to support drawing bitmaps

| Function Name | Colour support | Pixel size KiB |  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour array | (0-127) X 8  | Data vertically addressed |
| drawBitmap | bi-colour array | 2 | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 BMP files or array |  32 | ------ |
| drawBitmap24  | 24 bit color BMP files or array  | 48 | Converted by software to 16-bit color  |
| drawSprite| 16 bit color 565 array | 32 | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap Size (in KiB)= Image Width×Image Height×Bits Per Pixel(8×1024)
2. Pixel size column assumes 128 by 128 screen.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 5  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)
5. For 3 and 4 better just to use bmp files direct from file system see examples.

These class functions will return an error code in event of error see API for more details.

###  Color codes 

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

## Hardware

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC |  VCC |
| 2 | SCLK | SPI_CLK | GPIO6 |
| 3 | SDA | SPI_MOSI | GPIO5 |
| 4 | A0/DC | GPIO24 | GPIO24  |
| 5 | RESET | GPI025  | GPIO25 |
| 6 | SS/CS | SPI_CE0 | GPI21 |
| 7 | GND | GND | GND |
| 8 | VCC | VCC | VCC  |

1. Connect LED backlight pin 1 thru a 150R/220R ohm resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. Pick any GPIO you want for SW SPI for HW SPI: reset and DC lines are flexible.
5. User can select  SPI_CE0  or SPI_CE1 for HW SPI
6. Backlight control is left to user.

[![ Wiring pic ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/wiring.jpg)](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/wiring.jpg)

## Output

1. Clock Demo showing icons, small bitmaps
2. 24-bit color bitmap test image
3. 16-bit color bitmap test image

[![ Demo pic ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)

## Notes

### spidev buf size

The maximum transaction size that 'lgpio' can handle by default is 65536 bytes.
So this is also the maximum transaction size of Display_lib_RPI, if more than that needs to be sent
it is sent in blocks of 65536 bytes. 
Certain 'Display_lib_RPI' functions use buffered writes, the ones that draw bitmaps, write fonts, 
fill rectangles, clear screen, etc. In order for this to work the spidev.buf size must be at least 65536.
To check the setting on your device run command:

```sh
cat /sys/module/spidev/parameters/bufsiz
```

If it is lower than 65536 you can change it by adding 
this to the start of line in file /boot/firmware/cmdline.txt.(raspberry pi )
Make sure everything is on one line and there is space ' ' between this parameter and next one.
Then reboot machine. Verify again by running last cat command above

```sh
spidev.bufsiz=65536
```

bufsiz defines the number of bytes that the SPI driver will use as a buffer for data transfers.
 
