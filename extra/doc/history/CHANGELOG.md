Version control history
====================

* Version 1.0.1 02-2024
	* First version
	* A C++ Library to connect electronic displays to Raspberry Pi single board computers.
	* Displays supported in first version(st7735 ssd1306 ch1115 erm19264 hd44780 tm1638 nokia5110 max7219)

* Version 1.2.0 03-2024
	* Added Support for ST7789 LCD.
	* Added Support for ILI9341 LCD with XPT2046 touchscreen support.

* Version 1.3.0 04-2024
	* Added Support for Sh1106 & Sh1107 OLED I2C.

* Version 2.0.0 12-2024
	* Changed low level dependency from 'bcm2835' to 'lgpio', primarily so the
	library can now work on a raspberry 5 as 'bcm2835' cannot work on raspberry 5.
	'Lgpio' works with linux device driver rather than low level register access of 'bcm2835'
	software should now work on any linux based SBC which has SPI.
	* Changed font system, added some different fonts and font data is
	now horizontally addressed instead of vertically.
	This facilitates using buffered writes to write font much faster
	character by character in a frame buffer rather than pixel by pixel.
	In the writeChar method of 16-bit color graphic display.

* Version 2.1.0 01-2025
	* Added support for TM1637 LED module.
	* Added support for vectors, with a template to the print method.
	* Replaced the gll font data with better one, same name.
	* Changed MAX7219 code so it can use same ASCII font as TM163X modules.
	* Abstracted most 'lg' library calls to the common data file.
	* Added Scroll test example for ST7789.
	* Added Sprite function for 16 bit color displays

* Version 2.2.0 02-2025
	* Added support for SSD1331 OLED.
	* Added Config file, Logging and Debug modes.
	* Code optimisations.
	* Namespace improvements.

* Version 2.3.0 03-2025
	* Added Support for Ht16k33 LED segment controller module
	* Added more demos to color 16-bit section 
	* Added more graphics functions to color 16-bit section  (arc, polygon, ellipse etc)

* Version 2.4.0 05-2025
	* Added Support for GC9A01A 16 bit Colour Graphic TFT LCD.
	* Added optional frame buffer mode.

* Version 2.4.1 07-2025
	* Corrected code so Max7219 cascade examples works, just acquired second display
	* Added tm1637 example for version with decimal points in addition to semi-colon version

* Version 2.4.2 10-2025
	* Added read diagnostic function to ili9341 LCD display.
	* Added more demos in examples files for 16-bit displays.

* Version 2.5.0 02-2026
	* Added Support for GC9D01N 16 bit Colour Graphic TFT LCD.
	* Changed frame buffer mode enable to a enum variable rather than macro definition.

