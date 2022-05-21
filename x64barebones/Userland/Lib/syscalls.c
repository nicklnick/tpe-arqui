
#define WRITE_ID 1
#define READ_ID 0


unsigned int syscaller(unsigned int syscallID, unsigned int param1, unsigned int param2, unsigned int param3);

unsigned int sys_write(int fd, const char * string, int length){
    return syscaller(WRITE_ID, fd, (unsigned int) string, (unsigned int) length);
}   

unsigned int sys_read(int fd, const char * buf, int length){
    return syscaller(READ_ID, fd, (unsigned int) buf, (unsigned int) length);
}