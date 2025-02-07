#include "mtll.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


// function to free list of nodes
void free_list(struct node *head) {
    struct node *current = head;

    // freeing nodes and their data
    while (current != NULL) {
        struct node *next = current->next;
        if (current->type != STRUCT && current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }
}

// function to free list of structs
void free_all_lists(struct all_lists *head) {
    struct all_lists *current = head;

    // calling free list for each node
    while (current != NULL) {
        struct all_lists *next = current->next;
        free_list(current->head);
        free(current); 
        current = next;
    }
}

// function to add node to end of list
struct node *add_to_end(struct node *current_head, struct node *new_node) {
    struct node *p;
    
    // if is first list, set as head
    if (current_head == NULL) {
        return new_node;
    }

    // iterating to end of list
    for (p = current_head; p->next != NULL; p = p->next);

    p->next = new_node;
    
    return current_head;
}

// function to add new list to end of all lists
struct all_lists *add_to_end_all_lists(struct all_lists *current_head, struct all_lists *new_node) {
    struct all_lists *p;
    
    // if is first list, set as head
    if (current_head == NULL) {
        return new_node;
    }

    // iterate to end of list
    for (p = current_head; p->next != NULL; p = p->next);

    p->next = new_node;
    new_node->next = NULL;
    
    return current_head;
}

// function to print out list
void print_list(struct node *head, int index) {
    struct node *current = head;

    // if list is empty, print a new space
    if (current == NULL) {
        printf("\n");
        return;
    }

    // Iterate through list and print each element based on its type
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
                printf("{List %d}", ((struct all_lists*)(current->data))->list_index);
            default:
                break;
            }
        if (current->next != NULL) {
            printf(" -> ");
        } 
        // printing new line if not part of nested list
        else if (index == 0) {
            printf("\n");
        }
        current = current->next;
    }
    return;
}


// function to print out types
void print_type(struct node *head) {
    struct node *current = head;
    // Iterate through list and print each element

    while (current != NULL) {
        switch (current->type) {
        case INT:
            printf("int");
            break;
        case FLOAT:
            printf("float");
            break;
        case CHAR:
            printf("char");
            break;
        case STRING:
            printf("string");
            break;
        case STRUCT:
            printf("reference");
            break;
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

// function to print all lists
void print_all_lists(struct all_lists *head, int size) {
    struct all_lists *current = head;
    int number_of_lists = 0;

    // find total number of lists
    while (current != NULL) {
        number_of_lists ++;
        current = current->next;   
    }
    
    printf("Number of lists: %d\n", number_of_lists);

    struct all_lists *current_list = head;
    while (current_list != NULL) {
        if (is_nested(current_list) == 0) {
            printf("List %d\n", current_list->list_index);
        } else {
            printf("Nested %d\n", current_list->list_index);
        }
        current_list = current_list->next;   
    }
}

// function to determine type of input typing
enum dataType determine_type(char input[MAX_LINE_LENGTH]) {
    int a = 0;
    int is_float = 0;
    int num;
    char character;
    char int_check;
    double value;

    // checking if inputted a float
    while (input[a] != '\0') {
        if (input[a] == '.') {
            is_float = 1;
            break;
        }
        a++;
    }

    if (is_float == 1) {
        return FLOAT;
    }

    else if (sscanf(input, "%d%c", &num, &int_check) == 2 && int_check == '\n') {
        return INT;
    }
    
    // checking for scientific notation input
    else if (sscanf(input, "%le%c", &value, &int_check) == 2 && int_check == '\n') {
        return SCIENTIFIC;
    }

    // checking if inputted a character
    else if (sscanf(input, "%c\n", &character) == 1) {
        if (strlen(input) != 2) {
            return STRING;
        } else if(isprint(character)) {
            return CHAR;
        }
    }
    else {
        return STRING;
    }
    // for cases of empty strings or unusual input, return string
    return STRING;
}


// function to determine if a list exists for a nested function
struct all_lists* valid_existence(int index, struct all_lists *head) {
    struct all_lists *current = head;

    // iterating through to check list indexes
    while (current != NULL) {
        if (current->list_index == index) {
            return current;
        }
        current = current->next; 
        
    }

    // if have not found any, return null
    if (current == NULL) {
        return NULL;
    }
    return NULL;
}

// function to create a new node
struct node *create_node(char input[MAX_LINE_LENGTH], int index, int list_index, struct node *next) {
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    int num;
    float num1;
    char character;
    char string[MAX_LINE_LENGTH];
    double value;
    int spaces = 0;
    char *spaces_data;

    // checking if memory allocation fails
    if (new_node == NULL) {
        free(new_node);
    }

    // allocating types
    new_node->next = next;
    new_node->index = index;
    new_node->list_index = list_index;
    enum dataType return_value = determine_type(input);

    // assign node type and data based on data type
    switch (return_value) {
        case FLOAT:
            sscanf(input, "%f\n", &num1);
            new_node->data = malloc(sizeof(float));
            *((float*)(new_node->data)) = num1;
            new_node->type = FLOAT;
            break;
                
        case SCIENTIFIC:
            sscanf(input, "%le\n", &value);
            float float_value = (float)(value);
            new_node->data = malloc(sizeof(float));
            *((float*)(new_node->data)) = float_value;
            new_node->type = FLOAT;
            break;
                
        case INT:
            sscanf(input, "%d\n", &num);
            new_node->data = malloc(sizeof(int));
            new_node->type = INT;
            *((int*)(new_node->data)) = num;
            break;
                
        case STRING:
            sscanf(input, "%s\n", string);
            // checking for only spaces

            if (strlen(input) == 0) {
                sscanf(input, "%*s%n", &spaces);
                spaces_data = (char *)malloc(spaces + 1);
                for (int i = 0; i < spaces; i++) {
                    spaces_data[i] = ' ';
                }
                spaces_data[spaces] = '\0';
                strcpy(string, spaces_data);
                free(spaces_data);

            } else {
                strcpy(string, input);
            }

            string[strcspn(string, "\n")] = '\0';
            new_node->data = malloc(strlen(string) + 1);
            if (new_node->data == NULL) {
                free(new_node);
                return NULL;
            }
            
            strcpy((char*)(new_node->data), string);
            new_node->type = STRING;
            break;
                
        case CHAR:
            sscanf(input, "%c\n", &character);
            new_node->data = malloc(sizeof(char));
            *((char*)(new_node->data)) = character;
            new_node->type = CHAR;
            break;
                
        default:
            break;
    }
    return new_node;
}
