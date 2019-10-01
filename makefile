# Usage:
# make          # Same as "target"
# make target   # Compile the porgram
# make clean    # Clean the build folder

.PHONY: default target clean

# Compiler to use
CC = gcc
# Flags for compiling C
CFLAGS = -Iinclude
# Flags for linking
LFLAGS = -L$(LIB_DIR) -lws2_32
# Output target
TARGET = bin/fp_micro.exe

LIB_DIR = lib
SRC_DIR = src
OBJ_DIR = build

LIB = $(wildcard $(LIB_DIR)/*.a)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

default: clean target

target: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -g $^ $(LIB) -o $@ $(LFLAGS)

$(OBJ):
	$(CC) $(CFLAGS) -c $(SRC_DIR)/$(*F).c -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf *.o $(OBJ)
