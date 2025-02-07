#include "mtll.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// function to handle view inputs
int handle_view(char command[MAX_LINE_LENGTH], int size, struct all_lists *head) {
    enum dataType return_value;

    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(command)+2);
    strcpy(new_value, command);
    strcat(new_value, "\n");

    return_value = determine_type(new_value);
    free(new_value);

    // checking if inputted int, then call view function
    switch (return_value) {
        case INT:
            int num = atoi(command);
            if (num < 0) {
                num = size + num;
            }
            view(num, head);
            return 0;
            default:
                return 1;
            break;
    }
    return 1;
}

// function to handle type inputs
int handle_type(char command[MAX_LINE_LENGTH], int size, struct all_lists *head) {
    enum dataType return_value;

    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(command)+2);
    strcpy(new_value, command);
    strcat(new_value, "\n");

    return_value = determine_type(new_value);
    free(new_value);

    // checking if inputted int, then call view function
    switch (return_value) {
        case INT:
            int num = atoi(command);
            if (num < 0) {
                num = size + num;
            }
            type(num, head);
            return 0;
            default:
                return 1;
            break;
    }
    return 1;
}

// function to handle remove inputs
int handle_remove(char command[MAX_LINE_LENGTH], int size, struct all_lists *head) {
    enum dataType return_value;
    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(command)+2);
    struct all_lists *current = head;
    strcpy(new_value, command);
    strcat(new_value, "\n");
    return_value = determine_type(new_value);

    // checking if inputted int, then call remove function
    switch (return_value) {
        case INT:
            while (current != NULL) {
                if (current->list_index == atoi(new_value)) {

                    // checking if list is nested, then cannot remove
                    if (current->in_nest > 0) {
                        free(new_value);
                        return 1;
                    }
                }
                current = current->next; 
                
            }
            free(new_value);
            return 0;
        default:
            free(new_value);
            return 1;
    }
    return 1;
}

// function to handle new inputs
int handle_new(char command[MAX_LINE_LENGTH]) {
    enum dataType return_value;

    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(command)+2);
    strcpy(new_value, command);
    strcat(new_value, "\n");

    return_value = determine_type(new_value);
    free(new_value);

    // checking if inputted int, then call new function
    switch (return_value) {
        case INT:
            int num = atoi(command);
            if (num < 0) {
                return 1;
            }
            return 0;
            default:
                return 1;
            break;
    }
    return 1;
}

// function to handle insert inputs
int handle_insert(char command[MAX_LINE_LENGTH], char command2[MAX_LINE_LENGTH], int size, struct all_lists *head) {
    enum dataType return_value;
    int list;
    int index;
    char str1[MAX_LINE_LENGTH];
    char str2[MAX_LINE_LENGTH];
    char *space;
    char *new_string;

    // // checking first input

    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(command)+2);
    strcpy(new_value, command);
    strcat(new_value, "\n");

    return_value = determine_type(new_value);
    free(new_value);

    // checking if inputted an int
    switch (return_value) {
        case INT:
            list = atoi(command);
            if (list < 0) {
                list = size + list;
            }
            break;
        default:
            return 1;
        break;
    }

    // //checking second input

    // checking if inputted empty string
    int num_words =  sscanf(command2, "%s %[^\n]", str1, str2);

    char *new_value2 = malloc(strlen(str1)+2);
    strcpy(new_value2, str1);
    strcat(new_value2, "\n");

    return_value = determine_type(new_value2);
    free(new_value2);

    // checking if inputted an int
    switch (return_value) {
        case INT:
            index = atoi(str1);
            break;
        default:
            return 1;
        break;
    }


    if (num_words != 1) {
        command2 = str2;
    } else if (strlen(command2) != 1) {
        space = strchr(command2, ' ');

        size_t remaining_length = strlen(space + 1);
        new_string = malloc(remaining_length+1);

        strcpy(new_string, space + 1);
        int result = insert(head, list, index, new_string);
        free(new_string);
        return result;
    }

    if (num_words == 1 && strlen(command2) == 1) {
        new_string = malloc(1);
        new_string[0] = '\0';

        int result = insert(head, list, index, new_string);
        free(new_string);
        return result;
    }

    return insert(head, list, index, command2);
}

// function to handle delete inputs
int handle_delete(char command[MAX_LINE_LENGTH], char command2[MAX_LINE_LENGTH], int size, struct all_lists *head) {
    enum dataType return_value;

    // // checking first input

    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(command)+2);
    strcpy(new_value, command);
    strcat(new_value, "\n");
    int list;
    int index;
    
    return_value = determine_type(new_value);
    free(new_value);

    // checking if inputted int
    switch (return_value) {
        case INT:
            list = atoi(command);
            if (list < 0) {
                list = size + list;
            }
            break;
        default:
            return 1;
        break;
    }
    
    // // checking first input

    // concatenating with a newline so can use determineType function correctly

    char *new_value2 = malloc(strlen(command2)+2);
    strcpy(new_value2, command2);
    strcat(new_value2, "\n");

    return_value = determine_type(new_value2);
    free(new_value2);

    // checking if inputted int
    switch (return_value) {
        case INT:
            index = atoi(command2);
            break;
        default:
            return 1;
        break;
    }

    if (delete(head, list, index) == 0){
        return 0;
    }

    return 1;
}

// function to handle view_nested
int handle_view_nested(char command[MAX_LINE_LENGTH], int size, struct all_lists *head) {
    enum dataType return_value;

    // concatenating with a newline so can use determineType function correctly
    char *new_value = malloc(strlen(command)+2);
    strcpy(new_value, command);
    strcat(new_value, "\n");

    return_value = determine_type(new_value);
    free(new_value);

    // checking if inputted int
    switch (return_value) {
        case INT:
            int num = atoi(command);
            if (num < 0) {
                num = size + num;
            }
            view_nested(num, head);
            return 0;
            default:
                return 1;
            break;
    }
    return 1;
}