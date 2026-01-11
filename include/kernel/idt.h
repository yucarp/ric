#include <stdint.h>

extern struct x86Registers *_isr0(struct x86Registers*);
extern struct x86Registers *_isr1(struct x86Registers*);
extern struct x86Registers *_isr2(struct x86Registers*);
extern struct x86Registers *_isr3(struct x86Registers*);
extern struct x86Registers *_isr4(struct x86Registers*);
extern struct x86Registers *_isr5(struct x86Registers*);
extern struct x86Registers *_isr6(struct x86Registers*);
extern struct x86Registers *_isr7(struct x86Registers*);
extern struct x86Registers *_isr8(struct x86Registers*);
extern struct x86Registers *_isr9(struct x86Registers*);
extern struct x86Registers *_isr10(struct x86Registers*);
extern struct x86Registers *_isr11(struct x86Registers*);
extern struct x86Registers *_isr12(struct x86Registers*);
extern struct x86Registers *_isr13(struct x86Registers*);
extern struct x86Registers *_isr14(struct x86Registers*);
extern struct x86Registers *_isr15(struct x86Registers*);
extern struct x86Registers *_isr16(struct x86Registers*);
extern struct x86Registers *_isr17(struct x86Registers*);
extern struct x86Registers *_isr18(struct x86Registers*);
extern struct x86Registers *_isr19(struct x86Registers*);
extern struct x86Registers *_isr20(struct x86Registers*);
extern struct x86Registers *_isr21(struct x86Registers*);
extern struct x86Registers *_isr22(struct x86Registers*);
extern struct x86Registers *_isr23(struct x86Registers*);
extern struct x86Registers *_isr24(struct x86Registers*);
extern struct x86Registers *_isr25(struct x86Registers*);
extern struct x86Registers *_isr26(struct x86Registers*);
extern struct x86Registers *_isr27(struct x86Registers*);
extern struct x86Registers *_isr28(struct x86Registers*);
extern struct x86Registers *_isr29(struct x86Registers*);
extern struct x86Registers *_isr30(struct x86Registers*);
extern struct x86Registers *_isr31(struct x86Registers*);

extern struct x86Registers *_irq0(struct x86Registers*);
extern struct x86Registers *_irq1(struct x86Registers*);
extern struct x86Registers *_irq2(struct x86Registers*);
extern struct x86Registers *_irq3(struct x86Registers*);
extern struct x86Registers *_irq4(struct x86Registers*);
extern struct x86Registers *_irq5(struct x86Registers*);
extern struct x86Registers *_irq6(struct x86Registers*);
extern struct x86Registers *_irq7(struct x86Registers*);
extern struct x86Registers *_irq8(struct x86Registers*);
extern struct x86Registers *_irq9(struct x86Registers*);
extern struct x86Registers *_irq10(struct x86Registers*);
extern struct x86Registers *_irq11(struct x86Registers*);
extern struct x86Registers *_irq12(struct x86Registers*);
extern struct x86Registers *_irq13(struct x86Registers*);
extern struct x86Registers *_irq14(struct x86Registers*);
extern struct x86Registers *_irq15(struct x86Registers*);

extern struct x86Registers *system_call(struct x86Registers *);
struct x86Registers {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

    uint32_t interrupt_no, error_code;

    uint32_t eip, cs, eflags, uesp, ss;
};

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

void set_irq_function(uint8_t number, void *function);
