#Command reference
#https://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html

# Install
BIN = scheduler

tool_path := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))../
MUSL_PATH = $(tool_path)/dependencies/musl

#Tool Names (use the musl-gcc wrapper for gcc)
CC=$(MUSL_PATH)/bin/musl-gcc

# Flags
CFLAGS += -g -static

SRC = src/main.c
OBJ = $(SRC:.c=.o)

#Export the musl install directory for musl-gcc and musl-gcc.specs file
$(BIN): export MUSL_DIR = $(MUSL_PATH)

$(BIN):
	@mkdir -p bin
	rm -f bin/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o bin/$(BIN)
