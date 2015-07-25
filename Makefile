CC=gcc
CFLAGS=-O

snake: snake.c snake.h
	$(CC) $(CFLAGS) -o snake snake.c -lncurses
