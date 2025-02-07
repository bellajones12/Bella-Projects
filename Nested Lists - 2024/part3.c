#include "mtll.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// function to view nested lists
int view_nested(int index, struct all_lists *head) {
    struct all_lists *current = head;

    // iterating through to find list
    while (current != NULL) {
        if (current->list_index == index) {
            print_nested_list(current->head);
            break;
        }
        current = current->next; 
        
          
    }

    // no list found
    if (current == NULL) {
        printf("INVALID COMMAND: VIEW\n");
        return 1;
    }
    return 0;
}

// function to print nested lists
void print_nested_list(struct node *head) {
    struct node *current = head;

    // Iterate through list and print each element
    if (current == NULL) {
        printf("\n");
        return;
    }
    while (current != NULL) {
        switch (current->type) {
        case INT:
            printf("%d", *((int*)((current->data))));
            break;
        case FLOAT:
            printf("%.2f", *((float*)((current->data))));
            break;
        case CHAR:
            printf("%c", *((char*)((current->data))));
            break;
        case STRING:
            printf("%s", (char*)(current->data));
            break;
        case STRUCT:
            printf("{");
            // printing data of nested list
            print_list(((struct all_lists*)current->data)->head, 1);
            printf("}");
        default:
            break;
        }
        if (current->next != NULL) {
            printf(" -> ");
        } else {
            printf("\n");
        }
        current = current->next;
    }
}

// function to determine if list is nested
int is_nested(struct all_lists *head) {
    struct node *current = head->head;

    while (current != NULL) {
        if (current->type == STRUCT) {
            return 1;
        }
        current = current->next; 
    }

    return 0;
}
