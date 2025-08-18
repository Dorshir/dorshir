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

FIXED_VECTOR = $(CUT_SRC_DIR)/fixed_vector.o
OBJS = $(FIXED_VECTOR) utest.o

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(FIXED_VECTOR): $(CUT_INC_DIR)/fixed_vector.hpp
utest.o: $(CUT_INC_DIR)/fixed_vector.hpp


clean:
	$(RM) $(OBJS) $(TARGET)

