#include "../include/syscalls.h"

#define SYS_WRITE_TO_SCREEN 1
#define SYS_READ_FROM_SCREEN 0
#define SYS_CLEAR_SCREEN 2
#define SYS_RTC 4
#define SYS_REGISTER_PROCESS 3 
#define SYS_CONSUME_STDIN 7
#define SYS_KILL_PROCESS 8

#define RTC_TIME 1
#define RTC_DAY 2

extern uint64_t syscaller(uint64_t syscallID, uint64_t param1, uint64_t param2, uint64_t param3);

unsigned int sys_write(const char * string, int length){
    return syscaller(SYS_WRITE_TO_SCREEN, (uint64_t) string, (uint64_t) length, 0);        // feo arreglar
}   

unsigned int sys_read(char * buf, int length){
    return syscaller(SYS_READ_FROM_SCREEN, (uint64_t) buf, (uint64_t) length, 0);        // feo arreglar
}

unsigned int sys_clear_screen(){
    return syscaller(SYS_CLEAR_SCREEN, 0, 0, 0 );        // feo arreglar
}
unsigned int sys_register_process(uint64_t entryPoint, int screen){
    return syscaller(SYS_REGISTER_PROCESS, (uint64_t) entryPoint, (uint64_t) screen, 0);        // feo arreglar
}
unsigned int sys_rtc(uint64_t mode){
    return syscaller(SYS_RTC, mode, 0, 0);        // feo arreglar
}
unsigned int sys_consume_stdin(char * buf, unsigned int count){
    return syscaller(SYS_CONSUME_STDIN, buf, count, 0);
}
unsigned int sys_kill_process(unsigned int pid){
    return syscaller(SYS_KILL_PROCESS, pid,0,0);
}

unsigned int sys_time(){
    return syscaller(SYS_RTC, RTC_TIME, 0 , 0);
}

unsigned int sys_date(){
    return syscaller(SYS_RTC, RTC_DAY, 0, 0);
}
