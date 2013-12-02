//
// kernel/src/debug.h
//

#undef assert
#undef dputc
#undef dputs
#undef dputn
#undef dprintf

#ifdef DEBUG
	#include "io.h"

	#define dputc kputc
	#define dputs kputs
	#define dputn kputn
	#define dprintf kprintf

	#define __assert(C,L,F) \
		if (!(C)) {  \
			dputs("Assertion \"" #C "\" failed at line " #L " in " #F "\n"); \
		}
	#define _assert(C,L,F) __assert(C,L,F)
	#define assert(C) _assert(C,__LINE__,__FILE__)
	#define do_assert(C) _assert(C,__LINE__,__FILE__)
#else
	#define dputc(X)
	#define dputs(X)
	#define dputn(X,Y)
	#define dprintf(...)
	#define assert(X)
	#define do_assert(X) X
#endif
