#include "./include/stdlib.h"
#include "./include/stdio.h"

// = = = = = = = = PRINTMEM = = = = = = = = 

#define MAX_MEM_READ 16
#define BYTE_LENGTH 2

int hex_to_string(uint64_t num, char * buffer, int fixedLength){
    int i = 0;
    for(int aux ; num > 0 ; i++, num/=16){
        aux = num % 16;
        if(aux >=0 && aux < 10)                     // convierto a hex
            buffer[i] = aux + '0';
        else
            buffer[i] = aux - 10 + 'A';

    }
    while(i<fixedLength){                   // le agrego 0 por deltante para llegar a la longitud deseada
        buffer[i++] = '0';
    }
    buffer[i] = 0;
    return i;
}

void printMem(uint64_t position){
    uint64_t current;
    char buffer[BYTE_LENGTH + 1];
    for(int i=0; i<MAX_MEM_READ; i++){
        if(i!=0 && i%4==0){
            print(" ",1);
        }
        current = *((uint8_t * )position + i);
        hex_to_string(current,buffer,BYTE_LENGTH);
        print(buffer,BYTE_LENGTH);
    }
}


// = = = = = = = = FIBONACCI = = = = = = = = 

_Noreturn void fibonacci(){
    char buffer[30];
    uint64_t prev1=0, prev2=1, current;
    int length;

    print("0",1);               // casos especiales
    print(" - ",3);

    print("1",1);
    print(" - ",3);

    while(1){
        current = prev1 + prev2;
        length = num_to_string(current,buffer);
        print(buffer,length);
        print(" - ",3);

        prev1 = prev2;
        prev2 = current;
    }
}


// = = = = = = = = PRIMOS = = = = = = = =  

// !!! Es ineficiente aproposito, asi no llena la pantalla demasiado rapido !!!
char isPrime(uint64_t num){
    for(int i=2; i<num ;i++){
        if(num % i == 0) {
            return 0;
        }
    }
    return 1;
}

_Noreturn void primos(){
    char buffer[30];
    uint64_t current = 3;
    int length;

    print("2",1);           // caso especial
    print(" - ",3);

    while(1){
        if(isPrime(current)){
            length = num_to_string(current,buffer);
            print(buffer,length);
            print(" - ",3);
        }
        current++;
    }
}
