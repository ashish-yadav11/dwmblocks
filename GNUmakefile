#PREFIX := /usr/local
PREFIX := ${HOME}/.local

CC := gcc
CFLAGS := -O3 -Wall -Wextra
CFLAGSEXTRA := -Wno-missing-field-initializers -Wno-unused-parameter

X11CFLAGS := $(shell pkg-config --cflags x11)
X11LIBS := $(shell pkg-config --libs x11)

all: dwmblocks sigdwmblocks

dwmblocks: dwmblocks.c blocks.h
	${CC} -o $@ ${CFLAGS} ${CFLAGSEXTRA} ${X11CFLAGS} $< ${X11LIBS}

sigdwmblocks: sigdwmblocks.c
	${CC} -o $@ ${CFLAGS} $<

xgetrootname: xgetrootname.c
	${CC} -o $@ ${CFLAGS} ${X11CFLAGS} $< ${X11LIBS}

clean:
	rm -f dwmblocks sigdwmblocks

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 dwmblocks ${DESTDIR}${PREFIX}/bin/dwmblocks
	install -m 0755 sigdwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

.PHONY: all clean install uninstall
