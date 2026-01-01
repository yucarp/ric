#include <kernel/mmu.h>
#include <kernel/task.h>

int task_number = 0;
int current_task = 0;
struct Task *tasks[128] = {0};

void initialize_task(struct Task *task){
    allocate_page(0x1000000 + (task_number - 1) * 0x1000, 0x1000000 + (task_number - 1 )* 0x1000);
    allocate_page(0x1000000 + task_number * 0x1000, 0x1000000 + task_number * 0x1000);
    task->esp = (0x1000000 + task_number * 0x1000);
    uint32_t *pesp = (uint32_t *) task->esp;
    *--pesp = task->eip;
    *--pesp = task->ebx;
    *--pesp = task->esi;
    *--pesp = task->edi;
    *--pesp = task->ebp;
    task->esp = (uint32_t) pesp;
    tasks[task_number] = task;
    task_number++;
}

void schedule(){
    if(current_task + 1 > task_number){
        switch_task(tasks[current_task], tasks[0]);
        current_task = 0;
    }
    *(char *)(0xB8060) = 'a';
    switch_task(tasks[current_task], tasks[current_task + 1]);
    *(char *)(0xB8062) = 'b';
    ++current_task;
}
