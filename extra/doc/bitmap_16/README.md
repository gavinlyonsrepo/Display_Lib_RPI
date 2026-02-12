# Bitmap 16 bit color displays Readme


## Functions to support drawing bitmaps

| Function Name | Colour support | Pixel size KiB |  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour array | (0-127) X 8  | Data vertically addressed |
| drawBitmap | bi-colour array | 2 | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 BMP files or array |  32 | ------ |
| drawBitmap24  | 24 bit color BMP files or array  | 48 | Converted by software to 16-bit color  |
| drawSprite| 16 bit color 565 array | 32 | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap Size (in KiB)= (Image Width×Image Height×Bits Per Pixel)/(8×1024)
2. Pixel size column assumes 128 by 128 screen.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 5  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)
5. For 3 and 4 better just to use BMP files direct from file system see examples.

These class functions will return an error code in event of error, see API for more details.
