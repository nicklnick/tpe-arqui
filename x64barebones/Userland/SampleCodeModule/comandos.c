#include "./include/stdlib.h"
#include "./include/stdio.h"

// = = = = = = = = PRINTMEM = = = = = = = = 
#define MAX_MEM_READ 16
#define BYTE_LENGTH 2
#define BUFFER_LENGTH 150

void printmem(uint64_t position)
{
    uint64_t current;
    char buffer[BYTE_LENGTH + 1];

    for(int i=0; i < MAX_MEM_READ; i++) {
        if(i!=0 && i%4==0)
            print(" ",1);
        current = *((uint8_t * )position + i);
        hex_to_string(current,buffer,BYTE_LENGTH);
        print(buffer,BYTE_LENGTH);
    }
}


// = = = = = = = = FIBONACCI = = = = = = = = 

#define FIBO_MAX 999999999999999999


void fibonacci()
{
    char buffer[30];
    uint64_t prev1=0, prev2=1, current;
    int length;

    print("0",1);               // casos especiales
    putchar('\n');

    print("1",1);
    putchar('\n');

    while(1) {
        current = prev1 + prev2;
        length = num_to_string(current,buffer);
        print(buffer,length);
        putchar('\n');

        prev1 = prev2;
        prev2 = current;

        if(current >= FIBO_MAX){            // uint64_t entra en overflow despues de este numero
            prev1 = 0;
            prev2 = 1;
        }
    }
}


// = = = = = = = = PRIMOS = = = = = = = =  

// !!! Es ineficiente aproposito, asi no llena la pantalla demasiado rapido !!!
char isPrime(uint64_t num)
{
    for(int i=2; i<num ;i++){
        if(num % i == 0) {
            return 0;
        }
    }

    return 1;
}


void primos() 
{
    char buffer[30];
    uint64_t current = 3;
    int length;

    print("2",1);           // caso especial
    putchar('\n');

    while(1) {
        if(isPrime(current)) {
            length = num_to_string(current,buffer);
            print(buffer,length);
            putchar('\n');
        }
        current++;
    }
}


// = = = = = = = = INFOREG = = = = = = = =  
extern void getRegisters(uint64_t * buffer);

#define BUFF_SIZE 20
#define TOTAL_REGISTERS 15
#define REGISTER_LENGTH 16

static char * registerOrder[] = {
    "RAX: ","RBX: ","RCX: ","RDX: ",
    "RBP: ","RSI: ","RDI: ","R8: ", 
    "R9: ","R10: ","R11: ","R12: ",
    "R13: ","R14: ","R15: " 
};

void inforeg()
{
    char stringBuffer[BUFF_SIZE];
    uint64_t regBuffer[TOTAL_REGISTERS];

    sys_inforeg(regBuffer);        // meto valor de registros en buffer

    for(int i=0; i<TOTAL_REGISTERS ; i++) {
        hex_to_string(regBuffer[i], stringBuffer, REGISTER_LENGTH);                  // Esto probablemente este roto
        print(registerOrder[i], strlen(registerOrder[i]));
        puts(stringBuffer);
    }
}

// = = = = = = = = = HELP = = = = = = = = = = 

void help(){
    int size;
    char buffer[BUFF_SIZE];
    int currPage = 0;
    page0();
    while(1){
        size = consume_buffer(buffer, BUFFER_LENGTH-1);
        buffer[size] = 0;
        if(strContainsChar(buffer, '.')>=0){                    // ## REMPLAZAR ##
            currPage++;
            currPage = currPage%2;
            switchPage(currPage);
        } else if(strContainsChar(buffer, ',')>=0){                    // ## REMPLAZAR ##
            currPage--;
            if(currPage < 0)
                currPage += 2;
            switchPage(currPage);
        }
    }
}

void switchPage(int page){
    clear_screen();
    switch(page){
        case 0: 
            page0();
            break;
        case 1:
            page1();
            break;
        default:
            break;
    }
}

void page0(){
    puts("COMANDOS:");
    puts("   - fibo:\n       imprime la serie de fibonacci\n       se corta ejecucion con la tecla ESC\n");
    puts("   - primos:\n       imprime los numeros primos\n        se corta ejecucion con la tecla ESC\n");
    puts("   - inforeg:\n       imprime el contenido de los registros\n");
    puts("   - printmem:\n       recibe un parametro e imprime un vuelco (32 bytes) desde dicha direccion\n");    
    puts("   - time:\n       imprime el dia y hora del sistema\n");
    puts("   - div-error:\n       ejecuta una division por 0 para testear la excepcion\n");
    puts("   - opcode-error:\n       ejecuta una operacion invalida para testear la excepcion\n");
    puts("Pag 1/2 |  proxima apretando . | previa apretando ,");

    return;
}

void page1(){
    puts("\n\nTECLA ESCPECIAL:\n");
    puts("       Con tecla - F5 - se hace una captura de los registros");
    puts("\nMULTITASKING:\n");
    puts("   - Para ejecucion simultanea comando1 | comando2\n");
    puts("       Con tecla - F1 - se pausa/reanuda ejecucion de modo normal");
    puts("       Con tecla - F2 - se pausa/reanuda ejecucion de lado izq");
    puts("       Con tecla - F3 - se pausa/reanuda ejecucion de lado der");
    puts("       Con tecla - ESC - regresa a la terminal unica");
    print("\n\n\n\n\n\n\n\n\n\n\n\n",12);
    puts("Pag 2/2 |  proxima apretando . | previa apretando ,");
}


// = = = = = = = = =  TIME  = = = = = = = = = 


#define RTC_TIME 1
#define RTC_DAY 2

void formatString(char * string, char character){
    char aux[9];
    int length = strlen(string);
    int i = 0;
    if(length < 6)
        aux[0] = '0';
    else
        aux[0] = string[i++];
    aux[1] = string[i++];
    aux[2] = character;
    aux[3] = string[i++];
    aux[4] = string[i++];
    aux[5] = character;
    aux[6] = string[i++];
    aux[7] = string[i];
    aux[8] = '\0';
    strncpy(string, aux, 9);
}

void getTime(char * buffer){
    unsigned int num = sys_rtc(RTC_TIME);
    num_to_string((uint64_t) num, buffer);
    formatString(buffer, ':');
}

void getDate(char * buffer){
    unsigned int num = sys_rtc(RTC_DAY);
    num_to_string((uint64_t) num, buffer);
    formatString(buffer, '/');
}

void time(){
    char buffer[9];
    print("DATE: ",6);
    getDate(buffer);
    puts(buffer);
    print("TIME: ",6);
    getTime(buffer);
    puts(buffer);
}