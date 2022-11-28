
![ OLED ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/oled.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Test](#test)
  * [Hardware](#hardware)
  * [Features](#features)
 
  
Overview
--------------------
* Name : ER_OLEDM1_CH1115_RPI
* Description : 

0. C++ Library to support the ER-OLEDM1.09-1 128X64 OLED Display Module 
   driven by the CH1115 controller for the Raspberry PI.
1. Raspberry Pi C++ library.      
2. Inverse colour, vertical rotate, sleep, fade effect, horizontal scroll and contrast control. 
3. 6 fonts included 
4. Graphics class included.
5. Multi-buffer mode.
6. Bitmaps supported.
7. Hardware and Software SPI 

* Developed on 
	1. Raspberry PI 3 model b, 
	2. C++ complier g++ (Raspbian 6.3.0-18)
	3. Raspbian 9.13 stretch OS
	4. bcm2835 Library 1.68 (Dependency)
    
* Author: Gavin Lyons
* Port of my Arduino [library](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115)
 

Output
-----------------------------

Output Screenshots, From left to right, top to bottom.

1. Full screen bitmap displayed 
2. Multi buffer mode screen divided into buffers
3. Multi buffer mode screen divided into buffers.
4. Different size and inverted text.
5. Available ASCII font printed out 0-127
6. Extended ASCII font printed out 128-255  
7. Font 1-4 
8. Font 5 

![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115/blob/main/extras/image/output.jpg)
![ output ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/blob/main/extras/image/fontpic.jpg)

Installation
------------------------------

1. Make sure SPI bus is enabled on your raspberry PI

2. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.68.)
	* The bcm2835 library is a dependency and provides SPI bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

3. Download the ER_OLEDM1_CH1115_RPI library 
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.
    
```sh
curl -sL https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/archive/1.3.tar.gz | tar xz
```

4. Run "make" to run the makefile at repo root folder to install library, it will be 
    installed to usr/lib and usr/include
    
```sh
cd ER_OLEDM1_CH1115_RPI-1.3
sudo make
```

Test
----------------------------------

1. Next step is to test OLED and installed library with an example.
Wire up your OLED. Next enter the examples folder and run the makefile in THAT folder, 
This makefile builds the examples file using the just installed library.
and creates a test exe file in "bin". Be sure to use "sudo" as the bcm2835 requires root permissions by default [ see here for more details on that](http://www.airspayce.com/mikem/bcm2835/) 
The default example file is "hello world",  user should see hello world on the OLED
by end of this step.

```sh
cd examples/
make
sudo bin/test
```

2. There are six examples files to try out. 
To decide which one the makefile builds simply edit "SRC" variable at top of the makefile in examples folder.
in the "User SRC directory Option Section". Pick an example "SRC" directory path and ONE ONLY.
Comment out the rest and repeat 1.


Hardware
----------------------------

CH1115 is a single-chip CMOS OLED driver with controller for organic light emitting diode dot-matrix graphic display system. CH1115 consists of 128 segments, 64 commons that can support a maximum display resolution of 128 X 64. It is designed for Common Cathode type OLED panel. ER-OLEDM1.09-1W-SPI is a White 1.09" OLED Display Panel with Breakout Board. This module is a combination of the two.(controller and OLED)

For SWSPI pick any GPIO you want for the 5 control lines.

| pin no| GPIO HWSPI | pin name | pin desc | 
|---|---|---| ---| 
| 1 | n/a |Gnd | Ground |
| 2 | n/a |VCC | voltage supply |
| 3 | SPISCLK GPIO 11 |SCL | Serial clock input |
| 4 | SPIMOSI GPIO 10 |SDA | Serial data input |
| 5 | Any you want | RES | This pin is reset signal input.  |
| 6 | Any you want | DC | This pin is Data or Command control pin. |
| 7 | SPICEO GPIO 8 |  CS | This pin is the chip select input.  |


![ wiring ](https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI/blob/main/extras/image/wiring.png)

Features
-------------------------

There are two makefiles

    1. Makefile at root directory builds and installs library at a system level.
    2. Makefile in example directory build chosen example file to an executable.

*SPI*

Hardware and software SPI. Two different class constructors. 
User can pick the relevant constructor, see examples files. 
Hardware SPI is recommended, far faster and more reliable 
but Software SPI allows for more flexible GPIO selection.
The SPI switches on and off after each data transfer to allow
other devices with different SPI settings to use bus.
The SPI settings are in OLEDSPIon function.
Speed is currently at BCM2835_SPI_CLOCK_DIVIDER_64. 
6.25MHz on RPI3. This can be adjusted in code.

*buffers*

User can create as many buffers as they want by creating a named "MultiBuffer" struct
of any size or offset and setting the "activebuffer" to that struct. Thus dividing up 
the screen into as many buffers as they want. 

*fonts*

There are six fonts.
A print class is available to print out most passed data types.
The fonts 1-4 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
Font 5 & 6 are special fixed size large font but it is numbers only + ':' &'.' only

Six fonts available : 

| Font num | Font name | Font size xbyy |  Note |
| ------ | ------ | ------ | ------ |  
| 1 | Default | 5x8 | Full Extended ASCII 0x00 - 0xFF |
| 2 | Thick   | 7x8 | no lowercase letters , ASCII  0x20 - 0x5A |
| 3 | Seven segment | 4x8 | ASCII  0x20 - 0x7A |
| 4 | Wide | 8x8 | no lowercase letters, ASCII 0x20 - 0x5A |
| 5 | Big Nums | 16x32 | ASCII 0x30-0x3A ,Numbers + : . only |
| 6 | Med Nums | 16x16 | ASCII 0x30-0x3A ,Numbers + : . only |

*bitmaps*


There is a few different ways of displaying bitmaps, 

| Num | Method |  Data addressing | Note |
| ------ | ------  | ------ |  ------ |  
| 1 | OLEDBitmap() |  Vertical |  Writes directly to screen , no buffer used. | 
| 2 | OLEDBuffer() |  Vertical  |  For internal use mostly | 
| 3 | drawBitmap() |  Vertical | default,  setDrawBitmapAddr(true) | 
| 4 | drawBitmap() |  Horizontal | setDrawBitmapAddr(false) |

See the bitmap example file for more details on each method. Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) , Bitmaps should be defined as const  buffers non-const, for method 2 buffer can be initialised with bitmap data.

*User adjustments*

When the user calls OLEDbegin() to start OLED they can specify a contrast setting from 0x00 to 0xFF. Datasheet says 0x80 is default. User can also change contrast on the fly.
Screen can be disabled to enter a sleep type mode where OLED draws 500uA.
Screen pixel colour can be inverted and also screen can be vertically rotated. 

There is a "fade or breath display effect". Whose parameters(time brightness) can be adjusted by passing data to function see "OLEDfadeEffect" function header  in .cpp and datasheet for details.
default is 0x81.

There is a  Horizontal scroll effect. Whose parameters(TimeInterval , direction , mode)
can be adjusted by passing data to function see "OLEDscrollSetup" function header in .cpp and datasheet for details. defaults are in header file are 6 frames , right , continuous mode.

