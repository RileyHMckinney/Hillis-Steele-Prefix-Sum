# Compiler and flags
CC = g++
CFLAGS = -Wall -std=c++11
TARGET = my-sum

# List of source files and object files
SRC = src/main.cpp src/validate.cpp src/barrier.cpp src/io_handling.cpp
OBJS = $(SRC:.cpp=.o)

# Default target: Compile the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@$(MAKE) clean  # Automatically clean .o files after compilation

# Generic rule to compile .cpp files into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJS)
