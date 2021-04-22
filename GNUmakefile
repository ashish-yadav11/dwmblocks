PREFIX = /usr/local

CC = gcc
CFLAGS = -O3 -Wall -Wextra

X11CFLAGS = $(shell pkg-config --cflags x11)
X11LIBS = $(shell pkg-config --libs x11)

all: dwmblocks sigdwmblocks/sigdwmblocks xgetrootname/xgetrootname

dwmblocks: dwmblocks.c config.h block.h
	${CC} -o $@ -Wno-missing-field-initializers -Wno-unused-parameter ${CFLAGS} ${X11CFLAGS} $< ${X11LIBS}

E0BLOCKS = $(abspath blocks)
# two level escaping of `\', one for sed and one for C
E1BLOCKS = $(subst \,\\\\,${E0BLOCKS})
# escaping special character `&' and delimiter `=' for sed
E2BLOCKS = $(subst &,\&,${E1BLOCKS})
E3BLOCKS = $(subst =,\=,${E2BLOCKS})
# escaping `"' for C
E4BLOCKS = $(subst ",\\",${E3BLOCKS})
# escaping `'' for shell
EFBLOCKS = $(subst ','\'',${E4BLOCKS})
# this comment is a workaround for syntax highlighting bug in vim')

config.h:
	[ -d blocks ] || cp -R blocks.def blocks
	sed '2s=<path to the folder containing block scripts>=${EFBLOCKS}=' config.def.h >$@

sigdwmblocks/sigdwmblocks: sigdwmblocks/sigdwmblocks.c
	${CC} -o $@ ${CFLAGS} $<

xgetrootname/xgetrootname: xgetrootname/xgetrootname.c
	${CC} -o $@ ${CFLAGS} ${X11CFLAGS} $< ${X11LIBS}

clean:
	rm -f dwmblocks sigdwmblocks/sigdwmblocks xgetrootname/xgetrootname

BINDIR = ${DESTDIR}${PREFIX}/bin

install: all
	mkdir -p ${BINDIR}
	cp -f dwmblocks sigdwmblocks/sigdwmblocks xgetrootname/xgetrootname ${BINDIR}
	chmod 755 ${BINDIR}/dwmblocks ${BINDIR}/sigdwmblocks ${BINDIR}/xgetrootname

uninstall:
	rm -f ${BINDIR}/dwmblocks ${BINDIR}/sigdwmblocks ${BINDIR}/xgetrootname

.PHONY: all clean install uninstall
