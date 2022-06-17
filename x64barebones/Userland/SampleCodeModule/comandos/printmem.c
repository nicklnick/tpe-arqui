#include "../include/comandos.h"
#include "../include/syscalls.h"

void printmem(char * arg)
{
    if(!isNum(arg)) {                                            // recibe un string que puede o no ser un numero
        puts("Invalid argument!\nArgument must be number!");
        return;
    }
    uint64_t position = atoi(arg);


    char buffer[BUFFER_LENGTH];

    if(sys_printmem(position, buffer) < 0)
        puts("Invalid memory address!");
    else
        puts(buffer);
}
