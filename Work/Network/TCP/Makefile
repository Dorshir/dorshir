TARGETS = tcpClient.out tcpServer.out tcpMultiClient.out

CC = gcc
CFLAGS = -g -pedantic -I../../DoubleLinkedList/
LDFLAGS = -L../../DS_LIB -lds -lm


.PHONY: all clean

all: $(TARGETS)

tcpClient.out: tcpClient.c
	$(CC) $(CFLAGS) tcpClient.c -o tcpClient.out

tcpServer.out: tcpServer.c
	$(CC) $(CFLAGS) tcpServer.c -o tcpServer.out ../../DS_LIB/new_gen_dlist.o $(LDFLAGS)

tcpMultiClient.out: tcpMultiClient.c
	$(CC) $(CFLAGS) tcpMultiClient.c -o tcpMultiClient.out

clean:
	rm -f $(TARGETS)
