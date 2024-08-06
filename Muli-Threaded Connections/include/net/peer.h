#ifndef PEER_H
#define PEER_H

#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <semaphore.h>

#include "net/package.h"


struct peer {
	int port;
	struct peer* next;
	char *ip_address;
	int socket;
	int result;
	pthread_mutex_t mutex;
	struct btide_packet* packet;
	sem_t send_semaphore;
};

struct peer_head {
    struct peer* head;
};


struct peer *add_peer(struct peer_head* current_head, struct peer *new_node);

struct peer *create_new_peer(char* ip_address, int port, struct peer* next);

void print_all_peers(struct peer_head* current_head);

int disconnect_peer(char* ip_address, int port, struct peer_head** head);

struct peer* find_peer(char* ip_address, int port, struct peer_head** head);

void free_all_peers(struct peer_head *head);

void connect_peer(char* ip, int port, struct peer_head** head, pthread_t* thread_list, int i);

void fetch(struct package* new_package, char* hash, char* identifier, char* ip, int port, struct peer* new_peer, pthread_t* thread_list, int i, struct package_head** package_head, struct config_obj* config);

#endif
