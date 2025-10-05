# Error codes Readme

Error codes for rdlib::Return_Codes_e returned by most functions.
The enum has a utility function that can print out the label and its comment, (ReturnCodetoText())

| Enum Text Label               | Number | Details                                                                                          |
|-------------------------------|--------|--------------------------------------------------------------------------------------------------|
| rdlib::Success                | 0      |  Success, Function ran without defined Error                                                     |
| rdlib::Reserved               | 1      |  Reserved for future use                                                                         |
| rdlib::WrongFont              | 2      |  Wrong Font selected                                                                             |
| rdlib::CharScreenBounds       | 3      |  Text Character is out of Screen bounds, Check x and y                                           |
| rdlib::CharFontASCIIRange     | 4      |  Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range.    |
| rdlib::CharArrayNullptr       | 5      |  Text Character Array is an invalid pointer object                                               |
| rdlib::FontDataEmpty          | 6      |  Font span is empty object                                                                       |
| rdlib::BitmapDataEmpty        | 7      |  The Bitmap Span is an empty object                                                              |
| rdlib::BitmapScreenBounds     | 8      |  The Bitmap starting point is outside screen bounds, check x and y                               |
| rdlib::BitmapLargerThanScreen | 9      |  The Bitmap is larger than screen, check  w and h                                                |
| rdlib::BitmapVerticalSize     | 10     |  A vertical Bitmap's height must be divisible by 8.                                              |
| rdlib::BitmapHorizontalSize   | 11     |  A horizontal Bitmap's width  must be divisible by 8                                             |
| rdlib::BitmapSize             | 12     |  Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8),BitmapSize(horizontal)!=(w/8)*h |
| rdlib::CustomCharLen          | 13     |  CustomChar array must always be 5 bytes long                                                    |
| rdlib::BufferSize             | 14     |  Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)                                 |
| rdlib::BufferEmpty            | 15     |  The Buffer span is an empty object                                                              |
| rdlib::SPIOpenFailure         | 16     |  Failed to open HW SPI , lgpio                                                                   |
| rdlib::SPICloseFailure        | 17     |  Failed to close HW SPI , lgpio                                                                  |
| rdlib::I2CbeginFail           | 18     |  Failed to open I2C , lgpio                                                                      |
| rdlib::I2CcloseFail           | 19     |  Failed to close I2C , lgpio                                                                     |
| rdlib::ShapeScreenBounds      | 20     |  Shape is outside screen bounds, check x and y                                                   |
| rdlib::MemoryAError           | 21     |  Could not assign memory                                                                         |
| rdlib::WrongInputPCBType      | 22     |  Wrong input PCB type chosen                                                                     |
| rdlib::GpioChipDevice         | 23     |  Failed to open or close Gpio chip device, lgpio                                                 |
| rdlib::GpioPinClaim           | 24     |  Failed to claim a GPIO for output or input, lgpio                                               |
| rdlib::GpioPinFree            | 25     |  Failed to free a GPIO for output or input, lgpio                                                |
| rdlib::WrongModeChosen        | 26     |  Wrong SPI communication mode chosen by user                                                     |
| rdlib::GenericError           | 27     |  Generic Error message, for minor misc errors                                                    |
| rdlib::UnknownError           | 28     |  For unknown error events                                                                        |
| rdlib::InvalidRAMLocation     | 29     |  Invalid Display RAM location                                                                    |
| rdlib::SPIWriteFailure        | 30     |  Failed to write to HW SPI , lgpio                                                               |
