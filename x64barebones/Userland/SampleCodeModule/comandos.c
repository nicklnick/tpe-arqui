#include "./include/stdlib.h"
#include "./include/stdio.h"

void test(uint64_t arg0){
    char c = arg0 + '0';
    print(&c,1);
}

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

    getRegisters(regBuffer);        // meto valor de registros en buffer

    for(int i=0; i<TOTAL_REGISTERS ; i++) {
        hex_to_string(regBuffer[i], stringBuffer, REGISTER_LENGTH);                  // Esto probablemente este roto
        print(registerOrder[i], strlen(registerOrder[i]));
        puts(stringBuffer);
    }
}

// ==================== help ======================================

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
    putchar('\n');
    puts("COMANDOS:");
    putchar('\n');
    puts("   - fibo:");
    puts("       imprime la serie de fibonacci");
    puts("       se corta ejecucion con la tecla X");
    putchar('\n');
    puts("   - primos:");
    puts("       imprime los numeros primos");
    puts("       se corta ejecucion con la tecla X");
    putchar('\n');
    puts("   - inforeg:");
    puts("       imprime el contenido de los registros");
    putchar('\n');
    puts("   - printmem:");
    puts("       recibe un puntero e imprime un vuelco (32 bytes) desde dicha direccion");
    putchar('\n');    
    puts("   - time:");
    puts("       imprime el dia y hora del sistema");
    putchar('\n');
    puts("   - error:");
    puts("       menu para simulacion de excepciones de kernel");
    putchar('\n');
    putchar('\n');
    puts("Pag 1/2 proxima apretando .");

    return;
}

void page1(){
    putchar('\n');
    putchar('\n');
    puts("MULTITASKING:");
    putchar('\n');
    puts("   - Para ejecucion simultanea (comando1 | comando2)");
    putchar('\n');
    puts("       Con tecla - A - se pausa/reanuda ejecucion de lado izq");
    puts("       Con tecla - L - se pausa/reanuda ejecucion de lado der");
    puts("       Con tecla - ESC - regresa a la terminal unica");
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');    
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    puts("Pag 2/2 proxima apretando .");

}


// ================================== TIME ==============================

void time(){
    char buffer[9];
    getDate(buffer);
    puts(buffer);
    getTime(buffer);
    puts(buffer);
}