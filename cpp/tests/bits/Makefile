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

CUT_INC_DIR = $(INCLUDES_DIR)/bf
CUT_SRC_DIR = $(SOURCES_DIR)/bf


UTEST_OBJS = utest.o

OBJS =  $(UTEST_OBJS)

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)



$(UTEST_OBJS): 


clean:
	$(RM) $(OBJS) $(TARGET)

