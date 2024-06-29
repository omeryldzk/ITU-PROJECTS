/*
	Include Headers
*/
#include "common_threads.h"
#include "common_headers.h"
#include "blg312e.h"
#include "request.h"



/*
	Thread worker function recevies the socket descriptor from the scheduler.
	It provides the socket descriptor to the requestHandle() function,
	to handle the request.
*/
void* thread_worker(void* arg)
{	
	thread_arg* args = (thread_arg*)arg;
	while(1)
	{	  
		// Getting the request that this thread has to handle from the scheduler
		int conn_fd = get_from_scheduler(args->workers, args->scheduler);	
		printf("\n");
   		printf("*********************************************\n");
    	printf("\n");
		printf("thread = %d, conn_fd = %d\n", args->num_request, conn_fd);
		printf("\n");
    	printf("*********************************************\n");
    	printf("\n");	
		// Handle request
		requestHandle(conn_fd);

		// Close connection
		Close(conn_fd); 
	}
	return NULL;
}