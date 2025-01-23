# ST7789 TFT Readme

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)

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
     * [Display offsets](#display-offsets)

## Overview

* Name: ST7789_TFT
* Description:

0. C++ Library for a TFT SPI LCD, ST7789 Driver
1. Dynamic install-able Raspberry Pi C++ library.
2. Inverse colour, rotate, sleep modes supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color, bi-color Bitmaps & sprites supported.
5. Hardware and Software SPI
6. Dependency: lgpio Library
7. NB 'spidev.bufsiz' setting must be 65536 or greater see Notes section. 

* Author: Gavin Lyons

## Software

### User options

In the example files. There are 3 sections in "Setup()" function 
where user can make adjustments to select for SPI type used, and screen size.

1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 SPI SETTINGS

*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The SetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for SetupGPIO define which is used.


*USER OPTION 2 Screen size  + Offsets*

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge 
such as cropped data or defective pixels.
The function InitScreenSize sets them.

*USER OPTION 3  SPI Settings*

InitSPI function is overloaded(2 off, one for HW SPI the other for SW SPI).

| parameter | default value | note | SPi type |
| --- | --- | --- |  --- |
| HWSPI_DEVICE | 0| A SPI device, >= 0. which SPI interface to use , ls /dev/spi*|  Hardware  |
| HWSPI_CHANNEL | 0 |A SPI channel, >= 0. Which Chip enable pin to use usually 0 or 1| Hardware  |
| HWSPI_SPEED |  1000000| The speed of serial communication in bits per second.| Hardware  |
| HWSPI_FLAGS | 0|  mode 0 for this device | Hardware  |
| GPIO_CHIP_DEVICE | 0| gpio chip device >= 0, check ls/dev/gpiochip* | both |
| SWSPI_CommDelay | 0 | uS delay for GPIO | software | 

The user can adjust if  having reliability issues with SW SPI in some setups.
This is a microsecond delay in SW SPI GPIO loop. It is set to 0 by default, Increasing it will slow 
down SW SPI further.

### File system

In example folder:
The Main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder, 3 16-bit and 5 24-bit images.

| # | example file name  | Desc|
| ------ | ------ |  ------ |
| 1 | Hello_world| Basic use case |
| 2 | Text_Graphic_Functions | Tests text,graphics & function testing  |
| 3 | Bitmap_Tests | bitmap |
| 4 | Frame_rate_test_bmp | Frame rate per second (FPS) bitmaps |
| 5 | Frame_rate_test_two | rame rate per second (FPS) text |
| 6 | Hello_world_SWSPI | Basic use case software SPI |

### Bitmap

Functions to support drawing bitmaps

| Function Name | Colour support | Pixel size KiB|  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour array | (0-240) X 8  | Data vertically addressed |
| drawBitmap | bi-colour array |  16.4 | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 BMP files or array |131 | ------ |
| drawBitmap24  | 24 bit color BMP files or array  | 196  | Converted by software to 16-bit color  |
| drawSprite| 16 bit color 565 array | 32 | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Pixel size column assumes 240 by 280 bitmap.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 5  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)
5. For 3 and 4 better just to use bmp files direct from file system see examples.

These class functions will return an error code in event of error see API for more details.

###  Color codes 

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

## Hardware

Tested and developed on:

* Size 1.69 inch IPS color TFT LCD
* Resolution: 240 (H) RGB x280 (V)
* Control chip: st7789v2
* Display area 27.972 (H) x 32.634 (V)
* Panel size 30.07 (H) x37.43 (V) x1.56 (d)
* Logic voltage 3.3V

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC |  VCC |
| 2 | SS/CS | SPI_CE0 | GPIO21 |
| 3 | DC | GPIO24 | GPIO24  |
| 4 | RESET | GPI025  | GPIO25 |
| 5 | SDA | SPI_MOSI | GPIO5 |
| 6 | SCLK | SPI_CLK | GPIO6 |
| 7 | VCC | VCC | VCC  |
| 8 | GND | GND | GND |


1. Connect LED backlight pin 1 thru a resistor to VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. Pick any GPIO you want for SW SPI,  for HW SPI: reset and DC lines are flexible.
4. User can select  SPI_CE0  or SPI_CE1 for HW SPI
5. Backlight control is left to user.

## Output

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)

## Notes

### spidev buf size

The maximum transaction size that 'lgpio' can handle by default is 65536 bytes,
So this is also the maximum transaction size of Display_lib_RPI, if more than that needs to be sent
it is sent in blocks of 65536 bytes. 
Certain 'Display_lib_RPI' functions use buffered writes, the ones that draw bitmaps, write fonts, 
fill rectangles, clear screen, etc. In order for this to work the spidev.buf size setting  must be at least 65536.
To check the setting on your device run command:

```sh
cat /sys/module/spidev/parameters/bufsiz
```

If it is lower than 65536 you can change it by adding 
this to the start of line in file /boot/firmware/cmdline.txt. (raspberry pi )
Make sure everything is on one line and there is space ' ' between this parameter and next one.
Then reboot machine. Verify again by running last cat command above

```sh
spidev.bufsiz=65536
```

bufsiz defines the number of bytes that the SPI driver will use as a buffer for data transfers.

### Display offsets

The display initialisation requires an offset calculation which differs for different size and resolution displays.
This is in the code(Function AdjustWidthHeight()) but the many different size displays are not available for testing or dealt with.
If using a display other than 240x320(the default and size of ST7789 VRAM) and if user finds they cannot address all screen
or their data is offset. Try Setting the pixel width and height of your screen to 240X320 and do not write as 
much as possible to the part of the Video RAM you cannot see.
For example  if you have a 240X280 display in 0 degree rotation
1. Set pixel Width = 240 and pixel height = 320
2. Do not write to the missing 40 pixels in the Y-axis, you still can but it is inefficient.
