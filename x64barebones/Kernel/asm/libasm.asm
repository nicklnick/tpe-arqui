GLOBAL readKeyboard

section .text

readKeyboard:
	push rbp
	mov rbp, rsp


	readKeyboard_receive:
		mov ah, 1  					; 0000 0001
		in al, 64h					; status del teclado/8042
		and al, ah					; ???? bitwise(&)  0001 ; me quedo solo con el ultimo bit
		cmp al, 1 					; para poder recibir, bit 0 de status tiene que estar en 1
		jne readKeyboard_receive	; sino, sigo esperando hasta que sea 0

	mov rax, 0
	in al, 60h					; leo letra

	mov rsp, rbp
	pop rbp
	ret
