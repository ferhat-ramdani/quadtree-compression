CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99
DEBUG_CFLAGS=-g
LDLIBS=-lm

SRCS=main.c bw_tree.c c_tree.c color.c
OBJS=$(SRCS:.c=.o)
DEPS=bw_tree.h c_tree.h color.h

.PHONY: all clean debug

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

debug: CFLAGS+=$(DEBUG_CFLAGS)
debug: clean main

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main $(OBJS)
