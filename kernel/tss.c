#include <kernel/tss.h>
#include <stdint.h>

struct GDT {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access_byte;
    uint8_t flag_limit;
    uint8_t base_high;
}__attribute__((packed));

struct GDT_Pointer{
    uint16_t size;
    void *pointer
} __attribute__((packed));

struct TSS {
      uint32_t prev_tss;
      uint32_t esp0;
      uint32_t ss0;
      uint32_t reserved[24];
} __attribute__((packed));

extern struct GDT gdt_base[6];
extern struct GDT_Pointer gdtr;

struct TSS tss = {0, 0, 0, 0};

void initalize_tss(){
    extern void *stack_top;
    tss.esp0 = (int) &stack_top;
    gdt_base[5].limit = sizeof(tss);
    gdt_base[5].base_low = (int) &tss & 0xFFFF;
    gdt_base[5].base_middle = ((int) &tss >> 16) & 0xFF;
    gdt_base[5].base_high = ((int) &tss >> 24) & 0xFF;
    gdt_base[5].access_byte = 0x89;
    asm volatile (
        "lgdt %0\n"
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%ss\n"
        "mov $0x28, %%ax\n"
        "ltr %%ax\n"
    : : "m"(gdtr) : "eax", "memory");
}
