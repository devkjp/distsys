#**********************************************************************
# Berufsakademie Ravensburg - Praktikum Verteilte Kommunikationssysteme
#
# $Id: Makefile,v 1.2 2004/12/29 20:18:33 ralf Exp $
#
#**********************************************************************


SHELL		= /bin/sh

CC		= gcc
CFLAGS		= -Wall -g -O2 -pedantic
PS2PDF		= ps2pdf
A2PS		= a2ps
AOPT		= --line-numbers=1

SRC = echod.c

LIBS = libsockets/libsockets.a 

.PHONY : all
all : $(LIBS) echod

%.pdf : %.ps
	$(PS2PDF) $< $@

%.ps : %.c
	$(A2PS) $< $(AOPT) -o $@

libsockets/libsockets.a:
	$(MAKE) -C libsockets

echod : echod.o $(LIBS)
	$(CC) -o $@ echod.o $(LIBS) -lpthread


.PHONY: depend
depend:
	makedepend $(SRC)

.PHONY: clean
clean:
	rm -f *.o $(LIBS) echod

# DO NOT DELETE

echod.o: /usr/include/stdio.h /usr/include/features.h
echod.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
echod.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
echod.o: /usr/include/bits/typesizes.h /usr/include/libio.h
echod.o: /usr/include/_G_config.h /usr/include/wchar.h
echod.o: /usr/include/bits/wchar.h /usr/include/gconv.h
echod.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
echod.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/types.h
echod.o: /usr/include/time.h /usr/include/sys/socket.h /usr/include/sys/uio.h
echod.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
echod.o: /usr/include/limits.h /usr/include/bits/sockaddr.h
echod.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
echod.o: /usr/include/sys/errno.h /usr/include/errno.h
echod.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
echod.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
echod.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/signal.h
echod.o: /usr/include/signal.h /usr/include/bits/sigset.h
echod.o: /usr/include/bits/signum.h /usr/include/netinet/in.h
echod.o: /usr/include/stdint.h /usr/include/bits/in.h /usr/include/endian.h
echod.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
echod.o: /usr/include/arpa/inet.h /usr/include/netdb.h
echod.o: /usr/include/bits/netdb.h /usr/include/unistd.h
echod.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
echod.o: /usr/include/pthread.h /usr/include/sched.h
echod.o: /usr/include/bits/sched.h /usr/include/bits/pthreadtypes.h
echod.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
echod.o: libsockets/passive_tcp.h libsockets/socket_io.h
