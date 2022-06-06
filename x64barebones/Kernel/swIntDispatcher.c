#include <stdint.h>
#include <syscalls.h>

#define INVALID_SYS_CALL 255


//registros en asm:		rax		  rdi		 rsi	 rdx		r10		 r8			r9
//registros en c: 		rdi		  rsi		 rdx	 rcx		r8		 r9		   stack		// de derecha a izquierda se pasan a los registros
unsigned int swIntDispatcher(uint64_t mode, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) 
{
	switch (mode) {
		case 0:
			return sys_read_from_screen((char *) arg0, (unsigned int) arg1);
		case 1:
			return sys_write_to_screen((const char *) arg0,(unsigned int) arg1);
		case 2:
			return sys_clear_screen();
		case 3:
			return sys_register_process(arg0, (int) arg1, arg2);
		case 4:
			return sys_rtc((unsigned int) arg0);
		case 7:
			return sys_consume_stdin((char *) arg0 , (unsigned int) arg1);
		case 8:
			return sys_kill_process((unsigned int) arg0);
		case 9:
			return sys_pause_process((unsigned int) arg0);
		case 10:
			return sys_inforeg((uint64_t*) arg0);
		default:
			return INVALID_SYS_CALL;
	}
}


/*
	Sys_write y sys_read (que permitian elegir a la pantalla
	a la cual imprimir), fueron eliminadas ya que se considera 
	responsabilidad del kernel administrar la pantalla a cual
	se va a escribir.
*/
