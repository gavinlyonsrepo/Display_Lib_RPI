# Error codes Readme

Error codes for rpiDisplay_Return_Codes_e returned by most functions.

| Enum Text Label                   | Number | Details                                                                                          |
|-----------------------------------|--------|--------------------------------------------------------------------------------------------------|
| rpiDisplay_Success                | 0      |  Success, Function ran without defined Error                                                     |
| rpiDisplay_Reserved               | 1      |  Reserved for future use                                                                         |
| rpiDisplay_WrongFont              | 2      |  Wrong Font selected                                                                             |
| rpiDisplay_CharScreenBounds       | 3      |  Text Character is out of Screen bounds, Check x and y                                           |
| rpiDisplay_CharFontASCIIRange     | 4      |  Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range.    |
| rpiDisplay_CharArrayNullptr       | 5      |  Text Character Array is an invalid pointer object                                               |
| rpiDisplay_FontPtrNullptr         | 6      |  Pointer to the font is an invalid pointer object                                                |
| rpiDisplay_BitmapNullptr          | 7      |  The Bitmap data array is an invalid pointer object                                              |
| rpiDisplay_BitmapScreenBounds     | 8      |  The Bitmap starting point is outside screen bounds, check x and y                               |
| rpiDisplay_BitmapLargerThanScreen | 9      |  The Bitmap is larger than screen, check  w and h                                                |
| rpiDisplay_BitmapVerticalSize     | 10     |  A vertical Bitmap's height must be divisible by 8.                                              |
| rpiDisplay_BitmapHorizontalSize   | 11     |  A horizontal Bitmap's width  must be divisible by 8                                             |
| rpiDisplay_BitmapSize             | 12     |  Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8),BitmapSize(horizontal)!=(w/8)*h |
| rpiDisplay_CustomCharLen          | 13     |  CustomChar array must always be 5 bytes long                                                    |
| rpiDisplay_BufferSize             | 14     |  Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)                                 |
| rpiDisplay_BufferNullptr          | 15     |  The Buffer data array is an invalid pointer object                                              |
| rpiDisplay_SPIOpenFailure         | 16     |  Failed to open HW SPI , lgpio                                                                   |
| rpiDisplay_SPICloseFailure        | 17     |  Failed to close HW SPI , lgpio                                                                  |
| rpiDisplay_I2CbeginFail           | 18     |  Failed to open I2C , lgpio                                                                      |
| rpiDisplay_I2CcloseFail           | 19     |  Failed to close I2C , lgpio                                                                     |
| rpiDisplay_ShapeScreenBounds      | 20     |  Shape is outside screen bounds, check x and y                                                   |
| rpiDisplay_MemoryAError           | 21     |  Could not assign memory                                                                         |
| rpiDisplay_WrongInputPCBType      | 22     |  Wrong input PCB type chosen                                                                     |
| rpiDisplay_GpioChipDevice         | 23     |  Failed to open or close Gpio chip device, lgpio                                                 |
| rpiDisplay_GpioPinClaim           | 24     |  Failed to claim a GPIO for output or input, lgpio                                               |
| rpiDisplay_GpioPinFree            | 25     |  Failed to free a GPIO for output or input, lgpio                                                |
| rpiDisplay_WrongModeChosen        | 26     |  Wrong SPI communication mode chosen by user                                                     |
| rpiDisplay_GenericError           | 27     |  Generic Error message                                                                           |
