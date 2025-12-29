#include <kernel/mmu.h>
#include <kernel/task.h>

extern void restore_context(struct Task);
extern void save_context(struct Task);

int task_number = 0;
void initialize_task(struct Task *task){
    allocate_page(0x1000000 + task_number * 0x1000, 0x1000000 + task_number * 0x1000);
    task->esp = 0x1000000 + task_number * 0x1000;
    task_number++;
}

void switch_task(struct Task previous, struct Task next){
    save_context(previous);
    *(char *)(0xB8060)='s';
    asm volatile("mov $4, %eax");
    //while (1) {asm("hlt");}
    restore_context(next);
    *(char *)(0xB8060)='r';
    ((void(*)(void))(next.point))();
}
