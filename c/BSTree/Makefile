TARGET= BSTree_tests
CC= gcc
CFLAGS = -g -pedantic -Wall
OBJS= BSTree.o BSTree_tests.o 

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS)


BSTree_tests.o: BSTree_tests.c BSTree.h
		$(CC) $(CFLAGS) -c BSTree_tests.c

BSTree.o: BSTree.c BSTree.h
		$(CC) $(CFLAGS) -c BSTree.c


clean:
		rm -f $(TARGET) $(OBJS)