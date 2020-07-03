#PREFIX = /usr/local
PREFIX = ${HOME}/.local
CC = gcc
CFLAGS = -Wall -Wextra  -Os
CFLAGSEXTRA = -Wno-unused-parameter -Wno-missing-field-initializers

build: dwmblocks sigdwmblocks

dwmblocks: dwmblocks.c blocks.h
	${CC} ${CFLAGS} ${CFLAGSEXTRA} -lX11 dwmblocks.c -o dwmblocks

sigdwmblocks: sigdwmblocks.c
	${CC} ${CFLAGS} sigdwmblocks.c -o sigdwmblocks

xgetrootname: xgetrootname.c
	${CC} ${CFLAGS} xgetrootname.c -o xgetrootname

clean:
	rm -f dwmblocks sigdwmblocks

install: build
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 dwmblocks ${DESTDIR}${PREFIX}/bin/dwmblocks
	install -m 0755 sigdwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

.PHONY: build clean install uninstall
