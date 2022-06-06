#ifndef _STDIO_H
#define _STDIO_H

#define LEFT_SCREEN 3
#define RIGHT_SCREEN 5
#define NORMAL_SCREEN 1

#define ESCAPE_KEY 27


void print(char * string, int length);
void putchar(char c);
void puts(char * string);
void read_line(char * buf, int length);
char getchar();
void clear_screen();
unsigned int consume_buffer(char * buffer, int length);
char getchar();


#endif
