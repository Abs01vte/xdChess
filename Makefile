CC=gcc
LDFLAGS=
CFLAGS=-c -std=c99
TARGET=xdChess


$(TARGET): main.o
	$(CC) -o $@ $? $(LDFLAGS)


main.o: main.c
	$(CC) -o $@ $(CFLAGS) $<
