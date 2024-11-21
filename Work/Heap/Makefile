TARGET= genheap_tests
CC= gcc
CFLAGS = -g -pedantic -Wall
OBJS= genheap.o genheap_tests.o genvec.o

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS)


genheap_tests.o: genheap_tests.c genheap.h
		$(CC) $(CFLAGS) -c genheap_tests.c

genheap.o: genheap.c genheap.h genvec.h
		$(CC) $(CFLAGS) -c genheap.c

genvec.o: genvec.c genvec.h
		$(CC) $(CFLAGS) -c genvec.c
clean:
		rm -f $(TARGET) $(OBJS)