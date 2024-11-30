CC = gcc
CFLAGS = -g -pedantic -I../DynamicVector/ -I../Heap/
LDFLAGS = -L../DS_LIB -lds -lm
OBJS = executor.o calctime.o task.o

all : $(OBJS)

.PHONY: clean

executor.o: executor.c executor.h task.h
	$(CC) $(CFLAGS) -c executor.c

task.o: task.c task.h calctime.h
	$(CC) $(CFLAGS) -c task.c

calctime.o: calctime.c calctime.h
	$(CC) $(CFLAGS) -c calctime.c

clean:
	rm -f $(OBJS) $(TARGET)
