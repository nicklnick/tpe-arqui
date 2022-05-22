GLOBAL readKeyboard
GLOBAL registerDump

section .text

readKeyboard:
	push rbp
	mov rbp, rsp

	mov ah, 1  					; 0000 0001
	in al, 64h					; status del teclado/8042
	and al, ah					; ???? bitwise(&)  0001 ; me quedo solo con el ultimo bit
	cmp al, 1 					; para poder recibir, bit 0 de status tiene que estar en 1
	jne no_key_recieved			; sino, sigo esperando hasta que sea 0

	mov rax, 0
	in al, 60h					; leo letra
	jmp end

	no_key_recieved:
		mov rax, -1				; caso leatra no leaida
		

	end:

	mov rsp, rbp
	pop rbp
	ret

; void registerDump(uint64_t * buffer);
registerDump:		
	push rbp
	mov rbp, rsp

	mov [rdi + 64 * 0], rax
	mov [rdi + 64 * 1], rbx
	mov [rdi + 64 * 2], rcx
	mov [rdi + 64 * 3], rdx

	mov [rdi + 64 * 4], rbp
	mov [rdi + 64 * 5], rsp
	mov [rdi + 64 * 6], rsi
	mov [rdi + 64 * 7], rdi

	mov [rdi + 64 * 8], r9
	mov [rdi + 64 * 9], r10
	mov [rdi + 64 * 10], r11
	mov [rdi + 64 * 11], r12
	mov [rdi + 64 * 12], r13
	mov [rdi + 64 * 13], r14
	mov [rdi + 64 * 14], r15

	mov rsp, rbp 
	pop rbp
	ret
