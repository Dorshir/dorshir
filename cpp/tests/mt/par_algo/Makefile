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

CUT_INC_DIR = $(INCLUDES_DIR)/mt

OBJS = utest.o



TARGET = utest

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^


utest.o: $(CUT_INC_DIR)/par_algorithms.hpp


all: $(TARGET)


check: $(TARGET)
	./$(TARGET) -v


races: $(TARGET)
	valgrind --tool=helgrind -s ./$(TARGET)


leaks: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)


clean:
	$(RM) $(OBJS) $(TARGET)
