
%macro switchState param1
	mov rax, [param1 + 64 * 0]
	mov rbx, [param1 + 64 * 1]
	mov rcx, param1 + 64 * 2
	mov rdx, param1 + 64 * 3
	mov rbp, param1 + 64 * 4
	mov rdi, param1 + 64 * 5
	mov rsi, param1 + 64 * 6
	mov r8, param1 + 64 * 7
	mov r9, param1 + 64 * 8
	mov r10, param1 + 64 * 9
	mov r11, param1 + 64 * 10
	mov r12, param1 + 64 * 11
	mov r13, param1 + 64 * 12
	mov r14, param1 + 64 * 13
	mov r15, param1 + 64 * 14
%endmacro

%macro saveState param1
	mov [param1 + 64 * 0], rax
	mov [param1 + 64 * 1], rbx
	mov rcx, param1 + 64 * 2
	mov rdx, param1 + 64 * 3
	mov rbp, param1 + 64 * 4
	mov rdi, param1 + 64 * 5
	mov rsi, param1 + 64 * 6
	mov r8, param1 + 64 * 7
	mov r9, param1 + 64 * 8
	mov r10, param1 + 64 * 9
	mov r11, param1 + 64 * 10
	mov r12, param1 + 64 * 11
	mov r13, param1 + 64 * 12
	mov r14, param1 + 64 * 13
	mov r15, param1 + 64 * 14
%endmacro

; TO CHECKIATE
%macro timerHandler ?1, ?2
	saveState ?1

	mov rdi, 1	
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	switchState ?2
	iretq
%endmacro


preTimerHandler:			; swapea los estados Y llama al timerHandler que swapea programas
	push rax				; backup de los contenidos de rax

	mov rax, [prevState]
	push rax				; direc de prevState en stack

	mov rax, [currentState]	
	mov [prevState], rax	; ahora prevState contiene currentState

	pop rax
	mov [currentState], rax	; ahora currentState contiene prevState

	pop rax  				; restore rax

	timerHandler prevState, CurrentState


setpPrevState:

setCurrentState:


SECTION .bss
	state1 resq 15
	state2 resq 15
	prevState resq 1 	 	; es una posicion de memoria
	currentState resq 1  	; que van a ser state1 o state2