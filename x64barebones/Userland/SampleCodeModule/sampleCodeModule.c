/* sampleCodeModule.c */
#include "./include/stdio.h"
#include "./include/libStd.h"
#include "./include/syscalls.h"

extern void shell();
void enterSelection(char selection);

int main() {
	
	char string[5] = "";
	
	while(1){
		print("Hola soy userman\n");
		print("Opcion 1: Shell\n");
		print("Ingrese su opcion: ");
		scanf(&string, 5);
		//print(&selection);
		//num_to_string(selection, &string);
		//print(&string);
		print("\n");
		enterSelection(string[0]);
		
	}

	return 1;
}

void enterSelection(char selection){
	switch(selection){
		case '1':
			shell();
	}
}