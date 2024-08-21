# Set CROSS_COMPILE variable to specify the cross-compilation prefix (can be overridden on the command line)
CROSS_COMPILE ?=

# Conditionally define the compiler
ifdef CROSS_COMPILE
    CC := $(CROSS_COMPILE)gcc
else
    CC := gcc
endif

# Define other variables
TARGET := writer
SRC := writer.c
CFLAGS := -Wall -O2

# Default target to build the application
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean target to remove build artifacts
clean:
	rm -f $(TARGET) *.o
