## Gavin Lyons 01 -2024
# Makefile to build example program for Display_Lib_RPI 
## URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
# Run make help for options 

# === SUPPORTED DISPLAY LIST  ===
# Display 0 =  HT16K33
# Display 1 =  MAX7219
# Display 2 =  TM1638
# Display 3 =  TM1637
# Display 4 =  HD44780 LCD I2C
# Display 5 =  SSD1306 OLED I2C
# Display 6 =  ERM19264 LCD SPI
# Display 7 =  NOKIA5110 LCD SPI
# Display 8 =  CH1115 OLED I2C
# Display 9 =  SH1106 OLED I2C
# Display 10 = SH1107 OLED I2C
# Display 11 = ST7735X  TFT LCD SPI
# Display 12 = ST7789   TFT LCD SPI
# Display 13 = ILI9341  TFT LCD SPI
# Display 14 = SSD1331  OLED SPI
# Display 15 = GC9A01A  TFT LCD SPI
# ==================================

# ***********************************************
# ***********************************************
# ** User SRC directory path Option Section A ***
# 1. Pick an example file "SRC" path and ONE ONLY.
# 2. Comment out the others.

# ================= HT16K33 ======================
#SRC=ht16k33/test_7_segment
#SRC=ht16k33/test_14_segment
# ================================================

# ================= MAX7219_7SEG ================
#SRC=max7219/tests_hwspi
#SRC=max7219/tests_swspi
#SRC=max7219/bcdmode
#SRC=max7219/clock_demo
#SRC=max7219/cascade_demo
#================================================

# ================= TM1638 ======================
#SRC=tm1638/test_model_1_3
#SRC=tm1638/test_model2
# ================================================

# ================= TM1637 ======================
#SRC=tm1637/test_model4
# ================================================

# ================= HD44780 LCD I2C ==============
#SRC=hd44780/test_16x02
#SRC=hd44780/test_20x04
#=================================================

# ================= SSD1306 OLED I2C =============
#SRC=ssd1306/hello_world
#SRC=ssd1306/text_graphics_functions
#SRC=ssd1306/framerate_test
#SRC=ssd1306/bitmap
#SRC=ssd1306/clock_demo
#=================================================

# ========== ERM19264_UC1609 LCD SPI==============
#SRC=erm19264/hello_world
#SRC=erm19264/text_graphics_functions
#SRC=erm19264/framerate_per_sec
#SRC=erm19264/bitmap
#SRC=erm19264/software_spi
#=================================================

# ================ NOKIA5110 LCD SPI=============
#SRC=nokia5110/helloworld
#SRC=nokia5110/text_graphics_functions
#SRC=nokia5110/framerate_test
#SRC=nokia5110/framerate_test_swspi
#SRC=nokia5110/bitmap
#=================================================

# ============= CH1115 OLED SPI ==================
#SRC=ch1115/hello_world
#SRC=ch1115/text_graphics_functions
#SRC=ch1115/framerate_test
#SRC=ch1115/framerate_test_swspi
#SRC=ch1115/bitmap
#================================================

# =========== SH1106 or SH1107 OLED I2C ==========
#SRC=sh1106/hello_world
#SRC=sh1107/hello_world
#=================================================

# =============== ST7735 TFT LCD SPI =============
#SRC=st7735/hello_world
#SRC=st7735/hello_world_swspi
#SRC=st7735/hello_world_128x160
#SRC=st7735/text_graphics_functions
#SRC=st7735/bitmap_tests
#SRC=st7735/framerate_test
#SRC=st7735/demos
#SRC=st7735/advanced_screen_buffer_mode
#=================================================

# =============== ST7789 TFT LCD SPI =============
#SRC=st7789/hello_world
#SRC=st7789/hello_world_swspi
#SRC=st7789/bitmap_tests
#SRC=st7789/text_graphics_functions
#SRC=st7789/framerate_test
#SRC=st7789/demos
#=================================================

# =============== ILI9341 TFT LCD SPI ============
#SRC=ili9341/hello_world
#SRC=ili9341/hello_world_swspi
#SRC=ili9341/bitmap_tests
#SRC=ili9341/text_graphics_functions
#SRC=ili9341/demos
#SRC=ili9341/touch_screen
#=================================================

# =============== SSD1331 TFT LCD SPI ============
#SRC=ssd1331/hello_world
#SRC=ssd1331/hello_world_swspi
#SRC=ssd1331/bitmap_tests
#SRC=ssd1331/text_graphics_functions
#SRC=ssd1331/demos
#=================================================

# =============== GC9A01A TFT LCD SPI =============
#SRC=gc9a01/hello_world
#SRC=gc9a01/hello_world_swspi
#SRC=gc9a01/bitmap_tests
#SRC=gc9a01/text_graphics_functions
#SRC=gc9a01/demos
#=================================================

# =========== XPT2046 touchscreen basic test =====
#SRC=xpt2046/xpt_test
#=================================================

#========= Other misc Test ======================
#SRC=misc_test/utilities_test
#SRC=misc_test/multiple_displays_test
#SRC=misc_test/unit_test_bicolor_class
#SRC=misc_test/unit_test_color16_class
#================================================
 
# ************************************************
# ************************************************

# The --no-print-directory option of make tells make not to print the message about entering 
# and leaving the working directory. 
MAKEFLAGS += --no-print-directory

# Check for SRC example file path not set with error function
ifndef SRC
$(error SRC example file path is not set, pick a SRC path in Section A, exiting nothing done)
endif

# Pre-compiler and Compiler and linker flags
LDFLAGS=  -lrpidisplaygl -llgpio
CFLAGS= -Iinclude/ -c -Wall -Wextra -std=c++23

# Build directories and output
MD=mkdir
BUILD=Build
BIN=Bin
TARGET=$(BIN)/test.out

# Project directory structure
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,  $(BUILD)/%.o, $(SRCS))

# Main task,  makes build directory, updates your objects, builds your executable
.PHONY: all
all: clean build

# Task producing target from built files
$(TARGET): $(OBJS) $(BUILD)
	@echo 'MAKE EXE FILE'
	$(MD) -vp $(BIN)
	$(MD) -vp $(BUILD)
	$(CXX)  $(OBJS) -o $@ $(LDFLAGS)
	@echo 'DONE!'

# Compile all cpp files
$(BUILD)/%.o :  $(SRC)/%.cpp   $(BUILD) 
	@echo 'MAKE OBJECT FILE'
	$(CXX) $(CFLAGS) $< -o  $@ 


# Build task
.PHONY:	build
build:
	@echo 'BUILDING: ' $(SRC)
	$(MD) -vp $(BIN)
	$(MD) -vp $(BUILD)
	$(MAKE) $(TARGET)
	@echo '***************'

# Run task
.PHONY: run
run:
	$(TARGET)

# Clean task
.PHONY: clean
clean:
	@echo 'CLEANUP!'
	rm -rvf $(BUILD)
	rm -rvf $(BIN)
	@echo '****************'

# explains the options
.PHONY: help
help:
	@echo 'HELP!'
	@echo "make       - Cleans + builds project"
	@echo "make clean - Removes object file folder and executable"
	@echo "make build - Complies and Builds project"
	@echo "make run   - Runs built executable file"
	@echo "make help  - Prints help message"
	@echo '***************'
