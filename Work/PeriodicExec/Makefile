TARGET = main.out

CC = gcc
CFLAGS = -g -pedantic -I../DynamicVector/ -I../Heap/
LDFLAGS = -L../DS_LIB -lds -lm
OBJS = main.o executor.o calctime.o task.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

.PHONY: clean

main.o: main.c executor.h calctime.h task.h
	$(CC) $(CFLAGS) -c main.c

executor.o: executor.c executor.h task.h
	$(CC) $(CFLAGS) -c executor.c

task.o: task.c task.h calctime.h
	$(CC) $(CFLAGS) -c task.c

calctime.o: calctime.c calctime.h
	$(CC) $(CFLAGS) -c calctime.c

clean:
	rm -f $(OBJS) $(TARGET)
