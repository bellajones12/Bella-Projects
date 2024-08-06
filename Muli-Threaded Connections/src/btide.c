#include "net/btide.h"
#include "net/handling_connect.h"
#include "net/package.h"

int main(int argc, char** argv) {

    struct config_obj* config_object = NULL;
    
    // creating config object
    if (argc >= 1) {
        const char* config = argv[1];
        config_object = load_config(config);
    } else {
        return -1;
    }

    char input[MAX_INPUT_CHARACTERS];

    // initialising thread array
    pthread_t* thread_list = (pthread_t*)malloc(config_object->max_peers * sizeof(pthread_t));
    int i = 0;

    // initialsing package list
    struct package *head = NULL;
    struct package_head *package_head = (struct package_head*)malloc(sizeof(struct package_head*));
    package_head->head = head;


    // initialising peer list
    struct peer *peer_head = NULL;
    struct peer_head *all_peers = (struct peer_head*)malloc(sizeof(struct peer_head*));
    all_peers->head = peer_head;

    // pthread_t server_thread;
    uint16_t port = (config_object->port);
    struct server control;
    control.port = port;
    control.stop = 0;
    control.socket = -1;
    control.head = &package_head;
    control.client_count = 0;

    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, connect_server, &control) != 0) {
        perror("failed to createserver thread");
        return EXIT_FAILURE;
    }

    
    while (fgets(input, MAX_INPUT_CHARACTERS, stdin) != NULL) {
        // if eof is reached or QUIT is inputted, exit
        if (feof(stdin) || handle_input(input, &package_head, config_object, &all_peers, i, thread_list) == 1) {
            free_all_packages(package_head);
            free(package_head);
            free_all_peers(all_peers);
            free(all_peers);
            free(config_object);
            free_all_clients(&control);
            close(control.socket);
            free(thread_list);
            return 0;
        }
        i++;
    }
    return 0;

}