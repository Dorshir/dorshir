TARGETS = tcpclient.out tcpserver.out

CC = gcc
CFLAGS = -g -pedantic -I../../DoubleLinkedList/
LDFLAGS = -L../../DS_LIB -lds -lm


.PHONY: all clean

all: $(TARGETS)

tcpclient.out: tcpclient.c
	$(CC) $(CFLAGS) tcpclient.c -o tcpclient.out ../../DS_LIB/new_gen_dlist.o $(LDFLAGS)

tcpserver.out: tcpserver.c
	$(CC) $(CFLAGS) tcpserver.c -o tcpserver.out ../../DS_LIB/new_gen_dlist.o $(LDFLAGS)

clean:
	rm -f $(TARGETS)
