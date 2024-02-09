# Gavin Lyons 01-2024
# Makefile to build example program for 
# Display_Lib_RPI Display = Character Displays file
# Display 1 = HD44780_LCD_I2C
# URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
# Run make help for options 

# ************************************************
# **** User SRC directory Option Section     ****
# Pick on example "#SRC" dir and ONE ONLY. 
# Comment out the others. 

# ================= HD44780_LCD_I2C ==============
SRC=hd44780/TEST_16x02
#SRC=hd44780/TEST_20x04
#SRC=hd44780/CLOCK_16x02
#SRC=hd44780/TEST_I2C_16x02
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

# explains the options
.PHONY: help
help:
	@echo 'HELP!'
	@echo "make       - Cleans + builds project"
	@echo "make clean - Removes object file folder and executable"
	@echo "make build - Complies and Builds project"
	@echo "make run   - Runs exe(wraps sudo, bcm2835 lib needs sudo)"
	@echo "make help  - Prints help message"
	@echo '***************'
