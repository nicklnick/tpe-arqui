#ifndef _STDLIB_H
#define _STDLIB_H

#include <stdint.h>

int num_to_string(uint64_t num, char * buffer);

int strcmp(const char * s1, const char * s2);

int strlen(const char * string);

int atoi(const char * string);

void removeEnter(char * string);

char * strncpy(char * dest, const char * src, unsigned int n);

uint8_t isNum(const char * string);

int hex_to_string(uint64_t num, char * buffer, int fixedLength);
unsigned int strContainsChar(char * string, char c);

#endif
