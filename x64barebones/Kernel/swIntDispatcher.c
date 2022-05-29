#include <stdint.h>
#include <syscalls.h>

//registros en asm:		rax		  rdi		 rsi	 rdx		r10		 r8			r9
//registros en c: 		rdi		  rsi		 rdx	 rcx		r8		 r9		   stack		// de derecha a izquierda se pasan a los registros
unsigned int swIntDispatcher(uint64_t mode, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) 
{
	switch (mode) {
		case 0:
			return sys_read_from_screen(arg1, arg2);
		case 1:
			return sys_write_to_screen(arg0,arg1);
		case 2:
			return sys_clear_screen();
		case 3:
			return sys_register_process(arg0, arg1);
		case 4:
			return sys_rtc(arg0);
		case 5:
			return sys_read(arg0, arg1, arg2);			// deprecated?
		case 6:
			return sys_write(arg0, arg1, arg2);			// deprecated?

	}
}
