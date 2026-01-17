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
char* ps2_buffer;

int i = 0;
struct File *file = 0;
struct Message *file_message = 0;

void task1(){
      int i = 0;
      while (1) {
            struct Message *m = recieve_message(1);
            if (m) *(char *)(0xB8090) = (int) m->content + 0x30;
      }
}

void task2(){

      send_message(0, (void *) 4);
      send_message(0, (void *) "ps2");
      send_message(0, (void *) ps2_buffer);
      send_message(0, (void *) 3);
      send_message(0, (void *) "ps2");

      while(1){
            file_message = recieve_message(0);
            i = 1;
            if(file_message) {file = (struct File *) file_message->content; break;}
      }


      while (1) {
            send_message(0, (void *) 1);
            send_message(0, (void *) file);
            struct Message *message = 0;
            while(!message){message = recieve_message(0);}
            *(char *)(0xB8090) = (int) message->content;
      }
}

void kernel_startpoint(){
   set_idt();
   initalize_tss();
   initialize_pic(0x20, 0x28);

   struct MultibootModule *terminal_module = (struct MultibootModule *) get_module(0);
   kernel_symbols[0] = load_symbol((void *) terminal_module->mod_start, 0);
   struct MultibootModule *vfs_module = (struct MultibootModule *) get_module(1);
   void *vfs_entry = load_program((void *) vfs_module->mod_start);
   void (*kprintf)(char*) = kernel_symbols[0];
   kprintf(" This is LYRIC 0.0.1.");

   struct Task *Task1 = malloc(sizeof(struct Task));
   struct Task *Task2 = malloc(sizeof(struct Task));
   struct Task *Kernel_Task = malloc(sizeof(struct Task));
   ps2_buffer = malloc(64);
   Task1->eip = (uint32_t) vfs_entry;
   Task2->eip = (uint32_t) &task2;

   initialize_process(Task1);
   initialize_process(Task2);
   initialize_process(Kernel_Task);

   initialize_ps2();
   initialize_pit();

   while(1) asm("hlt");
}
