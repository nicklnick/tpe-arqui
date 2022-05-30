#include "./include/stdio.h"
#include "./include/stdlib.h"

#define SYMBOL "$> "

#define BUFFER_LENGTH 150
#define MAX_COMMANDS 3
#define MAX_LENGTH 30


/*;
    En buffer tengo separado por espacios los comandos
    En commands devuelvo los comandos en orden separados

    buffer: hola que tal
    commands: {"hola", "que", "tal"}

    ESTO ES HORRIBLE EMPROLIJAR DESPUES
*/
void parseCommands(char * buffer, char ** commands){
    int i, j = 0;
    char aux[30] = {0};
    int commandCount = 0;
    for(i = 0 ; buffer[i] ; i ++){
        if(buffer[i] != ' ' && buffer[i] != '\n')
            aux[j++] = buffer[i];
        else{
            aux[j] = '\0';
            strncpy(commands[commandCount++], &aux, MAX_LENGTH);
            puts(commands[commandCount-1]);
            cleanBuffer(&aux, j);
            j = 0;
        }
    }
}

void cleanBuffer(char * pointer, int length){
    for(int i = 0 ; i < length ; i++)
        pointer[i] = '\0';    
}

void commandsDispatcher(char ** commands){
    if(strcmp(commands[1], "p") == 0){
        //caso del PIPE
        puts("Estoy en el caso del PIPE");
    } else if(strcmp(commands[0], "help") == 0){
        //Caso del HELP
        puts("Estoy en el caso del HELP");
    } else if(strcmp(commands[0], "fibo") == 0 ){
        //Caso de FIBONACCI
        puts("Estoy en el caso de FIBONACCI");
        fibo(0, 1);
    } else if(strcmp(commands[0], "primos") == 0){
        //Caso de numeros primos
        puts("Estoy en el caso de PRIMOS");
    }
}

void cleanCommands(char ** commands){
    for(int i = 0 ; i < MAX_COMMANDS ; i++){
        cleanBuffer(commands[i], MAX_LENGTH);
    }
}

void shell(){
    char buffer[BUFFER_LENGTH];
    char command1 [MAX_LENGTH] = {0}; 
    char command2 [MAX_LENGTH] = {0}; 
    char command3 [MAX_LENGTH] = {0}; 
    void * commands[MAX_COMMANDS] = {&command1, &command2, &command3};

    while(1){
        print(SYMBOL);
        scanf(buffer, BUFFER_LENGTH);
        parseCommands(buffer, (char*) &commands);
        puts("Entro al DISPATCHER");
        commandsDispatcher((char *) &commands);
        cleanCommands((char *) &commands);
        cleanBuffer(&buffer,  BUFFER_LENGTH);
    }
}