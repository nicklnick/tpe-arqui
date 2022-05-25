/* sampleCodeModule.c */
#include "./include/stdio.h"
#include "./include/libStd.h"
#include "./include/syscalls.h"

extern void shell();
void enterSelection(int selection);

int main() {
	
	char string[100] = " ";
	int selection;

	while(1){
		print("Hola soy userman\n");
		print("Opcion 1: Shell\n");
		print("Ingrese su opcion: ");
		scanf(&string, 100);
		removeEnter(&string);
		print(&string);
		//selection = atoi(&string);
		//print(&selection);
		//num_to_string(selection, &string);
		//print(&string);

		//enterSelection(selection);
	}

	return 1;
}

void enterSelection(int selection){
	switch(selection){
		case 1:
			shell();
	}

}