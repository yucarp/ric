#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/pit.h>

void pit_handler(){
    acknowledge_irq(0);
}

void initialize_pit(){
    set_irq_function(0, &pit_handler);
}
