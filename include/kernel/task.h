#include <stdint.h>

struct Task {
    uint32_t eip, ebp, edi, esi, ebx, esp;
} __attribute__((packed));

extern void switch_task(struct Task *old_task, struct Task *new_task);
void initialize_task(struct Task *task);
void schedule();
