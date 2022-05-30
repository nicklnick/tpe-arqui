#include "./include/stdlib.h"

char buffer[150];

void fibo(long term1, long term2){
    long ret = term1 + term2;
    num_to_string(ret, &buffer);
    puts(&buffer);
    for(int i = 0 ; i < 50000000 ; i++);
    fibo(term2, ret);
}