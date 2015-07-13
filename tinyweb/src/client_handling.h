#include <tinyweb.h>

int send_response(http_res_t * response,int sd);
int handle_client(int sd, char* root_dir);
