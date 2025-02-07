#ifndef HELPER_H
#define HELPER_H
#include <stddef.h>

#define MAX_LINE_LENGTH 129

void free_list(struct node *head);

struct node *add_to_end(struct node *current_head, struct node *new_node);

struct all_lists *add_to_end_all_lists(struct all_lists *current_head, struct all_lists *new_node);

void print_list(struct node *head, int nested);

void print_type(struct node *head);

void print_all_lists(struct all_lists *head2, int size);

enum dataType determine_type(char input[MAX_LINE_LENGTH]);

void free_all_lists(struct all_lists *head);

void printIndex(struct node *head);

struct all_lists* valid_existence(int index, struct all_lists *head);

void print_nested_list(struct node *head);

struct node* create_node(char input[MAX_LINE_LENGTH], int i, int list_index, struct node *next);

#endif