# Gavin Lyons 01 -2024
# Makefile to build example program for 
# Display_Lib_RPI = Seven Segment Displays file
# Display 1 = MAX7219
# Display 2 = TM1638 
# URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
# Run make help for options 

# ************************************************
# **** User SRC directory Option Section     ****
# Pick on example "#SRC" dir and ONE ONLY. 
# Comment out the others. 

# ================= MAX7219_7SEG ================
SRC=max7219/HELLOWORLD_HWSPI
#SRC=max7219/TESTS
#SRC=max7219/TESTS_HWSPI
#SRC=max7219/BCDMODE
#SRC=max7219/CLOCK_DEMO
#SRC=max7219/CASCADE_DEMO
#================================================

# ================= TM1638 ======================
#SRC=tm1638/HELLOWORLD_Model1
#SRC=tm1638/HELLOWORLD_Model2
#SRC=tm1638/HELLOWORLD_Model3
#SRC=tm1638/TEST_Model1
#SRC=tm1638/TEST_Model2
#SRC=tm1638/TEST_Model3
# ================================================

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
