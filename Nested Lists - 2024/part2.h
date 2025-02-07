#ifndef PART2_H
#define PART2_H
#include <stddef.h>

#define MAX_LINE_LENGTH 129

int insert(struct all_lists *head, int list, int index, void *value);

int delete(struct all_lists *head, int list, int index);

#endif