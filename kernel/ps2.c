#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/ps2.h>
#include <kernel/port.h>

#include <stdint.h>

#define PS2_DATA 0x60
#define PS2_COMMAND 0x64

void ps2_handler(){
    uint8_t key = inb(PS2_DATA);
    *(char *)(0xB8000) = (char) key;
    acknowledge_irq(1);
}

void wait_ps2(){
        for(int i = 0; i < 1000; ++i){
            inb(PS2_COMMAND);
        }
}

int initialize_ps2(){
        uint8_t status = 0;

        outb(PS2_COMMAND, 0xAD);
        wait_ps2();
        outb(PS2_COMMAND, 0xA7);
        wait_ps2();

        int timeout = 0;
        for(; timeout < 1024; ++timeout){

            inb(PS2_DATA);

            if(!(inb(PS2_COMMAND) & 1)){
                break;
            }
        }

        if(timeout == 1023) return -1;

        outb(PS2_COMMAND, 0x20);
        wait_ps2();
        status = inb(PS2_DATA);
        wait_ps2();

        status |= 0b01010010;

        outb(PS2_COMMAND, 0x60);
        wait_ps2();
        outb(PS2_DATA, status);
        wait_ps2();

        set_irq_function(1, &ps2_handler);
        return 0;
}
