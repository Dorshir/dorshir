TARGET= hearts.out
CC= gcc
CFLAGS = -g -pedantic -Wall
LDFLAGS = -lm
OBJS= hearts.o player.o deck.o game.o bstree.o genvec.o round.o ui.o

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)


hearts.o: hearts.c game.h ui.h
		$(CC) $(CFLAGS) -c hearts.c

player.o: player.c player.h genvec.h bstree.h ui.h
		$(CC) $(CFLAGS) -c player.c

deck.o: deck.c deck.h genvec.h ui.h
		$(CC) $(CFLAGS) -c deck.c

game.o: game.c game.h deck.h player.h round.h ui.h
		$(CC) $(CFLAGS) -c game.c

round.o: round.c round.h player.h deck.h game.h ui.h
		$(CC) $(CFLAGS) -c round.c

ui.o: ui.c
		$(CC) $(CFLAGS) -c ui.c

bstree.o: bstree.c bstree.h
		$(CC) $(CFLAGS) -c bstree.c

genvec.o: genvec.c genvec.h
		$(CC) $(CFLAGS) -c genvec.c


clean:
		rm -f $(TARGET) $(OBJS)