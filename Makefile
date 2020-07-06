#PREFIX = /usr/local
PREFIX = ${HOME}/.local

CC = gcc
CFLAGS = -Wall -Wextra -Wextra -O3
CFLAGSEXTRA = -Wno-unused-parameter -Wno-missing-field-initializers

all: dwmblocks sigdwmblocks

dwmblocks: dwmblocks.c
	${CC} -o $@ -lX11 ${CFLAGS} ${CFLAGSEXTRA} $<

sigdwmblocks: sigdwmblocks.c
	${CC} -o $@ ${CFLAGS} sigdwmblocks.c

xgetrootname: xgetrootname.c
	${CC} -o $@ -lX11 ${CFLAGS} xgetrootname.c

clean:
	rm -f dwmblocks sigdwmblocks

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 dwmblocks ${DESTDIR}${PREFIX}/bin/dwmblocks
	install -m 0755 sigdwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwmblocks ${DESTDIR}${PREFIX}/bin/sigdwmblocks

.PHONY: all clean install uninstall
