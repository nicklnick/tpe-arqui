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

#define TOTAL_UNARY_COMMANDS 4
static char * unaryCommands[] = {
    //"help", "inforeg"                                          // !!!!! AGREGAR !!!!!
    "fibonacci", "primos"
};

static uint64_t unaryFunctions[] = {
    //(uint64_t) &help, (uint64_t)&inforeg                       // !!!!! AGREGAR !!!!!
    (uint64_t) &fibonacci, (uint64_t)&primos
};

#define TOTAL_BINARY_COMMANDS 1
static char * binaryCommands[] = {
    "printmem"
};
static uint64_t binaryFunctions[] = {
    (uint64_t)&printmem
};


int parseCommands(char * string, char ** words){         // noto las posiciones de la palabra en words
    int count=0;
    for(int i=0 , postSpace=1; string[i]!=0 && string[i]!='\n'; i++){
        if(string[i]==' '){
            postSpace = 1;
            string[i] = 0;                         // corto el string original en los espacios
        }
        else if(postSpace){
            words[count++] = string + i;
            postSpace = 0;
        }
    }
    return count;                               // cantidad de palabras tokenizadas
}


// devuelve la posicion en el array de punteros a funciones
// o -1 si no dio ninguno
unsigned int checkUnaryCommand(char * string){

    for(int i=0; i<TOTAL_UNARY_COMMANDS; i++){
        if(strcmp(string, unaryCommands[i])){
            return i;
        }
    }
    return -1;
}
unsigned int checkBinaryCommand(char * string){         // quizas generalizar comportamiento

    for(int i=0; i<TOTAL_BINARY_COMMANDS; i++){
        if(strcmp(string, binaryCommands[i])){
            return i;
        }
    }
    return -1;
}


void commandsDispatcher(char ** words, int count){
    char finishedExecution = 0;
    int pos1, pos2, num1, num2;
    switch(count){
        case 0:
            puts("Too few arguments!");
            return; 
        case 1:
                pos1 = checkUnaryCommand(words[0]);
                if(pos1 >= 0 ){
                    sys_clear_screen();
                    sys_register_process(unaryFunctions[pos1], NORMAL_SCREEN);
                }
                else{
                    puts(INVALID_COMMAND_MSG);
                    return;                                 // !!! Va salir del switch o de la funcion
               }
        case 2:
                pos1 = checkBinaryCommand(words[0]);
                if(pos1 >= 0 && isNum(words[1])){
                    num1  = atoi(words[1]);
                    sys_clear_screen();
                    sys_register_process(binaryFunctions[pos1], NORMAL_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                }
                else{
                    puts(INVALID_COMMAND_MSG);
                    return;
               }
                
        case 3:
               if(strcmp(words[1],PIPE)){
                    pos1 = checkUnaryCommand(words[0]);
                    pos2 = checkUnaryCommand(words[2]);
                    if(pos1 >= 0 && pos2 >= 0){
                        sys_clear_screen();
                        sys_register_process(unaryFunctions[pos1], LEFT_SCREEN);        // !!! cuiada con argregar uno y despues otro
                        sys_register_process(unaryFunctions[pos2], RIGHT_SCREEN);
                    }
               }
               else{
                    puts(INVALID_COMMAND_MSG);
                    return;
               }
        case 4:
            if(strcmp(words[1],PIPE)){
                pos1 = checkUnaryCommand(words[0]);
                pos2 = checkBinaryCommand(words[2]);
                if(pos1 >=0 && pos2 >=0 && isNum(words[3])){
                    num1 = atoi(words[3]);
                    sys_clear_screen();
                    sys_register_process(unaryFunctions[pos1], LEFT_SCREEN);
                    sys_register_process(binaryFunctions[pos2], RIGHT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                }
            }
            else if(strcmp(words[2],PIPE)){
                pos1 = checkBinaryCommand(words[0]);
                pos2 = checkUnaryCommand(words[3]);
                if(pos1 >=0 && pos2 >=0 && isNum(words[1])){
                    num1 = atoi(words[1]);
                    sys_clear_screen();
                    sys_register_process(binaryFunctions[pos1], LEFT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                    sys_register_process(unaryFunctions[pos2], RIGHT_SCREEN);
                }
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return;
            }
        case 5:
                if(strcmp(words[2],PIPE) && isNum(words[1]) && isNum(words[4])){
                    pos1 = checkBinaryCommand(words[0]);
                    pos2 = checkBinaryCommand(words[3]);
                    if(pos1 >= 0 && pos2 >=0 ){{
                        num1  = atoi(words[1]);
                        num2 = atoi(words[4]);
                        sys_clear_screen();
                        sys_register_process(binaryFunctions[pos1], LEFT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                        sys_register_process(binaryFunctions[pos2], RIGHT_SCREEN); // !!!!!! PASAJE DE PARAMETROS
                    }
                }
                else{
                    puts(INVALID_COMMAND_MSG);
                    return;
                }
        default:
            puts("Too many arguments!");
            return;
    }
    while(!finishedExecution){ 
          // TODO: Consumo buffer para fijarme si puso corte de ejecucion
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