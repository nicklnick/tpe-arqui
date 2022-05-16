GLOBAL print
print:
		mov eax, 1
		mov rdi, 1
		mov rsi, string
		mov rdx, 81

		int 80h

		ret


section .data
string db "Este es un string largo a ver si funciona estooooooooooo pimba pumba timon y pumba"
