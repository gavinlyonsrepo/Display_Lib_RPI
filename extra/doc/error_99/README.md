# Error 99 Readme

## Problem

If the user tries to write an SPI buffer bigger than the computers maximum SPI write buffer size you will get
'Error 99' message in console. To fix you need to increase your computers maximum SPI write buffer size.
Also make sure SPI is enabled on your computer.

| Device or library | maximum SPI transaction size bytes| where defined |
| --- | --- | --- |
| lgpio | 65536 | LG_MAX_SPI_DEVICE_COUNT in lgpio.h |
| Display_lib_RPI | 2 <-> 65536(default) | Display_SPI_BLK_SIZE |
| Raspberry pis | set same or bigger as Display_SPI_BLK_SIZE | /boot/firmware/cmdline.txt spidev.bufsiz |
| other SBC | set same or bigger as Display_SPI_BLK_SIZE | ??? |

The maximum SPI transaction size that low level dependency 'lgpio' can handle by default is 65536 bytes.(see LG_MAX_SPI_DEVICE_COUNT in lgpio.h)
So this is also the maximum transaction size of Display_lib_RPI, if more than that needs to be sent
it is sent in blocks of Display_SPI_BLK_SIZE(default 65536) bytes.
Certain color 16-bit  'Display_lib_RPI' functions use buffered writes, the ones that draw bitmaps, write fonts, 
fill rectangles, clear screen, etc.  User can also change and view maximum SPI transaction size( Display_SPI_BLK_SIZE) of 'Display_lib_RPI' by using getDisplaySPIBlockSize() and 
setDisplaySPIBlockSize() functions, by default it is 65536. Due not set it higher than this or 'lgppio' will give an spi Write error.
In order for buffered writes to work the spidev buffer size of users computer must be bigger than or equal Display_SPI_BLK_SIZE(max and default 65536), 
but no bigger than 65536. The maximum size of buffer will vary by display screen size( for example ST7735  with 128x128 pixel size screen = 32768)


## Fix

*On Raspberry Pi's SBC*

spidev bufsiz defines the number of bytes that the SPI driver will use as a buffer for data transfers.
To check the  spidev buf size setting on your device run command:

```sh
cat /sys/module/spidev/parameters/bufsiz
```

If it is lower than 65536(or value indictaed by Error 99 message report) you can change it by adding 
this to the start of line in file /boot/firmware/cmdline.txt.
Make sure everything is on one line and there is space ' ' between this parameter and next one.
Then reboot machine. Verify again by running last cat command above

```sh
spidev.bufsiz=65536
```

## Software SPI

Alternately you can use software SPI which is not affected by these settings but can be slower.
