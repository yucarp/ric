#include <elf.h>
#include <stdlib.h>
#include <drivers/terminal.h>
#include <kernel/idt.h>
#include <kernel/mmu.h>
#include <kernel/multiboot.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/ps2.h>

void *kernel_symbols[2];

void kernel_startpoint(){
   set_idt();
   initialize_ps2();
   initialize_pit();
   initialize_pic(0x20, 0x28);
   struct MultibootModule *module = (struct MultibootModule *) get_module(0);
   kernel_symbols[0] = load_symbol((void *) module->mod_start);
   void (*kprintf)(char*) = kernel_symbols[0];
   kprintf(" This is LYRIC 0.0.1.");
   while (1) {asm("hlt");}
}
