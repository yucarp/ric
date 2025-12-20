#include <kernel/port.h>
#include <stdint.h>

void outb(uint16_t port, uint8_t value){
    asm volatile("outb %b0, %w1" :: "a"(value), "Nd"(port) : "memory");
}

uint8_t inb(uint16_t port){
    uint8_t value = 0;
    asm volatile("inb %w1, %b0" :: "a"(value), "Nd"(port) : "memory");
    return value;
}

void iowait(){
    //Send zero to an unused IO port in order to wait
    outb(0x80, 0);
}
