#include <stddef.h>
#include <kernel/multiboot.h>

struct MultibootHeader *multiboot_pointer;

void *get_module(int n){
    if (!multiboot_pointer->mod_count) return 0;
    if (n > multiboot_pointer->mod_count) return 0;
    return (void *)(multiboot_pointer->mod_addr + 16 * n);
}
