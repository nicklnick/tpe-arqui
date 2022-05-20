GLOBAL print
GLOBAL read
print:
		push rbp
		mov rbp, rsp
		mov eax, 1
		int 80h
		mov rsp, rbp
		pop rbp
		ret
read:
		push rbp
		mov rbp, rsp


		mov eax, 0
		int 80h

		mov rsp, rbp
		pop rbp
		ret