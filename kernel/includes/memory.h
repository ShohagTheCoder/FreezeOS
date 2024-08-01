#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

typedef struct dynamic_mem_node
{
    uint32_t size;
    bool used;
    struct dynamic_mem_node* prev;
    struct dynamic_mem_node* next;
} dynamic_mem_node_t;

void init_dynamic_mem();

void* malloc(size_t size);
void free(void* ptr);
void memset(char* ptr, char c, int count);

#endif