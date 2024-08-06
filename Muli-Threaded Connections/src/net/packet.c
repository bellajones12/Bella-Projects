#include <string.h>
#include <stdint.h>

#include "net/packet.h"



/**
 * Serializes a btide_packet into a byte buffer
 *
 * @param packet btide_packet to serialize
 * @param buffer byte buffer where serialized data will be written
 */
void serialise_packet(struct btide_packet *packet, uint8_t *buffer) {
    memcpy(buffer, &packet->msg_code, sizeof(packet->msg_code));
    memcpy(buffer + sizeof(packet->msg_code), &packet->error, sizeof(packet->error));

    // serialising for REQ packet
    if (packet->msg_code == PKT_MSG_REQ) {
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error), &packet->pl.req->offset, sizeof(packet->pl.req->offset));
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error) + sizeof(packet->pl.req->offset), packet->pl.req->hash, sizeof(packet->pl.req->hash));
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error) + sizeof(packet->pl.req->offset) + sizeof(packet->pl.req->hash), packet->pl.req->identifier, sizeof(packet->pl.req->identifier));
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error) + sizeof(packet->pl.req->offset) + sizeof(packet->pl.req->hash) + sizeof(packet->pl.req->identifier), &(packet->pl.req->data_len), sizeof(packet->pl.req->data_len));
    } 
    
    // serialising for RES packet
    else if (packet->msg_code == PKT_MSG_RES) {
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error), &packet->pl.res->offset, sizeof(packet->pl.res->offset));
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error) + sizeof(packet->pl.res->offset), packet->pl.res->hash, sizeof(packet->pl.res->hash));
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error) + sizeof(packet->pl.res->offset) + sizeof(packet->pl.res->hash), packet->pl.res->identifier, sizeof(packet->pl.res->identifier));
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error) + sizeof(packet->pl.res->offset) + sizeof(packet->pl.res->hash) + sizeof(packet->pl.res->identifier), &(packet->pl.res->data_len), sizeof(packet->pl.res->data_len));
        memcpy(buffer + sizeof(packet->msg_code) + sizeof(packet->error) + sizeof(packet->pl.res->offset) + sizeof(packet->pl.res->hash) + sizeof(packet->pl.res->identifier) + sizeof(packet->pl.res->data_len), (packet->pl.res->data), sizeof(packet->pl.res->data_len));
  
    } 

    // serialising for other packets
    else {
        memcpy(packet->pl.data, buffer + sizeof(packet->msg_code) + sizeof(packet->error), PAYLOAD_MAX);
    }
}


/**
 * Deserializes btide_packet from a byte buffer
 *
 * @param buffer byte buffer containing serialized packet data
 * @param packet btide_packet struct where deserialised data will be written to
 */
void deserialise_packet(uint8_t *buffer, struct btide_packet *packet) {
    size_t offset = 0;

    // Copy msg_code
    memcpy(&packet->msg_code, buffer + offset, sizeof(packet->msg_code));
    offset += sizeof(packet->msg_code);

    // Copy error
    memcpy(&packet->error, buffer + offset, sizeof(packet->error));
    offset += sizeof(packet->error);

    // deserialising for REQ packets
    if (packet->msg_code == PKT_MSG_REQ) {
        packet->pl.req = malloc(sizeof(struct btide_req));

        // Copy offset
        memcpy(&packet->pl.req->offset, buffer + offset, sizeof(packet->pl.req->offset));
        offset += sizeof(packet->pl.req->offset);

        // Copy hash
        memcpy(packet->pl.req->hash, buffer + offset, sizeof(packet->pl.req->hash));
        offset += sizeof(packet->pl.req->hash);

        // Copy identifier
        memcpy(packet->pl.req->identifier, buffer + offset, sizeof(packet->pl.req->identifier));
        offset += sizeof(packet->pl.req->identifier);

        // Copy data_len
        memcpy(&packet->pl.req->data_len, buffer + offset, sizeof(packet->pl.req->data_len));
        offset += sizeof(packet->pl.req->data_len);

    } 
    // deserialising for RES packets
    else if (packet->msg_code == PKT_MSG_RES) {
        packet->pl.res = malloc(sizeof(struct btide_res));

        // Copy offset
        memcpy(&packet->pl.res->offset, buffer + offset, sizeof(packet->pl.res->offset));
        offset += sizeof(packet->pl.res->offset);

        // Copy hash
        memcpy(packet->pl.res->hash, buffer + offset, sizeof(packet->pl.res->hash));
        offset += sizeof(packet->pl.res->hash);

        // Copy identifier
        memcpy(packet->pl.res->identifier, buffer + offset, sizeof(packet->pl.res->identifier));
        offset += sizeof(packet->pl.res->identifier);

        // Copy data_len
        memcpy(&packet->pl.res->data_len, buffer + offset, sizeof(packet->pl.res->data_len));
        offset += sizeof(packet->pl.res->data_len);

        // Allocate and copy data
        packet->pl.res->data = malloc(packet->pl.res->data_len);

        memcpy(packet->pl.res->data, buffer + offset, packet->pl.res->data_len);
        offset += packet->pl.res->data_len;

    } 
    // deserialising for other packets
    else {
        memcpy(packet->pl.data, buffer + offset, PAYLOAD_MAX);
    }
}


/**
 * Creates new ACP btide_packet
 *
 * @return newly created btide_packet
 */
struct btide_packet* create_ACP_packet() {
    struct btide_packet *new_packet = malloc(sizeof(struct btide_packet));
    new_packet->msg_code = PKT_MSG_ACP;
    new_packet->error = 0;
    memset(new_packet->pl.data, 0, PAYLOAD_MAX);
    return new_packet;
}


/**
 * Frees specific btide_packet
 *
 * @param packet  btide_packet to be freed
 */
void free_packet(struct btide_packet *packet) {
    if (!packet) {
        return;
    }

    // switch case for specific packets to free necessary memory
    switch (packet->msg_code) {
        case PKT_MSG_RES:
            if (packet->pl.res) {
                free(packet->pl.res->data);
                free(packet->pl.res);
            }
            break;
        case PKT_MSG_REQ:
            if (packet->pl.req) {
                free(packet->pl.req);
            }
            break;
    }
    free(packet);
}


/**
 * Creates a new ACK btide_packet
 *
 * @return ACK btide_packet
 */
struct btide_packet* create_ACK_packet() {
    struct btide_packet *new_packet = malloc(sizeof(struct btide_packet));
    new_packet->msg_code = PKT_MSG_ACK;
    new_packet->error = 0;
    memset(new_packet->pl.data, 0, PAYLOAD_MAX);
    return new_packet;
}



/**
 * Creates a new REQ btide_packet
 *
 * @param hash requested hash
 * @param identifier requested identifier
 * @param offset offset of requested data block
 * @param data_len length of requested data
 * @return REQ btide_packet
 */
struct btide_packet* create_REQ_packet(char* hash, char* identifier, uint32_t offset, uint16_t data_len) {
    struct btide_packet *new_packet = malloc(sizeof(struct btide_packet));
    new_packet->msg_code = PKT_MSG_REQ;
    new_packet->error = 0;

    new_packet->pl.req = malloc(sizeof(struct btide_req));
    new_packet->pl.req->offset = offset;
    new_packet->pl.req->data_len = data_len;

    strncpy(new_packet->pl.req->hash, hash, HASH_MAX_SIZE);
    new_packet->pl.req->hash[HASH_MAX_SIZE-1] = '\0';

    strncpy(new_packet->pl.req->identifier, identifier, IDENT_MAX_SIZE);
    new_packet->pl.req->identifier[IDENT_MAX_SIZE-1] = '\0';

    return new_packet;
}


/**
 * Creates a new REQ btide_packet
 *
 * @param hash requested hash
 * @param identifier requested identifier
 * @param offset offset of requested data block
 * @param data byte data to be send
 * @param data_len length of requested data
 * @return RES btide_packet
 */
struct btide_packet* create_RES_packet(char* hash, char* identifier, uint32_t offset, uint8_t* data, uint16_t data_len) {
    struct btide_packet *new_packet = malloc(sizeof(struct btide_packet));
    new_packet->msg_code = PKT_MSG_RES;
    new_packet->error = 0;

    new_packet->pl.res = malloc(sizeof(struct btide_res));
    new_packet->pl.res->offset = offset;

    strncpy(new_packet->pl.res->hash, hash, HASH_MAX_SIZE);
    new_packet->pl.res->hash[HASH_MAX_SIZE-1] = '\0';

    strncpy(new_packet->pl.res->identifier, identifier, IDENT_MAX_SIZE);
    new_packet->pl.res->identifier[IDENT_MAX_SIZE-1] = '\0';
    
    new_packet->pl.res->data = malloc(data_len);

    new_packet->pl.res->data_len = data_len;

    memcpy(new_packet->pl.res->data, data, data_len);

    return new_packet;
}