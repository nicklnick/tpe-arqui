#include "./include/stdlib.h"

#define NULL ((char *) 0)

void num_to_string(long num, char * string){
    int i = 0;
    while(num > 0){
        string[i++] = num%10 + '0';
        num /= 10;
    }
    reverseString(string);
    string[i] = 0;
}

void reverseString(char * string)
{
    char aux;

    for(int i = 0, j = strlen(string) - 1; i < j ; i++, j--) {
        aux = string[i];
        string[i] = string[j];
        string[j] = aux;
    }
}

int strContains(char * string1, char * string2)
{
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

// 0 si son ==
// s1 < s2 ==> <0
// s1 > s2 ==> >0
int strcmp(const char * s1, const char * s2)
{
        while(*s1 && (*s1 == *s2))
                s1++, s2++;

        return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int strlen(const char * string)
{
    int i = 0;
    while(string[i])
        i++;

    return i;
}

int atoi(const char * string) 
{
    int result = 0;

    for(int i = 0 ; string[i] ; i++)
        result = result*10 + string[i] - '0';

    return result;
}

char * strncpy(char * dest, const char * src, unsigned int n)
{
        if(dest == NULL)
                return NULL;

        char * out = dest;
        while(*src && n--) {
                *dest = *src;
                dest++; src++;
        }
        *dest = '\0';

        return out;
}

void removeEnter(char * string){
    int i;
    int removed = 0;
    for(i = 0 ; i < strlen(string) && !removed; i++){
        if(string[i] == '\n'){
            string[i] = '\0';
            removed = 1;
        }
    }
}
