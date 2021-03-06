#include <stdint.h>
#include <syscalls.h>

#define INVALID_SYS_CALL 255


//registros en asm:		rax		  rdi		 rsi	 rdx		r10		 r8			r9
//registros en c: 		rdi		  rsi		 rdx	 rcx		r8		 r9		   stack		// de derecha a izquierda se pasan a los registros
unsigned int swIntDispatcher(uint64_t mode, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) 
{
	switch (mode) {
		case SYS_READ_FROM_SCREEN:
			return sys_read_from_screen((char *) arg0, (unsigned int) arg1);
			
		case SYS_WRITE_TO_SCREEN:
			return sys_write_to_screen((const char *) arg0,(unsigned int) arg1);

		case SYS_CLEAR_SCREEN:
			return sys_clear_screen();

		case SYS_REGISTER_PROCESS:
			return sys_register_process(arg0, (int) arg1, arg2);

		case SYS_RTC:
			return sys_rtc((unsigned int) arg0);

		case SYS_CONSUME_STDIN:
			return sys_consume_stdin((char *) arg0 , (unsigned int) arg1);

		case SYS_KILL_PROCESS:
			return sys_kill_process((unsigned int) arg0);

		case SYS_PAUSE_PROCESS:
			return sys_pause_process((unsigned int) arg0);

		case SYS_INFOREG:
			return sys_inforeg((uint64_t*) arg0);

		case SYS_PRINTMEM:
			return sys_printmem((uint64_t)arg0, (char *) arg1);

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
