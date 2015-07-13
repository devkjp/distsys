/*===================================================================
 * DHBW Ravensburg - Campus Friedrichshafen
 *
 * Vorlesung Verteilte Systeme
 *
 * Author:  Ralf Reutemann
 *
 *===================================================================*/
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
#include <signal.h>
#include <getopt.h>

#include "tinyweb.h"
#include "connect_tcp.h"
#include "passive_tcp.h"
#include "client_handling.h"

#include "safe_print.h"
#include "sem_print.h"


// Must be true for the server accepting clients,
// otherwise, the server will terminate
static volatile sig_atomic_t server_running = false;

#define IS_ROOT_DIR(mode)   (S_ISDIR(mode) && ((S_IROTH || S_IXOTH) & (mode)))

//
// TODO: Include your function header here
//
static void
sig_handler(int sig)
{
    switch(sig) { 
        case SIGINT:
            // treat interrupt
            safe_printf("\n[%d] Server terminated due to keyboard interrupt\n", getpid());
            server_running = false;
            break;
        case SIGCHLD:
        	// treat child process signal
        	safe_printf("\n[%d] Signal from child process detected\n", getpid());
            break;
        case SIGSEGV:
        	// treat segfault
        	safe_printf("\n[%d] Server terminated due to segmentation violation\n", getpid());
			server_running = false;
            break;
        case SIGABRT:
        	// treat abort
			safe_printf("\n[%d] Server terminated due to system abort\n", getpid());
			server_running = false;
            break;
        default:
            break;
    } /* end switch */
} /* end of sig_handler */


//
// TODO: Include your function header here
//
static void
print_usage(const char *progname)
{
  fprintf(stderr, "Usage: %s options\n", progname);
  // TODO: Print the program options
} /* end of print_usage */


//
// TODO: Include your function header here
//
static int
get_options(int argc, char *argv[], prog_options_t *opt)
{
    int                 c;
    int                 err;
    int                 success = 1;
    char               *p;
    struct addrinfo     hints;

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

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;   /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    while (success) {
        int option_index = 0;
        static struct option long_options[] = {
            { "file",    required_argument, 0, 0 },
            { "port",    required_argument, 0, 0 },
            { "dir",     required_argument, 0, 0 },
            { "verbose", no_argument,       0, 0 },
            { "debug",   no_argument,       0, 0 },
            { NULL,      0, 0, 0 }
        };

        c = getopt_long(argc, argv, "f:p:d:v", long_options, &option_index);
        if (c == -1) break;

        switch(c) {
            case 'f':
                // 'optarg' contains file name
                opt->log_filename = (char *)malloc(strlen(optarg) + 1);
                if (opt->log_filename != NULL) {
                    strcpy(opt->log_filename, optarg);
                } else {
                    err_print("cannot allocate memory");
                    return EXIT_FAILURE;
                } /* end if */
                break;
            case 'p':
                // 'optarg' contains port number
                if((err = getaddrinfo(NULL, optarg, &hints, &opt->server_addr)) != 0) {
                    fprintf(stderr, "Cannot resolve service '%s': %s\n", optarg, gai_strerror(err));
                    return EXIT_FAILURE;
                } /* end if */
                break;
            case 'd':
                // 'optarg contains root directory */
                opt->root_dir = (char *)malloc(strlen(optarg) + 1);
                if (opt->root_dir != NULL) {
                    strcpy(opt->root_dir, optarg);
                } else {
                    err_print("cannot allocate memory");
                    return EXIT_FAILURE;
                } /* end if */
                break;
            case 'v':
                opt->verbose = 1;
                break;
            default:
                success = 0;
        } /* end switch */
    } /* end while */

    // check presence of required program parameters
    success = success && opt->server_addr && opt->root_dir;

    // additional parameters are silently ignored, otherwise check for
    // ((optind < argc) && success)

    return success;
} /* end of get_options */


static void
open_logfile(prog_options_t *opt)
{
    // open logfile or redirect to stdout
    if (opt->log_filename != NULL && strcmp(opt->log_filename, "-") != 0) {
        opt->log_fd = fopen(opt->log_filename, "w");
        if (opt->log_fd == NULL) {
            perror("ERROR: Cannot open logfile");
            exit(EXIT_FAILURE);
        } /* end if */
    } else {
        printf("Note: logging is redirected to stdout.\n");
        opt->log_fd = stdout;
    } /* end if */
} /* end of open_logfile */


static void
check_root_dir(prog_options_t *opt)
{
    struct stat stat_buf;

    // check whether root directory is accessible
    if (stat(opt->root_dir, &stat_buf) < 0) {
        /* root dir cannot be found */
        perror("ERROR: Cannot access root dir");
        exit(EXIT_FAILURE);
    } else if (!IS_ROOT_DIR(stat_buf.st_mode)) {
        err_print("Root dir is not readable or not a directory");
        exit(EXIT_FAILURE);
    } /* end if */
} /* end of check_root_dir */


static void
install_signal_handlers(void)
{
    struct sigaction sa;

    // init signal handler(s)
    // TODO: add other signals
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sig_handler;
    if(sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction(SIGINT)");
        exit(EXIT_FAILURE);
    } /* end if */
    
    // add SIGCHLD
	if(sigaction(SIGCHLD, &sa, NULL) < 0) {
        perror("sigaction(SIGCHLD)");
        //exit(EXIT_FAILURE);
    } /* end if */
    // add SIGSEGV
    if(sigaction(SIGSEGV, &sa, NULL) < 0) {
        perror("sigaction(SIGSEGV)");
        exit(EXIT_FAILURE);
    } /* end if */
    
    // add SIGABRT
    if(sigaction(SIGABRT, &sa, NULL) < 0) {
        perror("sigaction(SIGABRT)");
        exit(EXIT_FAILURE);
    } /* end if */
} /* end of install_signal_handlers */


int
main(int argc, char *argv[])
{
    int retcode = EXIT_SUCCESS;
    prog_options_t my_opt;

    // read program options
    if (get_options(argc, argv, &my_opt) == 0) {
        print_usage(my_opt.progname);
        exit(EXIT_FAILURE);
    } /* end if */

    // set the time zone (TZ) to GMT in order to
    // ignore any other local time zone that would
    // interfere with correct time string parsing
    setenv("TZ", "GMT", 1);
    tzset();

    // do some checks and initialisations...
    open_logfile(&my_opt);
    check_root_dir(&my_opt);
    install_signal_handlers();
    init_logging_semaphore();

    // get root_dir to handle it later in child process
    char* root_dir = my_opt.root_dir;
    
    // start the server and create socket
    unsigned short PORT = 8080;
    print_log("Starting server '%s'...\n", my_opt.progname);
    int accepting_socket = passive_tcp(PORT, 5);
    if (accepting_socket < 0){
        err_print("Error when opening accepting socket!");
        exit(-1);
    }
    struct sockaddr_in from_client;
    
    int req_no = 0;
    server_running = true;
    while(server_running) {
        socklen_t from_client_len = sizeof(from_client);
        int listening_socket = accept(accepting_socket, (struct sockaddr *) &from_client, &from_client_len);
        
        if (listening_socket >= 0){ /* Accept was ok */
            ++req_no;
            pid_t pid = fork();
            if (pid == 0) 
            { /* Child Process */
                print_log("Process created to handle new request #%d\n", req_no);
                close(accepting_socket);            
                handle_client(listening_socket, root_dir);
                exit(0);
            } 
            else if (pid > 0) 
            { /* Parent Process */
                close(listening_socket);
            } 
            else 
            { /* Fork Failed */
                err_print("Could not fork for new request!");
                exit(-1);
            }
        } /*else {
            print_log("Accept failed!\n");
        }   */   
    } /* end while */

    printf("[%d] Good Bye...\n", getpid());
    exit(retcode);
} /* end of main */

