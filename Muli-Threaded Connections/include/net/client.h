#ifndef CLIENT_H
#define CLIENT_H

#define BUFFER_SIZE (1024)

#include <pthread.h>

#include "net/peer.h"
#include "net/server.h"

void* connect_client(void* arg);

void first_connection(size_t buffer_size, int sock);

#endif