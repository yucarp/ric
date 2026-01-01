.section .text

.macro ISR index
	.global _isr\index
	.type _isr\index, @function
	_isr\index:
		pushl $0
		pushl $\index
		jmp isr_common
.endm

.macro ISR_ERR index
	.global _isr\index
	.type _isr\index, @function
	_isr\index:
		pushl $\index
		jmp isr_common
.endm

.macro IRQ index byte
	.global _irq\index
	.type _irq\index, @function
	_irq\index:
		pushl $0
		pushl $\byte
		jmp isr_common
.endm

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_ERR 8
ISR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

.extern isr_handler
.type isr_handler, @function

.extern next_task
.type next_task, @function

.global isr_common
isr_common:
    pushal
	push %esp
	cld
    call isr_handler

    pop %esp
    popal
    add $8, %esp
    iretl

.global switch_task
switch_task:
	mov 4(%esp), %eax
	mov 8(%esp), %ecx

	push %ebx
	push %edi
	push %esi
	push %ebp

	mov %esp, 20(%eax)
	mov 20(%ecx), %esp

	pop %ebp
	pop %esi
	pop %edi
	pop %ebx

	ret
