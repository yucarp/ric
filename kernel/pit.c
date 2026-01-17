#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/task.h>

unsigned int ticks = 0;

struct x86Registers *pit_handler(struct x86Registers *registers){
    ++ticks;
    *(char *)(0xB80A0) = ticks % 10 + 0x30;
    acknowledge_irq(0);
    schedule(registers->eip);
}

void initialize_pit(){
    set_irq_function(0, &pit_handler);
}
