GLOBAL syscaller

section .text

;rdi - rsi - rdx
syscaller:
    push rbp
    mov rbp, rsp

    push rbx

    mov rax, rdi        ; Syscall ID

   	mov rbx, rsi 	    ;1° param syscall
	mov [aux], rcx 
	mov rcx, rdx	    ;2° param syscall
	mov rdx, [aux]	    ;3° param syscall

	int 80h


    pop rbx

    mov rsp, rbp
    pop rbp
    ret

section .bss
    aux resq 1