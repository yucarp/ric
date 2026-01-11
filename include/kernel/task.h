#include <stdint.h>

struct Task {
    uint32_t eip, ebp, edi, esi, ebx, esp;
} __attribute__((packed));

struct Process {
    int id;
    struct Task *task;
    struct Message **message_queue;
};

struct Message {
    int sender_id;
    int reciever_id;
    void *content;
};

extern void switch_task(struct Task *old_task, struct Task *new_task);
void initialize_process(struct Task *task);
int get_current_process_id();
void schedule();
void send_message(int reciever_id, void * message);
struct Message *recieve_message(int sender_id);
