# Compiler and flags
CC = g++
CFLAGS = -Wall -std=c++11
TARGET = my-sum

# List of object files (use src/ prefix for .cpp files)
OBJS = src/main.o src/validate.o src/barrier.o src/io_handling.o

# Default target (compiles the program)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rules for compiling .cpp files into .o files (specify src/ for .cpp files)
src/main.o: src/main.cpp include/validate.h include/barrier.h include/io_handling.h
	$(CC) $(CFLAGS) -c src/main.cpp -o src/main.o

src/validate.o: src/validate.cpp include/validate.h
	$(CC) $(CFLAGS) -c src/validate.cpp -o src/validate.o

src/barrier.o: src/barrier.cpp include/barrier.h
	$(CC) $(CFLAGS) -c src/barrier.cpp -o src/barrier.o

src/io_handling.o: src/io_handling.cpp include/io_handling.h
	$(CC) $(CFLAGS) -c src/io_handling.cpp -o src/io_handling.o

# Clean up compiled files
clean:
	rm -f $(TARGET) src/*.o
