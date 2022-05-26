#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

int hex_to_string(uint64_t num, char * buffer, int fixedLength);
int str_len(const char * string);

#endif