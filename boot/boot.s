.set FLAGS, (1 << 0) | (1 << 1)
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.align 4
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

.extern multiboot_pointer

.global _startpoint
.type _startpoint, @function
_startpoint:

mov $multiboot_pointer, %edx
mov %ebx, (%edx)

lgdt gdtr
ljmp $0x08,$_longjump

_longjump:

mov $0x10, %ax
mov %ax, %ds
mov %ax, %ss

mov $stack_top, %esp

.extern page_directory
.extern page_table

mov $1024, %ecx
mov $2, %ebx
mov $page_directory, %edi

set_directory:
    mov %ebx, (%edi)
    add $4, %edi
    loop set_directory

mov $1024, %ecx
mov $5, %ebx
mov $page_table, %edi

set_table:
    mov %ebx, (%edi)
    add $4, %edi
    add $0x1000, %ebx
    loop set_table

mov $page_directory, %edi
mov $page_table, (%edi)
orl $3, (%edi)

movl %edi, %eax
mov %eax, %cr3

or $0x80000001, %eax
mov %eax, %cr0

mov $stack_top, %esp

jmp final

final:
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
