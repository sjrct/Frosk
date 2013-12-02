//
// kernel/src/io/kprint.c
//

#include <stdarg.h>
#include "../io.h"

#if DISPLAY != DISPLAY_NOTHING

void kputs(char * str)
{
	for (; *str != 0; str++) {
		kputc(*str);
	}
}

void kputu(ulong val, int radix)
{
	char ch;
	ulong div, tmp;

	if (val == 0) {
		kputc('0');
		return;
	}
	
	div = 1;
	tmp = val / radix;
	
	while (tmp > 0) {
		tmp /= radix;
		div *= radix;
	}
	
	while (div > 0) {
		ch = val / div;
		val %= div;
		div /= radix;
		
		if (ch < 10) ch += '0';
		else ch += 'A' - 10;
		
		kputc(ch);
	}
}

void kputn(long val, int radix)
{
	if (val < 0) {
		kputc('-');
		val = -val;
	}
	
	kputu(val, radix);
}

void kprintf(const char * fmt, ...)
{
	va_list ls;
	va_start(ls, fmt);
	vkprintf(fmt, ls);
	va_end(ls);
}

void vkprintf(const char * fmt, va_list ls)
{
	for (; *fmt != 0; fmt++) {
		if (*fmt == '%') {
			switch (*++fmt) {
			case 'c':
				kputc(va_arg(ls, int));
				break;

			case 's':
				kputs(va_arg(ls, char *));
				break;

			case 'd':
			case 'i':
				kputn(va_arg(ls, int), 10);
				break;

			case 'D':
			case 'I':
			case 'l':
				kputn(va_arg(ls, long), 10);
				break;

			case 'u':
				kputu(va_arg(ls, uint), 10);
				break;

			case 'o':
				kputu(va_arg(ls, uint), 010);
				break;

			case 'x':
				kputu(va_arg(ls, uint), 0x10);
				break;

			case 'X':
				kputu(va_arg(ls, ulong), 0x10);
				break;

			case 'p':
				kputu((ulong)va_arg(ls, void *), 0x10);
				break;

			default:
				kputc(*fmt);
			}
		} else {
			kputc(*fmt);
		}
	}
}

#endif
