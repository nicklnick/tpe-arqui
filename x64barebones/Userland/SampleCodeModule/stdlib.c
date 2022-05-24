#include "./include/libStd.h"

void num_to_string(long num, char * string){
    int i = 0;
    while(num > 0){
        string[i++] = num%10 + '0';
        num /= 10;
    }
    string[i] = 0;
    reverseString(string);
}

void reverseString(char * string){
    int i, j;
    char aux;
    j = strlen(string)-1;
    for(i = 0 ; i<j ; i++, j--){
        aux = string[i];
        string[i] = string[j];
        string[j] = aux;
    }
}

int strContains(char * string1, char * string2){
    int i, j;
    int contains = 0;
    for(i = 0 ; i < strlen(string1) && !contains ; i++){
        for(j = 0 ; j < strlen(string2) ; j++){
            if(string1[i] == string2[j])
                contains = 1;
            else   
                contains = 0;
        }
    }
    return contains;
}

int strlen(char * string){
    int i = 0;
    while(string[i] != '\0'){
        i++;
    }
    return i;
}

