#include <stdint.h>

struct MultibootHeader {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_higher;
    uint32_t boot_device;
    uint32_t cmd_line;
    uint32_t mod_count;
    uint32_t mod_addr;
};

struct MultibootModule {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t cmdline;
    uint32_t padding;
};

void *get_module(int n);
