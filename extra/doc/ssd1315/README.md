# SSD1315 OLED Readme

The SSD1315 is very similar to SSD1306. With the exception of some internal registers.
From library user point of view they are near identical.
The SSD1315 uses the SSD1306 software with a few internal register changes. To select SSD1315 mode
pass the relevant enum label to the begin function.

```c
myOLED.OLEDbegin(myOLED.OLED_Controller_e::SSD1315); 
```

There is only one example hello world program included for SSD1315.
Please use the other SSD1306 examples after passing enum as above.
Refer to SSD1306 Readme for more detailed information.

Tested on A 124x64 pixel I2C display. The SSD1315 library tested with: the first 16 pixels are yellow and rest of screen is blue and there is a black line of pixels in between which cannot be toggled, there are other colors and permutations on market.

[![ Oled ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1315.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1315.jpg)
