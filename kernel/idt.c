#include <kernel/idt.h>

static struct IDTPointer idt_pointer;
struct IDTEntry idt_entries[256];

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t selector, uint8_t attributes){
    idt_entries[n].offset_low = (handler & 0xFFFF);
    idt_entries[n].offset_high = (handler & 0xFFFF0000);
    idt_entries[n].selector = selector;
    idt_entries[n].attributes = attributes;
}

void set_idt(){
    idt_pointer.offset = &idt_entries;
    idt_pointer.size = sizeof(idt_entries) - 1;

    idt_set_gate(0, (uint32_t)&_isr0, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)&_isr8, 0x08, 0x8E);

    asm volatile (
        "lidt %0"
        : : "m"(idt_pointer)
    );
}

void isr_handler(){
    *((char *)(0xB8000)) = 'I';
}
