# Advanced Screen Buffer Mode Readme

This mode was added in Version 2.4.0. This is for use by the 16 color bit displays only.

Advanced buffer mode allows the library to write to a global screen buffer (`_screenBuffer`) instead of directly writing to the VRAM of the display. This mode is off by default but can be enabled for specific use cases where buffering is advantageous.

Advanced buffer mode offers several key benefits. It enables double buffering, where all drawing operations are first performed in memory and then written to the display in a single operation. With batch drawing, multiple graphics operations can be completed before updating the screen, improving rendering efficiency. The memory-resident buffer also allows for custom effects like fading or blending to be applied before display. Additionally, since memory operations are faster than direct display writes, this approach leads to optimized performance for applications with frequent updates.

However, it comes with limitations. It requires heap memory to store the buffer vector object and flushing the buffer to the display can introduce a performance overhead. Use advanced buffer mode when you need complex rendering, visual effects, or performance optimizations. For simpler applications where memory is limited, direct display drawing may be more suitable. Bear in mind many functions in normal
mode have some form of local buffered writes already. 


## Enabling Advanced Buffer Mode

To enable advanced buffer mode, you need turn it on

```
myTFT.setAdvancedScreenBuffer_e(myTFT.AdvancedScreenBuffer_e::On);
```
By default it is OFF, See function SetupBufferMode() in relevant examples for full example code.
Once enabled, the library will use the `_screenBuffer` for many drawing operations instead of writing directly to the display VRAM. 

## Usage

1. setBuffer, sets the buffer allocates memory for a buffer to cover entire screen, call at setup of Display operations.
2. destroyBuffer, destroys buffer de-allocates memory, call at end of Display operations.
3. clearBuffer, fills buffer with a color, by default black.
4. writeBuffer, writes buffer contents in a single SPI buffered write to screen.

## Functions 

Once enabled The following functions will write to screen Buffer instead of VRAM of display.

1. drawPixel()
2. All Bitmap functions.
3. All Draw text functions only IF **textCharPixelOrBuffer = true** : non default.
4. All graphics functions except fillRectBuffer. It uses a local buffer in function to write to display VRAM. fillRectBuffer is wrapped by fillScreen. So fillRectBuffer and fillscreen will still write directly to VRAM of display.

## Examples

There is one example for Advanced screen buffer mode, for st7735. Path =  examples/st7735/advanced_screen_buffer_mode.

All examples for GC9D01 display are for Advanced screen buffer mode.
This is because at time of writing there are issues
with this display when attempting to draw pixel by pixel into the VRAM made worse by rotation.
See my arduino port (GC9D01_LTSM) on github for details.


