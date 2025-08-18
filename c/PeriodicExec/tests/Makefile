TARGETS = testCalctime.out testExecutor.out testTask.out

CC = gcc
CFLAGS = -g -pedantic -I.. -I../../DynamicVector/ -I../../Heap/
LDFLAGS = -L../../DS_LIB -lds -lm
BASE_DIR = ..

all: $(TARGETS)

testCalctime.out: testCalctime.o $(BASE_DIR)/calctime.o
	$(CC) -o testCalctime.out testCalctime.o $(BASE_DIR)/calctime.o $(LDFLAGS)

testExecutor.out: testExecutor.o $(BASE_DIR)/executor.o $(BASE_DIR)/task.o $(BASE_DIR)/calctime.o
	$(CC) -o testExecutor.out testExecutor.o $(BASE_DIR)/executor.o $(BASE_DIR)/task.o $(BASE_DIR)/calctime.o $(LDFLAGS)

testTask.out: testTask.o $(BASE_DIR)/task.o $(BASE_DIR)/calctime.o
	$(CC) -o testTask.out testTask.o $(BASE_DIR)/task.o $(BASE_DIR)/calctime.o $(LDFLAGS)

testCalctime.o: testCalctime.c $(BASE_DIR)/calctime.h
	$(CC) $(CFLAGS) -c testCalctime.c

testExecutor.o: testExecutor.c $(BASE_DIR)/executor.h $(BASE_DIR)/task.h $(BASE_DIR)/calctime.h
	$(CC) $(CFLAGS) -c testExecutor.c

testTask.o: testTask.c $(BASE_DIR)/task.h $(BASE_DIR)/calctime.h
	$(CC) $(CFLAGS) -c testTask.c

$(BASE_DIR)/calctime.o: $(BASE_DIR)/calctime.c $(BASE_DIR)/calctime.h
	$(CC) $(CFLAGS) -c $(BASE_DIR)/calctime.c -o $(BASE_DIR)/calctime.o

.PHONY: clean

clean:
	rm -f $(TARGETS) testCalctime.o testExecutor.o testTask.o $(BASE_DIR)/calctime.o
