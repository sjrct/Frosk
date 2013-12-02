//
// kernel/src/io.h
//

#ifndef IO_H
#define IO_H

#define DISPLAY_NOTHING 1
#define DISPLAY_SERIAL  2

#ifndef DISPLAY
	#define DISPLAY DISPLAY_SERIAL
#endif

#if DISPLAY == DISPLAY_NOTHING
	#define setup_io()
	#define kputc(X)
	#define kputs(X)
	#define kputu(X)
	#define kputn(X)
	#define kprintf(X,...)
	#define vkprintf(X,Y)
#else
	#include <stdarg.h>

	#if DISPLAY == DISPLAY_SERIAL
		#include "io/net/serial.h"
		#define setup_display() setup_serial()
		#define kputc(X) serial_putc(X, 0x3f8)
	#else
		#warning Unrecognized DISPLAY value
		#undef DISPLAY
		#include "io.h"
	#endif

	void kputs(char *);
	void kputu(ulong, int);
	void kputn(long, int);
	void kprintf(const char *, ...);
	void vkprintf(const char *, va_list);
#endif

#include "io/io.h"

#endif
