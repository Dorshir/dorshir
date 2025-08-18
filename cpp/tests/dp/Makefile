CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc
SOURCES_DIR = ../../src

CUT_INC_DIR = $(INCLUDES_DIR)/dp


UTEST_OBJS = utest.o

OBJS =  $(UTEST_OBJS)

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(OBJECT_POOL): $(CUT_INC_DIR)/object_pool.hpp $(CUT_INC_DIR)/object_pool.txx 
$(TOKEN): $(CUT_INC_DIR)/object_pool.hpp $(CUT_INC_DIR)/token.hpp
$(POLICY_THROW): $(CUT_INC_DIR)/policy_throw.hpp
$(POLICY_EXPAND): $(CUT_INC_DIR)/policy_expand.hpp
$(FACTORY): $(CUT_INC_DIR)/factory_method.hpp
$(POLICY_BACK_TO_NORMAL): $(CUT_INC_DIR)/policy_back_to_normal.hpp
$(OBJECT_POOL_EXCEPTION): $(CUT_INC_DIR)/object_pool_exepction.hpp



clean:
	$(RM) $(OBJS) $(TARGET)

