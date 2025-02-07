#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define MAX_CHARACTERS (1024)

struct config_obj {
	char directory[MAX_CHARACTERS];
	int max_peers;
	uint16_t port;
};

struct config_obj* load_config(const char* config);

int validate_directory(struct config_obj* object) ;

int validate_port(struct config_obj* object);

int validate_max_peers(struct config_obj* object);

#endif
