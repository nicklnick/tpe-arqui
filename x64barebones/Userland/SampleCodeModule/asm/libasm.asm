GLOBAL getRegisters

section .text


;void getRegisters(uint64_t * buffer);
getRegisters:
		push rbp
		mov rbp, rsp

		mov [rdi + 8 * 0], rax
		mov [rdi + 8 * 1], rbx
		mov [rdi + 8 * 2], rcx
		mov [rdi + 8 * 3], rdx
		mov [rdi + 8 * 4], rbp
		mov [rdi + 8 * 5], rdi
		mov [rdi + 8 * 6], rsi
		mov [rdi + 8 * 7], r8
		mov [rdi + 8 * 8], r9
		mov [rdi + 8 * 9], r10
		mov [rdi + 8 * 10], r11
		mov [rdi + 8 * 11], r12
		mov [rdi + 8 * 12], r13
		mov [rdi + 8 * 13], r14
		mov [rdi + 8 * 14], r15


		mov rsp, rbp
		pop rbp
		ret
