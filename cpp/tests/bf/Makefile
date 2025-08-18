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

MEMORY = $(CUT_SRC_DIR)/memory.o
IO = $(CUT_SRC_DIR)/io.o
COMPILER = $(CUT_SRC_DIR)/compiler.o
CPU = $(CUT_SRC_DIR)/cpu.o
VM = $(CUT_SRC_DIR)/vm.o
OPTIMIZER = $(CUT_SRC_DIR)/optimizer.o

UTEST_OBJS = utest.o

OBJS = $(MEMORY) $(IO) $(COMPILER) $(CPU) $(VM) $(OPTIMIZER) $(UTEST_OBJS)

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(MEMORY): $(CUT_INC_DIR)/memory.hpp
$(IO): $(CUT_INC_DIR)/io.hpp
$(COMPILER): $(CUT_INC_DIR)/compiler.hpp
$(CPU): $(CUT_INC_DIR)/cpu.hpp
$(VM): $(CUT_INC_DIR)/vm.hpp

$(UTEST_OBJS): $(CUT_INC_DIR)/memory.hpp $(CUT_INC_DIR)/io.hpp $(CUT_INC_DIR)/compiler.hpp $(CUT_INC_DIR)/cpu.hpp $(CUT_INC_DIR)/vm.hpp


clean:
	$(RM) $(OBJS) $(TARGET)

