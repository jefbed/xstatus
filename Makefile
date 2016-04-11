#CFLAGS+=-O0
#CFLAGS+=-ggdb
#CFLAGS=-Os
CFLAGS+=-Wall -W
CFLAGS+=-std=c11
CFLAGS+=-D_XOPEN_SOURCE=700
LDFLAGS+=-lX11
PREFIX=/usr
prog=xstatus
srcs=${prog}.c battery.c clock.c util.c button.c
objs=${srcs:.c=.o}
installdir=${DESTDIR}${PREFIX}

all: ${prog}

*.h:
	make clean

${objs}: *.h

${prog}: ${objs} 
	${CC} ${CFLAGS} ${objs} -o $@ ${LDFLAGS}

clean:
	rm -f ${prog} ${objs}

install:
	install -s xstatus ${installdir}/bin
