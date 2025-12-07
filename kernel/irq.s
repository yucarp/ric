.global _isr0
.type _isr0, @function
_isr0:
    pushl $0
    pushl $0
    jmp isr_common

.global _isr8
.type _isr8, @function
_isr8:
    pushl $8
    jmp isr_common

.extern isr_handler
.type isr_handler, @function

.global isr_common
isr_common:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi
    pushl %edi
    pushl %ebp
    pushl %esp

    cld
    call isr_handler

    popl %esp
    popl %ebp
    popl %edi
    popl %esi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax

    add $8, %esp

    iretl
