//
// kernel/src/error.c
//

#include <stdarg.h>
#include "io.h"
#include "asm.h"
#include "error.h"

void fatal(const char * str, ...)
{
	va_list ls;

	kputs("Fatal Error: ");

	va_start(ls, str);
	vkprintf(str, ls);
	va_end(ls);

	kputs("\nSystem halted\n");

	cli();
	while (1) hlt();
}
