#include <fs/file.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/ps2.h>
#include <kernel/port.h>
#include <kernel/task.h>

#include <stdint.h>

#define PS2_DATA 0x60
#define PS2_COMMAND 0x64

extern char *ps2_buffer;
struct File ps2_file = {0, 0};

char ps2_keymap[] = {' ', 'E', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?', '?',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.'
};
void ps2_handler(struct x86Registers *regs){
    uint8_t key = inb(PS2_DATA);
    if(key < 52){
        send_message(0, (void *) 2);
        send_message(0, &ps2_file);
        send_message(0, (void *)ps2_keymap[key]);
    }
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

        status |= 0b0101001;

        outb(PS2_COMMAND, 0x60);
        wait_ps2();
        outb(PS2_DATA, status);
        wait_ps2();

        outb(PS2_COMMAND, 0xAE);

        ps2_file.buffer = ps2_buffer;
        set_irq_function(1, &ps2_handler);
        return 0;
}
