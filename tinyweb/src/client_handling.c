/*===================================================================
 * DHBW Ravensburg - Campus Friedrichshafen
 *
 * Vorlesung Verteilte Systeme
 * 
 * Gruppe:  1 - Reutebuch, Schulvbz, Polkehn
 * Author:  Polkehn
 *
 *===================================================================*/
 
 
#include <stdio.h>
#include <stdlib.h>
 
// Spezielle Socket Bibliotheken
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <passive_tcp.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <socket_io.h>
#include <sys/stat.h>

#include <tinyweb.h>
#include <client_handling.h>
#include <request_parser.h>
#include <safe_print.h>
#include <sem_print.h>
#include <content.h>
#include <file_handling.h>

#define BUFSIZE 100000
#define WRITE_TIMEOUT 1000

#define _DEBUG
 
 // GLOBAL REQUEST COUNTER
//static int request_counter = 0;

char * get_path(char * dirpath, char * resource)
{	
	char * outStr = malloc(2048);
	// if dir and res are empty, return empty string.
	if (strlen(dirpath) == 0 && strlen(resource)==0)
	{
		return "";
	}
	
	// Remove leading / from resource
	if(resource == strstr(resource, "/"))
	{
		resource += 1;
	}
	
	// Add / between dirpath and resource
	int len = strlen(dirpath);
	if(len > 0 && dirpath[len-1] != '/')
	{	
		char * newDirpath = malloc(2048);
		strcpy(newDirpath, dirpath);
		strcat(newDirpath, "/");
		dirpath = newDirpath;
	}
	
	strcat(outStr, dirpath);
	strcat(outStr, resource);
	return outStr;
}

/*
 * function:		send_response
 * purpose:			concatenate the header lines and write them to the socket if they exist
 * IN:				http_res_t* response - struct with response headers / body to send
 *					int sd - socket descriptor for writing socket
 * OUT:				-
 * globals used:	-
 * return value:	zero if okay, anything else if not
*/
int send_response(http_res_t * response, int sd)
{
	// error code for socket-write
    int err = 0;

	// status zeile vorbereiten
	int index = response->status;
	http_status_entry_t status = http_status_list[index];
	char status_code[50];
	sprintf(status_code, "%u", status.code);
	print_log("[INFO] STATUS CODE: %s\n", status_code);
	// status zeile zusammen bauen
	char* status_line = malloc(BUFSIZE);
	strcpy(status_line, "HTTP/1.1 ");
	strcat(status_line, status_code);
	strcat(status_line, " ");
	strcat(status_line, status.text);
	strcat(status_line, "\r\n");
	
	// status zeile auf den socket schreiben
	err = write_to_socket(sd, status_line, strlen(status_line), WRITE_TIMEOUT);
	if ( err < 0 ) {
	    safe_printf("Error: Unable to write status_line to socket.\n");
	}
	
	// get server and write to socket
	if ( strcmp(response->server, "") ) {
		char* server = (char*)malloc(BUFSIZE);
 		strcpy(server, "Server: ");
		strcat(server, response->server);
 		strcat(server, "\r\n");
 		err = write_to_socket(sd, server, strlen(server), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write date to socket.\n");
 		}
	}
	
	// get date and write date to socket
	if ( strcmp(response->date, "") ) {
		char* date = (char*)malloc(BUFSIZE);
 		strcpy(date, "Date: ");
		strcat(date, response->date);
 		strcat(date, "\r\n");
 		err = write_to_socket(sd, date, strlen(date), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 		    safe_printf("Error: Unable to write date to socket.\n");
 		}
	}
 	
    // get last_modified and write to socket
    if ( strcmp(response->date, "") ) {
		char* lm = (char*)malloc(BUFSIZE);
 		strcpy(lm, "Last-Modified: ");
		strcat(lm, response->last_modified);
 		strcat(lm, "\r\n");
 		err = write_to_socket(sd, lm, strlen(lm), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write date to socket.\n");
 		}
    }
	
	// get content_length and write to socket
	if ( strcmp(response->content_length, "") ) {
		char* content_length = (char*)malloc(BUFSIZE);
 		strcpy(content_length, "Content-Length: ");
		strcat(content_length, response->content_length);
 		strcat(content_length, "\r\n");
 		err = write_to_socket(sd, content_length, strlen(content_length), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write content_length to socket.\n");
 		}
	}
 	
 	// get content_type and write to socket
 	if ( strcmp(response->content_type, "") ) {
		char* content_type = (char*)malloc(BUFSIZE);
 		strcpy(content_type, "Content-Type: ");
		strcat(content_type, response->content_type);
 		strcat(content_type, "\r\n");
 		err = write_to_socket(sd, content_type, strlen(content_type), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write content_type to socket.\n");
 		}
 	}
	
	// get connection and write to socket
	if ( strcmp(response->connection, "") ) {
		char* connection = (char*)malloc(BUFSIZE);
 		strcpy(connection, "Connection: ");
		strcat(connection, response->connection);
 		strcat(connection, "\r\n");
 		err = write_to_socket(sd, connection, strlen(connection), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to connection date to socket.\n");
 		}
	}
 	
 	// get accept_ranges and write to socket
 	if ( strcmp(response->accept_ranges, "") ) {
		char* accept_ranges = (char*)malloc(BUFSIZE);
 		strcpy(accept_ranges, "Accept-Ranges: ");
		strcat(accept_ranges, response->accept_ranges);
 		strcat(accept_ranges, "\r\n");
 		err = write_to_socket(sd, accept_ranges, strlen(accept_ranges), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write accept_ranges to socket.\n");
 		}
 	}
 	
 	// get location and write to socket
 	if ( strcmp(response->location, "") ) {
		char* location = (char*)malloc(BUFSIZE);
 		strcpy(location, "Location: ");
		strcat(location, response->location);
 		strcat(location, "\r\n");
 		err = write_to_socket(sd, location, strlen(location), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write location to socket.\n");
 		}
 	}
 	
 	// append new line
 	char* newline = (char*)malloc(BUFSIZE);
 	strcpy(newline, "\r\n");
 	err = write_to_socket(sd, newline, strlen(newline), WRITE_TIMEOUT);
 	if ( err < 0 ) {
 	    safe_printf("Error: Unable to write new line to socket.\n");
 	}
 	
	// get body and write to socket
	if ( strcmp(response->body, "") ) {
		char* body = (char*)malloc(BUFSIZE);
 		strcpy(body, response->body);
 		strcat(body, "\r\n\0");
 		err = write_to_socket(sd, body, strlen(body), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write body to socket.\n");
 		}
	}
 	
	return 0;
}

/*
 * function:		handle_client
 * purpose:			concatenate the header lines and write them to the socket if they exist
 * IN:				int sd - socket descriptor for writing socket
 * OUT:				-
 * globals used:	-
 * return value:	zero if okay, anything else if not
*/
int handle_client(int sd, char* root_dir)
{	 
	int err = 0;
	http_req_t req;
	http_res_t res;
	char* req_string = malloc(BUFSIZE);
	
	// initialize response
	res.status = HTTP_STATUS_INTERNAL_SERVER_ERROR;
	
	// get current time
	struct tm *ts;
	char* timestr = malloc(BUFSIZE);
	time_t current_time = time(NULL);
	ts = localtime(&current_time);
	strftime(timestr, BUFSIZE, "%a, %d %b %Y %T GMT", ts);
	res.date = timestr;
	res.server = "Tinyweb Polkehn/Reutebuch/Schulz";
	res.last_modified = "";
	res.content_length = "";
    res.content_type = "";
    res.connection = "";
    res.accept_ranges = "";
    res.location = "";
    res.body = "";
    char* path = "";

	// read the request from the socket
	read_from_socket(sd, req_string, BUFSIZE, 1);

	err = parse_request(&req, req_string);
	if (err < 0)
	{
		exit(-1);
	}
		
	/* request handling ----------------------------------------------------------
	 *
	 * use positive logic within the if-statements
	 *			- check if CGI and execute
	 *
	 */
	// check http method if its GET or HEAD
	if ( req.method == HTTP_METHOD_GET || req.method == HTTP_METHOD_HEAD ) {
		
		// get the correct path
		path = get_path(root_dir, req.resource);
		struct stat fstatus;
		int stat_return = stat(path, &fstatus);
		
		// check if is not a directory
		if ( !(fstatus.st_mode & S_IFDIR) ) {	
			
			// check if it exists
			if ( stat_return >= 0 && S_ISREG(fstatus.st_mode) ) { 
				
				//check if file is accessible (read rights)
				if ( fstatus.st_mode & S_IROTH ) { 
					
					// if modified since timestamp from request
					if ( fstatus.st_mtime > req.if_modified_since ) {
						
						// check if range is valid
						if ( req.range_start <= fstatus.st_size &&
							 req.range_end   <= fstatus.st_size &&
							 req.range_start <= req.range_end  ) {
							
							// --- happy path ---
							
							// correct ranges
							if ( req.range_start < 0 ) {
								req.range_start = 0;
							}
							if ( req.range_end < 0 ) {
								req.range_end = fstatus.st_size;
							}
							
							
							// set content type
							http_content_type_t contType = get_http_content_type(path);
							char* contStr = get_http_content_type_str(contType);
							res.content_type = contStr;
							
							// set last_modified
							struct tm *ts;
							char* lastStr = malloc(BUFSIZE);
							ts = localtime(&fstatus.st_mtim.tv_sec);
							strftime(lastStr, BUFSIZE, "%a, %d %b %Y %T GMT", ts);
							res.last_modified = lastStr;
					
							// set content length
							char* rangeStr = malloc(BUFSIZE);
							sprintf(rangeStr, "%d", (req.range_end - req.range_start)); 
							res.content_length = rangeStr;
							int file = open(path, O_RDONLY);
							if ( file >= 0 ) {
								char* buf = malloc(BUFSIZE);
								
								read_from_socket(file, buf, BUFSIZE, 1);
								
								//res.content_length = malloc(BUFSIZE);
								//sprintf(res.content_length, "%d", (int)fstatus.st_size);
								
								// set status okay
								res.status = HTTP_STATUS_OK;
								
							}else{
								err_print("Resource could not be opened");
								res.status = HTTP_STATUS_INTERNAL_SERVER_ERROR;
							}
							
						}else{ /* else of range check */
							// range is not satisfiable - 416
							res.status = HTTP_STATUS_RANGE_NOT_SATISFIABLE;
						}	
					 		
					}else{ /* else of last_modified of the resource */ 
						// dont send the ressource, just 304 not modified
						res.status = HTTP_STATUS_NOT_MODIFIED;
					}
				
				}else{ /* else of file not accessible */
					// resource is not accessible - return 403 - forbidden
					res.status = HTTP_STATUS_FORBIDDEN;
					// directly write status to socket and exit
					err = send_response(&res, sd);
					if ( err < 0 ) {
						safe_printf("Failed to send the response (403): %d\n", err);
					}
				} /* endif file accessible */
			
			}else{
				// resource doesn't exist - return 404 - not found
				res.status = HTTP_STATUS_NOT_FOUND;
				// directly write status to socket and exit
				err = send_response(&res, sd);
				if ( err < 0 ) {
					safe_printf("Failed to send the response (404): %d\n", err);
				}
				return 0;
			}/* endif file exists */
	
		}else{ /* else of check if exists */
			// requested resource is a directory - respond 301
			res.status = HTTP_STATUS_MOVED_PERMANENTLY;
			// write path an add slash
			strcat(path, "/\n\0");
			res.location = path;
			// directly write status to socket and exit
			err = send_response(&res, sd);
			if ( err < 0 ) {
				safe_printf("Failed to send the response (301): %d\n", err);
			}
		}	
		
	}else{ /* else of check directory */
		// return status 501 - not implemented
		res.status = HTTP_STATUS_NOT_IMPLEMENTED;
		// directly write status to socket and exit
		err = send_response(&res, sd);
		if ( err < 0 ) {
			safe_printf("Failed to send the response (501): %d\n", err);
		}
		return 0;
	} /* endif GET HEAD */

	

	// build header lines and send response
	err = send_response(&res, sd);
	if ( err >= 0 ) {
		if (req.method != HTTP_METHOD_HEAD && strlen(path) > 0) {
			err = send_file_as_body(sd, path, req.range_start, req.range_end);		
			if (err < 0){
				err_print("Failed to send response body!");
			}
		} 
	} else {
		err_print("Failed to send response header!");
 	}
	return 0;
}

