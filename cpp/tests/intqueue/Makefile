CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc
SOURCES_DIR = ../../src

CUT_INC_DIR = $(INCLUDES_DIR)/ds
CUT_SRC_DIR = $(SOURCES_DIR)/ds

INTQUEUE = $(CUT_SRC_DIR)/intqueue.o
INTSTACK = $(CUT_SRC_DIR)/intstack.o

OBJS = $(INTQUEUE) $(INTSTACK) utest.o

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(INTSTACK): $(CUT_INC_DIR)/intstack.hpp

$(INTQUEUE): $(CUT_INC_DIR)/intqueue.hpp $(CUT_INC_DIR)/intstack.hpp
utest.o: $(CUT_INC_DIR)/intqueue.hpp


clean:
	$(RM) $(OBJS) $(TARGET)

