CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR) -I$(CUT_INC_DIR)

INCLUDES_DIR = ../../inc
SOURCES_DIR = ../../src

CUT_INC_DIR = $(INCLUDES_DIR)/rpn
CUT_SRC_DIR = $(SOURCES_DIR)/rpn


LEXER = $(CUT_SRC_DIR)/lexer.o
PARSER = $(CUT_SRC_DIR)/parser.o
POST_FIX_EXP = $(CUT_SRC_DIR)/post_fix_expression.o

OBJS = $(LEXER) $(PARSER) $(POST_FIX_EXP) utest.o

TARGET = utest

all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


$(TARGET) : $(OBJS)

$(LEXER): $(CUT_INC_DIR)/lexer.hpp
$(PARSER): $(CUT_INC_DIR)/parser.hpp $(CUT_INC_DIR)/lexer.hpp
utest.o: $(LEXER) $(PARSER)


clean:
	$(RM) $(OBJS) $(TARGET)

