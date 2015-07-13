#include <safe_print.h>
#include <string.h>
#include <tinyweb.h>

#define BUFSIZE 1000

int parse_header(char * buffer, http_req_t * request)
{	
	char * header_string = malloc(BUFSIZE);
	char * header_value = malloc(BUFSIZE);
	if(buffer == (strstr(buffer, "\r\n"))){
		return 0; // Done with parsing
	} 
	else
	{
		char * walker = strstr(buffer, ":");
		if (walker)
		{
			strncpy(header_string, buffer, walker-buffer);
			header_string[walker-buffer+1] = '\0';
			buffer = walker + 1;
			walker = strstr(buffer, "\r\n");
			if (strcmp(header_string, "Range") == 0)
			{	
				if (walker)
				{
					strncpy(header_value, buffer, walker-buffer);
					header_value[walker-buffer] = '\0';
					request->range = header_value;
				} 
				else 
				{
					return -1;
				}
			}

			walker = strstr(buffer, "\r\n");
			if (walker)
			{	
				free(header_string);
				return parse_header(walker+2, request);
			}
			else
			{	
				free(header_string);
				return -1;
			}
					
		}
		else
		{
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
		parse_version(++walker, request);
		return 0;
	}	
	return -1;	
}

int parse_method(char * buffer, http_req_t * request){
	char * walker; 
	
	walker = buffer;
	
	if (*walker == 'G')
	{
		if (*(++walker) != 'E') { return -1;}
		if (*(++walker) != 'T') { return -1;}
		request->method = HTTP_METHOD_GET;
	}
	if (*walker == 'H')
	{
		if (*(++walker) != 'E') { return -1;}
		if (*(++walker) != 'A') { return -1;}
		if (*(++walker) != 'D') { return -1;}
		request->method = HTTP_METHOD_HEAD;
	}
	
	if (*(++walker) != ' ') { return -1;}
	
	int err = parse_resource_string(++walker, request);
	return err;
}

int parse_request(http_req_t * request, char *req_string)
{
	char * buffer = malloc(BUFSIZE);
	strncpy(buffer, req_string, strlen(req_string));

	int err = parse_method(buffer, request);
	if (err < 0 )
	{
		safe_printf("Error on parsing request!\n");
	}
	free(buffer);
	return err;
}

