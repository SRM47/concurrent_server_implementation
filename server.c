#include "queue.h"
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 4747
#define THREAD_POOL_SIZE 10
#define SERVER_BACKLOG 20
#define BUFF_SIZE 4096

// Initialize Queue for incoming connection requests.
struct Node head = (struct Node){.next = NULL, .val = -1};
struct Queue q = (struct Queue){.head = &head, .tail = &head};

// Initialize thread pool
pthread_t thread_pool[THREAD_POOL_SIZE];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

int check_error(int rc)
{
       if (rc < 0)
       {
              perror("[RC ERROR]: ");
              exit(EXIT_FAILURE);
       }
       return rc;
}

int initialize_server()
{
       // Create server socket.
       int server_socket_fd;
       check_error(server_socket_fd = socket(AF_INET, SOCK_STREAM, 0));

       // Make the socket address.
       struct sockaddr_in server_address;

       bzero(&server_address, sizeof(server_address));
       server_address.sin_family = AF_INET;
       server_address.sin_port = htons(PORT);
       server_address.sin_addr.s_addr = INADDR_ANY;

       // Bind the server socket to server address.
       check_error(bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(struct sockaddr)));

       // Set server socket available to listen to up to SERVER_BACKLOG incoming requests.
       check_error(listen(server_socket_fd, SERVER_BACKLOG));

       return server_socket_fd;
}

void *handle_client(void *arg)
{
       for (;;)
       {
              int client_socket_fd;
              pthread_mutex_lock(&mutex);
              while ((client_socket_fd = dequeue(&q)) == -1)
              {
                     pthread_cond_wait(&cv, &mutex);
              }
              printf("Client Received!");
              fflush(stdout);
              pthread_mutex_unlock(&mutex);
              write(client_socket_fd, "Connect received! Yay!\n", 23);
              close(client_socket_fd);
       }
}

int main(int argc, char *argv[])
{
       // Initialize thread pool
       for (int i = 0; i < THREAD_POOL_SIZE; i++)
       {
              pthread_create(thread_pool + i, NULL, handle_client, NULL);
       }
       // Initial the server and obtain the server socket file descriptor.
       int server_socket_fd = initialize_server();

       // Accept incoming requests from clients
       for (;;)
       {
              struct sockaddr_in client_address;
              int client_socket_fd;
              socklen_t size;
              check_error(client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_address, &size));
              pthread_mutex_lock(&mutex);
              enqueue(&q, client_socket_fd);
              pthread_cond_signal(&cv);
              pthread_mutex_unlock(&mutex);
       }

       return 0;
}