/*
 * client.c: A very, very primitive HTTP client.
 * 
 * To run, try: 
 *      client www.cs.wisc.edu 80 /
 *
 * Sends one HTTP request to the specified HTTP server.
 * Prints out the HTTP response.
 *
 * blg312e: For testing your server, you will want to modify this client.  
 * For example:
 * 
 * You may want to make this multi-threaded so that you can 
 * send many requests simultaneously to the server.
 *
 * You may also want to be able to request different URIs; 
 * you may want to get more URIs from the command line 
 * or read the list from a file. 
 *
 * When we test your server, we will be using modifications to this client.
 *
 */
#include <pthread.h>
#include "blg312e.h"

/*
 * Send an HTTP request for the specified file 
 */
typedef struct _client_data {
    char *host;
    char *filename;
    int port;
} client_data;

void clientSend(int fd, char *filename)
{
  char buf[MAXLINE];
  char hostname[MAXLINE];

  Gethostname(hostname, MAXLINE);

  /* Form and send the HTTP request */
  sprintf(buf, "GET %s HTTP/1.1\n", filename);
  sprintf(buf, "%s host: %s\n\r\n", buf, hostname);
  Rio_writen(fd, buf, strlen(buf));
}
  
/*
 * Read the HTTP response and print it out
 */
void clientPrint(int fd)
{
  rio_t rio;
  char buf[MAXBUF];  
  int length = 0;
  int n;
  
  Rio_readinitb(&rio, fd);

  /* Read and display the HTTP Header */
  n = Rio_readlineb(&rio, buf, MAXBUF);
  while (strcmp(buf, "\r\n") && (n > 0)) {
    printf("Header: %s", buf);
    n = Rio_readlineb(&rio, buf, MAXBUF);

    /* If you want to look for certain HTTP tags... */
    if (sscanf(buf, "Content-Length: %d ", &length) == 1) {
      printf("Length = %d\n", length);
    }
  }

  /* Read and display the HTTP Body */
  n = Rio_readlineb(&rio, buf, MAXBUF);
  while (n > 0) {
    printf("%s", buf);
    n = Rio_readlineb(&rio, buf, MAXBUF);
  }
}

void* single_client(void* arg) {
    
    client_data *t = (client_data*)(arg);
    char* host = t->host;
    char* filename = t->filename;
    int port = t->port;
    /* Open a single connection to the specified host and port */
    int clientfd = Open_clientfd(host, port);
    
    clientSend(clientfd, filename);
    clientPrint(clientfd);
    
    Close(clientfd);
    return NULL;

}

int main(int argc, char *argv[])
{
  char *host;
  int port;

  if (argc < 4) {
    fprintf(stderr, "Usage: %s <host> <port> <filename>\n", argv[0]);
    exit(1);
  }

  host = argv[1];
  port = atoi(argv[2]);

  // Total number of files given as input from the command line

  int concur_clients = argc - 3;
  pthread_t threads[concur_clients];

  // Creating threads to server requests for all the files concurrently
  for(int i = 0; i < concur_clients; i++) {
        
        client_data *d = malloc(sizeof(client_data));
        if(d == NULL) {
            printf("Could not create request for: %s\n", argv[3 + i]);
            continue;
        }
        d->host = host;
        d->port = port;
        d->filename = argv[3 + i];
        pthread_create(&threads[i], NULL, single_client, (void *)d);
        
    }
    for(int i = 0; i < concur_clients; i++) {
        pthread_join(threads[i], NULL);
    }

    exit(0);
}
