#ifndef _STDIO_H
#define _STDIO_H

#define LEFT_SCREEN 3
#define RIGHT_SCREEN 5
#define NORMAL_SCREEN 1


void print(char * string, int length);
void puts(char * string);
void read_line(char * buf, int length);
char getchar();
unsigned int consume_buffer(char * buffer, int length);



#endif
