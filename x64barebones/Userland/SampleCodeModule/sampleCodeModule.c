/* sampleCodeModule.c */
#include "./include/stdio.h"
#include "./include/syscalls.h"



int main() {
	//All the following code may be removed 
	char string[5] = "Hola";
	//string[5] = '\0';
	//sys_write(1, &string, 5);
	scanf(&string, 100);
	print("\n");
	print(&string);
	return 1;
}