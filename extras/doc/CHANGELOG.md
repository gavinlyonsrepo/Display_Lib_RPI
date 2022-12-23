# Changelog

* version 1.0.0 May 2021
	* First release
	
* version 1.1 June 2021
	* Packaged files as an installable system-level dynamic C++ library
	and created makefile to build library and install.
	* Added extra fonts (2-5)
	* Modified SPI so it start and stops on data transfer only.
	thus freeing up SPI module for other devices with different settings. 

* version 1.2 July 2021
	* added font 6
	* added Print class support for all Fonts

* Version 1.3 Feb 2022
	* drawBitmap() function is now set up for both horizontal and vertical addressed bitmap data,
	Vertical is default. The addressing mode is changed by setDrawBitmapAddr(), new function.  
	* Enum added for font name labels instead of raw numbers, 
	This may cause font compiler warnings for sketch's written on versions before 1.3.0 , 
 	The sketch's will still compile and work,
	Simply replace font numbers  with the relevant enum text labels to get rid of warnings. 
	* A new function added for initialising a multibuffer struct , OLEDinitBufferStruct(),
	backward compatible with old manual workflow. 
	
* Version 1.3.2 Dec 2022
	* Added Fonts, tiny and homespun.
	* Added Drawtext method.
	* Changed from multibuffer to single buffer
	* Allowed User to pick SPI bus baudrate and Chip enable pin.

