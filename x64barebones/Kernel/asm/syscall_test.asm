GLOBAL div_by_zero


div_by_zero:
	mov rdx, 0
	mov rax, 1
	mov rcx, 0 
	mov rbx, 3	; vemos si funciona
	div rcx

	ret