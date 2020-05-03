.POSIX:
.SUFFIXES:

CC ?= cc
CFLAGS = -Wall -Wextra -pedantic-errors -Wno-padded -std=c17

TARGET_EXEC ?= batt

.PHONY: all
all: build/$(TARGET_EXEC)

build/$(TARGET_EXEC): build/batt.o
	$(CC) $(LDFLAGS) $^ -o $@

build/batt.o: batt.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r build/*
