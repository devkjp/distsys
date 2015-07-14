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
send_file_as_body(int sd, char * path, int range_start, int range_end);

char *
process_cgi(char * path);