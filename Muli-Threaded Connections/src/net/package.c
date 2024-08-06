#include "net/package.h"


/**
 * adds new package node to linked list struct
 *
 * @param current_head  current head of package list
 * @param new_node new package to be added
 * @return returns head of list after insertion
 */
struct package *add_package(struct package_head *current_head, struct package *new_node) {
    struct package *temp = current_head->head;
    
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
 * Creating new package and adds to linked list
 *
 * @param filename of the package.
 * @param next pointer to next package node in list
 * @param config config_object struct
 * @return returns new package node
 */
struct package *create_new_package(const char* filename, struct package* next, struct config_obj* config) {
    struct package *new_package = (struct package*)malloc(sizeof(struct package));
    
    // checking if memory allocation was successful
    if (new_package == NULL) {
        free(new_package);
        return NULL;
    }


    new_package->object = bpkg_load(filename);
    if (new_package->object == NULL) {
        printf("Unable to parse bpkg file.\n");
        bpkg_obj_destroy(new_package->object);
        free(new_package);
    }

    // creating absolute path based on directory in config file
    char *absolute_path = malloc(MAX_INPUT_CHARACTERS);
    snprintf(absolute_path, MAX_INPUT_CHARACTERS, "%s/%s", config->directory, new_package->object->filename);
    new_package->absolute_path = absolute_path;

    new_package->next = next;
    return new_package;
}


/**
 * Removes package based on identifier
 *
 * @param ident identifier of package to remove
 * @param head head of the package list
 * @return int rerpesenting success of removal
 */
int remove_package(char* ident, struct package_head** head) {
    struct package *temp = (*head)->head;
    struct package *prev = NULL;

    // if are removing head node, need to change head
    if (strcmp((temp->object)->ident, ident) == 0) {
        (*head)->head = temp->next;
        bpkg_obj_destroy(temp->object);
        free_package(temp);
        return 0;
    }

    // iterating through list to find specified node
    while (temp != NULL && strcmp((temp->object)->ident, ident) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // package was not found
    if (temp == NULL) {
        return 1;
    }

    prev->next = temp->next;
    bpkg_obj_destroy(temp->object);
    free_package(temp);
    return 0;
}



/**
 * Prints information about all packages
 *
 * @param head head of the package list
 * @param config config_object containing configuration data.
 */
void print_all_packages(struct package_head* current_head, struct config_obj* config) {
    // checking if are any packages in list
    struct package* head = current_head->head;
    if (head == NULL) {
        printf("No packages managed\n");
        return;
    }
    int i = 1;

    // iterating through linked list
    while (head != NULL) {
        strcpy(head->object->filename, head->absolute_path);
        // checking if package is complete
        if (return_completed_hash(head->object) == 0) {
            printf("%d. %.32s, %s : COMPLETED\n", i, (head->object)->ident, head->absolute_path);
        } else {
            printf("%d. %.32s, %s : INCOMPLETE\n", i, (head->object)->ident, head->absolute_path);
        }
        head = head->next;
        i++;
    }
}


/**
 * Frees all packages in list
 *
 * @param head head of pacakge list
 */
void free_all_packages(struct package_head *head) {
    struct package *current = head->head;

    // iterating through list
    while (current != NULL) {
        struct package *next = current->next;
        // freeing all memory allocation
        bpkg_obj_destroy(current->object);
        free_package(current); 
        current = next;
    }
}

/**
 * Free memory for single package
 *
 * @param node package to be freed.
 */
void free_package(struct package *node) {
    free(node->absolute_path);
    free(node);
}


/**
 * Finds packages matching a specific identifier in the list.
 *
 * @param ident A character pointer containing the identifier to search for.
 * @param head A pointer to the head of the package list.
 * @return matching package node
 */
struct package* find_packages(char* ident, struct package_head** head) {
    struct package *temp = (*head)->head;

    while (temp != NULL && strcmp(temp->object->ident, ident) != 0) {
        printf("temp: %s: ident: %s\n", temp->object->ident, ident);
        temp = temp->next;
    }

    return temp;
}