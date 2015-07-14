#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <socket_io.h>

#define BUFSIZE 100000

/*
 * function:		send_file_as_body
 * purpose:			send the given file as response body to the given socket
 * IN:				int sd - socket descriptor for writing socket
 *					char * path - path to file to send
 * OUT:				-
 * globals used:	-
 * return value:	zero if okay, anything else if not
*/
int 
send_file_as_body(int sd, char * path)
{	
	char * buf = malloc(BUFSIZE);
	int fd = open(path, O_RDONLY);
	if (fd >= 0){
		int cc;
		while((cc = read(fd, buf, BUFSIZE))){
			if (cc<0){ /* Error on Read */
				return cc;
			}
			int err = write_to_socket(sd, buf, cc, 1);
			if (err < 0){ /* Error on Write */
				return err;
			}
		}
	} else { /* Error on Open */
		return fd;
	}
	return 0;
}