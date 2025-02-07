#include "net/client.h"
#include "tree/merkletree.h"
#include "net/package.h"
#include "net/peer.h"
#include "net/packet.h"


/**
 * Adds a new peer to the list
 *
 * @param current_head current head of peer list
 * @param new_node new peer to be added
 * @return new head of peer list after insertion
 */
struct peer *add_peer(struct peer_head *current_head, struct peer *new_node) {
    struct peer *temp;
    
    // if is first list, set as head
    if (current_head->head == NULL) {
        current_head->head = new_node;
        return new_node;
    }

    // iterating to end of list
    for (temp = current_head->head; temp->next != NULL; temp = temp->next);

    temp->next = new_node;
    return current_head->head;
}


/**
 * Creates a new peer node with specified IP address and port.
 *
 * @param ip_address peer's IP address
 * @param port peer's port number
 * @param next pointer to next peer
 * @return newly created peer
 */
struct peer *create_new_peer(char* ip_address, int port, struct peer* next) {
    struct peer *new_peer = (struct peer*)malloc(sizeof(struct peer));
    if (new_peer == NULL) {
        free(new_peer);
        return NULL;
    }
    
    // assigning attributes
    new_peer->port = port;
    new_peer->ip_address = strdup(ip_address);
    
    new_peer->next = next;
    new_peer->socket = -1;
    new_peer->result = -1;
    pthread_mutex_init(&new_peer->mutex, NULL);
    return new_peer;
}


/**
 * Disconnects from a peer and removes it from the list
 *
 * @param ip_address peer's IP address.
 * @param port peer's port number
 * @param head head of peer list
 * @return 0 on success, 1 on failure
 */
int disconnect_peer(char* ip_address, int port, struct peer_head** head) {
    struct peer *temp = (*head)->head;
    struct peer *prev = NULL;

    // if are removing head node, need to change head
    if (temp->port == port) {
        (*head)->head = temp->next;
        free(temp->ip_address);
        free(temp);
        return 0;
    }

    while (temp != NULL && (temp->port != port)) {
        prev = temp;
        temp = temp->next;
    }

    // package was not found
    if (temp == NULL) {
        return 1;
    }
    if (prev != NULL) {
        prev->next = temp->next;
    }
    free(temp->ip_address);
    free(temp);
    return 0;
}

/**
 * Prints information about all peers
 *
 * @param current_head head of the peer list
 */
void print_all_peers(struct peer_head* current_head) {
    struct peer* head = current_head->head;

    // if not connected to any peers
    if (head == NULL) {
        printf("Not connected to any peers\n");
        return;
    }
    printf("Connected to:\n\n");
    int i = 1;

    // printing info about all peers
    while (head != NULL) {
        if (head->ip_address != NULL) {
            printf("%d. %s:%d\n", i, head->ip_address, head->port);
        }
        head = head->next;
        i++;
    }
}


/**
 * Frees all peers in list
 *
 * @param head head of peer list
 */
void free_all_peers(struct peer_head *head) {
    struct peer *current = head->head;

    // calling free list for each node
    while (current != NULL) {
        struct peer *next = current->next;
        free(current->ip_address);
        free(current); 
        current = next;
    }
}


/**
 * Finds a peer based on its IP address and port.
 *
 * @param ip_address peer's IP address.
 * @param port peer's port
 * @param head head of peer list
 * @return peer found
 */
struct peer* find_peer(char* ip_address, int port, struct peer_head** head) {
    struct peer *temp = (*head)->head;
    
    while (temp != NULL && temp->port != port) {
        temp = temp->next;
    }

    // peer was not found
    if (temp == NULL) {
        return NULL;
    }

    return temp;
}


/**
 * Connecting to new peer and adding to the list
 *
 * @param ip peer's IP address.
 * @param port peer's port
 * @param head head of peer list
 * @param thread_list array of thread IDs
 * @param i index of current thread id
 */
void connect_peer(char* ip, int port, struct peer_head** head, pthread_t* thread_list, int i) {

    struct peer *new_peer = create_new_peer(ip, port, NULL);
    new_peer->packet = NULL;

    // creating new thread to execute connect on
    int ret = pthread_create(&thread_list[i], NULL, connect_client, new_peer);
    sem_init(&new_peer->send_semaphore, 0, 0);

    if (ret != 0) {
        perror("pthread_create");
    } 
    else {
        pthread_join(thread_list[i], NULL);
        pthread_mutex_lock(&new_peer->mutex);

        // checking result of connection  
        if (new_peer->result == 0) {
            printf("Connection established with peer\n");
            (*head)->head = add_peer(*head, new_peer);
        } else {
            printf("Unable to connect to requested peer\n");
            free(new_peer->ip_address);
            free(new_peer);
        }
        pthread_mutex_unlock(&new_peer->mutex);
        pthread_mutex_destroy(&new_peer->mutex);
        }
}


/**
 * fetching a specific hash from another client
 *
 * @param new_package package where hash is located
 * @param hash specified hash
 * @param identifier identifer of file
 * @param ip peer's IP address
 * @param port peer's port
 * @param new_peer peer to make connection to
 * @param package_head head of packages
 * @param thread_list array of thread IDs
 * @param i index of current thread
 */
void fetch(struct package* new_package, char* hash, char* identifier, char* ip, int port, struct peer* new_peer, pthread_t* thread_list, int i, struct package_head** package_head, struct config_obj* config) {
    struct merkle_tree_node* root = creating_tree(new_package->object);
    struct merkle_tree* tree = create_tree_struct(root, new_package->object);
    struct merkle_tree_node* specified_node = find_hash(root, hash);
    if (specified_node == NULL) {
        printf("Unable to request chunk, chunk hash does not belong to package\n");
        destroyNode(root);
        free(tree);
        return;
    } 
    
    //  sending REQ package with identifer, chunk_hash and offset
    uint32_t offset;
    uint16_t data_len;

    for (int i = 0; i < new_package->object->nchunks; i++) {
        if (strcmp(new_package->object->chunks[i]->hash, hash) == 0) {
            offset = new_package->object->chunks[i]->offset;
            data_len = new_package->object->chunks[i]->size;
            break;
        }
    }
    destroyNode(root);
    free(tree);


    // creating REQ packet before sending it
    struct btide_packet* packet = create_REQ_packet(hash, identifier, offset, data_len);
    new_peer->packet = packet;
    size_t buffer_size = sizeof(struct btide_packet) + PAYLOAD_MAX;
    uint8_t *buffer = (uint8_t *)malloc(buffer_size);

    serialise_packet(packet, buffer);

    struct sockaddr_in serv_addr;
    int sock = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        free(buffer);
        free_packet(packet);
        return;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);


    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        free(buffer);
        free_packet(packet);
        return;
    }


    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Unable to connect to request peer");
        close(sock);
        free(buffer);
        free_packet(packet);
        return;
    }

    ssize_t nwritten = send(sock, buffer, buffer_size, 0);
    if (nwritten <= 0) {
        fprintf(stderr, "could not write entire message to client: socket fd %d\n", sock);
        free(buffer);
        close(sock);
        sock = 0;
        return;
    }

    free(buffer);
    free_packet(packet);

    ssize_t nread;
    uint8_t *response_buffer = receive_data(sock, buffer_size, &nread);


    // receiving RES packet from client

    int bytes_received = 0;
    uint8_t* data_buffer = malloc(data_len);

    if (data_buffer == NULL) {
        free(response_buffer);
        return;
    }

    struct btide_packet *received_packet = NULL;



    while (bytes_received < data_len) {
        if(nread <= 0) {
            free(response_buffer);
            free(data_buffer);
            return;
        }
        received_packet = process_request_packet(response_buffer, nread, sock);

        if (received_packet == NULL) {
            free(data_buffer);
        }


        if (received_packet->msg_code == PKT_MSG_RES) {
            bytes_received += received_packet->pl.res->data_len;

            memcpy(data_buffer + bytes_received - received_packet->pl.res->data_len, received_packet->pl.res->data, received_packet->pl.res->data_len);
            if (bytes_received == data_len) {
                // breaking out of while loop before freeing packet data so can use to open file
                break;
            }
        }

        free_packet(received_packet);
        response_buffer = receive_data(sock, buffer_size, &nread);

    }

    struct package* package = find_packages(received_packet->pl.res->identifier, package_head);
                    
    pthread_mutex_lock(&new_peer->mutex);
    FILE* file = fopen(package->absolute_path, "ab");

    if (file == NULL) {
        printf("Error opening file\n");
        pthread_mutex_unlock(&new_peer->mutex);
        free(data_buffer);
        free_packet(received_packet);
        return;
    }

    if (fseek(file, received_packet->pl.res->offset, SEEK_SET) != 0) {
        printf("Seek error\n");
        fclose(file);
        free(data_buffer);
        free_packet(received_packet);
        pthread_mutex_unlock(&new_peer->mutex);
        return;
    }

    size_t bytes_written = fwrite(data_buffer, 1, data_len, file);

    if (bytes_written != data_len) {
        perror("Error writing to file");
        fclose(file);
        free(data_buffer);
        pthread_mutex_unlock(&new_peer->mutex);
        free_packet(received_packet);
        return;
    }

    // clearing up resources
    fclose(file);
    pthread_mutex_unlock(&new_peer->mutex);
    free_packet(received_packet);
    free(data_buffer);   
}

