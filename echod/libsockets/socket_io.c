

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

#include "socket_io.h"


int
select_socket_fd (int fd, int maxtime, int writep)
{
  fd_set fds, exceptfds;
  struct timeval timeout;

  FD_ZERO (&fds);
  FD_SET (fd, &fds);
  FD_ZERO (&exceptfds);
  FD_SET (fd, &exceptfds);
  timeout.tv_sec = maxtime;
  timeout.tv_usec = 0;

  return select (fd + 1, writep ? NULL : &fds, writep ? &fds : NULL,
		 &exceptfds, &timeout);
} /* end of select_socket_fd */


int
read_from_socket (int fd, char *buf, int len, int timeout)
{
  int res;

  do {
    if (timeout > 0) {
      do {
	res = select_socket_fd (fd, timeout, 0);
      } while (res == -1 && errno == EINTR);
      if (res <= 0)  {
	return SOCKET_TIMEOUT;
      } /* end if */
    } /* end if */
    res = read(fd, buf, len);
  } while (res == -1 && errno == EINTR);

  return res;
} /* end of read_from_socket */


int
write_to_socket (int fd, char *buf, int len, int timeout)
{
  int res = 0;

  /* `write' may write less than LEN bytes, thus the outward loop
     keeps trying it until all was written, or an error occurred.  The
     inner loop is reserved for the usual EINTR f*kage, and the
     innermost loop deals with the same during select().  */
  printf("write_to_socket s\n");
  while (len > 0) {
    do {
      if (timeout) {
	do {
	  printf("write_to_socket select 1\n");
	  res = select_socket_fd (fd, timeout, 1);
	  printf("write_to_socket select 2\n");
	} while (res == -1 && errno == EINTR);
	if (res <= 0) {
	  return -1;
	} /* end if */
      }
      printf("write_to_socket write, len=%d, res=%d\n", len, res);
      fflush(stdout);
      res = write(fd, buf, len);
      printf("write_to_socket write 2\n");
      fflush(stdout);
    } while (res == -1 && errno == EINTR);
    if (res <= 0) break;
    buf += res;
    len -= res;
  } /* end while */
  printf("write_to_socket e\n");
  return res;
} /* end of write_to_socket */
