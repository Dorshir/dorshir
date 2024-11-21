TARGETS = test_deck.out test_player.out test_game.out test_round.out test_ui.out
CC = gcc
CFLAGS = -g -pedantic -Wall -I$(BASE_DIR)
LDFLAGS = -lm
BASE_DIR = ..
BASE_OBJS = $(BASE_DIR)/player.o $(BASE_DIR)/deck.o $(BASE_DIR)/game.o $(BASE_DIR)/bstree.o $(BASE_DIR)/genvec.o $(BASE_DIR)/round.o $(BASE_DIR)/ui.o

.PHONY: all clean

all: $(TARGETS)

test_deck.out: test_deck.c $(BASE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_player.out: test_player.c $(BASE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_game.out: test_game.c $(BASE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_round.out: test_round.c $(BASE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_ui.out: test_ui.c $(BASE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BASE_OBJS):
	$(MAKE) -C $(BASE_DIR)

clean:
	rm -f $(TARGETS)
