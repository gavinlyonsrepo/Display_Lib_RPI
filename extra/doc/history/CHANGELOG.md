Version control history
====================

* Version 1.0.1 02-2024
	* First version
	* A C++ Library to connect electronic displays to Raspberry Pi single board computers.
	* Displays supported in first version(st7735 ssd1306 ch1115 erm19264 hd44780 tm1638 nokia5110 max7219)

* Version 1.2.0 03-2024
	* Added Support for ST7789 LCD.
	* Added Support for ILI9341 LCD with XPT2046 touchscreen support.

* Version 1.3.0 20-03-2024
	* Added Support for Sh1106 & Sh1107 OLED I2C.

* Version 2.0.0 05-12-2024
	* Changed low level dependency from 'bcm2835' to 'lgpio', primarily so the library can now work on a raspberry 5
	as 'bcm2835' cannot work on raspberry 5. 'Lgpio' works with linux device driver rather than low level register access of 'bcm2835'
	software should now work on any linux based SBC which has SPI.
	* Changed font system, added some different fonts and font data is now horizontally addressed instead of vertically.
	This facilitates using buffered writes to write font much faster character by character in a frame buffer rather than pixel by pixel.
	In the writeChar method of 16-bit color graphic display.
