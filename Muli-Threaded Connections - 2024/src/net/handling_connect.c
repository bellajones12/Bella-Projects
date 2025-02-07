#include "net/handling_connect.h"
#include "net/package.h"
#include "net/peer.h"


/**
 * Handles fetch input.
 *
 * @param input users input
 * @param head head of package list
 * @param peer_head head of the peer list.
 * @param thread_list array of thread IDs
 * @param i index of current thread ID
 */
void handle_fetch(char* input, struct package_head** head, struct peer_head** peer_head, pthread_t* thread_list, int i, struct config_obj* config) {
    char command[MAX_INPUT_CHARACTERS];
    char command2[MAX_INPUT_CHARACTERS];
    char identifier[MAX_INPUT_CHARACTERS];
    char hash[MAX_INPUT_CHARACTERS];

    char *ip;
    char *port;
    int port_int;

    if (sscanf(input, "%s %s %s %s\n", command, command2, identifier, hash) == 4) {
        // extracting ip and port info
        ip = strtok(command2, ":");
        port = strtok(NULL, ":");
        port_int = strtol(port, NULL, 10);
        struct package* new_package = find_packages(identifier, head);
        struct peer* new_peer = find_peer(ip, port_int, peer_head);
        
        // checking if peer is in list
        if (new_peer == NULL) {
            printf("Unable to request chunk, peer not in list\n");
        }

        // checking if package is managed
        else if (new_package == NULL) {
            printf("Unable to request chunk, package is not managed\n");
        } 
        // calling fetch
        else {
            fetch(new_package, hash, identifier, ip, port_int, new_peer, &thread_list[i], i, head, config);
        }
    } else {
        printf("Missing arguments from command\n");
    }
}


/**
 * Handles connect input
 *
 * @param input users input
 * @param peer_head head of the peer list.
 * @param thread_list array of thread IDs
 * @param i index of current thread ID
 */
void handle_connect(char* input, struct peer_head** peer_head, pthread_t* thread_list, int i) {
    char command[MAX_INPUT_CHARACTERS];
    char command2[MAX_INPUT_CHARACTERS];

    char *ip;
    char *port;
    int port_int;

    if (sscanf(input, "%s %s\n", command, command2) == 2) {
        // extracting ip and port info
        ip = strtok(command2, ":");
        port = strtok(NULL, ":");
        port_int = strtol(port, NULL, 10);

        // checking if peer is already connected
        if (find_peer(ip, port_int, peer_head) == NULL) {
            // calling connect
            connect_peer(ip, port_int, peer_head, thread_list, i);

        } else {
            printf("Already connected to peer\n");
        }
    }
}


/**
 * Handles disconnect input
 *
 * @param input users input
 * @param peer_head head of the peer list.
 * @param thread_list array of thread IDs
 * @param i index of current thread ID
 */
void handle_disconnect(char* input, struct peer_head** peer_head, pthread_t* thread_list, int i) {
    char command[MAX_INPUT_CHARACTERS];
    char command2[MAX_INPUT_CHARACTERS];

    char *ip;
    char *port;
    int port_int;

    if (sscanf(input, "%s %s\n", command, command2) == 2) {
        // extracting ip and port address
        ip = strtok(command2, ":");
        port = strtok(NULL, ":");
        port_int = strtol(port, NULL, 10);
        struct peer* peer_to_delete = find_peer(ip, port_int, peer_head);

        // checking if specified peer is in list
        if (peer_to_delete != NULL) {
            close(peer_to_delete->socket);
            // checking if disconnect was successful
            int status = disconnect_peer(ip, port_int, peer_head);
            if (status == 0) {
                printf("Disconnected from peer\n");
            } else {
               printf("Unknown peer, not connected\n");  
            }
        } else {
           printf("Unknown peer, not connected\n"); 
        }
    } else {
        printf("Missing address and port argument.\n");
    }
}

/**
 * Handles addpackage input.
 *
 * @param input users input
 * @param head head of package list
 * @param config pointer to config structure
 */
void handle_add_package(char* input, struct package_head** head, struct config_obj* config) {
    char command[MAX_INPUT_CHARACTERS];
    char command2[MAX_INPUT_CHARACTERS];

    if (sscanf(input, "%s %s\n", command, command2) == 2) {
       struct stat file_exists;

        // checking if have permission to open file
        if (stat(command2, &file_exists) != 0) {
            printf("Cannot open file\n");
        }

        else {
            // adding package to list
            struct package *new_package = create_new_package(command2, NULL, config);
            (*head)->head = add_package(*head, new_package);
        } 
    } else {
        printf("Missing file argument\n");
    }
}


/**
 * Handles rem_package input.
 *
 * @param input users input
 * @param head head of package list
 * @param config pointer to config structure
 */
void handle_rem_package(char* input, struct package_head** head, struct config_obj* config) {
    char command[MAX_INPUT_CHARACTERS];
    char command2[MAX_INPUT_CHARACTERS];

    if (sscanf(input, "%s %s\n", command, command2) == 2) {
        int status = remove_package(command2, head);
        if (status == 0) {
            printf("Package has been removed\n");
        } else {
            printf("Identifier provided does not match managed packages\n");
        }
    } else {
        printf("Missing identifier argument, please specify whole 1024 character or at least 20 characters.\n");
    }
}



/**
 * @param input user input
 * @param head head of package list
 * @param config config object
 * @param peer_head head of peer list
 * @param i index of thread list
 * @param thread_list array of pthread_t threads
 * @return integer to signify to main function to exit
 */
int handle_input(char* input, struct package_head** head, struct config_obj* config, struct peer_head** peer_head, int i, pthread_t* thread_list) {
    
    // tokenising input to find first word
    char *input_copy = strdup(input);
    char *first_word = strtok(input_copy, " ");


    if (strcmp(first_word, "FETCH") == 0) {
        handle_fetch(input, head, peer_head, thread_list, i, config);
    }

    else if(strcmp(first_word, "CONNECT") == 0) {
        handle_connect(input, peer_head, thread_list, i);
    }

    else if (strcmp(first_word, "DISCONNECT") == 0) {
        handle_disconnect(input, peer_head, thread_list, i);
    }

    else if (strcmp(first_word, "ADDPACKAGE") == 0) {
        handle_add_package(input, head, config);
    }

    else if (strcmp(first_word, "REMPACKAGE") == 0) {
        handle_rem_package(input, head, config);
    }

    else if (strcmp(input, "PACKAGES\n") == 0) {
        print_all_packages(*head, config);
    }

    else if (strcmp(input, "PEERS\n") == 0) {
        print_all_peers(*peer_head);
    }

    else if (strcmp(input, "QUIT\n") == 0) {
        free(input_copy);
        return 1;
    }

    else {
        printf("Invalid Input\n");
    }
    free(input_copy);
    return 0;
}