#ifndef PART3_H
#define PART3_H
#include <stddef.h>

#define MAX_LINE_LENGTH 129

void delete_nested(struct node *node);

int view_nested(int index, struct all_lists *head);

int is_nested(struct all_lists *head);

#endif