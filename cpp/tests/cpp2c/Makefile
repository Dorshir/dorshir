CXX = g++
CC = gcc

CXXFLAGS = -std=c++17 -pedantic -Wall -Wextra -Werror
CFLAGS = -pedantic -Wall -Wextra -Werror

OBJS_C = main_c.o
OBJS_CPP = main_cpp.o

TARGET_C = main_c
TARGET_CPP = main_cpp

all: $(TARGET_C) $(TARGET_CPP)

$(TARGET_C): $(OBJS_C)
	$(CC) $(OBJS_C) -o $(TARGET_C)

$(TARGET_CPP): $(OBJS_CPP)
	$(CXX) $(OBJS_CPP) -o $(TARGET_CPP)

main_c.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main_c.o

main_cpp.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main_cpp.o

check: all
	./$(TARGET_C)
	./$(TARGET_CPP)

clean:
	rm -f $(OBJS_C) $(OBJS_CPP) $(TARGET_C) $(TARGET_CPP)
