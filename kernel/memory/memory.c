#include "../includes/memory.h"
#include "../includes/string.h"

#define NULL_POINTER ((void*)0)
#define DYNAMIC_MEM_TOTAL_SIZE (4 * 1024)
#define DYNAMIC_MEM_NODE_SIZE sizeof(dynamic_mem_node_t)  // 16

static uint8_t dynamic_mem_area[DYNAMIC_MEM_TOTAL_SIZE];
static dynamic_mem_node_t* dynamic_mem_start;

// Initialize dynamic memory
void init_dynamic_mem()
{
    dynamic_mem_start = (dynamic_mem_node_t*)dynamic_mem_area;
    dynamic_mem_start->size = DYNAMIC_MEM_TOTAL_SIZE - DYNAMIC_MEM_NODE_SIZE;
    dynamic_mem_start->prev = NULL_POINTER;
    dynamic_mem_start->next = NULL_POINTER;
    dynamic_mem_start->used = false;  // Initialize the used member
}

dynamic_mem_node_t* find_best_mem_block(dynamic_mem_node_t* dynamic_mem, size_t size)
{
    dynamic_mem_node_t* best_mem_block = (dynamic_mem_node_t*)NULL_POINTER;
    uint32_t best_mem_block_size = DYNAMIC_MEM_TOTAL_SIZE + 1;

    dynamic_mem_node_t* current_mem_block = dynamic_mem;
    while (current_mem_block)
    {
        if ((!current_mem_block->used) &&
            (current_mem_block->size >= (size + DYNAMIC_MEM_NODE_SIZE)) &&
            (current_mem_block->size <= best_mem_block_size))
        {
            best_mem_block = current_mem_block;
            best_mem_block_size = current_mem_block->size;
        }
        current_mem_block = current_mem_block->next;
    }
    return best_mem_block;
}

dynamic_mem_node_t* merge_next_node_into_current(dynamic_mem_node_t* current_mem_node)
{
    dynamic_mem_node_t* next_mem_node = current_mem_node->next;
    if (next_mem_node != NULL_POINTER && !next_mem_node->used)
    {
        current_mem_node->size += next_mem_node->size;
        current_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        current_mem_node->next = next_mem_node->next;
        if (current_mem_node->next != NULL_POINTER)
        {
            current_mem_node->next->prev = current_mem_node;
        }
    }

    return current_mem_node;
}

void merge_current_node_into_previous(dynamic_mem_node_t* current_mem_node)
{
    dynamic_mem_node_t* prev_mem_node = current_mem_node->prev;

    if (prev_mem_node != NULL_POINTER && !prev_mem_node->used)
    {
        prev_mem_node->size += current_mem_node->size;
        prev_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        prev_mem_node->next = current_mem_node->next;
        if (prev_mem_node->next != NULL_POINTER)
        {
            prev_mem_node->next->prev = prev_mem_node;
        }
    }
}

void* mem_alloc(size_t size)
{
    dynamic_mem_node_t* best_mem_block = find_best_mem_block(dynamic_mem_start, size);

    if (best_mem_block != NULL_POINTER)
    {
        best_mem_block->size = best_mem_block->size - size - DYNAMIC_MEM_NODE_SIZE;

        dynamic_mem_node_t* mem_node_allocate =
            (dynamic_mem_node_t*)(((uint8_t*)best_mem_block) + DYNAMIC_MEM_NODE_SIZE +
                                  best_mem_block->size);
        mem_node_allocate->size = size;
        mem_node_allocate->used = true;
        mem_node_allocate->prev = best_mem_block;
        mem_node_allocate->next = best_mem_block->next;

        if (best_mem_block->next != NULL_POINTER)
        {
            best_mem_block->next->prev = mem_node_allocate;
        }
        best_mem_block->next = mem_node_allocate;

        return (void*)((uint8_t*)mem_node_allocate + DYNAMIC_MEM_NODE_SIZE);
    }

    return NULL_POINTER;
}

void mem_free(void* ptr)
{
    if (ptr == NULL_POINTER)
    {
        return;
    }

    dynamic_mem_node_t* current_mem_node =
        (dynamic_mem_node_t*)((uint8_t*)ptr - DYNAMIC_MEM_NODE_SIZE);

    if (current_mem_node == NULL_POINTER)
    {
        return;
    }

    current_mem_node->used = false;

    current_mem_node = merge_next_node_into_current(current_mem_node);
    merge_current_node_into_previous(current_mem_node);
}

void memset(char* ptr, char c, int count)
{
    while (count--)
    {
        *ptr++ = c;
    }
}