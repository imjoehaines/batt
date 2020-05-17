.POSIX:
.SUFFIXES:

CC ?= gcc
CFLAGS = -Wall -Wextra -Werror -Wswitch-enum -Wduplicated-branches \
	 -Wduplicated-cond -Wstrict-prototypes -Wmissing-prototypes \
	 -Wold-style-definition -Wmissing-declarations \
	 -pedantic -pedantic-errors \
	 -fanalyzer \
	 -std=c17
INSTALL_PATH ?= /usr/bin

TARGET_EXEC ?= batt

.PHONY: all install uninstall clean
all: build/$(TARGET_EXEC)

build/$(TARGET_EXEC): build/batt.o
	$(CC) $(LDFLAGS) $^ -o $@

build/batt.o: batt.c
	$(CC) $(CFLAGS) -c $< -o $@

install: build/$(TARGET_EXEC)
	mv build/$(TARGET_EXEC) $(INSTALL_PATH)/$(TARGET_EXEC)

uninstall:
	rm $(INSTALL_PATH)/$(TARGET_EXEC)

clean:
	rm -r build/*
