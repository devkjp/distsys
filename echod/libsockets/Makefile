#
# Makefile - libsockets 
#
# $Id:$
#

CC = gcc
CFLAGS = -g -O2 -Wall -pedantic

OBJ = connect_tcp.o passive_tcp.o socket_info.o socket_io.o

SRC = connect_tcp.c passive_tcp.c socket_info.c socket_io.c

LIB = libsockets.a

$(LIB) : $(OBJ)
	rm -f $(LIB)
	ar qcv $(LIB) $(OBJ)
	ranlib $(LIB)

.phony:
depend:
	makedepend $(SRC)

.phony:
clean:
	rm -f *.o $(LIB)

#
# DO NOT DELETE

connect_tcp.o: /usr/include/stdio.h /usr/include/features.h
connect_tcp.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
connect_tcp.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
connect_tcp.o: /usr/include/bits/typesizes.h /usr/include/libio.h
connect_tcp.o: /usr/include/_G_config.h /usr/include/wchar.h
connect_tcp.o: /usr/include/bits/wchar.h /usr/include/gconv.h
connect_tcp.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
connect_tcp.o: /usr/include/sys/types.h /usr/include/time.h
connect_tcp.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
connect_tcp.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
connect_tcp.o: /usr/include/limits.h /usr/include/bits/sockaddr.h
connect_tcp.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
connect_tcp.o: /usr/include/sys/errno.h /usr/include/errno.h
connect_tcp.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
connect_tcp.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
connect_tcp.o: /usr/include/asm-generic/errno-base.h
connect_tcp.o: /usr/include/netinet/in.h /usr/include/stdint.h
connect_tcp.o: /usr/include/bits/in.h /usr/include/endian.h
connect_tcp.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
connect_tcp.o: /usr/include/arpa/inet.h /usr/include/syslog.h
connect_tcp.o: /usr/include/sys/syslog.h /usr/include/netdb.h
connect_tcp.o: /usr/include/bits/netdb.h /usr/include/stdlib.h
connect_tcp.o: /usr/include/string.h connect_tcp.h
passive_tcp.o: /usr/include/stdio.h /usr/include/features.h
passive_tcp.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
passive_tcp.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
passive_tcp.o: /usr/include/bits/typesizes.h /usr/include/libio.h
passive_tcp.o: /usr/include/_G_config.h /usr/include/wchar.h
passive_tcp.o: /usr/include/bits/wchar.h /usr/include/gconv.h
passive_tcp.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
passive_tcp.o: /usr/include/sys/types.h /usr/include/time.h
passive_tcp.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
passive_tcp.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
passive_tcp.o: /usr/include/limits.h /usr/include/bits/sockaddr.h
passive_tcp.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
passive_tcp.o: /usr/include/sys/errno.h /usr/include/errno.h
passive_tcp.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
passive_tcp.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
passive_tcp.o: /usr/include/asm-generic/errno-base.h
passive_tcp.o: /usr/include/netinet/in.h /usr/include/stdint.h
passive_tcp.o: /usr/include/bits/in.h /usr/include/endian.h
passive_tcp.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
passive_tcp.o: /usr/include/arpa/inet.h /usr/include/syslog.h
passive_tcp.o: /usr/include/sys/syslog.h /usr/include/netdb.h
passive_tcp.o: /usr/include/bits/netdb.h /usr/include/unistd.h
passive_tcp.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
passive_tcp.o: /usr/include/stdlib.h /usr/include/string.h passive_tcp.h
socket_info.o: /usr/include/stdio.h /usr/include/features.h
socket_info.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
socket_info.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
socket_info.o: /usr/include/bits/typesizes.h /usr/include/libio.h
socket_info.o: /usr/include/_G_config.h /usr/include/wchar.h
socket_info.o: /usr/include/bits/wchar.h /usr/include/gconv.h
socket_info.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
socket_info.o: /usr/include/sys/types.h /usr/include/time.h
socket_info.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
socket_info.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
socket_info.o: /usr/include/limits.h /usr/include/bits/sockaddr.h
socket_info.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
socket_info.o: /usr/include/sys/errno.h /usr/include/errno.h
socket_info.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
socket_info.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
socket_info.o: /usr/include/asm-generic/errno-base.h
socket_info.o: /usr/include/netinet/in.h /usr/include/stdint.h
socket_info.o: /usr/include/bits/in.h /usr/include/endian.h
socket_info.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
socket_info.o: /usr/include/arpa/inet.h /usr/include/syslog.h
socket_info.o: /usr/include/sys/syslog.h /usr/include/netdb.h
socket_info.o: /usr/include/bits/netdb.h /usr/include/unistd.h
socket_info.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
socket_info.o: /usr/include/stdlib.h socket_info.h /usr/include/string.h
socket_io.o: /usr/include/stdio.h /usr/include/features.h
socket_io.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
socket_io.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
socket_io.o: /usr/include/bits/typesizes.h /usr/include/libio.h
socket_io.o: /usr/include/_G_config.h /usr/include/wchar.h
socket_io.o: /usr/include/bits/wchar.h /usr/include/gconv.h
socket_io.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
socket_io.o: /usr/include/sys/types.h /usr/include/time.h
socket_io.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
socket_io.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
socket_io.o: /usr/include/limits.h /usr/include/bits/sockaddr.h
socket_io.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
socket_io.o: /usr/include/sys/errno.h /usr/include/errno.h
socket_io.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
socket_io.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
socket_io.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/signal.h
socket_io.o: /usr/include/signal.h /usr/include/bits/sigset.h
socket_io.o: /usr/include/bits/signum.h /usr/include/sys/time.h
socket_io.o: /usr/include/bits/time.h /usr/include/sys/select.h
socket_io.o: /usr/include/bits/select.h /usr/include/sys/wait.h
socket_io.o: /usr/include/sys/resource.h /usr/include/bits/resource.h
socket_io.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
socket_io.o: /usr/include/netinet/in.h /usr/include/stdint.h
socket_io.o: /usr/include/bits/in.h /usr/include/endian.h
socket_io.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
socket_io.o: /usr/include/arpa/inet.h /usr/include/syslog.h
socket_io.o: /usr/include/sys/syslog.h /usr/include/netdb.h
socket_io.o: /usr/include/bits/netdb.h /usr/include/unistd.h
socket_io.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
socket_io.o: /usr/include/stdlib.h socket_io.h
