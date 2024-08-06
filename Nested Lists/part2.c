#include "mtll.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// function to insert node
int insert(struct all_lists *head, int list, int index, void *value) {
    int reference = 0;
    struct all_lists *current = head;

    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(value)+2);
    strcpy(new_value, value);
    strcat(new_value, "\n");

    // iterating through to find correct list
    while (current != NULL) {
        if (current->list_index == list) {

            // finding length of list incase of negative inputs
            int size = 0;
            struct node* size_track = current->head;

            while (size_track != NULL) {
                size ++;
                size_track = size_track->next;
            }
            free(size_track);

            if (index < 0) {
                index = (size + index+1);
            }

            struct node *correct_list = current->head;
            struct node *previous = NULL;
            struct node* new_node;

            // checking list is valid to insert if inserting nested node
            if (sscanf(value, "{%d}\n", &reference) == 1) {
                struct all_lists *referenced_list = valid_existence(reference, head);
                if (referenced_list == NULL) {
                    free(new_value);
                    return 1;
                }
                
                if (referenced_list->is_nested > 0 || current->in_nest > 0) {
                    free(new_value);
                    return 1;
                }

                // creating new nested node
                if (referenced_list != NULL) {
                    new_node = (struct node*)malloc(sizeof(struct node));
                    new_node->data = referenced_list;
                    new_node->type = STRUCT;
                    new_node->index = index;
                    new_node->list_index = list;
                    referenced_list->in_nest++;
                }
            } 

            // else create new node
            else {
                new_node = create_node(new_value, index, list, correct_list);
            }


            while (correct_list != NULL && correct_list->index < index) {
                previous = correct_list;
                correct_list = correct_list->next;
            }

            // inserting at beginning of list
            if (previous == NULL) {
                current->head = new_node;
            }
            else {
                previous->next = new_node;
            }

            new_node->next = correct_list;

            // changing index value of nodes in list
            struct node *node_after_new = new_node->next;

            while (node_after_new != NULL) {
                node_after_new->index ++;

                node_after_new = node_after_new->next;
            }

            // updating nested values
            if(is_nested(current) == 1) {
                current->is_nested++;
                printf("Nested %d: ", list);
            } else {
                printf("List %d: ", list);
            }
            free(new_value);
            return 0;

        }
        current = current->next;   
    }
    free(new_value);
    return 1;
}

// function to delete node
int delete(struct all_lists *head, int list, int index) {
    if (head == NULL) {
        return 1;
    }

    struct all_lists *current = head;
    struct node *previous = NULL;
    struct all_lists *previous_list = NULL;
    struct node *node_to_delete = NULL;
    struct node *node_after_deletion = NULL;

    // iterating through to find current list
    while (current != NULL) {
        if (current->list_index == list) {
            // finding length of list incase of negative inputs
            int size = 0;
            struct node* size_track = current->head;
            struct node *node = current->head;

            while (size_track != NULL) {
                size ++;
                size_track = size_track->next;
            }

            if (index < 0) {
                index = (size + index);
            }
            while (node != NULL) {
                if (node->index == index) {
                    // checking if deleting first node
                    if (previous == NULL) {
                        if (previous_list == NULL) {
                        }
                        current->head = node->next;
                    } else {
                        previous->next = node->next;
                    }
                    node_to_delete = node;
                    break;
                }
                previous = node;
                node = node->next;
            }
            break;
        }
        previous_list = current;
        current = current->next;
    }

    // if node was not found
    if (node_to_delete == NULL) {
        return 1;
    }
    node_after_deletion = node_to_delete->next;

    // updating linked list
    while (node_after_deletion != NULL) {
        node_after_deletion->index --;
        node_after_deletion = node_after_deletion->next;
    }

    // deleting node
    if (node_to_delete->type == STRUCT) {
        current->is_nested--;
        struct all_lists *new_list = head;
        while (new_list != NULL) {
            if (new_list->list_index == node_to_delete->list_index) {
                new_list->in_nest--;
            }
            new_list = new_list->next;
        }
    }

    if(is_nested(current) == 1) {
        printf("Nested %d: ", list);
    } else {
        printf("List %d: ", list);
    }
    node_to_delete->next = NULL;
    free_list(node_to_delete);
    return 0;
}

