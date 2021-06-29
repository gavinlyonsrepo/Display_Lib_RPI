# Makefile to build example program for ER_OLEDM1_CH1115 library.
# URL: https://github.com/gavinlyonsrepo/ER_OLEDM1_CH1115_RPI

# ************************************************
# *** User SRC directory Option Section **********
# pick on example "SRC" dir and ONE ONLY.
# Comment out the other 5. #

SRC=src/HELLO_WORLD
#SRC=src/BITMAP
#SRC=src/MULTI_BUFFER
#SRC=src/OLED_FUNCTIONS
#SRC=src/SOFTWARE_SPI
#SRC=src/TEXT_GRAPHICS
# ************************************************

CC=g++
LDFLAGS= -lbcm2835 -lrt -lER_OLEDM1_CH1115_RPI 
CFLAGS = -Iinclude/ -c -Wall 
MD=mkdir
OBJ=obj
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,  $(OBJ)/%.o, $(SRCS))
BIN=./bin
TARGET=$(BIN)/test

all: clean pre-build $(TARGET) 

pre-build:
	@echo '*******'
	@echo '[START!]'
	$(MD) -vp $(BIN)
	$(MD) -vp $(OBJ)


$(TARGET): $(OBJS) $(OBJ)
	@echo '[MAKE EXE FILE!]'
	$(CC)  $(OBJS) -o $@ $(LDFLAGS)
	@echo '[DONE!]'

$(OBJ)/%.o :  $(SRC)/%.cpp   $(OBJ) 
	@echo '[MAKE OBJECT FILE!]'
	$(CC) $(CFLAGS) $< -o  $@ 

clean:
	@echo '*******'
	@echo '[CLEANUP!]'
	rm -vf $(OBJS) $(TARGET)
	rm -rvf $(BIN) $(OBJ)
	@echo '[DONE!]'
