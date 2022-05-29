#include "./include/stdio.h"
#include "./include/stdlib.h"

#define SYMBOL "$> "

#define BUFFER_LENGTH 150
#define MAX_COMMANDS 5
#define MAX_LENGTH 30


/*;
    En buffer tengo separado por espacios los comandos
    En commands devuelvo los comandos en orden separados

    buffer: hola que tal
    commands: {"hola", "que", "tal"}

    ESTO ES HORRIBLE EMPROLIJAR DESPUES
*/
void parseCommands(char * buffer, char ** commands)
{
    char aux[MAX_LENGTH] = {0};
    int commandCount = 0;

    for(int i=0, j=0; buffer[i] ; i++){
        if(buffer[i] != ' ' && buffer[i] != '\n')
            aux[j++] = buffer[i];
        else {
            aux[j] = '\0';
            strncpy(commands[commandCount], aux, MAX_LENGTH);
            puts(commands[commandCount++]);
            j = 0;
        }
    }
}

void cleanBuffer(char * buffer, int length){
    int i = 0;
    for(i = 0 ; i < length ; i++){
        buffer[i] = '\0';
    }
}

void commandsDispacher(char ** commands){
    if(strcmp(commands[1], "p") == 0){
        //caso del PIPE
        //en commands[1] el primer comando
        //en commands[2] el segundo comando
        print("Estoy en el caso del PIPE");
    } else if(strcmp(commands[0], "help") == 0){
        //Caso del HELP
        print("Estoy en el caso del HELP");
    }
}

void shell(){
    char buffer[BUFFER_LENGTH];
    char commands [MAX_COMMANDS][MAX_LENGTH] = {0};          //array de 5 palabras de 30 letras

    while(1){
        puts(SYMBOL);
        scanf(buffer, BUFFER_LENGTH);
        puts("ENTRO AL PARSE:");
        parseCommands(buffer, commands);
        puts("SALI DEL PARSE:");
        testParser(commands);
        commandsDispacher(&commands);
    }
}

void testParser(char * s) {
        s[0];
        s[1];
        s[2];
}
