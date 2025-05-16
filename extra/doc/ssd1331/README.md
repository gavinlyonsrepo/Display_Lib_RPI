# SSD1331 OLED Readme

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system)
      * [Bitmap](#bitmap)
      * [Color codes](#color-codes)
      * [Advanced screen buffer mode](#Advanced-screen-buffer-mode)
  * [Hardware](#hardware)
  * [Output](#output)
  * [Notes](#notes)
      * [Error 99](#error-99)

## Overview

* Name: SSD1331
* Description:

0. C++ Library for a OLED SPI LCD, SSD1331 Driver,
    SSD1331 96x 64 RGB Dot Matrix OLED
1. Dynamic install-able Raspberry Pi C++ library.
2. Contrast , dim mode, Invert colour, scroll, rotate, sleep modes supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color, bi-color Bitmaps & sprites supported.
5. Hardware and Software SPI
6. Dependency: lgpio Library
7. NB The Spidev buffer size setting must be larger than biggest buffer the code will attempt
    to write If not you will get 'Error 99'. See Notes section for more details.
8. Make sure SPI is enabled in your computers configuration.


* Author: Gavin Lyons

## Software

### User options

In the example files, there are 3 sections in "Setup()" function 
where user can make adjustments. The constructor also takes a number of arguments.

0. User Option constructor()  Color and contrast settings
1. User Option Setup()        GPIO/SPI Type
2. User Option Setup()        Screen size section  
3. User Option Setup()        SPI Settings


*User Option 0 Color and contrast settings*

Here the user can select RGB or BGR mode depending on type of display, They can also
set the color contrast levels for the 3 channels (A B and C) for normal and Dim mode.
The range is 0-255. 

*User Option 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The SetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for SetupGPIO define which is used.

*User Option 2 Screen size  + Offsets*

User can adjust screen pixel height, screen pixel width.
The function InitScreenSize sets them.

*User Option 3  SPI Settings*

InitSPI function is overloaded(2 off, one for HW SPI the other for SW SPI).

| parameter | default value | note | SPi type |
| --- | --- | --- |  --- |
| HWSPI_DEVICE | 0| A SPI device, >= 0. which SPI interface to use , ls /dev/spi*|  Hardware  |
| HWSPI_CHANNEL | 0 |A SPI channel, >= 0. Which Chip enable pin to use usually 0 or 1| Hardware  |
| HWSPI_SPEED |  1000000| The speed of serial communication in bits per second.| Hardware  |
| HWSPI_FLAGS | 0|  mode 0 for this device | Hardware  |
| GPIO_CHIP_DEVICE | 0| gpio chip device >= 0, check ls/dev/gpiochip* | both |
| SWSPI_CommDelay | 0 | uS delay for GPIO | software | 

The user can adjust if having reliability issues with SW SPI in some setups.
This is a microsecond delay in SW SPI GPIO loop. It is set to 0 by default, Increasing it will slow 
down SW SPI further.

### File system

In example folder:
The Main.cpp file contains tests showing library functions.
A bitmap data file contains data for bi-color bitmaps and icons tests.
The color bitmaps used in testing are in bitmap folder, 2 16-bit and 2 24-bit images.

| # | example file name  | Desc|
| ------ | ------ |  ------ |
| 1 | hello_world| Basic use case |
| 2 | hello_world_SWSPI | Basic use case software SPI |
| 3 | text_graphic_functions | Tests text,graphics & function testing  |
| 4 | bitmap_tests | bitmap tests |
| 5 | demos| various demos |

### Bitmap

Functions to support drawing bitmaps

| Function Name | Colour support | Pixel size KiB|  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour array | (0-96) X 8  | Data vertically addressed |
| drawBitmap | bi-colour array | 6 | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 BMP files or array |12 | ------ |
| drawBitmap24  | 24 bit color BMP files or array  | 18  | Converted by software to 16-bit color  |
| drawSprite| 16 bit color 565 array | 12 | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Pixel size column assumes 96 by 64 bitmap.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 5  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)
5. For 3 and 4 better just to use bmp files direct from file system see examples.

These class functions will return an error code in event of error see API for more details.

###  Color codes 

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

### Advanced screen buffer mode

Advanced screen buffer mode. There is advanced buffer mode where the code writes to a global screen buffer instead of the VRAM of display. It is off by default more details at readme, 
which is in the 'doc' folder [at link.](../buffer_mode/README.md)

## Hardware

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | GND | GND | GND |
| 2 | VCC | VCC 3.3V | VCC 3.3V  |
| 3 | SCLK | SPI_CLK | GPIO6 |
| 4 | SDA | SPI_MOSI | GPIO5 |
| 5 | RESET | GPI025  | GPIO25 |
| 6 | DC | GPIO24 | GPIO24  |
| 7 | CS | SPI_CE0 | GPIO21 |

1. Pick any GPIO you want for SW SPI,  for HW SPI: reset and DC lines are flexible.
2. User can select  SPI_CE0  or SPI_CE1 for HW SPI

[![Image SSD](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331.jpg)

## Output

[![ demo pic 2](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331output.jpg)

## Notes

### Error 99

[Error 99 Documentation](../error_99/README.md)


