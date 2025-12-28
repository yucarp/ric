#include <stdint.h>
#include <kernel/mmu.h>

void *page_directory[1024] __attribute__((aligned(4096)));
void *page_table[1024] __attribute__((aligned(4096)));

void fill_table(uintptr_t physical_address, uintptr_t *table, int index){
    table[index] = physical_address + 4096 * index;
    table[index] |= 3;
}

int allocate_page(uintptr_t virtual_address, uint32_t physical_address){
    int address_dir_index = virtual_address >> 22;
    int page_table_index = (virtual_address >> 12) & 0x3FF;
    void *new_table = page_table + ((address_dir_index) << 12);
    fill_table(physical_address, new_table, page_table_index);
    page_directory[address_dir_index] = (void *)((uint32_t) new_table | 3);
}
