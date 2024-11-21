TARGET= sorts
CC= gcc
CFLAGS = -g -pedantic -Wall
LDFLAGS = -lm
OBJS= sorts.o

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)


sorts.o: sorts.c
		$(CC) $(CFLAGS) -c sorts.c

clean:
		rm -f $(TARGET) $(OBJS)