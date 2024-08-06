#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include "net/client.h"
#include "net/peer.h"
#include <pthread.h>
#include "net/packet.h"

#define MAX_CLIENTS (10)


/**
 * Establishing new connection to server.
 *
 * @param arg peer structure containing necessary information for connection including port and ip address
 */
void* connect_client(void* arg) {
    struct peer* data = (struct peer*)arg;
    
    int port = data->port;
    struct sockaddr_in serv_addr;
    int sock = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return NULL;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);


    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, data->ip_address, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return NULL;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        // perror("Unable to connect to request peer");
        close(sock);
        return NULL;
    }


    // read message
    size_t buffer_size = sizeof(struct btide_packet) + PAYLOAD_MAX;
    ssize_t nread;
    uint8_t *buffer = receive_data(sock, buffer_size, &nread);

    struct btide_packet *new_packet = process_request_packet( buffer, nread, sock);


    // first check if any packets were sent
    // checking correct packet was sent (0x02 is 2 in decimal)
    if (new_packet->msg_code == PKT_MSG_ACP) {

        pthread_mutex_lock(&data->mutex);
        data->result = 0;
        data->socket = sock;

        // received ACP packet, now needs to send ACK back to server
        first_connection(buffer_size, sock);
        pthread_mutex_unlock(&data->mutex);
        free_packet(new_packet);
    }

    return NULL;
}



/**
 * Function to create and send an ACK packet to initial connections
 *
 * @param buffer_size size of buffer to send to server
 * @param sock socket of client connection
 */
void first_connection(size_t buffer_size, int sock) {
    
    // creating ACK packet
    struct btide_packet* next_packet = create_ACK_packet();

    // serialising packet in order to send
    uint8_t *new_buffer = (uint8_t *)malloc(buffer_size);
    serialise_packet(next_packet, new_buffer);
    free_packet(next_packet);

    ssize_t nwritten = send(sock, new_buffer, buffer_size, 0);
    if (nwritten <= 0) {
        fprintf(stderr, "could not write entire message to client: socket fd %d\n", sock);
        free(new_buffer);
        close(sock);
        sock = 0;
        return;;
    }
    free(new_buffer);
    return;
}