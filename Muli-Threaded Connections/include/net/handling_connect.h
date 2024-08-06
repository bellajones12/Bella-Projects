#ifndef HANDLE_H
#define HANDLE_H

#include <pthread.h>
#include "net/btide.h"

void handle_fetch(char* input, struct package_head** head, struct peer_head** peer_head, pthread_t* thread_list, int i, struct config_obj* config);

void handle_connect(char* input, struct peer_head** peer_head, pthread_t* thread_list, int i);

void handle_disconnect(char* input, struct peer_head** peer_head, pthread_t* thread_list, int i);

void handle_add_package(char* input, struct package_head** head, struct config_obj* config);

void handle_rem_package(char* input, struct package_head** head, struct config_obj* config);

int handle_input(char* input, struct package_head** head, struct config_obj* config, struct peer_head** peer_head, int i, pthread_t* thread_list);

#endif