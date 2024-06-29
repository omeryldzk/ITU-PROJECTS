#include "blg312e.h"
#include "request.h"
#include "common_headers.h"
#include "common_threads.h"
#include "io_helper.h"

#include <bits/getopt_core.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// blg312e: Parse the new arguments too
void getargs(int *port, int *num_threads, int *buf_size, char *sched_alg, int argc, char *argv[])
{
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <port> <num_threads> <buf_size> <sched_alg>\n", argv[0]);
        exit(1);
    }
    *port = atoi(argv[1]);
    *num_threads = atoi(argv[2]);
    *buf_size = atoi(argv[3]);
    strcpy(sched_alg, argv[4]);
}

  //Set default directory
  char default_root[] = ".";
  int buffer_size, no_of_threads;
  int front = 0, rear = 0, count = 0;
  char scheduling_policy[MAXLINE];


int main(int argc, char *argv[])
{
    int listenfd, connfd, clientlen,port;
    struct sockaddr_in clientaddr;
    char *root_dir = default_root;

    getargs(&port, &no_of_threads, &buffer_size, scheduling_policy, argc, argv);

    // The scheduler is responsible maintaing the requests according to scheduling policy
    scheduler* scheduler = init_scheduler(scheduling_policy, buffer_size);

    // The thread_pool maintains all the threadsl, locks and condition variables
    thread_pool* workers = init_thread_pool(no_of_threads, buffer_size);

    // Starting the threads
    start_threads(scheduler, workers);

    // Run out of this directory
    chdir_or_die(root_dir);

    // Listen on the specified port
    listenfd = Open_listenfd(port);
    if (listenfd < 0) {
        fprintf(stderr, "Error opening listening socket\n");
        exit(1);
    }
    printf("Server Started \n");

    while (1) {
		
		clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);		//
    if (connfd < 0) {
            perror("Accept failed");
            continue;
        }
    printf("Connection Accepted for FD: %d\n", connfd);
		
		// Schedule the current request
		give_to_scheduler(workers, scheduler, connfd);	
    }
    return 0;
}


    


 
