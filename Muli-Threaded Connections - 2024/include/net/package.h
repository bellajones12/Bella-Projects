#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "net/config.h"
#include "chk/pkgchk.h"

#define MAX_INPUT_CHARACTERS (5520)

struct package {
    struct package *next;
    struct bpkg_obj *object;
    char* absolute_path;
};

struct package_head {
    struct package* head;
};


void print_all_packages(struct package_head* head, struct config_obj* config);

int remove_package(char* ident, struct package_head** head);

struct package *create_new_package(const char* filename, struct package* next, struct config_obj* config);

struct package *add_package(struct package_head *current_head, struct package *new_node);

void free_all_packages(struct package_head *head);

struct package* find_packages(char* ident, struct package_head** head);

void free_package(struct package *node);

#endif