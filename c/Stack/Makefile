TARGET= stack_tests
CC= gcc
CFLAGS = -ansi -g -pedantic -Wall
OBJS= stack.o stack_tests.o dynamicVector.o

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS)


stack_tests.o: stack_tests.c stack.h
		$(CC) $(CFLAGS) -c stack_tests.c

stack.o: stack.c stack.h dynamicVector.h
		$(CC) $(CFLAGS) -c stack.c

dynamicVector.o: dynamicVector.c dynamicVector.h
		$(CC) $(CFLAGS) -c dynamicVector.c
clean:
		rm -f $(TARGET) $(OBJS)