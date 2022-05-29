#include <stdint.h>

#define WRITE_ID 1
#define READ_ID 0


extern uint64_t syscaller(uint64_t syscallID, uint64_t param1, uint64_t param2, uint64_t param3);

unsigned int sys_write(int fd, char * string, int length){
    return syscaller((uint64_t) WRITE_ID, fd, (uint64_t) string, (uint64_t) length);
}   

unsigned int sys_read(int fd, const char * buf, int length){
    return syscaller((uint64_t) READ_ID, fd, (uint64_t) buf, (uint64_t) length);
}
