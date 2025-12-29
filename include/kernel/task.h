#include <stdint.h>

struct Task {
    void (*point) (void);
    uint32_t esp, edi, esi, ebp, ebx, edx, ecx, eax;
};

void switch_task(struct Task previous, struct Task next);
void initialize_task(struct Task *task);
