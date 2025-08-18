CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc

CUT_INC_DIR = $(INCLUDES_DIR)/ds

LIST = $(CUT_SRC_DIR)/list.o

OBJS = utest.o

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)


utest.o: $(CUT_INC_DIR)/list.hpp


clean:
	$(RM) $(OBJS) $(TARGET)

