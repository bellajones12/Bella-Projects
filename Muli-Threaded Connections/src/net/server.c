#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#include "net/server.h"


/**
 * Signal handler for SIGINT (Ctrl+C).
 *
 * @param signum The signal number
 */
void sigint_handler(int signum) {
    printf("\nReceived SIGINT (Ctrl + C). Quitting...\n");
    exit(signum); // Exit the program with the signal number
}


/**
 * Establish a connection with a client.
 *
 * @param arg pointer to server struct continaing all necessary info including port, ip, client info etc.
 */
void* connect_server(void* arg) {
    // allow quit via ctrl-C
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Error registering signal handler for SIGINT");
        return NULL;
    }
    
    struct server* control = (struct server*)arg;
    struct package_head **head = control->head;

    int server_fd; 
    struct sockaddr_in address;


    // creating connection
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(control->port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }


    // listening for new connections until server socket is closed
    while (!control->stop) {
        int new_socket;
        socklen_t addrlen = sizeof(struct sockaddr);
        struct sockaddr_in client_address;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        } 
        size_t buffer_size = sizeof(struct btide_packet) + PAYLOAD_MAX;


        // checking if is a new or existing connection
        if (is_new_client(&client_address, control, new_socket)) {
            first_connection_ACP(new_socket, buffer_size);
        }

        else {

            ssize_t nread;
            uint8_t *new_buffer = receive_data(new_socket, buffer_size, &nread);
            struct btide_packet *new_packet = process_request_packet(new_buffer, nread, new_socket);

            if (new_packet == NULL) {
                free_packet(new_packet);
                return NULL;
            }

            if (new_packet->msg_code == PKT_MSG_REQ) {
            struct package *new_package = find_packages(new_packet->pl.req->identifier, head);
            if (new_package == NULL) {
                printf("Package not managed\n");
                close(new_socket);
                free_packet(new_packet);
                continue;
            }

            FILE *file = fopen(new_package->absolute_path, "rb");
            if (file == NULL) {
                printf("Error opening file\n");
                close(new_socket);
                free_packet(new_packet);
                continue;
            }

            size_t bytes_left = new_packet->pl.req->data_len;
            int bytes_sent = 0;

            while (bytes_sent < new_packet->pl.req->data_len) {
                size_t bytes_to_send = (bytes_left > 2998) ? 2998 : bytes_left;
                uint8_t *buf = read_file_data(file, new_packet->pl.req->offset, bytes_to_send);

                if (send_response_packet(new_socket, new_packet, buffer_size, buf, bytes_to_send, &control->clients_mutex) != 0) {
                    free(buf);
                    break;
                }

                free(buf);
                bytes_sent += bytes_to_send;
                bytes_left -= bytes_to_send;
                new_packet->pl.req->offset += bytes_to_send;
            }

            fclose(file);
        }

        free_packet(new_packet);
    }

    }
    // allocating server socket to server struct to keep track of
    control->socket = server_fd;
    pthread_exit(NULL);
}


/**
 * Checks if a connection is a new connection or an existing one
 *
 * @param address sockaddr_in structure containing the client's address information
 * @param control server control structure
 * @param socket file descriptor of the client socket
 * @return 0 if is existing connection, 1 if is a new connection, -1 if is an error
 */
int is_new_client(struct sockaddr_in *address, struct server* control, int socket) {
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address->sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(address->sin_port);

    // iterating through clients to see if list already contains current client
    pthread_mutex_lock(&control->clients_mutex);
    for (int i = 0; i < control->client_count; i++) {
        if (strcmp(control->clients[i]->ip, client_ip) == 0 && control->clients[i]->port == client_port) {
            // if does, return 0
            pthread_mutex_unlock(&control->clients_mutex);
            return 0;
        }
    }
    
    // if client not found, need to add clients to list only if is less than max clients
    if (control->client_count < MAX_CLIENTS) {
        struct client_info* new_client = create_client_info(client_ip, client_port, socket);
        if (new_client == NULL) {
            pthread_mutex_unlock(&control->clients_mutex);
            return -1;
        }

        // adding clients to list, and incrementing total clients
        control->clients[control->client_count] = new_client;
        control->client_count++;
        pthread_mutex_unlock(&control->clients_mutex);
        return 1;
    }
    else {
        pthread_mutex_unlock(&control->clients_mutex);
        return 1;
    }
    return -1;
}



/**
 * Finds specified client based on its address and socket
 *
 * @param control server control structure
 * @param address sockaddr_in structure containing the client's address information
 * @param socket file descriptor of client socket
 * @return client_info struct for the specified client
 */
struct client_info* find_client_info(struct server *control, struct sockaddr_in *address, int socket) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN);
    int port = ntohs(address->sin_port);

    pthread_mutex_lock(&control->clients_mutex);

    // iterating through client list to find specified client
    for (int i = 0; i < control->client_count; i++) {
        if (strcmp(control->clients[i]->ip, ip) == 0 && control->clients[i]->port == port) {
            pthread_mutex_unlock(&control->clients_mutex);
            return control->clients[i];
        }
    }
    pthread_mutex_unlock(&control->clients_mutex);
    return NULL;
}


/**
 * Creates new client_info structure for a new client connection
 *
 * @param ip client's IP address
 * @param port client's port number
 * @param socket file descriptor of the client socket
 * @return new client_info struct created
 */
struct client_info* create_client_info(const char* ip, int port, int socket) {
    struct client_info* client = (struct client_info*)malloc(sizeof(struct client_info));
    if (client == NULL) {
        perror("Failed to allocate memory for client_info");
        return NULL;
    }

    // creating new client
    strncpy(client->ip, ip, INET_ADDRSTRLEN);
    client->port = port;
    client->socket = socket;
    if (sem_init(&client->send_semaphore, 0, 0) != 0) {
        perror("Semaphore initialization failed");
        free(client);
        return NULL;
    }

    return client;
}


/**
 * Frees all client info structs
 *
 * @param server server control struct
 */
void free_all_clients(struct server* server) {
    for (int i = 0; i < server->client_count; i ++) {
        if (server->clients[i] != NULL) {
            // freeing specific client
            free_client_info(server->clients[i]);
        }
    }
}


/**
 * Frees specific client_info
 *
 * @param client client_info struct to be freed
 */
void free_client_info(struct client_info* client) {
    if (client != NULL) {
        // destorying semaphore
        sem_destroy(&client->send_semaphore);
        // freeing client
        free(client);
    }
}



/**
 * Sending ACP packet to client to acknowledge connection
 *
 * @param new_socket client's socket connection
 * @param buffer_size size of buffer to send to client
 */
void first_connection_ACP(int new_socket, size_t buffer_size) {
    uint8_t *buffer = (uint8_t *)malloc(buffer_size);
    struct btide_packet* packet = create_ACP_packet();

    serialise_packet(packet, buffer);
    free_packet(packet);
                
    // sending buffer to client
    ssize_t nwritten = send(new_socket, buffer, buffer_size, 0);
    free(buffer);
    if (nwritten <= 0) {
        fprintf(stderr, "could not write entire message to client: socket fd %d\n", new_socket);
        close(new_socket);
        new_socket = 0;
        return;
    }

    uint8_t *response_buffer = (uint8_t *)malloc(buffer_size);
            
    ssize_t nread = read(new_socket, response_buffer, buffer_size);
    if (nread <= 0) {
        fprintf(stderr, "Server disconnected: socket fd %d\n", new_socket);
        close(new_socket);
        free(response_buffer);
        return;
    }

    // need to deserialise first
    struct btide_packet *response_packet = malloc(sizeof(struct btide_packet));
    deserialise_packet(response_buffer, response_packet);
    free(response_buffer);
    free_packet(response_packet);
}


/**
 * Reading data
 *
 * @param socket client's socket connection
 * @param buffer_size size of buffer to send to client
 * @param nread to read data
 * @return buffer of data read
 */
uint8_t* receive_data(int socket, size_t buffer_size, ssize_t *nread) {
    uint8_t *buffer = (uint8_t *)malloc(buffer_size);
    *nread = recv(socket, buffer, buffer_size, 0);
    return buffer;
}



/**
 * Reading packet
 *
 * @param buffer of data to interpret
 * @param nread to read data
 * @param socket of client connection
 * @return packet constructed from data
 */
struct btide_packet* process_request_packet(uint8_t *buffer, ssize_t nread, int socket) {
    struct btide_packet *packet = malloc(sizeof(struct btide_packet));
    if (nread <= 0) {
        fprintf(stderr, "Server disconnected: socket fd %d\n", socket);
        close(socket);
        free(buffer);
        return NULL;
    }
    deserialise_packet(buffer, packet);
    free(buffer);
    return packet;
}


/**
 * sending constructed RES packet packet
 *
 * @param socket of client connection
 * @param packet REQ packet containing data to populate RES packet
 * @param buffer_size size of buffer to send
 * @param data data to send
 * @param data_len length of data to send
 * @param clients_mutex mutex as accessing shared data
 * @return int representing success
 */
int send_response_packet(int socket, struct btide_packet *packet, size_t buffer_size, uint8_t *data, size_t data_len, pthread_mutex_t *clients_mutex) {
    struct btide_packet* RES_packet = create_RES_packet(packet->pl.req->hash, packet->pl.req->identifier, packet->pl.req->offset, data, data_len);

    uint8_t *new_buffer = (uint8_t *)malloc(buffer_size);
    pthread_mutex_lock(clients_mutex);
    serialise_packet(RES_packet, new_buffer);
    free_packet(RES_packet);

    ssize_t nwritten = send(socket, new_buffer, buffer_size, 0);

    pthread_mutex_unlock(clients_mutex);
    free(new_buffer);

    if (nwritten <= 0) {
        fprintf(stderr, "Could not write entire message to client: socket fd %d\n", socket);
        return -1;
    }
    return 0;
}


/**
 * Reading file and returning data read
 *
 * @param file object to read
 * @param offset of where to read data from
 * @param data_len of how much to read
 * @return buffer of what was read
 */
uint8_t* read_file_data(FILE *file, size_t offset, size_t data_len) {
    fseek(file, offset, SEEK_SET);
    uint8_t *buf = malloc(data_len);
    fread(buf, 1, data_len, file);
    return buf;
}