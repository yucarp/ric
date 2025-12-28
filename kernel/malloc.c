//A basic linked-list allocator. A more efficient allocator would be nicer

#include <stddef.h>
#include <stdlib.h>
#include <kernel/mmu.h>

struct MemoryNode {
    void *prev;
    int size;
    void *address;
    void *next;
};

extern void *kernel_end;
void *last_available_address = NULL;
struct MemoryNode *last_node;

void allocate_new_node(int size){
     if(last_available_address == 0){ last_available_address = kernel_end; }
     struct MemoryNode new_memory_node = {last_node, size, last_available_address, NULL};
     last_node = &new_memory_node;
     last_available_address += 4 * size;
}

void *malloc(int size){
    allocate_new_node(size);
    return last_node->address;
}

void free(void *address){
    struct MemoryNode *main_node = last_node;
    for(int i = 0; main_node->address == address; ++i){
        main_node = main_node->prev;
    }

    ((struct MemoryNode *)(main_node->prev))->next = main_node->next;
    ((struct MemoryNode *)(main_node->next))->prev = main_node->prev;
}
