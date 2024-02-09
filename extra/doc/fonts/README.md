# Fonts

There are 12 fonts packaged with library. Fonts can be easily added or removed by user.
All the Font data are in file font_data_RDL.cpp. These fonts are used by the graphic libraries displays.

**Font data table** 

| num | Font enum name | character size XxY |  ASCII range | Size in bytes |
| ------ | ------ | ------ | ------ |  ------ | 
| 1 | font_default | 6x8 |  0 - 0xFE, Full Extended  | 1534 |
| 2 | font_wide | 9x8 | 0x20 - 0x5A, NO lowercase letters | 535 |
| 3 | font_pico | 4x6 | 0x20 - 0x7E  | 289 | 
| 4 | font_sinclairS  | 8x8 | 0x20 - 0x7E | 764 |
| 5 | font_retro | 8x16 | 0x20 - 0x7E | 1524 |
| 6 | font_mega | 16x16 | 0x20 - 0x7E | 3044 |
| 7 | font_arialBold  | 16x16 | 0x20 - 0x7E |  3044 |
| 8 | font_hallfetica | 16x16 | 0x20 - 0x7E | 3044 |
| 9 | font_orla | 16x24 | 0x20 - 0x7E | 4564 |
| 10 | font_arialRound| 16x24 | 0x20 - 0x7E | 4564 |
| 11 | font_groTesk | 16x32 | 0x20 - 0x7A |  5828 |
| 12 | font_sixteenSeg | 32x48 | 0x2D-0x3A , 0-10 : . / - only | 2692 |

Font size in bytes = ((X * (Y/8)) * numberOfCharacters) + (4*ControlByte)

| Font class Function | Notes |
| ------ | ------ | 
| writeChar| draws single character |
| writeCharString | draws character array |
| print | Polymorphic print class which will print out many data types |

These methods return an error code in event of an error such as, ASCII character outside chosen fonts range, 
character out of screen bounds and invalid character array pointer object.

**Adding or removing a font**

1. Add/remove the Font data in font_data_RDL.cpp file
2. Add/remove the pointer to font at bottom of font_data_RDL.cpp file
3. Add/remove the associated extern pointer declaration in the font_data_RDL.hpp file
4. Add/remove the font from the enum  'display_Font_name_e' in the font_data_RDL.hpp
5. Add/remove the font enum name from the switch case in function 'setFont' in font_data_RDL.cpp
6. re-compile and re-install library. 

The new ASCII font must have following font structure.
First 4 bytes are control bytes followed by vertically addressed font data.

```
// An 4 by 8 character size font starting at 
// ASCII offset 0x30 in ASCII table with 0x02 characters in font. 
// 0 and 1 
static const uint8_t FontBinaryExample[] =
{
0x04, 0x08, 0x30, 0x02,   // x-size, y-size, offset, total characters
(data),(data),(data),(data) // font data '0'
(data),(data),(data),(data) // font data '1'
}
```

**Sources**

1. Some of the fonts packaged with library came from [URL](http://rinkydinkelectronics.com/)
2. There is a monochrome font maker there at [URL](http://rinkydinkelectronics.com/t_make_font_file_mono.php)

**Images**

1. Default TODO

2. Wide Font

![w](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/wide.png)

3. Pico TODO

4. SinclairS

![s](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/sinclair.png)

5. Retro 

![r](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/retro.png)

6. Mega TODO

7. arial Bold

![ab](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/arialbold.png)

8. hallfetica

![h](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/hall.png)

9. Orla 

![o](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/orla.png)

10. ArialRound

![ar](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/arialround.png)

11. GroTesk 

![g](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/grotesk.png)

12. Sixteen Segment 

![ss](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/ss.png)

