#include <kernel/mmu.h>
#include <kernel/task.h>
#include <stdlib.h>

struct ProcessList {
    struct Process *process;
    struct ProcessList *next;
};

static int task_number = 0;
struct ProcessList *first_task;
struct ProcessList *main_task;
struct ProcessList *prev_task;

void initialize_stack(struct Task *task){
    uint32_t *pesp = (uint32_t *) task->esp;
    *--pesp = task->eip;
    *--pesp = 0;
    *--pesp = 0;
    *--pesp = 0;
    *--pesp = 0;
    task->esp = (uint32_t) pesp;
}

void initialize_process(struct Task *task){
    for(int i = 0; i < 16; ++i){
        allocate_page(0x1000000 + (task_number * 16 - i) * 0x1000, 0x1000000 + (task_number * 16 - i) * 0x1000);
    }
    task->esp = (0x1000000 + task_number * 16 * 0x1000);
    initialize_stack(task);
    struct ProcessList *pl = malloc(sizeof(struct ProcessList));
    struct Process *p = malloc(sizeof(struct Process));
    pl->process = p;
    pl->process->task = task;

    p->message_queue = malloc(16);
    for(int i = 0; i < 16; ++i){p->message_queue[i] = 0;}

    if(task_number == 0){
        first_task = pl;
        main_task = first_task;
        task_number++;
        return;
    }



    main_task->next = pl;
    main_task = pl;
    main_task->next = first_task;

    pl->process->id = task_number;
    task_number++;
}

int get_current_process_id(){
    return main_task->process->id;
}

void send_message(int reciever_id, void *message){
    int i = 0;
    struct ProcessList *pl = first_task;
    while(pl->process->id != reciever_id){
        pl = pl->next;
        if(pl->next == first_task) break;
    }
    struct Message *msg = malloc(sizeof(struct Message));
    msg->sender_id = get_current_process_id();
    msg->reciever_id = reciever_id;
    msg->content = message;

    for(; pl->process->message_queue[i] != 0; ++i){
    }

    pl->process->message_queue[i] = msg;
}

struct Message *recieve_message(int sender_id){
    int cid = get_current_process_id();
    struct ProcessList *pl = first_task;

    while(pl->process->id != cid){
        pl = pl->next;
    }

    int i = 0;
    for(; pl->process->message_queue[i]->sender_id != sender_id; ++i) {if (!pl->process->message_queue[i]){ return 0;}};
    if (pl->process->message_queue[i] != 0) {
        struct Message *msg = pl->process->message_queue[i];
        for(int j = 0; j < 15; ++j){
            pl->process->message_queue[j] = pl->process->message_queue[j + 1];
        }
        return msg;
    } else {return 0;}
}

void schedule(){
    prev_task = main_task;
    main_task = main_task->next;
    switch_task(prev_task->process->task, main_task->process->task);
}
