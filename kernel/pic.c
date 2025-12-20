#include <kernel/pic.h>
#include <kernel/port.h>

#define PIC0_COMMAND 0x20
#define PIC1_COMMAND 0xA0
#define PIC0_DATA 0x21
#define PIC1_DATA 0xA1

void initialize_pic(int offset1, int offset2){
    outb(PIC0_COMMAND, 0x11);
    iowait();
    outb(PIC1_COMMAND, 0x11);
    iowait();

    outb(PIC0_DATA, offset1);
    iowait();
    outb(PIC1_DATA, offset2);
    iowait();

    outb(PIC0_DATA, 4);
    iowait();
    outb(PIC1_DATA, 2);
    iowait();

    //Do not use the 8080 mode
    outb(PIC0_DATA, 1);
    iowait();
    outb(PIC1_DATA, 1);
    iowait();

    outb(PIC0_DATA, 0xFD);
    outb(PIC1_DATA, 0xFF);

    asm volatile ("sti");
}

void acknowledge_irq(uint8_t irq){
    if (irq >= 8){
        outb(PIC1_DATA, 0x20);
    } outb(PIC0_DATA, 0x20);
}
