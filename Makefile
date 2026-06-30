# Makefile for C-Thread-Pool example
# 
# This Makefile is designed for Unix-like systems (Linux, macOS, BSD)
# For Windows, use the provided .bat files instead:
#   - build.bat: Debug build
#   - build_release.bat: Release build
#   - clean.bat: Clean build artifacts
#
# Usage:
#   make          - Build the example (default, debug mode)
#   make debug    - Build with debug symbols
#   make release  - Build optimized for production
#   make clean    - Remove build artifacts
#   make run      - Build and run the example
#   make help     - Show available targets

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99
DEBUG_FLAGS = -D THPOOL_DEBUG
LDFLAGS = -pthread

# Source files
SOURCES = thpool.c example.c
TARGET = example

# Improved example
IMPROVED_SOURCES = thpool.c example_improved.c
IMPROVED_TARGET = example_improved

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build improved example
improved: $(IMPROVED_TARGET)

$(IMPROVED_TARGET): $(IMPROVED_SOURCES:.c=.o)
	$(CC) $^ -o $@ $(LDFLAGS)

# Build the example executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += -g -O0
debug: clean $(TARGET)

# Release build (optimized)
release: CFLAGS += -O2 -DNDEBUG
release: DEBUG_FLAGS =
release: clean $(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) $(IMPROVED_SOURCES:.c=.o) $(IMPROVED_TARGET)

# Run the example
run: $(TARGET)
	./$(TARGET)

# Run the improved example
run-improved: $(IMPROVED_TARGET)
	./$(IMPROVED_TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the basic example (default)"
	@echo "  improved  - Build the improved example"
	@echo "  debug     - Build with debug symbols and no optimization"
	@echo "  release   - Build with optimization for production"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the basic example"
	@echo "  run-improved - Build and run the improved example"
	@echo "  help      - Show this help message"

.PHONY: all improved debug release clean run run-improved help
