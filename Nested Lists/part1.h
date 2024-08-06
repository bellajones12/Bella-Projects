#ifndef PART1_H
#define PART1_H
#include <stddef.h>

#define MAX_LINE_LENGTH 129

struct node* new(int index, int list_index, struct all_lists *head);

int type(int index, struct all_lists *head);

void view_all(struct all_lists *head, int size);

int view(int index, struct all_lists *head);

int rem(int index, struct all_lists **head);

#endif