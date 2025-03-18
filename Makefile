# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable
TARGET = game

# Source files
SRCS = main.c character-manager.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target to build the program
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
