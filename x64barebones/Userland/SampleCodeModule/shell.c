#include "./include/stdio.h"
#include "./include/libStd.h"

#define SYMBOL "$> "
#define BUFFER_LENGHT 150
#define MAX_COMMANDS 5




void shell(){

    char buffer[BUFFER_LENGHT];
    char commands [MAX_COMMANDS][30] = {0};          //array de 5 palabras de 30 letras

    while(1){
        print(SYMBOL);
        scanf(&buffer, 150);
        print("ENTRO AL PARSE:\n");
        parseCommands(&buffer, &commands);
        putchar('\n');
        print("SALI DEL PARSE:\n");
        testParser(&commands);
        putchar('\n');
        commandsDispacher(&commands);
    }

}

void testParser(char ** words){
    print(words[0]);
    print(words[1]);
    print(words[2]);
}


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
            strncpy(commands[commandCount++], &aux);
            print(commands[commandCount-1]);
            cleanBuffer(&aux, j);
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
    if(strneql(commands[1], "p")){
        //caso del PIPE
        //en commands[1] el primer comando
        //en commands[2] el segundo comando
        print("Estoy en el caso del PIPE");
    } else if(strneql(commands[0], "help")){
        //Caso del HELP
        print("Estoy en el caso del HELP");
    }
}

