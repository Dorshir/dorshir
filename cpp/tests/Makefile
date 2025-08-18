CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc
SOURCES_DIR = ../../src

CUT_INC_DIR = $(INCLUDES_DIR)/algebra
CUT_SRC_DIR = $(SOURCES_DIR)/algebra

RATIONAL = $(CUT_SRC_DIR)/rational.o
OBJS = $(RATIONAL) utest.o

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(RATIONAL): $(CUT_INC_DIR)/rational.hpp
utest.o: $(CUT_INC_DIR)/rational.hpp


clean:
	$(RM) $(OBJS) $(TARGET)

