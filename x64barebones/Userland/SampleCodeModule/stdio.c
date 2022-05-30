#include "./include/syscalls.h"
#include "./include/stdlib.h"

// Entrada estandar
#define STDIN 0

// Normal mode
#define STDOUT 1
#define STDERR 2

// Split screen
#define STDOUT_LEFT 3
#define STDOUT_RIGHT 5
#define STDERR_LEFT 4
#define STDERR_RIGHT 6

char buf[20];

void printFd(char * string, int fd){
    sys_write(fd, string, strlen(string));
}

void print(char * string){
    printFd(string, STDOUT);
}

void puts(char * string) {
    printFd(string, STDOUT);
    putchar('\n');
}

char getchar(){
    scanf(&buf, 1);
    return buf[0];
}

void putchar(char letter){
    char word[2] = {letter, '\0'};
    print(&letter);
}

void printErr(char * error){
    printFd(error, STDERR);
}

void scanf(char * buf, int length){
    sys_read(STDIN, buf, length);
}


