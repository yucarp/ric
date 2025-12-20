#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/ps2.h>

void kernel_startpoint(){
   set_idt();
   int test = initialize_ps2();
   initialize_pic(0x20, 0x28);
   if(!test){
         *(char *)(0xB8004) = 'P';
   }
}
