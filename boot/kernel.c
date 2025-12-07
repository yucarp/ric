#include <kernel/idt.h>

void kernel_startpoint(){
   set_idt();
   asm volatile("int $0x00");
}
