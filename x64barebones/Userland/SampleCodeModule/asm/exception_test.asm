GLOBAL divError
GLOBAL opCodeError


divError:
	mov rdx, 0
	mov rax, 0
	mov rcx, 1234321h
	div rdx

opCodeError:
	mov rax, 0 
	mov rcx, 1234321h
	ud2