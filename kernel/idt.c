#include <kernel/idt.h>
#include <kernel/port.h>

static struct IDTPointer idt_pointer;
struct IDTEntry idt_entries[256];
static void (*irq_entries[16]) (void);

void idt_set_gate(uint8_t n, void* handler, uint16_t selector, uint8_t attributes){
    idt_entries[n].offset_low = ((uint32_t)handler & 0xFFFF);
    idt_entries[n].offset_high = (((uint32_t)handler >> 16) & 0xFFFF);
    idt_entries[n].selector = selector;
    idt_entries[n].attributes = attributes;
}

void set_irq_function(uint8_t number, void *function){
    irq_entries[number] = function;
};

void set_idt(){
    idt_pointer.offset = &idt_entries;
    idt_pointer.size = sizeof(idt_entries) - 1;

    void *isr_functions[32] = {_isr0, _isr1, _isr2, _isr3, _isr4, _isr5, _isr6, _isr7, _isr8, _isr9, _isr10,
        _isr11, _isr12, _isr13, _isr14, _isr15, _isr16, _isr17, _isr18, _isr19, _isr20, _isr21, _isr22, _isr23,
        _isr24, _isr25, _isr26, _isr27, _isr28, _isr29, _isr30, _isr31
    };

    void *irq_functions[16] = {_irq0, _irq1, _irq2, _irq3, _irq4, _irq5, _irq6, _irq7, _irq8, _irq9, _irq10, _irq11, _irq12, _irq13, _irq15};

    for(int i = 0; i < 32; ++i){
            idt_set_gate(i, isr_functions[i], 0x08, 0x8E);
    };

    for(int j = 0; j < 16; ++j){
        idt_set_gate(32 + j, irq_functions[j], 0x08, 0x8E);
    };

    asm volatile (
        "lidt %0"
        : : "m"(idt_pointer)
    );
}
void isr_handler(struct x86Registers* registers){
    if((registers->interrupt_no > 31) && (irq_entries[registers->interrupt_no - 32] != 0)){
        irq_entries[registers->interrupt_no - 32]();
    } else if (registers->interrupt_no == 7) {

    }

}
