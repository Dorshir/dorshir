CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc
SOURCES_DIR = ../../src
UTEST_DIR = ./tests

CUT_INC_DIR = $(INCLUDES_DIR)/iq


UTEST_OBJS = utest.o

OBJS = $(UTEST_OBJS)

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)


$(UTEST_OBJS): $(CUT_INC_DIR)/containers.hpp

clean:
	$(RM) $(OBJS) $(TARGET)

