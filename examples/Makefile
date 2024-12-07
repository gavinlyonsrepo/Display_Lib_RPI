## Gavin Lyons 01 -2024
# Makefile to build example program for Display_Lib_RPI 
## URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
# Run make help for options 

# === SUPPORTED DISPLAY LIST  ===
# Display 1 = MAX7219
# Display 2 = TM1638
# Display 3 = HD44780 LCD I2C
# Display 4 = SSD1306 OLED I2C
# Display 5 = ERM19264 LCD SPI
# Display 6 = NOKIA5110 LCD SPI
# Display 7 = CH1115 OLED I2C
# Display 8 = Sh1106 OLED I2C
# Display 9 = Sh1107 OLED I2C
# Display 10 = ST7735X  TFT LCD SPI
# Display 11 = ST7789   TFT LCD SPI
# Display 12 = ILI9341  TFT LCD SPI
# ==================================

# ***********************************************
# ***********************************************
# ** User SRC directory path Option Section A ***
# 1. Pick an example file "SRC" path and ONE ONLY.
# 2. Comment out the others.

# ================= MAX7219_7SEG ================
#SRC=max7219/TESTS_HWSPI
#SRC=max7219/TESTS_SWSPI
#SRC=max7219/BCDMODE
#SRC=max7219/CLOCK_DEMO
#SRC=max7219/CASCADE_DEMO
#================================================

# ================= TM1638 ======================
#SRC=tm1638/TEST_Model_1_3
#SRC=tm1638/TEST_Model2
# ================================================

# ================= HD44780 LCD I2C ==============
#SRC=hd44780/TEST_16x02
#SRC=hd44780/TEST_20x04
#SRC=hd44780/CLOCK_16x02
#SRC=hd44780/TEST_I2C_16x02
#=================================================

# ================= SSD1306 OLED I2C =============
#SRC=ssd1306/HELLO_WORLD
#SRC=ssd1306/TEXT_GRAPHICS_FUNCTIONS
#SRC=ssd1306/SPEED_TEST
#SRC=ssd1306/BITMAP
#SRC=ssd1306/CLOCK_DEMO
#=================================================

# ========== ERM19264_UC1609 LCD SPI==============
#SRC=erm19264/HELLO_WORLD
#SRC=erm19264/TEXT_GRAPHICS_FUNCTIONS
#SRC=erm19264/FRAMERATE_PER_SEC
#SRC=erm19264/BITMAP
#SRC=erm19264/SOFTWARE_SPI
#=================================================

# ================ NOKIA5110 LCD SPI=============
#SRC=nokia5110/HELLOWORLD
#SRC=nokia5110/TEXT_GRAPHICS_FUNCTIONS
#SRC=nokia5110/SPEEDTEST
#SRC=nokia5110/SPEEDTEST_SWSPI
#SRC=nokia5110/BITMAP
#=================================================

# ============= CH1115 OLED SPI =================
#SRC=ch1115/HELLO_WORLD
#SRC=ch1115/TEXT_GRAPHICS_FUNCTIONS
#SRC=ch1115/FPS_TEST
#SRC=ch1115/FPS_TEST_SW_SPI
#SRC=ch1115/BITMAP
#================================================

# =========== SH1106 or SH1107 OLED I2C ==========
#SRC=sh1106/HELLO_WORLD
#SRC=sh1107/HELLO_WORLD
#=================================================

# =============== ST7735 TFT LCD SPI =============
#SRC=st7735/Hello_world
#SRC=st7735/Hello_world_SWSPI
#SRC=st7735/Text_Graphics_Functions
#SRC=st7735/Bitmap_Tests
#SRC=st7735/Frame_rate_test_bmp
#SRC=st7735/Frame_rate_test_two
#SRC=st7735/Multiple_displays
#=================================================

# =============== ST7789 TFT LCD SPI =============
#SRC=st7789/Hello_world
#SRC=st7789/Hello_world_SWSPI
#SRC=st7789/Bitmap_Tests
#SRC=st7789/Text_Graphics_Functions
#SRC=st7789/Frame_rate_test_bmp
#SRC=st7789/Frame_rate_test_two
#=================================================

# =============== ILI9341 TFT LCD SPI ============
#SRC=ili9341/Hello_world
#SRC=ili9341/Hello_world_SWSPI
#SRC=ili9341/Bitmap_Tests
#SRC=ili9341/Text_Graphics_Functions
#SRC=ili9341/Mandelbrot_set
#SRC=ili9341/Touch_Screen
#=================================================

# =========== XPT2046 touchscreen basic test ======
#SRC=xpt2046/xpt_Test
#=================================================

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
LDFLAGS= -llgpio -lrpidisplaygl 
CFLAGS= -Iinclude/ -c -Wall -Wextra -std=c++20

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
