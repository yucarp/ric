#include <elf.h>
#include <stdlib.h>
#include <fs/file.h>
#include <kernel/idt.h>
#include <kernel/mmu.h>
#include <kernel/multiboot.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/ps2.h>
#include <kernel/task.h>
#include <kernel/tss.h>

void *kernel_symbols[2];
int i = 0;
struct Task Task0  = {0, 0, 0, 0, 0, 0};
struct Task Task1 = {0, 0, 0, 0, 0, 0};
struct Task Task2 = {0, 0, 0, 0, 0, 0};
struct File file = {0, "Hello world!"};
void task0(){
      while (1) asm("hlt");
}

void task1(){
      send_message(2, (void *) 2);
      send_message(2, (void *) 3);
      while (1) {
            struct Message *f = recieve_message(2);
            if (f) *(char *)(0xB8090 + (int) f->content * 2) = (int)f->content + 0x30;
      }
}

void task2(){
      int h = 0;
      send_message(1, (void *) 3);
      send_message(1, (void *) &file);
      while(1){
            struct Message *f = recieve_message(1);
            if (f){
                  *(char *)(0xB8080 + h * 2) = (int)f->content;
                  *(char *)(0xB8070) = h + 0x30;
            }
      }
      asm("hlt");
}

void kernel_startpoint(){
   set_idt();
   initalize_tss();
   initialize_ps2();
   initialize_pit();
   initialize_pic(0x20, 0x28);

   struct MultibootModule *terminal_module = (struct MultibootModule *) get_module(0);
   kernel_symbols[0] = load_symbol((void *) terminal_module->mod_start, 0);
   struct MultibootModule *vfs_module = (struct MultibootModule *) get_module(1);
   void *vfs_entry = load_program((void *) vfs_module->mod_start);
   void (*kprintf)(char*) = kernel_symbols[0];
   kprintf(" This is LYRIC 0.0.1.");

   Task0.eip = (uint32_t) &task0;
   Task1.eip = (uint32_t) vfs_entry;
   Task2.eip = (uint32_t) &task2;
   initialize_process(&Task0);
   initialize_process(&Task1);
   initialize_process(&Task2);
   while (1) {asm("hlt");}
}
