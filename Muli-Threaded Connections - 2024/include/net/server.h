#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <semaphore.h>

#include "net/packet.h"
#include "net/package.h"
#include "net/config.h"

#define MAX_CLIENTS (10)
#define BUFFER_SIZE (1024)


struct client_info {
	char ip[INET_ADDRSTRLEN];
	int port;
	int socket;
	sem_t send_semaphore;
};


struct server {
	uint16_t port;
	int stop;
    int socket;
	struct package_head **head;
	struct client_info* clients[MAX_CLIENTS];
	int client_count;
	pthread_mutex_t clients_mutex;
};

void sigint_handler(int signum);

void* connect_server(void* arg);

int is_new_client(struct sockaddr_in *address, struct server* control, int socket);

struct client_info* find_client_info(struct server *control, struct sockaddr_in *address, int socket);

struct client_info* create_client_info(const char* ip, int port, int socket);

void free_client_info(struct client_info* client);

void free_all_clients(struct server* server);

void first_connection_ACP(int new_socket, size_t buffer_size);

uint8_t* read_file_data(FILE *file, size_t offset, size_t data_len);

int send_response_packet(int socket, struct btide_packet *packet, size_t buffer_size, uint8_t *data, size_t data_len, pthread_mutex_t *clients_mutex);

struct btide_packet* process_request_packet(uint8_t *buffer, ssize_t nread, int socket);

uint8_t* receive_data(int socket, size_t buffer_size, ssize_t *nread);

#endif