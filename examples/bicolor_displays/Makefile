# Gavin Lyons 01-2024
# Makefile to build example program for
# Display_Lib_RPI Display = bicolor Graphic displays
# Display 1 = SSD1306 OLED I2C
# Display 2 = ERM19264 LCD SPI
# Display 3 = NOKIA5110 LCD SPI
# Display 4 = CH1115 OLED I2C
# Display 5 = Sh1106 OLED I2C
# Display 6 = Sh1107 OLED I2C
# URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
# NOTE : Run make help for options

# ************************************************
# *** User SRC directory Option Section **********
# Pick one example "SRC" dir path and ONE ONLY.
# Comment out the others.

# ================= SSD1306 OLED I2C =============
SRC=ssd1306/HELLO_WORLD
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
#SRC=nokia5110/BITMAP
#SRC=nokia5110/HELLOWORLD_SWSPI
#=================================================

# ============= OLED CH1115 LCD SPI ==============
#SRC=ch1115/HELLO_WORLD
#SRC=ch1115/TEXT_GRAPHICS_FUNCTIONS
#SRC=ch1115/FPS_TEST
#SRC=ch1115/FPS_TEST_SW_SPI
#SRC=ch1115/BITMAP
#================================================

# ================= SH1106 OLED I2C =============
#SRC=sh1106/HELLO_WORLD
#SRC=sh1106//BITMAP
#=================================================

# ================= SH1107 OLED I2C =============
#SRC=sh1107//HELLO_WORLD
#=================================================

# ************************************************

# The --no-print-directory option of make tells make not to print the message about entering
# and leaving the working directory.
MAKEFLAGS += --no-print-directory

# Pre-compiler and Compiler and linker flags
LDFLAGS= -lbcm2835 -lrpidisplaygl
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
	sudo $(TARGET)

# Clean task
.PHONY: clean
clean:
	@echo 'CLEANUP!'
	rm -rvf $(BUILD)
	rm -rvf $(BIN)
	@echo '****************'

# help task : print help to explains the options
.PHONY: help
help:
	@echo 'HELP!'
	@echo "make       - Cleans + builds project"
	@echo "make clean - Removes object file folder and executable"
	@echo "make build - Complies and Builds project"
	@echo "make run   - Runs exe(wraps sudo, bcm2835 lib needs sudo)"
	@echo "make help  - Prints help message"
	@echo '***************'
