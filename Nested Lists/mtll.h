#ifndef MTLL_H
#define MTLL_H
#include <stddef.h>

#define MAX_LINE_LENGTH 129

enum dataType {
    INT,
    FLOAT,
    CHAR,
    STRING,
    SCIENTIFIC,
    STRUCT
};

struct node
{
    void *data;
    enum dataType type;
    struct node *next;
    int index;
    int list_index;
};

struct all_lists
{
    struct node *head;
    struct all_lists* next;
    int list_index;    
    int is_nested;
    int in_nest;
};

int handle_new(char command[MAX_LINE_LENGTH]);

int handle_view(char command[MAX_LINE_LENGTH], int size, struct all_lists *head);

int handle_type(char command[MAX_LINE_LENGTH], int size, struct all_lists *head);

int handle_remove(char command[MAX_LINE_LENGTH], int size, struct all_lists *head);

int handle_insert(char command[MAX_LINE_LENGTH], char command2[MAX_LINE_LENGTH], int size, struct all_lists *head);

int handle_delete(char command[MAX_LINE_LENGTH], char command2[MAX_LINE_LENGTH], int size, struct all_lists *head);

int handle_view_nested(char command[MAX_LINE_LENGTH], int size, struct all_lists *head);

#endif