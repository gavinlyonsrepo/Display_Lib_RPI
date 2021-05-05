CC=g++
LDFLAGS= -lbcm2835 -lrt
CFLAGS = -Iinclude/ -c -Wall 
MD=mkdir

SRC=src
OBJ=obj
SRCS = $(wildcard $(SRC)/*.cpp)
HDRS=$(wildcard  $(HDR)/*.h)
OBJS = $(patsubst $(SRC)/%.cpp,  $(OBJ)/%.o, $(SRCS))
BIN=./bin
TARGET=$(BIN)/test

all: pre-build $(TARGET) 

pre-build:
	@echo 'START!'
	$(MD) -vp $(BIN)
	$(MD) -vp $(OBJ)


$(TARGET): $(OBJS) $(OBJ)
	@echo 'MAKE EXE FILE'
	$(CC)  $(OBJS) -o $@ $(LDFLAGS)
	@echo 'DONE!'

$(OBJ)/%.o :  $(SRC)/%.cpp   $(OBJ) 
	@echo 'MAKE OBJECT FILE'
	$(CC) $(CFLAGS) $< -o  $@ 

clean:
	@echo 'CLEANUP!'
	rm -vf  $(OBJS) $(TARGET)
	@echo 'DONE!'



