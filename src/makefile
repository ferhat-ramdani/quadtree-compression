CC=C:/msys64/mingw32/bin/gcc.exe
CFLAGS=-Wall -Wextra -pedantic -std=c99 -Iinclude
DEBUG_CFLAGS=-g
LDLIBS=-lm -lMLV -lmingw32 -lSDLmain dlls/SDL.dll dlls/SDL_image.dll dlls/SDL_ttf.dll dlls/libSDL_gfx-15.dll dlls/SDL_mixer.dll dlls/libxml2.dll dlls/libglib-2.0-0.dll dlls/libintl-8.dll dlls/libiconv-2.dll -mwindows

SRCS=main.c bw_tree.c c_tree.c color.c gui.c binary_converter.c minimizer.c
OBJS=$(SRCS:%.c=bin/%.o)
DEPS=include/bw_tree.h include/c_tree.h include/color.h include/gui.h include/binary_converter.h include/minimizer.h

.PHONY: all clean debug

all: dlls/main.exe

dlls/main.exe: $(OBJS)
	$(CC) $(CFLAGS) -o dlls/main.exe $(OBJS) $(LDLIBS)

debug: CFLAGS+=$(DEBUG_CFLAGS)
debug: clean dlls/main.exe

bin/%.o: %.c
	@mkdir bin 2>nul || type nul >nul
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f dlls/main.exe bin/*.o *.qtn *.qtc
