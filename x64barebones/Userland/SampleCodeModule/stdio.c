#include "./include/syscalls.h"
#include "./include/libStd.h"

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

void printFd(char * string, int fd){
    sys_write(fd, string, strlen(string));
}

void print(char * string){
    printFd(string, STDOUT);
}

void printErr(char * error){
    printFd(error, STDERR);
}

void scanf(char * buf, int length){
    sys_read(STDIN, buf, length);
}


