TARGET= dynamic_vector_tests
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS= dynamicVector.o tests.o

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS)


tests.o: tests.c dynamicVector.h
		$(CC) $(CFLAGS) -c tests.c

dynamicVector.o: dynamicVector.c dynamicVector.h
		$(CC) $(CFLAGS) -c dynamicVector.c

clean:
		rm -f $(TARGET) $(OBJS)