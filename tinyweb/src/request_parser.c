#define __USE_XOPEN
#define _GNU_SOURCE

#include <safe_print.h>
#include <string.h>
#include <tinyweb.h>
#include <sem_print.h>
#include <time.h>
#include <stdlib.h>

#define BUFSIZE 1000

int parse_header(char * buffer, http_req_t * request)
{	
	char * header_string = malloc(BUFSIZE);
	char * header_value = malloc(BUFSIZE);
	if(buffer == (strstr(buffer, "\r\n"))){
		return 0; // Done with parsing
	} else {
		char * walker = strstr(buffer, ":");
		if (walker)	{
			strncpy(header_string, buffer, walker-buffer);
			header_string[walker-buffer] = '\0';
			buffer = walker + 1;
			walker = strstr(buffer, "\r\n");
			if (strcmp(header_string, "Range") == 0){	
				if (walker)	{	
					strncpy(header_value, buffer, walker-buffer);
					// Get first number
					char * w = header_value;
					header_value = strstr(header_value, "=")+1;
					w = strstr(header_value, "-");
					if (w>header_value){
						char * first_number = malloc(BUFSIZE);
						strncpy(first_number, header_value, w - header_value);
						first_number[w-header_value] = '\0';
						request->range_start = atoi(first_number);
					}
					// Get second number
					header_value = w;
					header_value++;
					if(strlen(header_value) > 0){
						char * second_number = malloc(BUFSIZE);
						strncpy(second_number, header_value, strlen(header_value));
						request->range_end = atoi(second_number);
					}
					
					header_value[walker-buffer] = '\0';
				} else 	{
					return -1;
				}
			} else if (strcmp(header_string, "If-Modified-Since") == 0)	{	
				if (walker)	{	
					buffer +=1;
					strncpy(header_value, buffer, walker-buffer);
					header_value[walker-buffer] = '\0';
					struct tm imTimestamp;
					if( (strptime(header_value, "%a, %d %b %Y %T GMT", &imTimestamp)) != NULL){
						request->if_modified_since = timegm(&imTimestamp);
					} else {
						return -1;
					}
				} else {
					return -1;
				}
			} else if (strcmp(header_string, "") == 0) {
				return 0;
			}

			walker = strstr(buffer, "\r\n");
			if (walker)	{	
				free(header_string);
				return parse_header(walker+2, request);
			}else{	
				free(header_string);
				return -1;
			}
		} else {
			free(header_string);
			return -1;
		}
	}
	free(header_string);
	return 0;	
}

int parse_version(char * buffer, http_req_t * request)
{	
	char * version_string = malloc(BUFSIZE);
	char * walker = strstr(buffer, "\r\n");
	if (walker)
	{	
		strncpy(version_string, buffer, walker-buffer);
		version_string[walker-buffer+1] = '\0';
		if (strcmp(version_string, "HTTP/1.1") == 0)
		{	
			free(version_string);
			return parse_header(walker + 2, request);
		}
		free(version_string);
		return -1;
	} 
	free(version_string);
	return -1;
}

int parse_resource_string(char * buffer, http_req_t * request)
{
	char * resource_string = malloc(BUFSIZE);
	
	char * walker = strstr(buffer, " ");
	if (walker)
	{
		strncpy(resource_string, buffer, walker-buffer );
		resource_string[walker-buffer+1] = '\0';					
		request->resource = resource_string;
		return parse_version(++walker, request);
	}	
	return -1;	
}

int parse_method(char * buffer, http_req_t * request){
	char * walker; 
	walker = strstr(buffer, " ");
	if (walker == NULL){
		return -1;
	}
	
	if (strstr(buffer, "GET") == buffer){
		request->method = HTTP_METHOD_GET;
	} else if (strstr(buffer, "HEAD") == buffer){
		request->method = HTTP_METHOD_HEAD;
	} else if (strstr(buffer, "TEST") == buffer){
		request->method = HTTP_METHOD_TEST;
	} else if (strstr(buffer, "ECHO") == buffer){
		request->method = HTTP_METHOD_ECHO;
	} else {
		request->method = HTTP_METHOD_NOT_IMPLEMENTED;
	}

	int err = parse_resource_string(++walker, request);
	return err;
}

int parse_request(http_req_t * request, char *req_string)
{
	char * buffer = malloc(BUFSIZE);
	strncpy(buffer, req_string, strlen(req_string));
	
	request->range_start = -1;
	request->range_end = -1;

	int err = parse_method(buffer, request);
	if (err < 0 )
	{
		safe_printf("Error on parsing request!\n");
	}
	free(buffer);
	return err;
}

