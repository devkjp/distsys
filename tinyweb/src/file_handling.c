#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <socket_io.h>
#include <stdio.h>
#include <safe_print.h>
#include <tinyweb.h>
#include <string.h>

#define BUFSIZE 100000
#define CHUNK_SIZE 16000

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
send_file_as_body(int sd, char * path, int range_start, int range_end)
{	
	char * buf = malloc(BUFSIZE);
	int fd = open(path, O_RDONLY);
	int err;
	
	// append new line
 	char* newline = (char*)malloc(BUFSIZE);
 	strcpy(newline, "\r\n");
 	err = write_to_socket(sd, newline, strlen(newline), 1);
 	if ( err < 0 ) {
 	    err_print("Error: Unable to write new line to socket.\n");
 	}
	
	// jump to range_start in file
	if (range_start > 0) {
		err = lseek(fd, range_start, SEEK_SET);
		if (err < 0){
			err_print("Error on jumping to range!");
		}
	}
	
	if (fd >= 0){
		int cc;
		int left = range_end - range_start;
		int toWrite = left;
		toWrite = left;
		if ( toWrite > CHUNK_SIZE){
			toWrite = CHUNK_SIZE;
		}
		while(left > 0 && (cc = read(fd, buf, toWrite))){
			if (cc<0){ /* Error on Read */
				return cc;
			}
			err = write_to_socket(sd, buf, cc, 1);
			if (err < 0){ /* Error on Write */
				return err;
			}
			left -= cc;
			toWrite = left;
			if ( toWrite > CHUNK_SIZE){
				toWrite = CHUNK_SIZE;
			}
		}
	} else { /* Error on Open */
		return fd;
	}
	return 0;
}


char *
process_cgi(char * path)
{	
	int err;
	
	/* create pipe for parent child communication */
	int pipeline[2];
	if (pipe(pipeline) < 0) { /* Something went wrong */
		return NULL;
	}
	/* fork to run cgi in child process */
	pid_t child_pid = fork();
	if (child_pid == 0)	{ /* running in child process */ 
		
		/* Close reading end of pipe */
		err = close(pipeline[0]);
		if (err < 0 ){
			exit(-1);
		}

		/* Duplicate stdout and stderr to pipe */
		dup2(pipeline[1], STDOUT_FILENO);
		//dup2(pipeline[1], STDERR_FILENO);
		
		/* Set Environment and Excecute CGI Script */
		execle("/bin/sh", "sh", "-c", path, NULL, NULL);
		
		exit(0);
		
	} else if (child_pid > 0) { /* running in parent process */
		
		/* close writing end of pipe */
		err = close(pipeline[1]);
		if (err < 0){
			return NULL;
		}
		
		/* Read from pipeline into out string */
		char * result_string = malloc(BUFSIZE);
		int offset = 0;
		int cc;
		while((cc=read(pipeline[0], result_string + offset, BUFSIZE))){
			if (cc < 0){
				break;
			}
			offset += cc;
		}
		
		return result_string;
		
	} else { /* error on fork - abort */
		return NULL;
	}
	
	
	
	//return "Ich bin ein lustiger CGI-Handler. :)";
}
