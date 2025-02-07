#include "mtll.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 129

int main() {
    int size = 0;
    char input[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    char str1[MAX_LINE_LENGTH];
    char str2[MAX_LINE_LENGTH];
    int i = 0;
    struct all_lists *head1 = NULL;

    while (fgets(input, sizeof(input), stdin) != NULL) {
        // checking if EOF reached
        if (feof(stdin)) {
            free_all_lists(head1);
            return 0;
        } 

        // checking if has leading whitespaces
        if (isspace(input[0])) {
            printf("INVALID COMMAND: INPUT\n");
            continue;
        }

        // checking view all command
        else if (strcmp(input, "VIEW ALL\n") == 0) {
            view_all(head1, size);
            continue;
        }

        // checking  other commands
        else if (sscanf(input, "%s %s %[^\n]", command, str1, str2) == 3 || sscanf(input, "%s %s\n", command, str1) == 2) {
            // checking view command
            if (!(strcmp(command, "VIEW"))) {
                if (handle_view(str1, size, head1) == 1){
                    printf("INVALID COMMAND: VIEW\n");
                }
                continue;
            } 

            // checking type command
            else if (!(strcmp(command, "TYPE"))) {
                if (handle_type(str1, size, head1) == 1){
                    printf("INVALID COMMAND: TYPE\n");
                }
                continue;
        }

            // checking remove command
            else if (!(strcmp(command, "REMOVE"))) {
                if (handle_remove(str1, size, head1) == 1){
                    printf("INVALID COMMAND: REMOVE\n");
                } else {
                    int num = atoi(str1);

                    // checking if remove was successful
                    if (rem(num, &head1) == 0) {
                        size --;
                        view_all(head1, size);
                    } else {
                        printf("INVALID COMMAND: REMOVE\n");
                    }
                }
                continue;
            }

            // checking view nested command
            else if (!(strcmp(command, "VIEW-NESTED"))) {
                if (handle_view_nested(str1, size, head1) == 1){
                    printf("INVALID COMMAND: VIEW-NESTED\n");
                }
                continue;
            }
            
            // checking new command
            else if (!(strcmp(command, "NEW"))) {
                if (handle_new(str1) == 1){
                    printf("INVALID COMMAND: NEW\n");
                } else {
                    int num = atoi(str1);
                    struct node* new_value = new(num, i, head1);
                    if (new_value == NULL && num != 0) {
                        continue;
                    }

                    // creating new node
                    struct all_lists* new_node = (struct all_lists*)malloc(sizeof(struct all_lists));
                    new_node->head = new_value;
                    new_node->next = NULL;
                    new_node->list_index = i;
                    new_node->in_nest = 0;

                    // updating nested values
                    if (is_nested(new_node) == 1) {
                        new_node->is_nested = 1;
                    } else {
                        new_node->is_nested = 0;
                    }

                    // appending new node to all lists
                    head1 = add_to_end_all_lists(head1, new_node);
                    i ++;
                    size ++;
                }
                continue;
            }

            // checking insert command
            else if (!(strcmp(command, "INSERT"))) {
                if (handle_insert(str1, str2, size, head1) == 1){
                    printf("INVALID COMMAND: INSERT\n");
                } else {
                    int list_insert = atoi(str1);
                    view(list_insert, head1);
                }
                continue;
            }

            // checking delete command
            else if (!(strcmp(command, "DELETE"))) {
                if (handle_delete(str1, str2, size, head1) == 1){
                    printf("INVALID COMMAND: DELETE\n");
                } else {
                    int list = atoi(str1);
                    view(list, head1);
                }
                continue;
            }    

        }
        // invalid inputs
        sscanf(input, "%s ", command);
        if (!(strcmp(command, "view"))) {
            printf("INVALID COMMAND: VIEW\n");
            continue;
        } 

        else if (!(strcmp(command, "new"))) {
            printf("INVALID COMMAND: NEW\n");
            continue;
        }

        else if (!(strcmp(command, "type"))) {
            printf("INVALID COMMAND: TYPE\n");
            continue;
        }

        else if (!(strcmp(command, "insert"))) {
            printf("INVALID COMMAND: INSERT\n");
            continue;
        }

        else if (!(strcmp(command, "delete"))) {
            printf("INVALID COMMAND: DELETE\n");
            continue;
        }


        if (!(strcmp(command, "REMOVE"))) {
            printf("INVALID COMMAND: REMOVE\n");
        } else if (!(strcmp(command, "NEW"))) {
            printf("INVALID COMMAND: NEW\n");
        } else if (!(strcmp(command, "TYPE"))) {
            printf("INVALID COMMAND: TYPE\n");
        } else if (!(strcmp(command, "INSERT"))) {
            printf("INVALID COMMAND: INSERT\n");
        } else if (!(strcmp(command, "DELETE"))) {
            printf("INVALID COMMAND: DELETE\n");
        }else if (!(strcmp(command, "VIEW"))) {
            printf("INVALID COMMAND: VIEW\n");
        }else {
            printf("INVALID COMMAND: INPUT\n");
        }
 
    }  
    // freeing all data
    free_all_lists(head1);
     
    return 0;
}