#include <stdint.h>

struct ElfHeader {
    uint32_t magic;
    uint8_t bit;
    uint8_t endian;
    uint8_t header_version;
    uint8_t abi;
    uint32_t padding1;
    uint32_t padding2;
    uint16_t type;
    uint16_t instruction;
    uint32_t elf_version;
    uint32_t program_entry;
    uint32_t program_header;
    uint32_t section_header;
    uint32_t flags;
    uint16_t header_size;
    uint16_t prtaensi;
    uint16_t prtaennu;
    uint16_t setaensi;
    uint16_t setaennu;
    uint16_t section_index;
};

struct ProgramHeader {
    uint32_t type;
    uint32_t offset;
    uint32_t virtual_addr;
    uint32_t physical_addr;
    uint32_t file_size;
    uint32_t memory_size;
    uint32_t flags;
    uint32_t alignment;
};

struct SectionHeader {
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t address;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addr_align;
    uint32_t entry_size;
};

struct ElfSymbol {
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t index;
};

struct ElfRel {
    uint32_t offset;
    uint32_t info;
};

struct ElfRela {
    uint32_t offset;
    uint16_t info;
    uint16_t addend;
};

void load_program(void *file_start);
void * load_symbol(void *file_start);
