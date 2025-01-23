# ILI9341 LCD TFT Readme

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system)
      * [Bitmap](#bitmap)
      * [Color codes](#color-codes)
  * [Hardware](#hardware)
  * [Output](#output)
  * [Touchscreen](#touchscreen)
  * [Notes](#notes)
    * [spidev buf size](#spidev-buf-size)


## Overview

* Name: ILI9341_TFT
* Description:

0. C++ Library for a TFT SPI LCD, ILI9341 Driver (might also work with ILI9340)
1. Dynamic install-able or Linux Single Board Computers C++ library.
2. Inverse colour, rotate, scroll, modes supported.
3. Graphics + print class included.
4. 24 bit colour , 16 bit color, bi-color Bitmaps & sprites supported.
5. Hardware and Software SPI
6. Dependency: lgpio Library
7. Support for XPT2046 Touchscreen IC included
8. NB 'spidev.bufsiz' setting must be 65536 or greater see Notes section. 

* Author: Gavin Lyons

## Software

### User options

In the example files. There are 3 sections in "Setup()" function 
where user can make adjustments to select for SPI type used, and screen size.

1. USER OPTION 1 GPIO/SPI TYPE
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 SPI SETINGS

*USER OPTION 1 SPI TYPE / GPIO*

This library supports both Hardware SPI and software SPI.
The TFTSetupGPIO function is overloaded(2 off one for HW SPI the other for SW SPI).
The parameters set for TFTSetupGPIO define which is used.


*USER OPTION 2 Screen size*

User can adjust screen pixel height, screen pixel width 
The function TFTInitScreenSize sets them.

*USER OPTION 3  SPI Settings*

TFTInitSPI function is overloaded(2 off, one for HW SPI the other for SW SPI).

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
| 3 | Bitmap_Tests | bitmap + FPS testing |
| 4 | Mandelbrot_set | Drawing a mandelbrot set demo |
| 5 | Touch_Screen | Basic Touch screen  demo |
| 6 | xpt_Test | Touch screen test  without TFT |
| 7 | Hello_world_SWSPI | Basic use case software SPI |

### Bitmap

Functions to support drawing bitmaps

| Function Name | Colour support | Pixel max size KiB |  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour array | (0-240) X 8  | Data vertically addressed |
| drawBitmap | bi-colour array |  18.75  | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 BMP files or array |  150  | ------ |
| drawBitmap24  | 24 bit color BMP files or array  | 225  | Converted by software to 16-bit color  |
| drawSprite| 16 bit color 565 array |  150  | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap Size (in KiB)= Image Width×Image Height×Bits Per Pixel(8×1024)
2. Pixel size column assumes 240 by 320 screen.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 5  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)
5. For 3 and 4 better just to use BMP files direct from file system rather than data array, see examples.

These class functions will return an error code in event of error see API for more details.

###  Color codes 

For functions that accept a 16 bit color value. There is list of pre-defined colors in the 'colors' folder in doc.

## Hardware

Tested and developed on:

* Size 2.4 inch SPI Serial  IPS color TFT LCD
* Resolution: 240 (H) RGB x 320 (V)
* Color Depth: 262K/65K (65K used)
* Control chip:ILI9341
* Display area 27.972 (H) x 32.634 (V)
* Panel size 36.72(W)X48.96(H)mm
* Logic voltage 3.3V
* Touch panel with XPT2046 IC


Connections as setup in main.cpp test file.

| PinNum | Pin description | RPI | note |
| --- | --- | --- | --- |
| 1 | VCC | VCC | 3.3 or 5V ,CAUTION your display must have 3.3V regulator on back to connect to 5V |
| 2 | GND | GND | |
| 3 | CS | SPICE0 |TFT Chip select |
| 4 | RESET | GPIO25 |Reset, Use any GPIO for this, If no reset pin, pass -1 in here & display will use software rst|
| 5 | DC | GPIO24 | Data or command, Use any GPIO for this line |
| 6 | SDI(MOSI) | SPIMOSI | |
| 7 | SCLK | SPICLK | | 
| 8 | LED | VCC |CAUTION Your display may need current limit resistor|
| 9 | SDO(MISO) | nc |Only needed to read diagnostics from TFT (not implemented yet) |
| 10| T_CLK | SPICLK | |
| 11| T_CS | SPICE1 |XPT2046 Chip select |
| 12| T_DIN | SPIMOSI | |
| 13 | T_DO | SPIMISO | |
| 14 | T_IRQ | GPIO22 | |


1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. LED Backlight control is left to user.
3. Pins marked with T_ prefix are related to the touchscreen IC XP2046 if user is not using the touch
screen do not connect these. The touch screen and TFT share the SPI bus but have different chip select lines. TFT SPI settings(Speed, active chip select) should be refreshed after ever read cycle of XPT2046 sensor, see example.

## Output


Four-Byte Burger 240x320 16-bit bitmap test image, Credits [Ahoy](https://www.youtube.com/watch?v=i4EFkspO5p4)

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output2.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output2.jpg)


## Touchscreen

If the users display has a touchscreen controller IC on the back (XPT2046).
A simple class to interface with the XPT2046 touchscreen controller IC has been included.
Note :It may not be populated on your 
display or there may be a different model of IC there.

The T_IRQ line goes low when the touchscreen is touched and the data can be read from sensor.

Co-ord system returned by XPT_2046_RDL class is as follows:

1. Top left :    X = 1800 Y = 150
2. Top Right :   X = 150  Y = 150
3. Bottom Left : X = 1800 Y = 1800
3. Bottom Right : X = 150 Y = 1800

Output of the basic touch screen example included. 

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output.jpg)

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
