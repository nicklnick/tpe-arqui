#include <stdint.h>

#define SYS_WRITE_TO_SCREEN 1
#define SYS_READ_FROM_SCREEN 0
#define SYS_CLEAR_SCREEN 2
#define SYS_RTC 4
#define SYS_REGISTER_PROCESS 3 


extern uint64_t syscaller(uint64_t syscallID, uint64_t param1, uint64_t param2, uint64_t param3);

unsigned int sys_write(const char * string, int length){
    return syscaller(WRITE_ID, (uint64_t) string, (uint64_t) length);
}   

unsigned int sys_read(char * buf, int length){
    return syscaller(READ_ID, (uint64_t) buf, (uint64_t) length);
}

unsigned int sys_clear_screen(){
    return syscaller(SYS_CLEAR_SCREEN);
}
unsigned int sys_register_process(uint64_t entryPoint, int screen){
    return syscaller(SYS_REGISTER_PROCESS, (uint64_t) entryPoint, (uint64_t) screen);
}
unsigned int sys_rtc(uint64_t mode){
    return syscaller(SYS_RTC, mode);
}