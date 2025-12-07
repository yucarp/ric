AS := i686-elf-as
CC := i686-elf-gcc
CFLAGS := -O2 -ffreestanding -Wall -Wextra -Iinclude -fno-pie

OBJS=\
boot/boot.o \
boot/kernel.o \
kernel/idt.o \
kernel/irq.o \

.SUFFIXES: .o .c .s

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.s.o:
	$(AS) -MD -c $< -o $@

kernel.bin: $(OBJS)
	$(CC) -T linker.ld -o $@ -O2 -nostdlib -lgcc $(OBJS)
	grub-file --is-x86-multiboot kernel.bin
