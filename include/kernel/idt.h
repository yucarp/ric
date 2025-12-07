#include <stdint.h>

extern void  *_isr0(void*);
extern void  *_isr8(void*);

struct IDTPointer {
    uint16_t size;
    void *offset;
} __attribute__((packed));

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t null;
    uint8_t attributes;
    uint16_t offset_high;
} __attribute__((packed));

void set_idt();
