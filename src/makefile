CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -Iinclude
DEBUG_CFLAGS=-g
LDLIBS=-lm -lMLV

SRCS=main.c bw_tree.c c_tree.c color.c gui.c binary_converter.c minimizer.c
OBJS=$(SRCS:%.c=bin/%.o)
DEPS=include/bw_tree.h include/c_tree.h include/color.h include/gui.h include/binary_converter.h include/minimizer.h

.PHONY: all clean debug

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

debug: CFLAGS+=$(DEBUG_CFLAGS)
debug: clean main

bin/%.o: %.c $(DEPS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main bin/*.o *.qtn *.qtc
