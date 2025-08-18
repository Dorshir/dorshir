CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall 
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g
CXXFLAGS += -pthread

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc
UTEST_DIR = ./tests

CUT_INC_DIR = $(INCLUDES_DIR)/mt


UTEST_SRC = utest.cpp
UTEST_OBJ = utest.o

OBJS = $(UTEST_OBJ)

TARGET = utest

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^


utest.o: $(CUT_INC_DIR)/blocking_queue.hpp


all: $(TARGET)


check: $(TARGET)
	./$(TARGET) -v


races: $(TARGET)
	valgrind --tool=helgrind ./$(TARGET)


leaks: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)


clean:
	$(RM) $(OBJS) $(TARGET)
