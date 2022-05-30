GLOBAL syscaller

section .text

;rdi - rsi - rdx
syscaller:
    push rbp
    mov rbp, rsp

    push rbx

    mov rax, rdi        ; Syscall ID

   	mov rdi, rsi 	    ;1° param syscall
	mov [aux], rcx 
	mov rsi, rdx	    ;2° param syscall
	mov rdx, [aux]	    ;3° param syscall

	int 80h

    pop rbx

    mov rsp, rbp
    pop rbp
    ret

section .bss
    aux resq 1