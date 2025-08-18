CC = gcc
CFLAGS = -g -pedantic -ImyDS
LDFLAGS = -LmyDS
LIBS = -lmyDS
TARGETS = serverApp.out clientApp.out send_app.out receive_app.out

SERVER_OBJS = serverApp.o server_net.o protocol.o serverMain.o userManager.o user.o groupManager.o group.o

CLIENT_OBJS = clientApp.o clientnet.o UI.o ClientMain.o protocol.o

all: $(TARGETS)

serverApp.out: $(SERVER_OBJS)
	$(CC) -o serverApp.out $(SERVER_OBJS) $(LDFLAGS) $(LIBS)

clientApp.out: $(CLIENT_OBJS)
	$(CC) -o clientApp.out $(CLIENT_OBJS) $(LDFLAGS) $(LIBS)

send_app.out: send_app.c
	$(CC) $(CFLAGS) send_app.c -o send_app.out

receive_app.out: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) receive_app.c -o receive_app.out

serverApp.o: serverApp.c serverApp.h protocol.h userManager.h
	$(CC) $(CFLAGS) -c serverApp.c

server_net.o: server_net.c server_net.h protocol.h
	$(CC) $(CFLAGS) -c server_net.c

protocol.o: protocol.c protocol.h
	$(CC) $(CFLAGS) -c protocol.c

userManager.o: userManager.c userManager.h user.h
	$(CC) $(CFLAGS) -c userManager.c

groupManager.o: groupManager.c groupManager.h group.h
	$(CC) $(CFLAGS) -c groupManager.c

group.o: group.c group.h
	$(CC) $(CFLAGS) -c group.c

user.o: user.c user.h
	$(CC) $(CFLAGS) -c user.c

clientApp.o: ClientApp.c ClientApp.h clientnet.h UI.h limits.h
	$(CC) $(CFLAGS) -c ClientApp.c -o clientApp.o

clientnet.o: clientnet.c clientnet.h
	$(CC) $(CFLAGS) -c clientnet.c

UI.o: UI.c UI.h
	$(CC) $(CFLAGS) -c UI.c

ClientMain.o: ClientMain.c ClientApp.h clientnet.h UI.h limits.h protocol.h
	$(CC) $(CFLAGS) -c ClientMain.c -o ClientMain.o

serverMain.o: serverMain.c server_net.h serverApp.h protocol.h userManager.h
	$(CC) $(CFLAGS) -c serverMain.c


clean:
	rm -f $(TARGETS) $(SERVER_OBJS) $(CLIENT_OBJS)
