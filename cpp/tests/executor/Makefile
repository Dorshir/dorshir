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

CUT_INC_DIR = $(INCLUDES_DIR)/executor
CUT_SRC_DIR = $(SOURCES_DIR)/executor

PER_EXEC = $(CUT_SRC_DIR)/periodic_executor.o
MONO_CLOCK = $(CUT_SRC_DIR)/monotonic_clock.o
REALTIME_CLOCK = $(CUT_SRC_DIR)/realtime_clock.o
TIME = $(CUT_SRC_DIR)/time.o

UTEST_OBJS = utest.o

OBJS = $(PER_EXEC) $(TIME) $(MONO_CLOCK) $(REALTIME_CLOCK) $(UTEST_OBJS)

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(PER_EXEC): $(CUT_SRC_DIR)/periodic_executor.cpp $(CUT_INC_DIR)/periodic_executor.hpp
$(MONO_CLOCK) : $(CUT_SRC_DIR)/monotonic_clock.cpp $(CUT_INC_DIR)/monotonic_clock.hpp
$(REALTIME_CLOCK) : $(CUT_SRC_DIR)/realtime_clock.cpp $(CUT_INC_DIR)/realtime_clock.hpp
$(TIME): $(CUT_SRC_DIR)/time.cpp $(CUT_INC_DIR)/time.hpp

$(UTEST_OBJS): $(CUT_INC_DIR)/periodic_executor.hpp $(CUT_INC_DIR)/time.hpp


clean:
	$(RM) $(OBJS) $(TARGET)

