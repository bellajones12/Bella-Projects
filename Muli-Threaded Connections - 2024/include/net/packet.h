#ifndef NETPKT_H
#define NETPKT_H

#include <string.h>
#include <stdlib.h>

#include "chk/pkgchk.h"

#define PAYLOAD_MAX (4092)

#define PKT_MSG_ACK 0x0c
#define PKT_MSG_ACP 0x02
#define PKT_MSG_DSN 0x03
#define PKT_MSG_REQ 0x06
#define PKT_MSG_RES 0x07
#define PKT_MSG_PNG 0xFF
#define PKT_MSG_POG 0x00


union btide_payload {
    uint8_t data[PAYLOAD_MAX];
    struct btide_req *req;
    struct btide_res *res;

};

struct btide_req {
    uint32_t offset;
    char hash[HASH_MAX_SIZE];
    char identifier[IDENT_MAX_SIZE];
    uint16_t data_len;
};

struct btide_res {
    uint32_t offset;
    char hash[HASH_MAX_SIZE];
    char identifier[IDENT_MAX_SIZE];
    uint8_t *data;
    uint16_t data_len;
};

struct btide_packet {
    uint16_t msg_code;
    uint16_t error;
    union btide_payload pl;
};

void serialise_packet(struct btide_packet *packet, uint8_t *buffer);

void deserialise_packet(uint8_t *buffer, struct btide_packet *packet);

struct btide_packet* create_ACP_packet();

void free_packet(struct btide_packet *packet);

struct btide_packet* create_ACK_packet();

struct btide_packet* create_REQ_packet(char* hash, char* identifier, uint32_t offset, uint16_t data_len);

struct btide_packet* create_RES_packet(char* hash, char* identifier, uint32_t offset, uint8_t* data, uint16_t data_len);

#endif
