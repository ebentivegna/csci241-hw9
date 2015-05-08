CC = gcc
CFLAGS= -Wall -Wextra -std=c99 -pedantic -O0 -g
TARGETS = master test

.PHONY = all clean

all: $(TARGETS)

master: master.c master.h
	$(CC) $(CFLAGS) -o master master.c

clean:
	rm -f $(TARGETS)