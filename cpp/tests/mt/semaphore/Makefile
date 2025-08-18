CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall 
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g
CXXFLAGS += -pthread

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

UTEST_DIR = ./tests

INCLUDES_DIR = ../../../inc
SOURCES_DIR = ../../../src

CUT_INC_DIR = $(INCLUDES_DIR)/mt
CUT_SRC_DIR = $(SOURCES_DIR)/mt

SEMAPHORE = $(CUT_SRC_DIR)/semaphore.o
OBJS = $(SEMAPHORE) utest.o

UTEST_SRC = utest.cpp
UTEST_OBJ = utest.o


TARGET = utest

$(SEMAPHORE): $(CUT_INC_DIR)/semaphore.hpp

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^

utest.o: $(CUT_INC_DIR)/semaphore.hpp


all: $(TARGET)


check: $(TARGET)
	./$(TARGET) -v


races: $(TARGET)
	valgrind --tool=helgrind ./$(TARGET)


leaks: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)


clean:
	$(RM) $(OBJS) $(TARGET)
