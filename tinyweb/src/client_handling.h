#include <tinyweb.h>

int send_response(http_res_t * response,int sd);
int handle_client(int sd, char* root_dir, http_log_t logging, FILE* log_fd);
int write_log(http_log_t logging, FILE* log_fd);
