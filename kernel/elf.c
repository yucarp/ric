#include <elf.h>
#include <kernel/mmu.h>
#include <stdlib.h>
#include <stddef.h>

int loaded_programs = 0;
int loaded_modules = 0;

//TODO: Move this to a common place
void * memcpy(void * restrict dest, const void * restrict src, size_t n) {
    void *ret = dest;
    asm volatile("rep movsb" : "+c"(n), "+D"(dest), "+S"(src) :: "cc", "memory");
    return ret;
}

struct ElfHeader *parse_header(void *header_place){
    return (struct ElfHeader *) header_place;
}

void *load_symbol(void* file_start){
    struct ElfHeader *header = parse_header(file_start);
    void *symbol = 0;

    //Check the header to see if it's ELF'
    if (header->magic != 0x464C457F) {
        *((char *)(0xB8008)) = 'N';
        return NULL;
    }

    //Loading symbols
    for(int i = 0; i <= header->setaennu; ++i){
        struct SectionHeader *sheader = (struct SectionHeader *)(file_start + header->section_header + i * header->setaensi);

        //Copy PROGBITS sections to memory
        if (sheader->type == 1) {
            for(int i = 0; i < sheader->size; i += 0x1000){
                allocate_page(sheader->address, 0x0600000 + loaded_modules * 0x10000);
            }
            memcpy((void *)sheader->address, (void *)(file_start + sheader->offset), sheader->size);
        }

        //Set up NOBITS
        if (sheader->type == 8) {
            for(int i = 0; i < sheader->size; i += 0x1000){
                allocate_page(sheader->address, 0x0500000 + loaded_modules * 0x10000);
            }
        }

        //If not SYM_TAB continue
        else if(sheader->type != 2) continue;

        sheader->address = ((uint32_t)file_start + sheader->offset);
        struct SectionHeader *string_table = (file_start + header->section_header + header->setaensi * sheader->link);
        char *symbol_names = (char *)string_table->address;
        struct ElfSymbol *sym_table = (struct ElfSymbol *) sheader->address;

        for(int j = 0; j < sheader->size / sizeof(struct ElfSymbol) + 1; ++j){
            if(((sym_table[j].info & 0xf) == 2)){ //Return if a function
                symbol = (void *)sym_table[j].value;
            }
        }
    }

    //Set up relocation tables
    if(header->type == 1) {
    for(int i = 0; i <= header->setaennu; ++i){
        struct SectionHeader *sheader = (struct SectionHeader *)(file_start + header->section_header + i * header->setaensi);

            sheader->address = ((uint32_t)file_start + sheader->offset);
            struct ElfRela *table = (struct ElfRela *)sheader->address;
            struct SectionHeader *target_section = (struct SectionHeader *)(file_start + header->section_header + header->setaensi * sheader->info);
            struct SectionHeader *sym_section = (struct SectionHeader *)(file_start + header->section_header + header->setaensi * sheader->link);
            struct ElfSymbol *sym_table = (struct ElfSymbol *)sym_section->address;

            for(int rel = 0; rel < sheader->size / sizeof(struct ElfRela); ++rel){
                uintptr_t target = table[rel].offset + target_section->address;

                switch(table[rel].info){
                    case 10:
                        *((char *) + (0xB8000)) = 'O';
                        *((uint32_t *)target) = sym_table[table[rel].info].value + table[rel].addend;
                    break;
                    case 2:
                        *((char *) + (0xB8000)) = 'P';
                        *((uint32_t *)target) = sym_table[table[rel].info].value + table[rel].addend - target;
                    break;
                }
            }
        }
    }

    return symbol;
}

void load_program(void *file_start){
    struct ElfHeader *header = parse_header(file_start);

    if (header->magic != 0x464C457F) {
        *((char *)(0xB8008)) = 'N';
        return ;
    }

    for(int i = 0; i < header->prtaennu; ++i){
        struct ProgramHeader *pheader = (struct ProgramHeader *)(file_start + header->program_header + i * header->prtaensi);
        if(pheader->type == 1){
            for(int i = 0; i < pheader->memory_size; i += 0x1000){
                allocate_page(pheader->virtual_addr, 0x0300000 + loaded_programs * 0x10000);
            }
            memcpy((void *)pheader->virtual_addr, (void *)(file_start + pheader->offset), pheader->memory_size);

        }
    }


    int (*entry) (void) = (int (*)(void))(header->program_entry);
    entry();
}
