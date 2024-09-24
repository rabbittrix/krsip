CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -I/usr/local/opt/openssl/include -g
LDFLAGS = -L/usr/local/opt/openssl/lib -lssl -lcrypto

krsip: krsip.o

    $(CC) $(CFLAGS) -o krsip krsip.o $(LDFLAGS)

krsip.o: krsip.c

    $(CC) $(CFLAGS) -c krsip.c

# Define the directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
PLATFORMS = windows linux ios android

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Library name
LIBRARY = libkrsip.a

# Rules
.PHONY: all clean $(PLATFORMS)

all: $(LIBRARY)

# Build the library
$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Platform specific builds
windows: $(LIBRARY)
	@echo "Building for Windows"
	# Additional Windows specific commands can be added here

linux: $(LIBRARY)
	@echo "Building for Linux"
	$(CC) $(LDFLAGS) -o myapp $(LIBRARY)
	# Additional Linux specific commands can be added here

ios: $(LIBRARY)
	@echo "Building for iOS"
	# Additional iOS specific commands can be added here

android: $(LIBRARY)
	@echo "Building for Android"
	# Additional Android specific commands can be added here

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(LIBRARY)

# Help message
help:
	@echo "Makefile for KRSIP project"
	@echo "Usage:"
	@echo "  make            Build the library"
	@echo "  make clean      Remove build files"
	@echo "  make windows    Build for Windows"
	@echo "  make linux      Build for Linux"
	@echo "  make ios        Build for iOS"
	@echo "  make android    Build for Android"
	@echo "  make help       Show this help message"