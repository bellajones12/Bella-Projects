#include "mtll.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// function to create new list of nodes
struct node * new(int index, int list_index, struct all_lists *all_lists_head) {
    int i = 0;
    float num1;
    char input[MAX_LINE_LENGTH];
    struct node *head = NULL;
    int reference;
    int nested = 0;

    while (i < index) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            free_list(head);
            return NULL;
        }

        if (feof(stdin)) {
            free_all_lists(all_lists_head);
            return NULL;
        }

        // checking if inputted nested list
        if (sscanf(input, "{%d}\n", &reference) == 1) {
            struct all_lists *referenced_list = valid_existence(reference, all_lists_head);
            if (referenced_list == NULL || referenced_list->is_nested > 0) {
                printf("INVALID COMMAND: NEW\n");
                free_list(head);
                return NULL;
            }

            // created nested node
            if (referenced_list != NULL) {
                struct node* new_nested = (struct node*)malloc(sizeof(struct node));
                sscanf(input, "%f\n", &num1);
                new_nested->data = referenced_list;
                new_nested->next = NULL;
                new_nested->type = STRUCT;
                new_nested->index = i;
                referenced_list->in_nest ++;
                new_nested->list_index = referenced_list->list_index;
                head = add_to_end(head, new_nested);
                nested = 1;
            }

        } 
        
        // creating new node
        else {
            struct node* new_node = create_node(input, i, list_index, NULL);
            if (new_node == NULL) {
                free_list(head);
                return NULL;
            }
            head = add_to_end(head, new_node);
        }
        i++;
    }

    // printing newly created list
    if (nested == 0) {
        printf("List %d: ", list_index);
    } else {
        printf("Nested %d: ", list_index);
    }
    print_list(head, 0);
    return head;
}

// function to print type of list
int type(int index, struct all_lists *head) {
    struct all_lists *current = head;

    // iterating through list to find correct list
    while (current != NULL) {
        if (current->list_index == index) {
            print_type(current->head);
            break;
        }
        current = current->next;    
          
    }
    return 0;
}

// function to print all lists
void view_all(struct all_lists *head, int size) {
    print_all_lists(head, size);
    return;
}

// function to print one list
int view(int index, struct all_lists *head) {
    struct all_lists *current = head;

    // iterating through list to find correct list
    while (current != NULL) {
        if (current->list_index == index) {
            print_list(current->head, 0);
            break;
        }
        current = current->next; 
        
          
    }

    // if cannot find list, print null
    if (current == NULL) {
        printf("INVALID COMMAND: VIEW\n");
        return 1;
    }
    return 0;
}

// function to remove a list
int rem(int index, struct all_lists **head) {
    struct all_lists *current = *head;
    struct all_lists *previous = NULL;

    // iterating through to find correct list
    while (current != NULL) {
        if (current->list_index == index) {
            // update if is head of list else, iterate until found
            struct node *current_node = current->head;
            while (current_node != NULL) {
                if (current_node->type == STRUCT) {
                    struct all_lists *current_list = *head;

                    // updating next values
                    while (current_list != NULL) {
                        if (current_list->list_index == ((struct all_lists*)(current_node->data))->list_index) {
                            current_list->in_nest--;
                        }
                        current_list = current_list->next;
                    }
                }
                current_node = current_node->next;
            }

            if (previous == NULL) {
                *head = current->next;
            } else {
                previous->next = current->next;
            }

            // freeing removed list
            free_list(current->head);
            free(current);
            printf("List %d has been removed.\n\n", index);
            return 0;
        }
        previous = current;
        current = current->next; 
    }

    // returning error if list not found
    if (current == NULL) {
        return 1;
    }

    return 1;
}
