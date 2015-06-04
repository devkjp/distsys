//
// TODO: Include your module header here
//


#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <netdb.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#include "tinyweb.h"
#include "connect_tcp.h"



//
// TODO: Include your function header here
//
void
sig_handler(int sig)
{
    // TODO: Complete signal handler
} /* end of sig_handler */


//
// TODO: Include your function header here
//
void
print_usage(const char *progname)
{
  fprintf(stderr, "Usage: %s options\n", progname);
  // TODO: Print the program options
} /* end of print_usage */


//
// TODO: Include your function header here
//
int
get_options(int argc, char *argv[], prog_options_t *opt)
{
    int                 c;
    int                 index;
    int                 success = 1;
    char               *p;

    p = strrchr(argv[0], '/');
    if(p) {
        p++;
    } else {
        p = argv[0];
    } /* end if */

    opt->progname = (char *)malloc(strlen(p) + 1);
    if (opt->progname != NULL) {
        strcpy(opt->progname, p);
    } else {
        err_print("cannot allocate memory");
        return EXIT_FAILURE;
    } /* end if */

    opt->log_filename = NULL;
    opt->root_dir     = NULL;
    opt->server_addr  = NULL;
    opt->verbose      =    0;
    opt->timeout      =  120;

    while (success) {
        int option_index = 0;
        static struct option long_options[] = {
            { "file",    1, 0, 0 },
            { "port",    1, 0, 0 },
            { "dir",     1, 0, 0 },
            { "verbose", 0, 0, 0 },
            { NULL,      0, 0, 0 }
        };

        c = getopt_long(argc, argv, "f:p:d:v", long_options, &option_index);
        if (c == -1) break;
        index = (c == 0) ? option_index : -1;

        if (c == 'f' || index == 0) {
            // 'optarg' contains file name
        } else if (c == 'p' || index == 1) {
            // 'optarg' contains port number
        } else if (c == 'd' || index == 2) {
            // 'optarg contains root directory */
        } else if (c == 'v' || index == 3) {
            opt->verbose = 1;
        } else {
            success = 0;
        } /* end if */
    } /* end while */

    success = success && opt->server_addr;

    // additional parameters are silently ignored, otherwise check for
    // ((optind < argc) && success)

    return success;
} /* end of get_options */


int
main(int argc, char *argv[])
{
    int retcode = EXIT_SUCCESS;
    prog_options_t my_opt;

    if (get_options(argc, argv, &my_opt) == 0) {
        print_usage(my_opt.progname);
        return EXIT_FAILURE;
    } /* end if */

    // start the server and handle clients...

    return retcode;
} /* end of main */

