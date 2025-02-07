#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include "net/config.h"


/**
 * Loads the configuration data from a file.
 *
 * @param config_file The path to the configuration file.
 * @return A pointer to config_obj struct contianing all necessary information
 */
struct config_obj* load_config(const char* config) {
    struct config_obj* obj = NULL;
    int fd;
    struct stat stat_b;
    char* mapped = NULL;
    if (config != NULL) {
        fd = open(config, O_RDONLY);
        if (fd == -1) {
            perror("open");
            return NULL;
        }
        fstat(fd, &stat_b);
        if (fstat(fd, &stat_b) == -1) {
            perror("fstat");
            return NULL;
        }

        mapped = mmap(NULL, stat_b.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mapped == MAP_FAILED) {
            perror("mmap");
            close(fd);
            return NULL;
        }

        char *ptr = mapped;

        char directory[MAX_CHARACTERS];


        // assigning ident
        char *directory_start = strstr(ptr, "directory:");
        if (directory_start != NULL) {
            directory_start += strlen("directory:");
            char *newline = strchr(directory_start, '\n');
            if (newline != NULL) {
                size_t directory_length = newline - directory_start; 
                strncpy(directory, directory_start, directory_length);
                directory[directory_length] = '\0';
            }
        }
        obj = (struct config_obj*)malloc(sizeof(struct config_obj));
        strcpy(obj->directory, directory);



        // assigning size
        char *max_peers_start = strstr(ptr, "max_peers:");
        if (max_peers_start != NULL) {
            max_peers_start += strlen("max_peers:");

            int max_peers = strtol(max_peers_start, &ptr, 10);
            obj->max_peers = max_peers;
        }

        // assigning nhashes
        char *port_start = strstr(ptr, "port:");
        if (port_start != NULL) {
            port_start += strlen("port:");

            long port_long = strtol(port_start, &ptr, 10);
            obj->port = (size_t)port_long;
        }


    }
        close(fd);

    return obj;
}


/**
 * Validates directory path of config object.
 *
 * @param object config_obj struct
 * @return 0 on success, 3 on error
 */
int validate_directory(struct config_obj* object) {
    // checking if directory exists
    struct stat directory_info;

    if (!(stat(object->directory, &directory_info) == 0 && S_ISDIR(directory_info.st_mode))) {
        // directory does not exist
        if (!(mkdir(object->directory, 0777)) == 0) {
            // unable to create a directory
            return 3;
        }
        if (stat(object->directory, &directory_info) == 0) {
            // directory is a file
            return 3;
        }
    } 

    return 0;
}


/**
 * Validates the maximum number of peers in config object.
 *
 * @param object config_obj struct
 * @return 0 on success, 4 on error
 */
int validate_max_peers(struct config_obj* object) {
    // checking if max-peers is in correct range
    if (object->max_peers < 1 || object->max_peers > 2048) {
        return 4;
    }

    return 0;
}

/**
 * Validates port number in the config object.
 *
 * @param object The config_obj struct
 * @return 0 on success, 5 on error
 */
int validate_port(struct config_obj* object) {
    // checking if port is in correct range
    if (object->port < 1024 || object->port > 65535) {
        return 5;
    }

    return 0;
}