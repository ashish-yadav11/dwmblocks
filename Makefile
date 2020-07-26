#PREFIX = /usr/local
PREFIX = ${HOME}/.local

CC = gcc
CFLAGS = -O3 -Wall -Wextra
CFLAGSEXTRA = -Wno-missing-field-initializers -Wno-unused-parameter

all: dwmblocks sigdwmblocks

dwmblocks: dwmblocks.c blocks.h
	${CC} -o $@ -lX11 ${CFLAGS} ${CFLAGSEXTRA} $<

sigdwmblocks: sigdwmblocks.c
	${CC} -o $@ ${CFLAGS} $<

xgetrootname: xgetrootname.c
	${CC} -o $@ -lX11 ${CFLAGS} $<

clean:
	rm -f dwmblocks sigdwmblocks

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 dwmblocks ${DESTDIR}${PREFIX}/bin/dwmblocks
	install -m 0755 sigdwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

.PHONY: all clean install uninstall
