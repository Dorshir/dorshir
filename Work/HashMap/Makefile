TARGET= hashmap_tests
CC= gcc
CFLAGS = -g -pedantic -Wall
LDFLAGS = -lm
OBJS= hashmap.o hashmap_tests.o new_gen_dlist.o

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)


hashmap_tests.o: hashmap_tests.c hashmap.h
		$(CC) $(CFLAGS) -c hashmap_tests.c

hashmap.o: hashmap.c hashmap.h new_gen_dlist.h
		$(CC) $(CFLAGS) -c hashmap.c

new_gen_dlist.o: new_gen_dlist.c new_gen_dlist.h
		$(CC) $(CFLAGS) -c new_gen_dlist.c


clean:
		rm -f $(TARGET) $(OBJS)