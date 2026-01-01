#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/task.h>

unsigned int ticks = 0;

void pit_handler(){
    ++ticks;
    *(char *)(0xB8000) = ticks % 10 + 0x30;
    acknowledge_irq(0);
    if(ticks % 3 == 0) schedule();
    return ;
}

void initialize_pit(){
    set_irq_function(0, &pit_handler);
}
