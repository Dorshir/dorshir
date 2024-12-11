CC = gcc
CFLAGS = -g -pedantic -ImyDS
LDFLAGS = -LmyDS
LIBS = -lmyDS

TARGETS = serverApp clientApp
OBJS = serverApp.o server_net.o protocol.o clientApp.o clientnet.o UI.o main.o ClientMain.o

all: $(TARGETS)

serverApp: serverApp.o server_net.o protocol.o main.o
	$(CC) -o serverApp serverApp.o server_net.o protocol.o main.o $(LDFLAGS) $(LIBS)

clientApp: clientApp.o clientnet.o UI.o ClientMain.o protocol.o
	$(CC) -o clientApp clientApp.o clientnet.o UI.o ClientMain.o protocol.o $(LDFLAGS) $(LIBS)

serverApp.o: serverApp.c serverApp.h protocol.h
	$(CC) $(CFLAGS) -c serverApp.c

server_net.o: server_net.c server_net.h protocol.h
	$(CC) $(CFLAGS) -c server_net.c

protocol.o: protocol.c protocol.h
	$(CC) $(CFLAGS) -c protocol.c

clientApp.o: ClientApp.c ClientApp.h clientnet.h UI.h limits.h
	$(CC) $(CFLAGS) -c ClientApp.c -o clientApp.o

clientnet.o: clientnet.c clientnet.h
	$(CC) $(CFLAGS) -c clientnet.c

UI.o: UI.c UI.h
	$(CC) $(CFLAGS) -c UI.c

ClientMain.o: ClientMain.c ClientApp.h clientnet.h UI.h limits.h protocol.h
	$(CC) $(CFLAGS) -c ClientMain.c -o ClientMain.o

main.o: main.c server_net.h serverApp.h protocol.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(TARGETS) $(OBJS)

