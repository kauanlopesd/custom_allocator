CC = gcc
CFLAGS = -Wall -pedantic-errors -Werror -std=c17
MODULES = src/main.c src/memory/mem.c
TARGET = bin/main

all: build
build: $(MODULES)
	$(CC) $(CFLAGS) $(MODULES) -o $(TARGET)
clean: $(TARGET)
	rm -f $(TARGET)