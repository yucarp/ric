#include <elf.h>
#include <stdlib.h>
#include <drivers/terminal.h>
#include <kernel/idt.h>
#include <kernel/mmu.h>
#include <kernel/multiboot.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/ps2.h>
#include <kernel/task.h>

void *kernel_symbols[2];

struct Task Task0  = {0, 0, 0, 0, 0, 0};
struct Task Task1 = {0, 0, 0, 0, 0, 0};
struct Task Task2 = {0, 0, 0, 0, 0, 0};

void task1(){
      unsigned int i = 1;
      while(i){
            *(char *)(0xB8050) = (i % 10) + 48;
            i++;
            if(i == 0) i = 1;
      }
}

void task2(){
      unsigned int i = 1;
      while(i){
            *(char *)(0xB8052) = (i % 10) + 48;
      }
}

void kernel_startpoint(){
   set_idt();
   initialize_ps2();
   initialize_pit();
   initialize_pic(0x20, 0x28);
   struct MultibootModule *module = (struct MultibootModule *) get_module(0);
   kernel_symbols[0] = load_symbol((void *) module->mod_start);
   void (*kprintf)(char*) = kernel_symbols[0];
   kprintf(" This is LYRIC 0.0.1.");
   Task1.eip = (uint32_t) &task1;
   Task2.eip = (uint32_t) &task2;
   initialize_task(&Task1);
   initialize_task(&Task2);
   while (1) {asm("hlt");}
}
