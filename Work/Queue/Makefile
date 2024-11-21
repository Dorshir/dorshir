TARGET= genQueue_tests
CC= gcc
CFLAGS = -ansi -g -pedantic -Wall
OBJS= genQueue.o genQueue_tests.o 

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS)


genQueue_tests.o: genQueue_tests.c genQueue.h
		$(CC) $(CFLAGS) -c genQueue_tests.c

genQueue.o: genQueue.c genQueue.h
		$(CC) $(CFLAGS) -c genQueue.c


clean:
		rm -f $(TARGET) $(OBJS)