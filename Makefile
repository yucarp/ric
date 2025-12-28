AS := i686-elf-as
CC := i686-elf-gcc
CFLAGS := -O2 -ffreestanding -Wall -Wextra -Iinclude

TERMOBJS=\
drivers/terminal/terminal.so

OBJS=\
boot/boot.o \
boot/kernel.o \
boot/multiboot.o \
kernel/elf.o \
kernel/idt.o \
kernel/irq.o \
kernel/malloc.o \
kernel/mmu.o \
kernel/pic.o \
kernel/pit.o \
kernel/port.o \
kernel/ps2.o \

.SUFFIXES: .o .c .s .so

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.c.so:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) -fpic

.s.o:
	$(AS) -MD -c $< -o $@

all: kernel.bin terminal.so

kernel.bin: $(OBJS)
	$(CC) -T linker.ld -o $@ -O2 -ffreestanding -nostdlib -lgcc $(OBJS)
	grub-file --is-x86-multiboot kernel.bin

terminal.so: $(TERMOBJS)
	$(CC) -o $@ -O2 -ffreestanding -nostdlib -lgcc $(TERMOBJS) -shared
