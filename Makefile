# ===================
# Gavin lyons 01-2024
# Makefile to install library for Display_Lib_RPI library.
# Project Name: Display_Lib_RPI
# URL: https://github.com/gavinlyonsrepo/Display_Lib_RPI
# 
# Library is installed to /usr/local/lib and 
# include files are placed at /usr/local/include by default.
# Run 'make help' for user options
# ==================

#The --no-print-directory option of make tells make not to print the message 
#about entering #and leaving the working directory. 
MAKEFLAGS += --no-print-directory

# Pre-compiler and Compiler flags
CXX_FLAGS := -march=native -mtune=native -mcpu=native -Wall -Wextra  -std=c++20 

# Project directory structure
SRC := src
LIBRARY := lib
INC := include
# Build directories and output
BUILD := build
BIN := bin

# Where you want in system library installed
PREFIX=/usr/local
# where to place the library after install
LIBDIR=$(PREFIX)/lib
# library name , linker flag -lrpidisplaygl
LIB=librpidisplaygl
# shared library name
LIBNAME=$(LIB).so.1.0
# where to place the headers files after install
LIBHEADERDIR=$(PREFIX)/include/

# Other Library search directories and flags
EXT_LIB :=
LDFLAGS := 
LDPATHS := $(addprefix -L,$(LIBRARY) $(EXT_LIB))

# Include directories
INC_DIRS := $(INC) $(shell find $(SRC) -type d) 
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Construct build output and dependency filenames
SRCS := $(shell find $(SRC) -name *.cpp)
OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))
DEPS := $(OBJS:.o=.d)

# Include all dependencies
-include $(DEPS)

# Main task, clean then makes build directory, updates your objects, builds your library
.PHONY: all
all: clean buildlib

# Task producing library from built files
.PHONY: buildlib
buildlib: $(OBJS)
	@echo '[MAKING LIBRARY!]'
	mkdir -vp $(BIN)
	$(CXX) -shared -Wl,-soname,$(LIB).so.1 $(CXX_FLAGS) $(LDFLAGS) -o $(BIN)/$(LIBNAME) $^
	@echo '***************'

# Compile all cpp files
$(BUILD)/%.o: $(SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CXX)  -fPIC  $(CXX_FLAGS) $(INC_FLAGS) -c -o $@ $< $(LDPATHS) $(LDFLAGS) 

#install library
.PHONY: install
install:
	@echo "[INSTALL LIBRARY]"
	@if ( test ! -d $(LIBDIR) ) ; then mkdir -vp $(LIBDIR) ; fi
	@install -vm 0755 $(BIN)/$(LIBNAME) $(LIBDIR)
	@ln -svf $(LIBDIR)/$(LIBNAME) $(LIBDIR)/$(LIB).so.1
	@ln -svf $(LIBDIR)/$(LIBNAME) $(LIBDIR)/$(LIB).so
	@ldconfig
	@rm -vf $(BIN)/$(LIBNAME)
	@echo "[INSTALL HEADERS]"
	@if ( test ! -d $(LIBHEADERDIR)) ; then mkdir -p $(LIBHEADERDIR); fi
	@find include/ -type f -name "*.hpp" -exec  cp -v {} $(LIBHEADERDIR) \;
	@echo "*****************"

# Uninstall the library
.PHONY: uninstall
uninstall:
	@echo "[UNINSTALL LIBRARY]"
	@rm -vf $(LIBDIR)/$(LIB).*
	@echo "[UNINSTALL HEADERS]"
	@rm -vf  $(LIBHEADERDIR)*_data_RDL.hpp
	@rm -vf  $(LIBHEADERDIR)*_graphics_RDL.hpp
	@rm -vf  $(LIBHEADERDIR)MAX7219*_RDL.hpp
	@rm -vf  $(LIBHEADERDIR)TM163*plus_*_RDL.hpp
	@rm -vf  $(LIBHEADERDIR)*_LCD_RDL.hpp
	@rm -vf  $(LIBHEADERDIR)*_OLED_RDL.hpp
	@echo "******************"

# Clean task
.PHONY: clean
clean:
	@echo '[CLEANUP!]'
	rm -rvf $(BUILD)
	rm -rvf $(BIN)
	@echo '***************'

# Help task , explains the options you can use
.PHONY: help
help:
	@echo '[HELP!]'
	@echo "make           - Cleans then builds library"
	@echo "make clean     - Cleans, Removes object file folder and library folder(BIN & BUILD)"
	@echo "make buildlib  - Builds library"
	@echo "make install   - Installs library, may need sudo"
	@echo "make uninstall - Uninstalls library, may need sudo"
	@echo "make help      - Prints help message"
	@echo '***************'
