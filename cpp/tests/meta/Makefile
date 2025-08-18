CXX = g++
CC = $(CXX)

CXXFLAGS = -std=c++17 -pedantic -Wall 
CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -g

CPPFLAGS = -I$(INCLUDES_DIR)

INCLUDES_DIR = ../../inc



UTEST_SRC = utest.cpp
UTEST_OBJ = utest.o


OBJS = $(UTEST_OBJ)


TARGET = utest


$(UTEST_OBJ): $(INCLUDES_DIR)/meta.hpp
$(TARGET): $(OBJS)



all: $(TARGET)

check: $(TARGET)
	./$(TARGET) -v


leaks: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
