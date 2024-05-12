CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99
DEBUG_CFLAGS=-g

SRCS=main.c bw_tree.c
OBJS=$(SRCS:.c=.o)
DEPS=bw_tree.h

.PHONY: all clean debug

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: CFLAGS+=$(DEBUG_CFLAGS)
debug: clean main

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main $(OBJS)
