#include "./include/stdio.h"
#include "./include/stdlib.h"
#include "./include/comandos.h"
#include "./include/syscalls.h"

#define SYMBOL "$> "
#define SYMBOL_LENGTH 3
#define PIPE "|"
#define INVALID_COMMAND_MSG "Invalid command!"

#define BUFFER_LENGTH 150
#define MAX_WORDS 10

#define NUM_OF_COMMANDS 5

#define TOTAL_UNARY_COMMANDS 3                                  // !!!! CAMBIAR !!!!
#define TOTAL_BINARY_COMMANDS 1

static char * unaryCommands[] = {
    //"help", "inforeg"                                         // !!!!! AGREGAR !!!!!
    "fibonacci", "primos", "help"
};

static uint64_t unaryFunctions[] = {
    //(uint64_t) &help, (uint64_t)&inforeg                      // !!!!! AGREGAR !!!!!
    (uint64_t) &fibonacci, (uint64_t)&primos, (uint64_t)&help
};

static char * binaryCommands[] = {
    "printmem"
};
static uint64_t binaryFunctions[] = {
    (uint64_t)&printmem
};


int parseCommands(char * string, char ** words){         // noto las posiciones de la palabra en words
    int count=0;
    int i=0;
    for(int postSpace=1; string[i]!=0 && string[i]!='\n'; i++){
        if(string[i]==' '){
            postSpace = 1;
            string[i] = 0;                         // corto el string original en los espacios
        }
        else if(postSpace){
            words[count++] = string + i;
            postSpace = 0;
        }
    }
    string[i] = 0;                              // elimino el \n final
    return count;                               // cantidad de palabras tokenizadas
}


// devuelve la posicion en el array de punteros a funciones
// o -1 si no dio ninguno
unsigned int checkUnaryCommand(char * string){

    for(int i=0; i<TOTAL_UNARY_COMMANDS; i++){
        if(strcmp(string, unaryCommands[i])==0){
            return i;
        }
    }
    return -1;
}
unsigned int checkBinaryCommand(char * string){         // quizas generalizar comportamiento

    for(int i=0; i<TOTAL_BINARY_COMMANDS; i++){
        if(strcmp(string, binaryCommands[i])==0){
            return i;
        }
    }
    return -1;
}


void commandsDispatcher(char ** words, int count){
    char finishedExecution = 0;
    int pos1, pos2, num1, num2, pid1=-1, pid2=-1;
    char buffer[BUFFER_LENGTH];
    int size;
    switch(count){
        case 0:
            puts("Too few arguments!");
            return; 
        case 1:
            pos1 = checkUnaryCommand(words[0]);
            if(pos1 >= 0 ){
                sys_clear_screen();
                pid1 = sys_register_process(unaryFunctions[pos1], NORMAL_SCREEN);
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return; 
           }
           break;                                 // !!! Va salir del switch o de la funcion

        case 2:
            pos1 = checkBinaryCommand(words[0]);
            if(pos1 >= 0 && isNum(words[1])){
                num1  = atoi(words[1]);
                sys_clear_screen();
                pid1 = sys_register_process(binaryFunctions[pos1], NORMAL_SCREEN); // !!!!!! PASAJE DE PARAMETROS
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return; 
           }
            break; 

        case 3:
           if(strcmp(words[1],PIPE)==0){
                pos1 = checkUnaryCommand(words[0]);
                pos2 = checkUnaryCommand(words[2]);
                if(pos1 >= 0 && pos2 >= 0){
                    sys_clear_screen();
                    pid1 = sys_register_process(unaryFunctions[pos1], LEFT_SCREEN);        // !!! cuiada con argregar uno y despues otro
                    pid2 = sys_register_process(unaryFunctions[pos2], RIGHT_SCREEN);
                }
           }
           else{
                puts(INVALID_COMMAND_MSG);
                return; 
           }
           break; 

        case 4:
            if(strcmp(words[1],PIPE)==0){
                pos1 = checkUnaryCommand(words[0]);
                pos2 = checkBinaryCommand(words[2]);
                if(pos1 >=0 && pos2 >=0 && isNum(words[3])){
                    num1 = atoi(words[3]);
                    sys_clear_screen();
                    pid1 = sys_register_process(unaryFunctions[pos1], LEFT_SCREEN);
                    pid2 = sys_register_process(binaryFunctions[pos2], RIGHT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                }
            }
            else if(strcmp(words[2],PIPE)==0){
                pos1 = checkBinaryCommand(words[0]);
                pos2 = checkUnaryCommand(words[3]);
                if(pos1 >=0 && pos2 >=0 && isNum(words[1])){
                    num1 = atoi(words[1]);
                    sys_clear_screen();
                    pid1 = sys_register_process(binaryFunctions[pos1], LEFT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                    pid2 = sys_register_process(unaryFunctions[pos2], RIGHT_SCREEN);
                }
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return; 
            }
            break; 

        case 5:
            if(strcmp(words[2],PIPE)==0 && isNum(words[1]) && isNum(words[4])){
                pos1 = checkBinaryCommand(words[0]);
                pos2 = checkBinaryCommand(words[3]);
                if(pos1 >= 0 && pos2 >=0 ){
                    num1  = atoi(words[1]);
                    num2 = atoi(words[4]);
                    sys_clear_screen();
                    pid1 = sys_register_process(binaryFunctions[pos1], LEFT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                    pid2 = sys_register_process(binaryFunctions[pos2], RIGHT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                }
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return; 
            }
            break;

         default:
            puts("Too many arguments!");
            return;
        }

    while(finishedExecution==0){ 
        size = consume_buffer(buffer, BUFFER_LENGTH-1);
        buffer[size] = 0;
        if(strContainsChar(buffer,ESCAPE_KEY)>=0){                    // ## REMPLAZAR ##
            char p = pid1 + '0';
            putchar(p);
            sys_kill_process(pid1);
            if(pid2>0){
                sys_kill_process(pid2);
            }
            finishedExecution = 1;
            sys_clear_screen();
        }
    }
}

void shell(){
    char buffer[BUFFER_LENGTH];
    char * commands[MAX_WORDS];

    int amount;
    while(1){

        print(SYMBOL, SYMBOL_LENGTH);
        read_line(buffer, BUFFER_LENGTH);

        amount = parseCommands(buffer, commands);
        commandsDispatcher(commands, amount);
    }
}