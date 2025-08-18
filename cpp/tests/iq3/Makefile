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
CUT_SRC_DIR = $(SOURCES_DIR)/iq

# Source files
PART3_SRC = $(CUT_SRC_DIR)/part3.cpp
PART3_OBJ = $(CUT_SRC_DIR)/part3.o

UTEST_SRC = utest.cpp
UTEST_OBJ = utest.o

OBJS = $(UTEST_OBJ) $(PART3_OBJ)

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v

# Compile part3.cpp into an object file
$(PART3_OBJ): $(PART3_SRC) $(CUT_INC_DIR)/part3.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Compile the test file
$(UTEST_OBJ): $(UTEST_SRC) $(CUT_INC_DIR)/part3.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Link everything into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
