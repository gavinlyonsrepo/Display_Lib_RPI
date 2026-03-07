# Bitmap 16 bit color displays Readme


## Functions to support drawing bitmaps

| Function Name | Colour support | Pixel size KiB |  Note |
| ------ | ------ | ------ | ------ |
| drawIcon | bi-colour array | (0-127) X 8  | Data vertically addressed |
| drawBitmap | bi-colour array | 2 | Data horizontally  addressed |
| drawBitmap16 | 16 bit color 565 image files or array |  32 | ------ |
| drawBitmap24  | 24 bit color image files or array  | 48 | Converted by software to 16-bit color  |
| drawSprite| 16 bit color 565 array | 32 | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap Size (in KiB)= (Image Width×Image Height×Bits Per Pixel)/(8×1024)
2. Pixel size column assumes 128 by 128 screen.
3. The mentioned bitmap data arrays for images are created with this [file data conversion tool](https://github.com/gavinlyonsrepo/Guardian_LTSM)
4. For drawBitmap16 and DrawBitmap24 easier just to use image files direct from file system rather than arrays
see examples showing BMP file usage.
5. These class functions will return an error code in event of error, see API for more details.
