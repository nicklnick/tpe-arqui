#include "./include/stdio.h"
#include "./include/stdlib.h"
#include "./include/comandos.h"
#include "./include/syscalls.h"

extern void divError();             // exception_test.asm
extern void opCodeError();

// --- Simbolos ---
#define SYMBOL "$> "
#define SYMBOL_LENGTH 3
#define PIPE "|"
#define INVALID_COMMAND_MSG "Invalid command!"

// --- Dimensiones ---
#define BUFFER_LENGTH 150
#define MAX_WORDS 10
#define TOTAL_UNARY_COMMANDS 7
#define TOTAL_BINARY_COMMANDS 1

// --- Caracteres especiales ---
#define PAUSE_NORMAL_SCREEN 17
#define PAUSE_LEFT_SCREEN  18
#define PAUSE_RIGHT_SCREEN 19

static char specialKeys[] = {
    PAUSE_RIGHT_SCREEN, PAUSE_LEFT_SCREEN, PAUSE_NORMAL_SCREEN, ESCAPE_KEY
};
#define TOTAL_SPECIAL_KEYS 4

// --- Comandos ---
static char * unaryCommands[] = {
    "fibonacci", "primos", "help", "time", "inforeg",
    "div-error", "opcode-error"
};

static uint64_t unaryFunctions[] = {
    (uint64_t) &fibonacci, (uint64_t)&primos, (uint64_t)&help, (uint64_t)&time, (uint64_t)&inforeg,
    (uint64_t) &divError, (uint64_t)&opCodeError
};

static char * binaryCommands[] = {
    "printmem"
};
static uint64_t binaryFunctions[] = {
    (uint64_t)&printmem
};


/* - - - - - - CODIGO - - - - - - */

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
unsigned int checkCommand(char * string, char ** array, unsigned int dim){
    for(int i=0; i<dim; i++){
        if(strcmp(string, array[i])==0){
            return i;
        }
    }
    return -1;
}

// retorna la primera key especial que encuntra
char findSpecialKey(char * string, char * keys, unsigned int size){
    int i;
    for(i=0; string[i]!=0; i++){
        for(int j=0; j<size; j++){
            if(string[i]==keys[j]){
                return keys[j];
            }
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
            pos1 = checkCommand(words[0],unaryCommands, TOTAL_UNARY_COMMANDS);
            if(pos1 >= 0 ){
                sys_clear_screen();
                pid1 = sys_register_process(unaryFunctions[pos1], NORMAL_SCREEN,0);
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return; 
           }
            break;

        case 2:
            pos1 = checkCommand(words[0],binaryCommands, TOTAL_BINARY_COMMANDS);
            if(pos1 >= 0 && isNum(words[1])){
                num1  = atoi(words[1]);
                sys_clear_screen();
                pid1 = sys_register_process(binaryFunctions[pos1], NORMAL_SCREEN, num1);
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return; 
            }
            break; 


        case 3:
           if(strcmp(words[1],PIPE)==0){
                pos1 = checkCommand(words[0],unaryCommands, TOTAL_UNARY_COMMANDS);
                pos2 = checkCommand(words[2],unaryCommands, TOTAL_UNARY_COMMANDS);
                if(pos1 >= 0 && pos2 >= 0){
                    sys_clear_screen();
                    pid1 = sys_register_process(unaryFunctions[pos1], LEFT_SCREEN,0);      
                    pid2 = sys_register_process(unaryFunctions[pos2], RIGHT_SCREEN,0);
                }
                else{
                    puts(INVALID_COMMAND_MSG);          // ### FEO ###
                    return; 
                }
           }
           else{
                puts(INVALID_COMMAND_MSG);
                return; 
           }
           break; 

        case 4:
            if(strcmp(words[1],PIPE)==0){
                pos1 = checkCommand(words[0],unaryCommands, TOTAL_UNARY_COMMANDS);
                pos2 = checkCommand(words[2], binaryCommands, TOTAL_BINARY_COMMANDS);
                if(pos1 >=0 && pos2 >=0 && isNum(words[3])){
                    num1 = atoi(words[3]);
                    sys_clear_screen();
                    pid1 = sys_register_process(unaryFunctions[pos1], LEFT_SCREEN,0);
                    pid2 = sys_register_process(binaryFunctions[pos2], RIGHT_SCREEN,num1);
                }
                else{
                    puts(INVALID_COMMAND_MSG);          // ### FEO ###
                    return; 
                }
            }
            else if(strcmp(words[2],PIPE)==0){
                pos1 = checkCommand(words[0], binaryCommands, TOTAL_BINARY_COMMANDS);
                pos2 = checkCommand(words[3],unaryCommands, TOTAL_UNARY_COMMANDS);
                if(pos1 >=0 && pos2 >=0 && isNum(words[1])){
                    num1 = atoi(words[1]);
                    sys_clear_screen();
                    pid1 = sys_register_process(binaryFunctions[pos1], LEFT_SCREEN,num1);
                    pid2 = sys_register_process(unaryFunctions[pos2], RIGHT_SCREEN,0);
                }
                else{
                    puts(INVALID_COMMAND_MSG);          // ### FEO ###
                    return; 
                }
            }
            else{
                puts(INVALID_COMMAND_MSG);
                return; 
            }
            break; 

        case 5:
            if(strcmp(words[2],PIPE)==0 && isNum(words[1]) && isNum(words[4])){
                pos1 = checkCommand(words[0], binaryCommands, TOTAL_BINARY_COMMANDS);
                pos2 = checkCommand(words[3], binaryCommands, TOTAL_BINARY_COMMANDS);
                if(pos1 >= 0 && pos2 >=0 ){
                    num1  = atoi(words[1]);
                    num2 = atoi(words[4]);
                    sys_clear_screen();
                    pid1 = sys_register_process(binaryFunctions[pos1], LEFT_SCREEN,num1);
                    pid2 = sys_register_process(binaryFunctions[pos2], RIGHT_SCREEN,num2);
                }
                else{
                    puts(INVALID_COMMAND_MSG);          // ### FEO ###
                    return; 
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

    while(finishedExecution==0){                                // el shell sigue corriendo en el fondo, fijandose si se toco una tecla especial
        size = consume_buffer(buffer, BUFFER_LENGTH-1);
        buffer[size] = 0;
        char key = findSpecialKey(buffer, specialKeys, TOTAL_SPECIAL_KEYS);

        switch(key){
            case ESCAPE_KEY:
                sys_kill_process(pid1);     
                sys_kill_process(pid2);                          // en todos estos casos, nos aprovechamos con que si no existe un task con ese
                sys_clear_screen();                              // pid, no hace nada y listo.
                finishedExecution = 1;
                break;

            case PAUSE_NORMAL_SCREEN:
            case PAUSE_LEFT_SCREEN:
                sys_pause_process(pid1);
                break;

            case PAUSE_RIGHT_SCREEN:
                sys_pause_process(pid2);
                break;
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