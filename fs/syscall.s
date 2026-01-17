.global sys_call
sys_call:
	mov 4(%esp), %eax
	mov 8(%esp), %edi
	mov 12(%esp), %esi

	int $128

	ret
