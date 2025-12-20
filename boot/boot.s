.set FLAGS, (1 << 0) | (1 << 1)
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text

.global _startpoint
.type _startpoint, @function
_startpoint:

lgdt gdtr
ljmp $0x08,$_longjump

_longjump:

mov $0x10, %ax
mov %ax, %ds
mov %ax, %ss

mov $stack_top, %esp

call kernel_startpoint

lop: hlt
jmp lop

.align 4
gdtr:
    .word gdt_end - gdt_base
    .long gdt_base

gdt_base:
    .quad 0

    .word 0xFFFF
    .word 0
    .byte 0
    .byte 0x9A
    .byte 0xCF
    .byte 0

    .word 0xFFFF
    .word 0
    .byte 0
    .byte 0x92
    .byte 0xCF
    .byte 0
gdt_end:
