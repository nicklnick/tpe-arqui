GLOBAL print
GLOBAL read
print:
		mov eax, 1

		int 80h

		ret
read:
		mov eax, 0
		int 80h
		ret