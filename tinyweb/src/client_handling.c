/*===================================================================
 * DHBW Ravensburg - Campus Friedrichshafen
 *
 * Vorlesung Verteilte Systeme
 * 
 * Gruppe:  1 - Reutebuch, Schulvbz, Polkehn
 * Author:  Polkehn, Schulz, Reutebuch
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
 	
 	// get content_range and write to socket
 	if ( strcmp(response->content_range, "") ) {
		char* content_range = (char*)malloc(BUFSIZE);
 		strcpy(content_range, "Content-Range: ");
		strcat(content_range, response->content_range);
 		strcat(content_range, "\r\n");
 		err = write_to_socket(sd, content_range, strlen(content_range), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write content_range to socket.\n");
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
 	

 	
	// get body and write to socket
	if ( strcmp(response->body, "") ) {
		char* body = (char*)malloc(BUFSIZE);
 		strcpy(body, response->body);
 		//strcat(body, "\r\n\0");
 		err = write_to_socket(sd, body, strlen(body), WRITE_TIMEOUT);
 		if ( err < 0 ) {
 	    	safe_printf("Error: Unable to write body to socket.\n");
 		}
	}
 	
	return 0;
}


/*
 * function:		write_log
 * purpose:			write previously collected log information
 * IN:				http_log_t logging - struct with logging information
 *					FILE* log_fd - file descriptor to logfile
 * OUT:				-
 * globals used:	-
 * return value:	zero if okay, negative if something went wrong
*/
int write_log(http_log_t logging, FILE* log_fd) {
	char* nextLine = malloc(BUFSIZE);
	
	// 192.168.0.99 - - [13/Jun/2014:07:24:30 +0200] "GET /index.html HTTP/1.1" 206 1004
	sprintf(nextLine, "%s:%s -- [%s] \"%s %s HTTP/1.1\" %s %s\n", 
				logging.client_ip, 
				logging.client_port,
				logging.timestamp,
				logging.http_method,
				logging.resource,
				logging.http_status,
				logging.bytes );
	
	return fwrite(nextLine, sizeof(char), strlen(nextLine), log_fd);
}
	

/*
 * function:		handle_client
 * purpose:			concatenate the header lines and write them to the socket if they exist
 * IN:				int sd - socket descriptor for writing socket
 * OUT:				-
 * globals used:	-
 * return value:	zero if okay, anything else if not
*/
int handle_client(int sd, char* root_dir, http_log_t logging, FILE* log_fd)
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
    res.content_range = "";
    res.connection = "";
    res.accept_ranges = "";
    res.location = "";
    res.body = "";
    char* path = "";

	// copy timestamp to logging struct
	logging.timestamp = res.date;
	
	// set initial status for logging struct
	sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_INTERNAL_SERVER_ERROR].code);
	
	// read the request from the socket
	read_from_socket(sd, req_string, BUFSIZE, 1);

	err = parse_request(&req, req_string);
	if (err < 0)
	{
		exit(-1);
	}
	
	// write available info to logging struct
	logging.http_method = http_method_list[req.method].name;
	logging.resource = req.resource;
	
	
	struct stat fstatus;
		
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
		int stat_return = stat(path, &fstatus);
		if ( stat_return < 0 ) {
			res.status = HTTP_STATUS_NOT_FOUND;
			sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_NOT_FOUND].code);
			// directly write status to socket and exit
			err = send_response(&res, sd);
			if ( err < 0 ) {
				safe_printf("Failed to send the response (403): %d\n", err);
			}
			write_log(logging, log_fd);
			return 0;
			
		}
		//FILE* file = fopen(path, "r");
		//bool accessible_file = false;
	//	if ( file ) {
       // 	fclose(file);
        //	accessible_file = true;
    //	}
    
		// check if is not a directory
		if ( !(fstatus.st_mode & S_IFDIR) ) {	
			
			// check if it exists
			if ( stat_return >= 0 && S_ISREG(fstatus.st_mode) ) { 
				
				//check if file is accessible (read rights)
				if ( fstatus.st_mode & S_IROTH ) { 
					
					// if modified since timestamp from request
					if ( fstatus.st_mtime > req.if_modified_since ) {
						
						// check if cgi needs to be processed
						if(req.resource != strstr(req.resource, "/cgi-bin"))
						{
							
							// check if range is valid
							if ( req.range_start < fstatus.st_size &&
								 req.range_end   < fstatus.st_size &&
								 (req.range_end < 0 || req.range_start <= req.range_end  ))
								 {
							
								// --- happy path ---
							
								bool isPartial = false;
								if (req.range_start >= 0 || req.range_end >= 0){
									isPartial = true;
								}
							
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
								logging.bytes = rangeStr;
								
								if (! isPartial){
									res.status = HTTP_STATUS_OK;
									sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_OK].code);
								} else {
									res.status = HTTP_STATUS_PARTIAL_CONTENT;
									sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_PARTIAL_CONTENT].code);
									char * content_range_str = malloc(BUFSIZE);
									sprintf(content_range_str, "bytes %d-%d/%d", req.range_start, req.range_end-1, (int) fstatus.st_size);
									res.content_range = content_range_str;
								}
							
							}else{ /* else of range check */
								// range is not satisfiable - 416
								res.status = HTTP_STATUS_RANGE_NOT_SATISFIABLE;
								sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_RANGE_NOT_SATISFIABLE].code);
							}	
							
						} else { /* CGI needs to be processed */
							
							char * cgi_result = malloc(BUFSIZE);
							cgi_result = process_cgi(path);
							
							if (cgi_result != NULL) { /* Succesful execution */
								res.body = cgi_result;
								res.status = HTTP_STATUS_OK;
								sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_OK].code);
							} else {
								res.status = HTTP_STATUS_INTERNAL_SERVER_ERROR;
								sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_INTERNAL_SERVER_ERROR].code);
							}
							// directly write status to socket and exit
							err = send_response(&res, sd);
							if ( err < 0 ) {
								safe_printf("Failed to send the response (403): %d\n", err);
							}
							
							err = write_log(logging, log_fd);
							if ( err < 0 ) {
								err_print("Failed to print logging information.");
							}
							return 0;
							
						}

					}else{ /* else of last_modified of the resource */ 
						// dont send the ressource, just 304 not modified
						res.status = HTTP_STATUS_NOT_MODIFIED;
						sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_NOT_MODIFIED].code);
						// directly write status to socket and exit
						err = send_response(&res, sd);
						if ( err < 0 ) {
							safe_printf("Failed to send the response (403): %d\n", err);
						}
						err = write_log(logging, log_fd);
						if ( err < 0 ) {
							err_print("Failed to print logging information.");
						}
						return 0;
					}
				
				}else{ /* else of file not accessible */
					// resource is not accessible - return 403 - forbidden
					res.status = HTTP_STATUS_FORBIDDEN;
					sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_FORBIDDEN].code);
					// directly write status to socket and exit
					err = send_response(&res, sd);
					if ( err < 0 ) {
						safe_printf("Failed to send the response (403): %d\n", err);
					}
					err = write_log(logging, log_fd);
					if ( err < 0 ) {
						err_print("Failed to print logging information.");
					}
					return 0;
				} /* endif file accessible */
			
			}else{
				// resource doesn't exist - return 404 - not found
				res.status = HTTP_STATUS_NOT_FOUND;
				sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_NOT_FOUND].code);
				// directly write status to socket and exit
				err = send_response(&res, sd);
				if ( err < 0 ) {
					safe_printf("Failed to send the response (404): %d\n", err);
				}
				err = write_log(logging, log_fd);
				if ( err < 0 ) {
					err_print("Failed to print logging information.");
				}
				return 0;
			}/* endif file exists */
	
		}else{ /* else of check if directory */
			// requested resource is a directory - respond 301
			res.status = HTTP_STATUS_MOVED_PERMANENTLY;
			sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_MOVED_PERMANENTLY].code);
			// write path an add slash
			strcat(path, "/\n\0");
			res.location = path;
			// directly write status to socket and exit
			err = send_response(&res, sd);
			if ( err < 0 ) {
				safe_printf("Failed to send the response (301): %d\n", err);
			}
			err = write_log(logging, log_fd);
			if ( err < 0 ) {
				err_print("Failed to print logging information.");
			}
			return 0;
		}	
		
	}else{ /* else of check http method */
		// return status 501 - not implemented
		res.status = HTTP_STATUS_NOT_IMPLEMENTED;
		sprintf(logging.http_status, "%d", http_status_list[HTTP_STATUS_NOT_IMPLEMENTED].code);
		// directly write status to socket and exit
		err = send_response(&res, sd);
		if ( err < 0 ) {
			safe_printf("Failed to send the response (501): %d\n", err);
		}
		err = write_log(logging, log_fd);
		if ( err < 0 ) {
			err_print("Failed to print logging information.");
		}
		return 0;
	} /* endif GET HEAD */


	// build header lines and send response
	err = send_response(&res, sd);
	if ( err >= 0 ) {
		if (req.method != HTTP_METHOD_HEAD && 
			strlen(path) > 0 && 
			res.status != HTTP_STATUS_MOVED_PERMANENTLY) 
		{
			err = send_file_as_body(sd, path, req.range_start, req.range_end);		
			if (err < 0){
				err_print("Failed to send response body!");
			}
		} 
	} else {
		err_print("Failed to send response header!");
 	}
	
	
	
	err = write_log(logging, log_fd);
	if ( err < 0 ) {
		err_print("Failed to print logging information.");
	}
	
	return 0;
}

