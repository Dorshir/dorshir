# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g 

# Directories
SRC_DIR = ..
TEST_DIR = tests
OBJ_DIR = obj

# Source files
SRC_FILES = $(SRC_DIR)/calctime.c $(SRC_DIR)/executor.c $(SRC_DIR)/task.c
TEST_FILES = $(TEST_DIR)/testCalctime.c $(TEST_DIR)/testExecutor.c $(TEST_DIR)/testTask.c

# Object files
OBJ_FILES = $(OBJ_DIR)/calctime.o $(OBJ_DIR)/executor.o $(OBJ_DIR)/task.o

# Dynamic library
LIBS = -L. -lds 

# Include directories
INCLUDE = -I.

# Targets
TARGETS = testCalctime testExecutor testTask

# Default target: Build everything
all: $(OBJ_FILES) $(TARGETS)

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to compile object files
$(OBJ_DIR)/calctime.o: $(SRC_DIR)/calctime.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/executor.o: $(SRC_DIR)/executor.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/task.o: $(SRC_DIR)/task.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Rules to create test executables
testCalctime: $(OBJ_FILES) $(TEST_DIR)/testCalctime.c
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

testExecutor: $(OBJ_FILES) $(TEST_DIR)/testExecutor.c
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

testTask: $(OBJ_FILES) $(TEST_DIR)/testTask.c
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGETS)
