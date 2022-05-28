GLOBAL div_by_zero
GLOBAL read
GLOBAL print

div_by_zero:
	mov rdx, 0
	mov rax, 1
	mov rcx, 0 
	mov rbx, 3	; vemos si funciona
	div rcx

	ret

read:
	
	mov rax,0
	int 80h

	ret 
	
print:
	mov rax,1
	int 80h

	ret